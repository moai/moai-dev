/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2009, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: gtls.c,v 1.63 2009-10-19 18:10:47 gknauf Exp $
 ***************************************************************************/

/*
 * Source file for all GnuTLS-specific code for the TLS/SSL layer. No code
 * but sslgen.c should ever call or use these functions.
 *
 * Note: don't use the GnuTLS' *_t variable type names in this source code,
 * since they were not present in 1.0.X.
 */

#include "setup.h"
#ifdef USE_GNUTLS
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gcrypt.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include "urldata.h"
#include "sendf.h"
#include "inet_pton.h"
#include "gtls.h"
#include "sslgen.h"
#include "parsedate.h"
#include "connect.h" /* for the connect timeout */
#include "select.h"
#include "rawstr.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>
#include "curl_memory.h"
/* The last #include file should be: */
#include "memdebug.h"

/*
 Some hackish cast macros based on:
 http://library.gnome.org/devel/glib/unstable/glib-Type-Conversion-Macros.html
*/
#ifndef GNUTLS_POINTER_TO_INT_CAST
#define GNUTLS_POINTER_TO_INT_CAST(p) ((int) (long) (p))
#endif
#ifndef GNUTLS_INT_TO_POINTER_CAST
#define GNUTLS_INT_TO_POINTER_CAST(i) ((void*) (long) (i))
#endif

/* Enable GnuTLS debugging by defining GTLSDEBUG */
/*#define GTLSDEBUG */

#ifdef GTLSDEBUG
static void tls_log_func(int level, const char *str)
{
    fprintf(stderr, "|<%d>| %s", level, str);
}
#endif
static bool gtls_inited = FALSE;
/*
 * Custom push and pull callback functions used by GNU TLS to read and write
 * to the socket.  These functions are simple wrappers to send() and recv()
 * (although here using the sread/swrite macros as defined by setup_once.h).
 * We use custom functions rather than the GNU TLS defaults because it allows
 * us to get specific about the fourth "flags" argument, and to use arbitrary
 * private data with gnutls_transport_set_ptr if we wish.
 */
static ssize_t Curl_gtls_push(void *s, const void *buf, size_t len)
{
  return swrite(GNUTLS_POINTER_TO_INT_CAST(s), buf, len);
}

static ssize_t Curl_gtls_pull(void *s, void *buf, size_t len)
{
  return sread(GNUTLS_POINTER_TO_INT_CAST(s), buf, len);
}

/* Curl_gtls_init()
 *
 * Global GnuTLS init, called from Curl_ssl_init(). This calls functions that
 * are not thread-safe and thus this function itself is not thread-safe and
 * must only be called from within curl_global_init() to keep the thread
 * situation under control!
 */
int Curl_gtls_init(void)
{
  int ret = 1;
  if(!gtls_inited) {
    ret = gnutls_global_init()?0:1;
#ifdef GTLSDEBUG
    gnutls_global_set_log_function(tls_log_func);
    gnutls_global_set_log_level(2);
#endif
    gtls_inited = TRUE;
  }
  return ret;
}

int Curl_gtls_cleanup(void)
{
  if(gtls_inited) {
    gnutls_global_deinit();
    gtls_inited = FALSE;
  }
  return 1;
}

static void showtime(struct SessionHandle *data,
                     const char *text,
                     time_t stamp)
{
  struct tm *tm;
#ifdef HAVE_GMTIME_R
  struct tm buffer;
  tm = (struct tm *)gmtime_r(&stamp, &buffer);
#else
  tm = gmtime(&stamp);
#endif
  snprintf(data->state.buffer,
           BUFSIZE,
           "\t %s: %s, %02d %s %4d %02d:%02d:%02d GMT\n",
           text,
           Curl_wkday[tm->tm_wday?tm->tm_wday-1:6],
           tm->tm_mday,
           Curl_month[tm->tm_mon],
           tm->tm_year + 1900,
           tm->tm_hour,
           tm->tm_min,
           tm->tm_sec);
  infof(data, "%s", data->state.buffer);
}

