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
 * $Id: sslgen.c,v 1.48 2009-05-04 21:57:14 bagder Exp $
 ***************************************************************************/

/* This file is for implementing all "generic" SSL functions that all libcurl
   internals should use. It is then responsible for calling the proper
   "backend" function.

   SSL-functions in libcurl should call functions in this source file, and not
   to any specific SSL-layer.

   Curl_ssl_ - prefix for generic ones
   Curl_ossl_ - prefix for OpenSSL ones
   Curl_gtls_ - prefix for GnuTLS ones
   Curl_nss_ - prefix for NSS ones

   Note that this source code uses curlssl_* functions, and they are all
   defines/macros #defined by the lib-specific header files.

   "SSL/TLS Strong Encryption: An Introduction"
   http://httpd.apache.org/docs-2.0/ssl/ssl_intro.html
*/

#include "setup.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include "urldata.h"
#define SSLGEN_C
#include "sslgen.h" /* generic SSL protos etc */
#include "ssluse.h" /* OpenSSL versions */
#include "gtls.h"   /* GnuTLS versions */
#include "nssg.h"   /* NSS versions */
#include "qssl.h"   /* QSOSSL versions */
#include "sendf.h"
#include "rawstr.h"
#include "url.h"
#include "curl_memory.h"
#include "progress.h"
/* The last #include file should be: */
#include "memdebug.h"

static bool safe_strequal(char* str1, char* str2)
{
  if(str1 && str2)
    /* both pointers point to something then compare them */
    return (bool)(0 != Curl_raw_equal(str1, str2));
  else
    /* if both pointers are NULL then treat them as equal */
    return (bool)(!str1 && !str2);
}

bool
Curl_ssl_config_matches(struct ssl_config_data* data,
                        struct ssl_config_data* needle)
{
  if((data->version == needle->version) &&
     (data->verifypeer == needle->verifypeer) &&
     (data->verifyhost == needle->verifyhost) &&
     safe_strequal(data->CApath, needle->CApath) &&
     safe_strequal(data->CAfile, needle->CAfile) &&
     safe_strequal(data->random_file, needle->random_file) &&
     safe_strequal(data->egdsocket, needle->egdsocket) &&
     safe_strequal(data->cipher_list, needle->cipher_list))
    return TRUE;

  return FALSE;
}

bool
Curl_clone_ssl_config(struct ssl_config_data *source,
                      struct ssl_config_data *dest)
{
  dest->sessionid = source->sessionid;
  dest->verifyhost = source->verifyhost;
  dest->verifypeer = source->verifypeer;
  dest->version = source->version;

  if(source->CAfile) {
    dest->CAfile = strdup(source->CAfile);
    if(!dest->CAfile)
      return FALSE;
  }

  if(source->CApath) {
    dest->CApath = strdup(source->CApath);
    if(!dest->CApath)
      return FALSE;
  }

  if(source->cipher_list) {
    dest->cipher_list = strdup(source->cipher_list);
    if(!dest->cipher_list)
      return FALSE;
  }

  if(source->egdsocket) {
    dest->egdsocket = strdup(source->egdsocket);
    if(!dest->egdsocket)
      return FALSE;
  }

  if(source->random_file) {
    dest->random_file = strdup(source->random_file);
    if(!dest->random_file)
      return FALSE;
  }

  return TRUE;
}

void Curl_free_ssl_config(struct ssl_config_data* sslc)
{
  Curl_safefree(sslc->CAfile);
  Curl_safefree(sslc->CApath);
  Curl_safefree(sslc->cipher_list);
  Curl_safefree(sslc->egdsocket);
  Curl_safefree(sslc->random_file);
}

#ifdef USE_SSL

/* "global" init done? */
static bool init_ssl=FALSE;

/**
 * Global SSL init
 *
 * @retval 0 error initializing SSL
 * @retval 1 SSL initialized successfully
 */
int Curl_ssl_init(void)
{
  /* make sure this is only done once */
  if(init_ssl)
    return 1;
  init_ssl = TRUE; /* never again */

  return curlssl_init();
}


/* Global cleanup */
void Curl_ssl_cleanup(void)
{
  if(init_ssl) {
    /* only cleanup if we did a previous init */
    curlssl_cleanup();
    init_ssl = FALSE;
  }
}

CURLcode
Curl_ssl_connect(struct connectdata *conn, int sockindex)
{
  CURLcode res;
  /* mark this is being ssl-enabled from here on. */
  conn->ssl[sockindex].use = TRUE;
  conn->ssl[sockindex].state = ssl_connection_negotiating;

  res = curlssl_connect(conn, sockindex);

  if(!res)
    Curl_pgrsTime(conn->data, TIMER_APPCONNECT); /* SSL is connected */

  return res;
}