static gnutls_datum load_file (const char *file)
{
  FILE *f;
  gnutls_datum loaded_file = { NULL, 0 };
  long filelen;
  void *ptr;

  if (!(f = fopen(file, "r")))
    return loaded_file;
  if (fseek(f, 0, SEEK_END) != 0
      || (filelen = ftell(f)) < 0
      || fseek(f, 0, SEEK_SET) != 0
      || !(ptr = malloc((size_t)filelen)))
    goto out;
  if (fread(ptr, 1, (size_t)filelen, f) < (size_t)filelen) {
    free(ptr);
    goto out;
  }

  loaded_file.data = ptr;
  loaded_file.size = (unsigned int)filelen;
out:
  fclose(f);
  return loaded_file;
}

static void unload_file(gnutls_datum data) {
  free(data.data);
}


/* this function does a BLOCKING SSL/TLS (re-)handshake */
static CURLcode handshake(struct connectdata *conn,
                          gnutls_session session,
                          int sockindex,
                          bool duringconnect)
{
  struct SessionHandle *data = conn->data;
  int rc;
  if(!gtls_inited)
    Curl_gtls_init();
  do {
    rc = gnutls_handshake(session);

    if((rc == GNUTLS_E_AGAIN) || (rc == GNUTLS_E_INTERRUPTED)) {
      long timeout_ms = Curl_timeleft(conn, NULL, duringconnect);

      if(timeout_ms < 0) {
        /* a precaution, no need to continue if time already is up */
        failf(data, "SSL connection timeout");
        return CURLE_OPERATION_TIMEDOUT;
      }

      rc = Curl_socket_ready(conn->sock[sockindex],
                       conn->sock[sockindex], (int)timeout_ms);
      if(rc > 0)
        /* reabable or writable, go loop*/
        continue;
      else if(0 == rc) {
        /* timeout */
        failf(data, "SSL connection timeout");
        return CURLE_OPERATION_TIMEDOUT;
      }
      else {
        /* anything that gets here is fatally bad */
        failf(data, "select/poll on SSL socket, errno: %d", SOCKERRNO);
        return CURLE_SSL_CONNECT_ERROR;
      }
    }
    else
      break;
  } while(1);

  if(rc < 0) {
    failf(data, "gnutls_handshake() failed: %s", gnutls_strerror(rc));
    return CURLE_SSL_CONNECT_ERROR;
  }

  return CURLE_OK;
}

static gnutls_x509_crt_fmt do_file_type(const char *type)
{
  if(!type || !type[0])
    return GNUTLS_X509_FMT_PEM;
  if(Curl_raw_equal(type, "PEM"))
    return GNUTLS_X509_FMT_PEM;
  if(Curl_raw_equal(type, "DER"))
    return GNUTLS_X509_FMT_DER;
  return -1;
}


/*
 * This function is called after the TCP connect has completed. Setup the TLS
 * layer and do all necessary magic.
 */
CURLcode
Curl_gtls_connect(struct connectdata *conn,
                  int sockindex)

{
  static const int cert_type_priority[] = { GNUTLS_CRT_X509, 0 };
  struct SessionHandle *data = conn->data;
  gnutls_session session;
  int rc;
  unsigned int cert_list_size;
  const gnutls_datum *chainp;
  unsigned int verify_status;
  gnutls_x509_crt x509_cert,x509_issuer;
  gnutls_datum issuerp;
  char certbuf[256]; /* big enough? */
  size_t size;
  unsigned int algo;
  unsigned int bits;
  time_t certclock;
  const char *ptr;
  void *ssl_sessionid;
  size_t ssl_idsize;
  bool sni = TRUE; /* default is SNI enabled */
#ifdef ENABLE_IPV6
  struct in6_addr addr;
#else
  struct in_addr addr;
#endif

  if(conn->ssl[sockindex].state == ssl_connection_complete)
    /* to make us tolerant against being called more than once for the
       same connection */
    return CURLE_OK;

  if(!gtls_inited)
    Curl_gtls_init();

  /* GnuTLS only supports SSLv3 and TLSv1 */
  if(data->set.ssl.version == CURL_SSLVERSION_SSLv2) {
    failf(data, "GnuTLS does not support SSLv2");
    return CURLE_SSL_CONNECT_ERROR;
  }
  else if(data->set.ssl.version == CURL_SSLVERSION_SSLv3)
    sni = FALSE; /* SSLv3 has no SNI */

  /* allocate a cred struct */
  rc = gnutls_certificate_allocate_credentials(&conn->ssl[sockindex].cred);
  if(rc != GNUTLS_E_SUCCESS) {
    failf(data, "gnutls_cert_all_cred() failed: %s", gnutls_strerror(rc));
    return CURLE_SSL_CONNECT_ERROR;
  }

  if(data->set.ssl.CAfile) {
    /* set the trusted CA cert bundle file */
    gnutls_certificate_set_verify_flags(conn->ssl[sockindex].cred,
                                        GNUTLS_VERIFY_ALLOW_X509_V1_CA_CRT);

    rc = gnutls_certificate_set_x509_trust_file(conn->ssl[sockindex].cred,
                                                data->set.ssl.CAfile,
                                                GNUTLS_X509_FMT_PEM);
    if(rc < 0) {
      infof(data, "error reading ca cert file %s (%s)\n",
            data->set.ssl.CAfile, gnutls_strerror(rc));
      if(data->set.ssl.verifypeer)
        return CURLE_SSL_CACERT_BADFILE;
    }
    else
      infof(data, "found %d certificates in %s\n",
            rc, data->set.ssl.CAfile);
  }

  if(data->set.ssl.CRLfile) {
    /* set the CRL list file */
    rc = gnutls_certificate_set_x509_crl_file(conn->ssl[sockindex].cred,
                                              data->set.ssl.CRLfile,
                                              GNUTLS_X509_FMT_PEM);
    if(rc < 0) {
      failf(data, "error reading crl file %s (%s)\n",
            data->set.ssl.CRLfile, gnutls_strerror(rc));
      return CURLE_SSL_CRL_BADFILE;
    }
    else
      infof(data, "found %d CRL in %s\n",
            rc, data->set.ssl.CRLfile);
  }

  /* Initialize TLS session as a client */
  rc = gnutls_init(&conn->ssl[sockindex].session, GNUTLS_CLIENT);
  if(rc != GNUTLS_E_SUCCESS) {
    failf(data, "gnutls_init() failed: %d", rc);
    return CURLE_SSL_CONNECT_ERROR;
  }

  /* convenient assign */
  session = conn->ssl[sockindex].session;

  if ((0 == Curl_inet_pton(AF_INET, conn->host.name, &addr)) &&
#ifdef ENABLE_IPV6
      (0 == Curl_inet_pton(AF_INET6, conn->host.name, &addr)) &&
#endif
      sni &&
      (gnutls_server_name_set(session, GNUTLS_NAME_DNS, conn->host.name,
                              strlen(conn->host.name)) < 0))
    infof(data, "WARNING: failed to configure server name indication (SNI) "
          "TLS extension\n");

  /* Use default priorities */
  rc = gnutls_set_default_priority(session);
  if(rc != GNUTLS_E_SUCCESS)
    return CURLE_SSL_CONNECT_ERROR;

  if(data->set.ssl.version == CURL_SSLVERSION_SSLv3) {
    static const int protocol_priority[] = { GNUTLS_SSL3, 0 };
    gnutls_protocol_set_priority(session, protocol_priority);
    if(rc != GNUTLS_E_SUCCESS)
      return CURLE_SSL_CONNECT_ERROR;
  }

  /* Sets the priority on the certificate types supported by gnutls. Priority
     is higher for types specified before others. After specifying the types
     you want, you must append a 0. */
  rc = gnutls_certificate_type_set_priority(session, cert_type_priority);
  if(rc != GNUTLS_E_SUCCESS)
    return CURLE_SSL_CONNECT_ERROR;

  if(data->set.str[STRING_CERT]) {
    if( gnutls_certificate_set_x509_key_file(
          conn->ssl[sockindex].cred,
          data->set.str[STRING_CERT],
          data->set.str[STRING_KEY] ?
          data->set.str[STRING_KEY] : data->set.str[STRING_CERT],
          do_file_type(data->set.str[STRING_CERT_TYPE]) ) != GNUTLS_E_SUCCESS) {
      failf(data, "error reading X.509 key or certificate file");
      return CURLE_SSL_CONNECT_ERROR;
    }
  }

  /* put the credentials to the current session */
  rc = gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE,
                              conn->ssl[sockindex].cred);

  /* set the connection handle (file descriptor for the socket) */
  gnutls_transport_set_ptr(session,
                           GNUTLS_INT_TO_POINTER_CAST(conn->sock[sockindex]));

  /* register callback functions to send and receive data. */
  gnutls_transport_set_push_function(session, Curl_gtls_push);
  gnutls_transport_set_pull_function(session, Curl_gtls_pull);

  /* lowat must be set to zero when using custom push and pull functions. */
  gnutls_transport_set_lowat(session, 0);

  /* This might be a reconnect, so we check for a session ID in the cache
     to speed up things */

  if(!Curl_ssl_getsessionid(conn, &ssl_sessionid, &ssl_idsize)) {
    /* we got a session id, use it! */
    gnutls_session_set_data(session, ssl_sessionid, ssl_idsize);

    /* Informational message */
    infof (data, "SSL re-using session ID\n");
  }

  rc = handshake(conn, session, sockindex, TRUE);
  if(rc)
    /* handshake() sets its own error message with failf() */
    return rc;

  /* This function will return the peer's raw certificate (chain) as sent by
     the peer. These certificates are in raw format (DER encoded for
     X.509). In case of a X.509 then a certificate list may be present. The
     first certificate in the list is the peer's certificate, following the
     issuer's certificate, then the issuer's issuer etc. */

  chainp = gnutls_certificate_get_peers(session, &cert_list_size);
  if(!chainp) {
    if(data->set.ssl.verifypeer ||
       data->set.ssl.verifyhost ||
       data->set.ssl.issuercert) {
      failf(data, "failed to get server cert");
      return CURLE_PEER_FAILED_VERIFICATION;
    }
    infof(data, "\t common name: WARNING couldn't obtain\n");
  }

  if(data->set.ssl.verifypeer) {
    /* This function will try to verify the peer's certificate and return its
       status (trusted, invalid etc.). The value of status should be one or
       more of the gnutls_certificate_status_t enumerated elements bitwise
       or'd. To avoid denial of service attacks some default upper limits
       regarding the certificate key size and chain size are set. To override
       them use gnutls_certificate_set_verify_limits(). */

    rc = gnutls_certificate_verify_peers2(session, &verify_status);
    if(rc < 0) {
      failf(data, "server cert verify failed: %d", rc);
      return CURLE_SSL_CONNECT_ERROR;
    }

    /* verify_status is a bitmask of gnutls_certificate_status bits */
    if(verify_status & GNUTLS_CERT_INVALID) {
      if(data->set.ssl.verifypeer) {
        failf(data, "server certificate verification failed. CAfile: %s "
              "CRLfile: %s", data->set.ssl.CAfile?data->set.ssl.CAfile:"none",
              data->set.ssl.CRLfile?data->set.ssl.CRLfile:"none");
        return CURLE_SSL_CACERT;
      }
      else
        infof(data, "\t server certificate verification FAILED\n");
    }
    else
      infof(data, "\t server certificate verification OK\n");
  }
  else
    infof(data, "\t server certificate verification SKIPPED\n");

  /* initialize an X.509 certificate structure. */
  gnutls_x509_crt_init(&x509_cert);

  /* convert the given DER or PEM encoded Certificate to the native
     gnutls_x509_crt_t format */
  gnutls_x509_crt_import(x509_cert, chainp, GNUTLS_X509_FMT_DER);

  if (data->set.ssl.issuercert) {
    gnutls_x509_crt_init(&x509_issuer);
    issuerp = load_file(data->set.ssl.issuercert);
    gnutls_x509_crt_import(x509_issuer, &issuerp, GNUTLS_X509_FMT_PEM);
    rc = gnutls_x509_crt_check_issuer(x509_cert,x509_issuer);
    unload_file(issuerp);
    if (rc <= 0) {
      failf(data, "server certificate issuer check failed (IssuerCert: %s)",
            data->set.ssl.issuercert?data->set.ssl.issuercert:"none");
      return CURLE_SSL_ISSUER_ERROR;
    }
    infof(data,"\t server certificate issuer check OK (Issuer Cert: %s)\n",
          data->set.ssl.issuercert?data->set.ssl.issuercert:"none");
  }

  size=sizeof(certbuf);
  rc = gnutls_x509_crt_get_dn_by_oid(x509_cert, GNUTLS_OID_X520_COMMON_NAME,
                                     0, /* the first and only one */
                                     FALSE,
                                     certbuf,
                                     &size);
  if(rc) {
    infof(data, "error fetching CN from cert:%s\n",
          gnutls_strerror(rc));
  }

  /* This function will check if the given certificate's subject matches the
     given hostname. This is a basic implementation of the matching described
     in RFC2818 (HTTPS), which takes into account wildcards, and the subject
     alternative name PKIX extension. Returns non zero on success, and zero on
     failure. */
  rc = gnutls_x509_crt_check_hostname(x509_cert, conn->host.name);

  if(!rc) {
    if(data->set.ssl.verifyhost > 1) {
      failf(data, "SSL: certificate subject name (%s) does not match "
            "target host name '%s'", certbuf, conn->host.dispname);
      gnutls_x509_crt_deinit(x509_cert);
      return CURLE_PEER_FAILED_VERIFICATION;
    }
    else
      infof(data, "\t common name: %s (does not match '%s')\n",
            certbuf, conn->host.dispname);
  }
  else
    infof(data, "\t common name: %s (matched)\n", certbuf);

  /* Check for time-based validity */
  certclock = gnutls_x509_crt_get_expiration_time(x509_cert);

  if(certclock == (time_t)-1) {
    failf(data, "server cert expiration date verify failed");
    return CURLE_SSL_CONNECT_ERROR;
  }

  if(certclock < time(NULL)) {
    if(data->set.ssl.verifypeer) {
      failf(data, "server certificate expiration date has passed.");
      return CURLE_PEER_FAILED_VERIFICATION;
    }
    else
      infof(data, "\t server certificate expiration date FAILED\n");
  }
  else
    infof(data, "\t server certificate expiration date OK\n");

  certclock = gnutls_x509_crt_get_activation_time(x509_cert);

  if(certclock == (time_t)-1) {
    failf(data, "server cert activation date verify failed");
    return CURLE_SSL_CONNECT_ERROR;
  }

  if(certclock > time(NULL)) {
    if(data->set.ssl.verifypeer) {
      failf(data, "server certificate not activated yet.");
      return CURLE_PEER_FAILED_VERIFICATION;
    }
    else
      infof(data, "\t server certificate activation date FAILED\n");
  }
  else
    infof(data, "\t server certificate activation date OK\n");

  /* Show:

  - ciphers used
  - subject
  - start date
  - expire date
  - common name
  - issuer

  */

  /* public key algorithm's parameters */
  algo = gnutls_x509_crt_get_pk_algorithm(x509_cert, &bits);
  infof(data, "\t certificate public key: %s\n",
        gnutls_pk_algorithm_get_name(algo));

  /* version of the X.509 certificate. */
  infof(data, "\t certificate version: #%d\n",
        gnutls_x509_crt_get_version(x509_cert));


  size = sizeof(certbuf);
  gnutls_x509_crt_get_dn(x509_cert, certbuf, &size);
  infof(data, "\t subject: %s\n", certbuf);

  certclock = gnutls_x509_crt_get_activation_time(x509_cert);
  showtime(data, "start date", certclock);

  certclock = gnutls_x509_crt_get_expiration_time(x509_cert);
  showtime(data, "expire date", certclock);

  size = sizeof(certbuf);
  gnutls_x509_crt_get_issuer_dn(x509_cert, certbuf, &size);
  infof(data, "\t issuer: %s\n", certbuf);

  gnutls_x509_crt_deinit(x509_cert);

  /* compression algorithm (if any) */
  ptr = gnutls_compression_get_name(gnutls_compression_get(session));
  /* the *_get_name() says "NULL" if GNUTLS_COMP_NULL is returned */
  infof(data, "\t compression: %s\n", ptr);

  /* the name of the cipher used. ie 3DES. */
  ptr = gnutls_cipher_get_name(gnutls_cipher_get(session));
  infof(data, "\t cipher: %s\n", ptr);

  /* the MAC algorithms name. ie SHA1 */
  ptr = gnutls_mac_get_name(gnutls_mac_get(session));
  infof(data, "\t MAC: %s\n", ptr);

  conn->ssl[sockindex].state = ssl_connection_complete;

  {
    /* we always unconditionally get the session id here, as even if we
       already got it from the cache and asked to use it in the connection, it
       might've been rejected and then a new one is in use now and we need to
       detect that. */
    void *connect_sessionid;
    size_t connect_idsize;

    /* get the session ID data size */
    gnutls_session_get_data(session, NULL, &connect_idsize);
    connect_sessionid = malloc(connect_idsize); /* get a buffer for it */

    if(connect_sessionid) {
      /* extract session ID to the allocated buffer */
      gnutls_session_get_data(session, connect_sessionid, &connect_idsize);

      if(ssl_sessionid)
        /* there was one before in the cache, so instead of risking that the
           previous one was rejected, we just kill that and store the new */
        Curl_ssl_delsessionid(conn, ssl_sessionid);

      /* store this session id */
      return Curl_ssl_addsessionid(conn, connect_sessionid, connect_idsize);
    }

  }

  return CURLE_OK;
}