CURLcode
Curl_ssl_connect_nonblocking(struct connectdata *conn, int sockindex,
                             bool *done)
{
#ifdef curlssl_connect_nonblocking
  CURLcode res;
  /* mark this is being ssl requested from here on. */
  conn->ssl[sockindex].use = TRUE;
  res = curlssl_connect_nonblocking(conn, sockindex, done);
  if(!res && *done == TRUE)
    Curl_pgrsTime(conn->data, TIMER_APPCONNECT); /* SSL is connected */
  return res;
#else
  *done = TRUE; /* fallback to BLOCKING */
  conn->ssl[sockindex].use = TRUE;
  return curlssl_connect(conn, sockindex);
#endif /* non-blocking connect support */
}

/*
 * Check if there's a session ID for the given connection in the cache, and if
 * there's one suitable, it is provided. Returns TRUE when no entry matched.
 */
int Curl_ssl_getsessionid(struct connectdata *conn,
                          void **ssl_sessionid,
                          size_t *idsize) /* set 0 if unknown */
{
  struct curl_ssl_session *check;
  struct SessionHandle *data = conn->data;
  long i;

  if(!conn->ssl_config.sessionid)
    /* session ID re-use is disabled */
    return TRUE;

  for(i=0; i< data->set.ssl.numsessions; i++) {
    check = &data->state.session[i];
    if(!check->sessionid)
      /* not session ID means blank entry */
      continue;
    if(Curl_raw_equal(conn->host.name, check->name) &&
       (conn->remote_port == check->remote_port) &&
       Curl_ssl_config_matches(&conn->ssl_config, &check->ssl_config)) {
      /* yes, we have a session ID! */
      data->state.sessionage++;            /* increase general age */
      check->age = data->state.sessionage; /* set this as used in this age */
      *ssl_sessionid = check->sessionid;
      if(idsize)
        *idsize = check->idsize;
      return FALSE;
    }
  }
  *ssl_sessionid = NULL;
  return TRUE;
}

/*
 * Kill a single session ID entry in the cache.
 */
static int kill_session(struct curl_ssl_session *session)
{
  if(session->sessionid) {
    /* defensive check */

    /* free the ID the SSL-layer specific way */
    curlssl_session_free(session->sessionid);

    session->sessionid=NULL;
    session->age = 0; /* fresh */

    Curl_free_ssl_config(&session->ssl_config);

    Curl_safefree(session->name);
    session->name = NULL; /* no name */

    return 0; /* ok */
  }
  else
    return 1;
}

/*
 * Delete the given session ID from the cache.
 */
void Curl_ssl_delsessionid(struct connectdata *conn, void *ssl_sessionid)
{
  int i;
  for(i=0; i< conn->data->set.ssl.numsessions; i++) {
    struct curl_ssl_session *check = &conn->data->state.session[i];

    if (check->sessionid == ssl_sessionid) {
      kill_session(check);
      break;
    }
  }
}

/*
 * Store session id in the session cache. The ID passed on to this function
 * must already have been extracted and allocated the proper way for the SSL
 * layer. Curl_XXXX_session_free() will be called to free/kill the session ID
 * later on.
 */
CURLcode Curl_ssl_addsessionid(struct connectdata *conn,
                               void *ssl_sessionid,
                               size_t idsize)
{
  long i;
  struct SessionHandle *data=conn->data; /* the mother of all structs */
  struct curl_ssl_session *store = &data->state.session[0];
  long oldest_age=data->state.session[0].age; /* zero if unused */
  char *clone_host;

  /* Even though session ID re-use might be disabled, that only disables USING
     IT. We still store it here in case the re-using is again enabled for an
     upcoming transfer */

  clone_host = strdup(conn->host.name);
  if(!clone_host)
    return CURLE_OUT_OF_MEMORY; /* bail out */

  /* Now we should add the session ID and the host name to the cache, (remove
     the oldest if necessary) */

  /* find an empty slot for us, or find the oldest */
  for(i=1; (i<data->set.ssl.numsessions) &&
        data->state.session[i].sessionid; i++) {
    if(data->state.session[i].age < oldest_age) {
      oldest_age = data->state.session[i].age;
      store = &data->state.session[i];
    }
  }
  if(i == data->set.ssl.numsessions)
    /* cache is full, we must "kill" the oldest entry! */
    kill_session(store);
  else
    store = &data->state.session[i]; /* use this slot */

  /* now init the session struct wisely */
  store->sessionid = ssl_sessionid;
  store->idsize = idsize;
  store->age = data->state.sessionage;    /* set current age */
  if (store->name)
    /* free it if there's one already present */
    free(store->name);
  store->name = clone_host;               /* clone host name */
  store->remote_port = conn->remote_port; /* port number */

  if(!Curl_clone_ssl_config(&conn->ssl_config, &store->ssl_config))
    return CURLE_OUT_OF_MEMORY;

  return CURLE_OK;
}