/* return number of sent (non-SSL) bytes */
ssize_t Curl_gtls_send(struct connectdata *conn,
                       int sockindex,
                       const void *mem,
                       size_t len)
{
  ssize_t rc = gnutls_record_send(conn->ssl[sockindex].session, mem, len);

  if(rc < 0 ) {
    if(rc == GNUTLS_E_AGAIN)
      return 0; /* EWOULDBLOCK equivalent */
    rc = -1; /* generic error code for send failure */
  }

  return rc;
}

void Curl_gtls_close_all(struct SessionHandle *data)
{
  /* FIX: make the OpenSSL code more generic and use parts of it here */
  (void)data;
}

static void close_one(struct connectdata *conn,
                      int idx)
{
  if(conn->ssl[idx].session) {
    gnutls_bye(conn->ssl[idx].session, GNUTLS_SHUT_RDWR);
    gnutls_deinit(conn->ssl[idx].session);
    conn->ssl[idx].session = NULL;
  }
  if(conn->ssl[idx].cred) {
    gnutls_certificate_free_credentials(conn->ssl[idx].cred);
    conn->ssl[idx].cred = NULL;
  }
}

void Curl_gtls_close(struct connectdata *conn, int sockindex)
{
  close_one(conn, sockindex);
}

/*
 * This function is called to shut down the SSL layer but keep the
 * socket open (CCC - Clear Command Channel)
 */
int Curl_gtls_shutdown(struct connectdata *conn, int sockindex)
{
  ssize_t result;
  int retval = 0;
  struct SessionHandle *data = conn->data;
  int done = 0;
  char buf[120];

  /* This has only been tested on the proftpd server, and the mod_tls code
     sends a close notify alert without waiting for a close notify alert in
     response. Thus we wait for a close notify alert from the server, but
     we do not send one. Let's hope other servers do the same... */

  if(data->set.ftp_ccc == CURLFTPSSL_CCC_ACTIVE)
      gnutls_bye(conn->ssl[sockindex].session, GNUTLS_SHUT_WR);

  if(conn->ssl[sockindex].session) {
    while(!done) {
      int what = Curl_socket_ready(conn->sock[sockindex],
                             CURL_SOCKET_BAD, SSL_SHUTDOWN_TIMEOUT);
      if(what > 0) {
        /* Something to read, let's do it and hope that it is the close
           notify alert from the server */
        result = gnutls_record_recv(conn->ssl[sockindex].session,
                                    buf, sizeof(buf));
        switch(result) {
        case 0:
          /* This is the expected response. There was no data but only
             the close notify alert */
          done = 1;
          break;
        case GNUTLS_E_AGAIN:
        case GNUTLS_E_INTERRUPTED:
          infof(data, "GNUTLS_E_AGAIN || GNUTLS_E_INTERRUPTED\n");
          break;
        default:
          retval = -1;
          done = 1;
          break;
        }
      }
      else if(0 == what) {
        /* timeout */
        failf(data, "SSL shutdown timeout");
        done = 1;
        break;
      }
      else {
        /* anything that gets here is fatally bad */
        failf(data, "select/poll on SSL socket, errno: %d", SOCKERRNO);
        retval = -1;
        done = 1;
      }
    }
    gnutls_deinit(conn->ssl[sockindex].session);
  }
  gnutls_certificate_free_credentials(conn->ssl[sockindex].cred);

  conn->ssl[sockindex].cred = NULL;
  conn->ssl[sockindex].session = NULL;

  return retval;
}