void Curl_ssl_close_all(struct SessionHandle *data)
{
  long i;
  /* kill the session ID cache */
  if(data->state.session) {
    for(i=0; i< data->set.ssl.numsessions; i++)
      /* the single-killer function handles empty table slots */
      kill_session(&data->state.session[i]);

    /* free the cache data */
    free(data->state.session);
    data->state.session = NULL;
  }

  curlssl_close_all(data);
}

void Curl_ssl_close(struct connectdata *conn, int sockindex)
{
  DEBUGASSERT((sockindex <= 1) && (sockindex >= -1));
  curlssl_close(conn, sockindex);
}

CURLcode Curl_ssl_shutdown(struct connectdata *conn, int sockindex)
{
  if(curlssl_shutdown(conn, sockindex))
    return CURLE_SSL_SHUTDOWN_FAILED;

  conn->ssl[sockindex].use = FALSE; /* get back to ordinary socket usage */
  conn->ssl[sockindex].state = ssl_connection_none;

  return CURLE_OK;
}

/* Selects an SSL crypto engine
 */
CURLcode Curl_ssl_set_engine(struct SessionHandle *data, const char *engine)
{
  return curlssl_set_engine(data, engine);
}

/* Selects the default SSL crypto engine
 */
CURLcode Curl_ssl_set_engine_default(struct SessionHandle *data)
{
  return curlssl_set_engine_default(data);
}

/* Return list of OpenSSL crypto engine names. */
struct curl_slist *Curl_ssl_engines_list(struct SessionHandle *data)
{
  return curlssl_engines_list(data);
}

/* return number of sent (non-SSL) bytes */
ssize_t Curl_ssl_send(struct connectdata *conn,
                      int sockindex,
                      const void *mem,
                      size_t len)
{
  return curlssl_send(conn, sockindex, mem, len);
}

/* return number of received (decrypted) bytes */

/*
 * If the read would block (EWOULDBLOCK) we return -1. Otherwise we return
 * a regular CURLcode value.
 */
ssize_t Curl_ssl_recv(struct connectdata *conn, /* connection data */
                      int sockindex,            /* socketindex */
                      char *mem,                /* store read data here */
                      size_t len)               /* max amount to read */
{
  ssize_t nread;
  bool block = FALSE;

  nread = curlssl_recv(conn, sockindex, mem, len, &block);
  if(nread == -1) {
    if(!block)
      return 0; /* this is a true error, not EWOULDBLOCK */
    else
      return -1;
  }

  return nread;
}


/*
 * This sets up a session ID cache to the specified size. Make sure this code
 * is agnostic to what underlying SSL technology we use.
 */
CURLcode Curl_ssl_initsessions(struct SessionHandle *data, long amount)
{
  struct curl_ssl_session *session;

  if(data->state.session)
    /* this is just a precaution to prevent multiple inits */
    return CURLE_OK;

  session = calloc(sizeof(struct curl_ssl_session), amount);
  if(!session)
    return CURLE_OUT_OF_MEMORY;

  /* store the info in the SSL section */
  data->set.ssl.numsessions = amount;
  data->state.session = session;
  data->state.sessionage = 1; /* this is brand new */
  return CURLE_OK;
}

size_t Curl_ssl_version(char *buffer, size_t size)
{
  return curlssl_version(buffer, size);
}

/*
 * This function tries to determine connection status.
 *
 * Return codes:
 *     1 means the connection is still in place
 *     0 means the connection has been closed
 *    -1 means the connection status is unknown
 */
int Curl_ssl_check_cxn(struct connectdata *conn)
{
  return curlssl_check_cxn(conn);
}

bool Curl_ssl_data_pending(const struct connectdata *conn,
                           int connindex)
{
  return curlssl_data_pending(conn, connindex);
}

void Curl_ssl_free_certinfo(struct SessionHandle *data)
{
  int i;
  struct curl_certinfo *ci = &data->info.certs;
  if(ci->num_of_certs) {
    /* free all individual lists used */
    for(i=0; i<ci->num_of_certs; i++)
      curl_slist_free_all(ci->certinfo[i]);
    free(ci->certinfo); /* free the actual array too */
    ci->num_of_certs = 0;
  }
}
#endif /* USE_SSL */