/*
 * If the read would block we return -1 and set 'wouldblock' to TRUE.
 * Otherwise we return the amount of data read. Other errors should return -1
 * and set 'wouldblock' to FALSE.
 */
ssize_t Curl_gtls_recv(struct connectdata *conn, /* connection data */
                       int num,                  /* socketindex */
                       char *buf,                /* store read data here */
                       size_t buffersize,        /* max amount to read */
                       bool *wouldblock)
{
  ssize_t ret;

  ret = gnutls_record_recv(conn->ssl[num].session, buf, buffersize);
  if((ret == GNUTLS_E_AGAIN) || (ret == GNUTLS_E_INTERRUPTED)) {
    *wouldblock = TRUE;
    return -1;
  }

  if(ret == GNUTLS_E_REHANDSHAKE) {
    /* BLOCKING call, this is bad but a work-around for now. Fixing this "the
       proper way" takes a whole lot of work. */
    CURLcode rc = handshake(conn, conn->ssl[num].session, num, FALSE);
    if(rc)
      /* handshake() writes error message on its own */
      return rc;
    *wouldblock = TRUE; /* then return as if this was a wouldblock */
    return -1;
  }

  *wouldblock = FALSE;
  if(!ret) {
    failf(conn->data, "Peer closed the TLS connection");
    return -1;
  }

  if(ret < 0) {
    failf(conn->data, "GnuTLS recv error (%d): %s",
          (int)ret, gnutls_strerror((int)ret));
    return -1;
  }

  return ret;
}

void Curl_gtls_session_free(void *ptr)
{
  free(ptr);
}

size_t Curl_gtls_version(char *buffer, size_t size)
{
  return snprintf(buffer, size, "GnuTLS/%s", gnutls_check_version(NULL));
}

int Curl_gtls_seed(struct SessionHandle *data)
{
  /* we have the "SSL is seeded" boolean static to prevent multiple
     time-consuming seedings in vain */
  static bool ssl_seeded = FALSE;

  /* Quickly add a bit of entropy */
  gcry_fast_random_poll();

  if(!ssl_seeded || data->set.str[STRING_SSL_RANDOM_FILE] ||
     data->set.str[STRING_SSL_EGDSOCKET]) {

    /* TODO: to a good job seeding the RNG
       This may involve the gcry_control function and these options:
       GCRYCTL_SET_RANDOM_SEED_FILE
       GCRYCTL_SET_RNDEGD_SOCKET
    */
    ssl_seeded = TRUE;
  }
  return 0;
}

#endif /* USE_GNUTLS */
