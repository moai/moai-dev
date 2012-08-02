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
 * $Id: ftp.c,v 1.529 2009-09-17 16:11:54 yangtse Exp $
 ***************************************************************************/

#include "setup.h"

#ifndef CURL_DISABLE_FTP
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_UTSNAME_H
#include <sys/utsname.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef  VMS
#include <in.h>
#include <inet.h>
#endif

#if (defined(NETWARE) && defined(__NOVELL_LIBC__))
#undef in_addr_t
#define in_addr_t unsigned long
#endif

#include <curl/curl.h>
#include "urldata.h"
#include "sendf.h"
#include "easyif.h" /* for Curl_convert_... prototypes */

#include "if2ip.h"
#include "hostip.h"
#include "progress.h"
#include "transfer.h"
#include "escape.h"
#include "http.h" /* for HTTP proxy tunnel stuff */
#include "socks.h"
#include "ftp.h"

#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
#include "krb4.h"
#endif

#include "strtoofft.h"
#include "strequal.h"
#include "sslgen.h"
#include "connect.h"
#include "strerror.h"
#include "inet_ntop.h"
#include "inet_pton.h"
#include "select.h"
#include "parsedate.h" /* for the week day and month names */
#include "sockaddr.h" /* required for Curl_sockaddr_storage */
#include "multiif.h"
#include "url.h"
#include "rawstr.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>

#include "curl_memory.h"
/* The last #include file should be: */
#include "memdebug.h"

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#ifdef __SYMBIAN32__
/* Symbian OS panics when given a timeout much greater than 1/2 hour */
#define RESP_TIMEOUT (1800*1000)
#else
/* Default response timeout in milliseconds */
#define RESP_TIMEOUT (3600*1000)
#endif

#ifdef CURL_DISABLE_VERBOSE_STRINGS
#define ftp_pasv_verbose(a,b,c,d)  do { } while(0)
#endif

/* Local API functions */
static CURLcode ftp_sendquote(struct connectdata *conn,
                              struct curl_slist *quote);
static CURLcode ftp_quit(struct connectdata *conn);
static CURLcode ftp_parse_url_path(struct connectdata *conn);
static CURLcode ftp_regular_transfer(struct connectdata *conn, bool *done);
#ifndef CURL_DISABLE_VERBOSE_STRINGS
static void ftp_pasv_verbose(struct connectdata *conn,
                             Curl_addrinfo *ai,
                             char *newhost, /* ascii version */
                             int port);
#endif
static CURLcode ftp_state_post_rest(struct connectdata *conn);
static CURLcode ftp_state_post_cwd(struct connectdata *conn);
static CURLcode ftp_state_quote(struct connectdata *conn,
                                bool init, ftpstate instate);
static CURLcode ftp_nb_type(struct connectdata *conn,
                            bool ascii, ftpstate newstate);
static int ftp_need_type(struct connectdata *conn,
                         bool ascii);
static CURLcode ftp_do(struct connectdata *conn, bool *done);
static CURLcode ftp_done(struct connectdata *conn,
                              CURLcode, bool premature);
static CURLcode ftp_connect(struct connectdata *conn, bool *done);
static CURLcode ftp_disconnect(struct connectdata *conn);
static CURLcode ftp_nextconnect(struct connectdata *conn);
static CURLcode ftp_multi_statemach(struct connectdata *conn, bool *done);
static int ftp_getsock(struct connectdata *conn,
                            curl_socket_t *socks,
                            int numsocks);
static CURLcode ftp_doing(struct connectdata *conn,
                               bool *dophase_done);
static CURLcode ftp_setup_connection(struct connectdata * conn);

/* easy-to-use macro: */
#define FTPSENDF(x,y,z)    if((result = Curl_ftpsendf(x,y,z)) != CURLE_OK) \
                              return result
#define NBFTPSENDF(x,y,z)  if((result = Curl_nbftpsendf(x,y,z)) != CURLE_OK) \
                              return result


/*
 * FTP protocol handler.
 */

const struct Curl_handler Curl_handler_ftp = {
  "FTP",                                /* scheme */
  ftp_setup_connection,            /* setup_connection */
  ftp_do,                          /* do_it */
  ftp_done,                        /* done */
  ftp_nextconnect,                 /* do_more */
  ftp_connect,                     /* connect_it */
  ftp_multi_statemach,             /* connecting */
  ftp_doing,                       /* doing */
  ftp_getsock,                     /* proto_getsock */
  ftp_getsock,                     /* doing_getsock */
  ZERO_NULL,                       /* perform_getsock */
  ftp_disconnect,                  /* disconnect */
  PORT_FTP,                        /* defport */
  PROT_FTP                         /* protocol */
};


#ifdef USE_SSL
/*
 * FTPS protocol handler.
 */

const struct Curl_handler Curl_handler_ftps = {
  "FTPS",                               /* scheme */
  ftp_setup_connection,            /* setup_connection */
  ftp_do,                          /* do_it */
  ftp_done,                        /* done */
  ftp_nextconnect,                 /* do_more */
  ftp_connect,                     /* connect_it */
  ftp_multi_statemach,             /* connecting */
  ftp_doing,                       /* doing */
  ftp_getsock,                     /* proto_getsock */
  ftp_getsock,                     /* doing_getsock */
  ZERO_NULL,                       /* perform_getsock */
  ftp_disconnect,                  /* disconnect */
  PORT_FTPS,                       /* defport */
  PROT_FTP | PROT_FTPS | PROT_SSL  /* protocol */
};
#endif

#ifndef CURL_DISABLE_HTTP
/*
 * HTTP-proxyed FTP protocol handler.
 */

const struct Curl_handler Curl_handler_ftp_proxy = {
  "FTP",                                /* scheme */
  ZERO_NULL,                            /* setup_connection */
  Curl_http,                            /* do_it */
  Curl_http_done,                       /* done */
  ZERO_NULL,                            /* do_more */
  ZERO_NULL,                            /* connect_it */
  ZERO_NULL,                            /* connecting */
  ZERO_NULL,                            /* doing */
  ZERO_NULL,                            /* proto_getsock */
  ZERO_NULL,                            /* doing_getsock */
  ZERO_NULL,                            /* perform_getsock */
  ZERO_NULL,                            /* disconnect */
  PORT_FTP,                             /* defport */
  PROT_HTTP                             /* protocol */
};


#ifdef USE_SSL
/*
 * HTTP-proxyed FTPS protocol handler.
 */

const struct Curl_handler Curl_handler_ftps_proxy = {
  "FTPS",                               /* scheme */
  ZERO_NULL,                            /* setup_connection */
  Curl_http,                            /* do_it */
  Curl_http_done,                       /* done */
  ZERO_NULL,                            /* do_more */
  ZERO_NULL,                            /* connect_it */
  ZERO_NULL,                            /* connecting */
  ZERO_NULL,                            /* doing */
  ZERO_NULL,                            /* proto_getsock */
  ZERO_NULL,                            /* doing_getsock */
  ZERO_NULL,                            /* perform_getsock */
  ZERO_NULL,                            /* disconnect */
  PORT_FTPS,                            /* defport */
  PROT_HTTP                             /* protocol */
};
#endif
#endif


/*
 * NOTE: back in the old days, we added code in the FTP code that made NOBODY
 * requests on files respond with headers passed to the client/stdout that
 * looked like HTTP ones.
 *
 * This approach is not very elegant, it causes confusion and is error-prone.
 * It is subject for removal at the next (or at least a future) soname bump.
 * Until then you can test the effects of the removal by undefining the
 * following define named CURL_FTP_HTTPSTYLE_HEAD.
 */
#define CURL_FTP_HTTPSTYLE_HEAD 1

static void freedirs(struct ftp_conn *ftpc)
{
  int i;
  if(ftpc->dirs) {
    for (i=0; i < ftpc->dirdepth; i++){
      if(ftpc->dirs[i]) {
        free(ftpc->dirs[i]);
        ftpc->dirs[i]=NULL;
      }
    }
    free(ftpc->dirs);
    ftpc->dirs = NULL;
    ftpc->dirdepth = 0;
  }
  if(ftpc->file) {
    free(ftpc->file);
    ftpc->file = NULL;
  }
}

/* Returns non-zero if the given string contains CR (\r) or LF (\n),
   which are not allowed within RFC 959 <string>.
   Note: The input string is in the client's encoding which might
   not be ASCII, so escape sequences \r & \n must be used instead
   of hex values 0x0d & 0x0a.
*/
static bool isBadFtpString(const char *string)
{
  return (bool)((NULL != strchr(string, '\r')) ||
                (NULL != strchr(string, '\n')));
}

/***********************************************************************
 *
 * AllowServerConnect()
 *
 * When we've issue the PORT command, we have told the server to connect
 * to us. This function will sit and wait here until the server has
 * connected.
 *
 */
static CURLcode AllowServerConnect(struct connectdata *conn)
{
  struct SessionHandle *data = conn->data;
  curl_socket_t sock = conn->sock[SECONDARYSOCKET];
  long timeout_ms = Curl_timeleft(conn, NULL, TRUE);

  if(timeout_ms < 0) {
    /* if a timeout was already reached, bail out */
    failf(data, "Timed out before server could connect to us");
    return CURLE_OPERATION_TIMEDOUT;
  }

  switch (Curl_socket_ready(sock, CURL_SOCKET_BAD, (int)timeout_ms)) {
  case -1: /* error */
    /* let's die here */
    failf(data, "Error while waiting for server connect");
    return CURLE_FTP_PORT_FAILED;
  case 0:  /* timeout */
    /* let's die here */
    failf(data, "Timeout while waiting for server connect");
    return CURLE_FTP_PORT_FAILED;
  default:
    /* we have received data here */
    {
      curl_socket_t s = CURL_SOCKET_BAD;
#ifdef ENABLE_IPV6
      struct Curl_sockaddr_storage add;
#else
      struct sockaddr_in add;
#endif
      curl_socklen_t size = (curl_socklen_t) sizeof(add);

      if(0 == getsockname(sock, (struct sockaddr *) &add, &size)) {
        size = sizeof(add);

        s=accept(sock, (struct sockaddr *) &add, &size);
      }
      sclose(sock); /* close the first socket */

      if(CURL_SOCKET_BAD == s) {
        /* DIE! */
        failf(data, "Error accept()ing server connect");
        return CURLE_FTP_PORT_FAILED;
      }
      infof(data, "Connection accepted from server\n");

      conn->sock[SECONDARYSOCKET] = s;
      curlx_nonblock(s, TRUE); /* enable non-blocking */
    }
    break;
  }

  return CURLE_OK;
}

/* initialize stuff to prepare for reading a fresh new response */
static void ftp_respinit(struct connectdata *conn)
{
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  ftpc->nread_resp = 0;
  ftpc->linestart_resp = conn->data->state.buffer;
  ftpc->pending_resp = TRUE;
}

/* macro to check for a three-digit ftp status code at the start of the
   given string */
#define STATUSCODE(line) (ISDIGIT(line[0]) && ISDIGIT(line[1]) && \
                        ISDIGIT(line[2]))

/* macro to check for the last line in an FTP server response */
#define LASTLINE(line) (STATUSCODE(line) && (' ' == line[3]))

static CURLcode ftp_readresp(curl_socket_t sockfd,
                             struct connectdata *conn,
                             int *ftpcode, /* return the ftp-code if done */
                             size_t *size) /* size of the response */
{
  ssize_t perline; /* count bytes per line */
  bool keepon=TRUE;
  ssize_t gotbytes;
  char *ptr;
  struct SessionHandle *data = conn->data;
  char * const buf = data->state.buffer;
  CURLcode result = CURLE_OK;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  int code = 0;

  *ftpcode = 0; /* 0 for errors or not done */
  *size = 0;

  ptr=buf + ftpc->nread_resp;

  /* number of bytes in the current line, so far */
  perline = (ssize_t)(ptr-ftpc->linestart_resp);

  keepon=TRUE;

  while((ftpc->nread_resp<BUFSIZE) && (keepon && !result)) {

    if(ftpc->cache) {
      /* we had data in the "cache", copy that instead of doing an actual
       * read
       *
       * ftp->cache_size is cast to int here.  This should be safe,
       * because it would have been populated with something of size
       * int to begin with, even though its datatype may be larger
       * than an int.
       */
      DEBUGASSERT((ptr+ftpc->cache_size) <= (buf+BUFSIZE+1));
      memcpy(ptr, ftpc->cache, (int)ftpc->cache_size);
      gotbytes = (int)ftpc->cache_size;
      free(ftpc->cache);    /* free the cache */
      ftpc->cache = NULL;   /* clear the pointer */
      ftpc->cache_size = 0; /* zero the size just in case */
    }
    else {
      int res;
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
      enum protection_level prot = conn->data_prot;

      conn->data_prot = 0;
#endif
      DEBUGASSERT((ptr+BUFSIZE-ftpc->nread_resp) <= (buf+BUFSIZE+1));
      res = Curl_read(conn, sockfd, ptr, BUFSIZE-ftpc->nread_resp,
                      &gotbytes);
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
      conn->data_prot = prot;
#endif
      if(res < 0)
        /* EWOULDBLOCK */
        return CURLE_OK; /* return */

#ifdef CURL_DOES_CONVERSIONS
      if((res == CURLE_OK) && (gotbytes > 0)) {
        /* convert from the network encoding */
        res = Curl_convert_from_network(data, ptr, gotbytes);
        /* Curl_convert_from_network calls failf if unsuccessful */
      }
#endif /* CURL_DOES_CONVERSIONS */

      if(CURLE_OK != res) {
        result = (CURLcode)res; /* Set outer result variable to this error. */
        keepon = FALSE;
      }
    }

    if(!keepon)
      ;
    else if(gotbytes <= 0) {
      keepon = FALSE;
      result = CURLE_RECV_ERROR;
      failf(data, "FTP response reading failed");
    }
    else {
      /* we got a whole chunk of data, which can be anything from one
       * byte to a set of lines and possible just a piece of the last
       * line */
      ssize_t i;
      ssize_t clipamount = 0;
      bool restart = FALSE;

      data->req.headerbytecount += gotbytes;

      ftpc->nread_resp += gotbytes;
      for(i = 0; i < gotbytes; ptr++, i++) {
        perline++;
        if(*ptr=='\n') {
          /* a newline is CRLF in ftp-talk, so the CR is ignored as
             the line isn't really terminated until the LF comes */

          /* output debug output if that is requested */
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
          if(!conn->sec_complete)
#endif
          if(data->set.verbose)
            Curl_debug(data, CURLINFO_HEADER_IN,
                       ftpc->linestart_resp, (size_t)perline, conn);

          /*
           * We pass all response-lines to the callback function registered
           * for "headers". The response lines can be seen as a kind of
           * headers.
           */
          result = Curl_client_write(conn, CLIENTWRITE_HEADER,
                                     ftpc->linestart_resp, perline);
          if(result)
            return result;

          if(perline>3 && LASTLINE(ftpc->linestart_resp)) {
            /* This is the end of the last line, copy the last line to the
               start of the buffer and zero terminate, for old times sake (and
               krb4)! */
            char *meow;
            int n;
            for(meow=ftpc->linestart_resp, n=0; meow<ptr; meow++, n++)
              buf[n] = *meow;
            *meow=0; /* zero terminate */
            keepon=FALSE;
            ftpc->linestart_resp = ptr+1; /* advance pointer */
            i++; /* skip this before getting out */

            *size = ftpc->nread_resp; /* size of the response */
            ftpc->nread_resp = 0; /* restart */
            break;
          }
          perline=0; /* line starts over here */
          ftpc->linestart_resp = ptr+1;
        }
      }

      if(!keepon && (i != gotbytes)) {
        /* We found the end of the response lines, but we didn't parse the
           full chunk of data we have read from the server. We therefore need
           to store the rest of the data to be checked on the next invoke as
           it may actually contain another end of response already! */
        clipamount = gotbytes - i;
        restart = TRUE;
      }
      else if(keepon) {

        if((perline == gotbytes) && (gotbytes > BUFSIZE/2)) {
          /* We got an excessive line without newlines and we need to deal
             with it. First, check if it seems to start with a valid status
             code and then we keep just that in the line cache. Then throw
             away the rest. */
          infof(data, "Excessive FTP response line length received, %zd bytes."
                " Stripping\n", gotbytes);
          restart = TRUE;
          if(STATUSCODE(ftpc->linestart_resp))
            /* we copy 4 bytes since after the three-digit number there is a
               dash or a space and it is significant */
            clipamount = 4;
        }
        else if(ftpc->nread_resp > BUFSIZE/2) {
          /* We got a large chunk of data and there's potentially still trailing
             data to take care of, so we put any such part in the "cache", clear
             the buffer to make space and restart. */
          clipamount = perline;
          restart = TRUE;
        }
      }
      else if(i == gotbytes)
        restart = TRUE;

      if(clipamount) {
        ftpc->cache_size = clipamount;
        ftpc->cache = malloc((int)ftpc->cache_size);
        if(ftpc->cache)
          memcpy(ftpc->cache, ftpc->linestart_resp, (int)ftpc->cache_size);
        else
          return CURLE_OUT_OF_MEMORY;
      }
      if(restart) {
        /* now reset a few variables to start over nicely from the start of
           the big buffer */
        ftpc->nread_resp = 0; /* start over from scratch in the buffer */
        ptr = ftpc->linestart_resp = buf;
        perline = 0;
      }

    } /* there was data */

  } /* while there's buffer left and loop is requested */

  if(!result)
    code = atoi(buf);

#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  /* handle the security-oriented responses 6xx ***/
  /* FIXME: some errorchecking perhaps... ***/
  switch(code) {
  case 631:
    code = Curl_sec_read_msg(conn, buf, prot_safe);
    break;
  case 632:
    code = Curl_sec_read_msg(conn, buf, prot_private);
    break;
  case 633:
    code = Curl_sec_read_msg(conn, buf, prot_confidential);
    break;
  default:
    /* normal ftp stuff we pass through! */
    break;
  }
#endif

  *ftpcode=code; /* return the initial number like this */

  /* store the latest code for later retrieval */
  conn->data->info.httpcode=code;

  ftpc->pending_resp = FALSE;

  return result;
}

/* --- parse FTP server responses --- */

/*
 * Curl_GetFTPResponse() is a BLOCKING function to read the full response
 * from a server after a command.
 *
 */

CURLcode Curl_GetFTPResponse(ssize_t *nreadp, /* return number of bytes read */
                             struct connectdata *conn,
                             int *ftpcode) /* return the ftp-code */
{
  /*
   * We cannot read just one byte per read() and then go back to select() as
   * the OpenSSL read() doesn't grok that properly.
   *
   * Alas, read as much as possible, split up into lines, use the ending
   * line in a response or continue reading.  */

  curl_socket_t sockfd = conn->sock[FIRSTSOCKET];
  long timeout;              /* timeout in milliseconds */
  long interval_ms;
  struct SessionHandle *data = conn->data;
  CURLcode result = CURLE_OK;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  struct timeval now = Curl_tvnow();
  size_t nread;
  int cache_skip=0;
  int value_to_be_ignored=0;

  if(ftpcode)
    *ftpcode = 0; /* 0 for errors */
  else
    /* make the pointer point to something for the rest of this function */
    ftpcode = &value_to_be_ignored;

  *nreadp=0;

  while(!*ftpcode && !result) {
    /* check and reset timeout value every lap */
    if(data->set.ftp_response_timeout )
      /* if CURLOPT_FTP_RESPONSE_TIMEOUT is set, use that to determine
         remaining time.  Also, use "now" as opposed to "conn->now"
         because ftp_response_timeout is only supposed to govern
         the response for any given ftp response, not for the time
         from connect to the given ftp response. */
      timeout = data->set.ftp_response_timeout - /* timeout time */
        Curl_tvdiff(Curl_tvnow(), now); /* spent time */
    else if(data->set.timeout)
      /* if timeout is requested, find out how much remaining time we have */
      timeout = data->set.timeout - /* timeout time */
        Curl_tvdiff(Curl_tvnow(), conn->now); /* spent time */
    else
      /* Even without a requested timeout, we only wait response_time
         seconds for the full response to arrive before we bail out */
      timeout = ftpc->response_time -
        Curl_tvdiff(Curl_tvnow(), now); /* spent time */

    if(timeout <=0 ) {
      failf(data, "FTP response timeout");
      return CURLE_OPERATION_TIMEDOUT; /* already too little time */
    }

    interval_ms = 1 * 1000;  /* use 1 second timeout intervals */
    if(timeout < interval_ms)
      interval_ms = timeout;

    /*
     * Since this function is blocking, we need to wait here for input on the
     * connection and only then we call the response reading function. We do
     * timeout at least every second to make the timeout check run.
     *
     * A caution here is that the ftp_readresp() function has a cache that may
     * contain pieces of a response from the previous invoke and we need to
     * make sure we don't just wait for input while there is unhandled data in
     * that cache. But also, if the cache is there, we call ftp_readresp() and
     * the cache wasn't good enough to continue we must not just busy-loop
     * around this function.
     *
     */

    if(ftpc->cache && (cache_skip < 2)) {
      /*
       * There's a cache left since before. We then skipping the wait for
       * socket action, unless this is the same cache like the previous round
       * as then the cache was deemed not enough to act on and we then need to
       * wait for more data anyway.
       */
    }
    else {
      switch (Curl_socket_ready(sockfd, CURL_SOCKET_BAD, (int)interval_ms)) {
      case -1: /* select() error, stop reading */
        failf(data, "FTP response aborted due to select/poll error: %d",
              SOCKERRNO);
        return CURLE_RECV_ERROR;

      case 0: /* timeout */
        if(Curl_pgrsUpdate(conn))
          return CURLE_ABORTED_BY_CALLBACK;
        continue; /* just continue in our loop for the timeout duration */

      default: /* for clarity */
        break;
      }
    }
    result = ftp_readresp(sockfd, conn, ftpcode, &nread);
    if(result)
      break;

    if(!nread && ftpc->cache)
      /* bump cache skip counter as on repeated skips we must wait for more
         data */
      cache_skip++;
    else
      /* when we got data or there is no cache left, we reset the cache skip
         counter */
      cache_skip=0;

    *nreadp += nread;

  } /* while there's buffer left and loop is requested */

  ftpc->pending_resp = FALSE;

  return result;
}

/* This is the ONLY way to change FTP state! */
static void state(struct connectdata *conn,
                  ftpstate newstate)
{
#if defined(DEBUGBUILD) && !defined(CURL_DISABLE_VERBOSE_STRINGS)
  /* for debug purposes */
  static const char * const names[]={
    "STOP",
    "WAIT220",
    "AUTH",
    "USER",
    "PASS",
    "ACCT",
    "PBSZ",
    "PROT",
    "CCC",
    "PWD",
    "SYST",
    "NAMEFMT",
    "QUOTE",
    "RETR_PREQUOTE",
    "STOR_PREQUOTE",
    "POSTQUOTE",
    "CWD",
    "MKD",
    "MDTM",
    "TYPE",
    "LIST_TYPE",
    "RETR_TYPE",
    "STOR_TYPE",
    "SIZE",
    "RETR_SIZE",
    "STOR_SIZE",
    "REST",
    "RETR_REST",
    "PORT",
    "PASV",
    "LIST",
    "RETR",
    "STOR",
    "QUIT"
  };
#endif
  struct ftp_conn *ftpc = &conn->proto.ftpc;
#if defined(DEBUGBUILD) && !defined(CURL_DISABLE_VERBOSE_STRINGS)
  if(ftpc->state != newstate)
    infof(conn->data, "FTP %p state change from %s to %s\n",
          ftpc, names[ftpc->state], names[newstate]);
#endif
  ftpc->state = newstate;
}

static CURLcode ftp_state_user(struct connectdata *conn)
{
  CURLcode result;
  struct FTP *ftp = conn->data->state.proto.ftp;
  /* send USER */
  NBFTPSENDF(conn, "USER %s", ftp->user?ftp->user:"");

  state(conn, FTP_USER);
  conn->data->state.ftp_trying_alternative = FALSE;

  return CURLE_OK;
}

static CURLcode ftp_state_pwd(struct connectdata *conn)
{
  CURLcode result;

  /* send PWD to discover our entry point */
  NBFTPSENDF(conn, "PWD", NULL);
  state(conn, FTP_PWD);

  return CURLE_OK;
}

/* For the FTP "protocol connect" and "doing" phases only */
static int ftp_getsock(struct connectdata *conn,
                            curl_socket_t *socks,
                            int numsocks)
{
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if(!numsocks)
    return GETSOCK_BLANK;

  socks[0] = conn->sock[FIRSTSOCKET];

  if(ftpc->sendleft) {
    /* write mode */
    return GETSOCK_WRITESOCK(0);
  }

  /* read mode */
  return GETSOCK_READSOCK(0);
}

/* This is called after the FTP_QUOTE state is passed.

   ftp_state_cwd() sends the range of CWD commands to the server to change to
   the correct directory. It may also need to send MKD commands to create
   missing ones, if that option is enabled.
*/
static CURLcode ftp_state_cwd(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if(ftpc->cwddone)
    /* already done and fine */
    result = ftp_state_post_cwd(conn);
  else {
    ftpc->count2 = 0; /* count2 counts failed CWDs */

    /* count3 is set to allow a MKD to fail once. In the case when first CWD
       fails and then MKD fails (due to another session raced it to create the
       dir) this then allows for a second try to CWD to it */
    ftpc->count3 = (conn->data->set.ftp_create_missing_dirs==2)?1:0;

    if(conn->bits.reuse && ftpc->entrypath) {
      /* This is a re-used connection. Since we change directory to where the
         transfer is taking place, we must first get back to the original dir
         where we ended up after login: */
      ftpc->count1 = 0; /* we count this as the first path, then we add one
                          for all upcoming ones in the ftp->dirs[] array */
      NBFTPSENDF(conn, "CWD %s", ftpc->entrypath);
      state(conn, FTP_CWD);
    }
    else {
      if(ftpc->dirdepth) {
        ftpc->count1 = 1;
        /* issue the first CWD, the rest is sent when the CWD responses are
           received... */
        NBFTPSENDF(conn, "CWD %s", ftpc->dirs[ftpc->count1 -1]);
        state(conn, FTP_CWD);
      }
      else {
        /* No CWD necessary */
        result = ftp_state_post_cwd(conn);
      }
    }
  }
  return result;
}

typedef enum {
  EPRT,
  PORT,
  DONE
} ftpport;

static CURLcode ftp_state_use_port(struct connectdata *conn,
                                   ftpport fcmd) /* start with this */

{
  CURLcode result = CURLE_OK;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  struct SessionHandle *data=conn->data;
  curl_socket_t portsock= CURL_SOCKET_BAD;
  char myhost[256] = "";

  struct Curl_sockaddr_storage ss;
  Curl_addrinfo *res, *ai;
  curl_socklen_t sslen;
  char hbuf[NI_MAXHOST];
  struct sockaddr *sa=(struct sockaddr *)&ss;
  struct sockaddr_in * const sa4 = (void *)sa;
#ifdef ENABLE_IPV6
  struct sockaddr_in6 * const sa6 = (void *)sa;
#endif
  char tmp[1024];
  static const char mode[][5] = { "EPRT", "PORT" };
  int rc;
  int error;
  char *host=NULL;
  char *string_ftpport = data->set.str[STRING_FTPPORT];
  struct Curl_dns_entry *h=NULL;
  unsigned short port_min = 0;
  unsigned short port_max = 0;
  unsigned short port;

  char *addr = NULL;

  /* Step 1, figure out what is requested,
   * accepted format :
   * (ipv4|ipv6|domain|interface)?(:port(-range)?)?
   */

  if(data->set.str[STRING_FTPPORT] &&
     (strlen(data->set.str[STRING_FTPPORT]) > 1)) {

#ifdef ENABLE_IPV6
    size_t addrlen = INET6_ADDRSTRLEN > strlen(string_ftpport) ?
      INET6_ADDRSTRLEN : strlen(string_ftpport);
#else
    size_t addrlen = INET_ADDRSTRLEN > strlen(string_ftpport) ?
      INET_ADDRSTRLEN : strlen(string_ftpport);
#endif
    char *ip_start = string_ftpport;
    char *ip_end = NULL;
    char *port_start = NULL;
    char *port_sep = NULL;

    addr = calloc(addrlen+1, 1);
    if (!addr)
      return CURLE_OUT_OF_MEMORY;

#ifdef ENABLE_IPV6
    if(*string_ftpport == '[') {
      /* [ipv6]:port(-range) */
      ip_start = string_ftpport + 1;
      if((ip_end = strchr(string_ftpport, ']')) != NULL )
        strncpy(addr, ip_start, ip_end - ip_start);
    } else
#endif
      if( *string_ftpport == ':') {
        /* :port */
        ip_end = string_ftpport;
    } else
      if( (ip_end = strchr(string_ftpport, ':')) != NULL) {
        /* either ipv6 or (ipv4|domain|interface):port(-range) */
#ifdef ENABLE_IPV6
      if(Curl_inet_pton(AF_INET6, string_ftpport, sa6) == 1) {
        /* ipv6 */
        port_min = port_max = 0;
        strcpy(addr, string_ftpport);
        ip_end = NULL; /* this got no port ! */
      } else
#endif
      {
        /* (ipv4|domain|interface):port(-range) */
        strncpy(addr, string_ftpport, ip_end - ip_start );
      }
    } else {
      /* ipv4|interface */
      strcpy(addr, string_ftpport);
    }

    /* parse the port */
    if( ip_end != NULL ) {
      if((port_start = strchr(ip_end, ':')) != NULL) {
        port_min = (unsigned short)strtol(port_start+1, NULL, 10);
        if((port_sep = strchr(port_start, '-')) != NULL) {
          port_max = (unsigned short)strtol(port_sep + 1, NULL, 10);
        }
        else
          port_max = port_min;
      }
    }

    /* correct errors like:
     *  :1234-1230
     *  :-4711 , in this case port_min is (unsigned)-1,
     *           therefore port_min > port_max for all cases
     *           but port_max = (unsigned)-1
     */
    if(port_min > port_max )
      port_min = port_max = 0;


    if(*addr != '\0') {
      /* attempt to get the address of the given interface name */
      if(!Curl_if2ip(conn->ip_addr->ai_family, addr,
                     hbuf, sizeof(hbuf)))
        /* not an interface, use the given string as host name instead */
        host = addr;
      else
        host = hbuf; /* use the hbuf for host name */
    }else
      /* there was only a port(-range) given, default the host */
      host = NULL;
  } /* data->set.ftpport */

  if(!host) {
    /* not an interface and not a host name, get default by extracting
       the IP from the control connection */

    sslen = sizeof(ss);
    if(getsockname(conn->sock[FIRSTSOCKET], sa, &sslen)) {
      failf(data, "getsockname() failed: %s",
          Curl_strerror(conn, SOCKERRNO) );
      if (addr)
        free(addr);
      return CURLE_FTP_PORT_FAILED;
    }
    switch(sa->sa_family)
    {
#ifdef ENABLE_IPV6
      case AF_INET6:
        Curl_inet_ntop(sa->sa_family, &sa6->sin6_addr, hbuf, sizeof(hbuf));
        break;
#endif
      default:
        Curl_inet_ntop(sa->sa_family, &sa4->sin_addr, hbuf, sizeof(hbuf));
        break;
    }
    host = hbuf; /* use this host name */
  }

  /* resolv ip/host to ip */
  rc = Curl_resolv(conn, host, 0, &h);
  if(rc == CURLRESOLV_PENDING)
    rc = Curl_wait_for_resolv(conn, &h);
  if(h) {
    res = h->addr;
    /* when we return from this function, we can forget about this entry
       to we can unlock it now already */
    Curl_resolv_unlock(data, h);
  } /* (h) */
  else
    res = NULL; /* failure! */

  if (addr)
    free(addr);

  if (res == NULL) {
    failf(data, "Curl_resolv failed, we can not recover!");
    return CURLE_FTP_PORT_FAILED;
  }

  /* step 2, create a socket for the requested address */

  portsock = CURL_SOCKET_BAD;
  error = 0;
  for (ai = res; ai; ai = ai->ai_next) {
    /*
     * Workaround for AIX5 getaddrinfo() problem (it doesn't set ai_socktype):
     */
    if(ai->ai_socktype == 0)
      ai->ai_socktype = conn->socktype;

    portsock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(portsock == CURL_SOCKET_BAD) {
      error = SOCKERRNO;
      continue;
    }
    break;
  }
  if(!ai) {
    failf(data, "socket failure: %s", Curl_strerror(conn, error));
    return CURLE_FTP_PORT_FAILED;
  }

  /* step 3, bind to a suitable local address */

  memcpy(sa, ai->ai_addr, ai->ai_addrlen);
  sslen = ai->ai_addrlen;

  for( port = port_min; port <= port_max; ) {
    if( sa->sa_family == AF_INET )
      sa4->sin_port = htons(port);
#ifdef ENABLE_IPV6
    else
      sa6->sin6_port = htons(port);
#endif
    /* Try binding the given address. */
    if(bind(portsock, sa, sslen) ) {
      /* It failed. */
      if(errno == EADDRNOTAVAIL) {

        /* The requested bind address is not local
         * use the address used forthe control connection instead
         * restart the port loop
         */
        failf(data, "bind(port=%i) failed: %s", port,
              Curl_strerror(conn, SOCKERRNO) );

        sslen = sizeof(ss);
        if(getsockname(conn->sock[FIRSTSOCKET], sa, &sslen)) {
          failf(data, "getsockname() failed: %s",
                Curl_strerror(conn, SOCKERRNO) );
          sclose(portsock);
          return CURLE_FTP_PORT_FAILED;
        }
        port = port_min;
        continue;
      }else
      if(errno != EADDRINUSE && errno != EACCES) {
        failf(data, "bind(port=%i) failed: %s", port,
              Curl_strerror(conn, SOCKERRNO) );
        sclose(portsock);
        return CURLE_FTP_PORT_FAILED;
      }

    } else
      break;

    port++;
  }

  /* maybe all ports were in use already*/
  if (port > port_max) {
    failf(data, "bind() failed, we ran out of ports!");
    sclose(portsock);
    return CURLE_FTP_PORT_FAILED;
  }

  /* get the name again after the bind() so that we can extract the
     port number it uses now */
  sslen = sizeof(ss);
  if(getsockname(portsock, (struct sockaddr *)sa, &sslen)) {
    failf(data, "getsockname() failed: %s",
          Curl_strerror(conn, SOCKERRNO) );
    sclose(portsock);
    return CURLE_FTP_PORT_FAILED;
  }

  /* step 4, listen on the socket */

  if(listen(portsock, 1)) {
    failf(data, "socket failure: %s", Curl_strerror(conn, SOCKERRNO));
    sclose(portsock);
    return CURLE_FTP_PORT_FAILED;
  }

  /* step 5, send the proper FTP command */

  /* get a plain printable version of the numerical address to work with
     below */
  Curl_printable_address(ai, myhost, sizeof(myhost));

#ifdef ENABLE_IPV6
  if(!conn->bits.ftp_use_eprt && conn->bits.ipv6)
    /* EPRT is disabled but we are connected to a IPv6 host, so we ignore the
       request and enable EPRT again! */
    conn->bits.ftp_use_eprt = TRUE;
#endif

  for (; fcmd != DONE; fcmd++) {

    if(!conn->bits.ftp_use_eprt && (EPRT == fcmd))
      /* if disabled, goto next */
      continue;

    if((PORT == fcmd) && sa->sa_family != AF_INET)
      /* PORT is ipv4 only */
      continue;

    switch (sa->sa_family) {
    case AF_INET:
      port = ntohs(sa4->sin_port);
      break;
#ifdef ENABLE_IPV6
    case AF_INET6:
      port = ntohs(sa6->sin6_port);
      break;
#endif
    default:
      continue; /* might as well skip this */
    }

    if(EPRT == fcmd) {
      /*
       * Two fine examples from RFC2428;
       *
       * EPRT |1|132.235.1.2|6275|
       *
       * EPRT |2|1080::8:800:200C:417A|5282|
       */

      result = Curl_nbftpsendf(conn, "%s |%d|%s|%d|", mode[fcmd],
                               sa->sa_family == AF_INET?1:2,
                               myhost, port);
      if(result)
        return result;
      break;
    }
    else if(PORT == fcmd) {
      char *source = myhost;
      char *dest = tmp;

      /* translate x.x.x.x to x,x,x,x */
      while(source && *source) {
        if(*source == '.')
          *dest=',';
        else
          *dest = *source;
        dest++;
        source++;
      }
      *dest = 0;
      snprintf(dest, 20, ",%d,%d", port>>8, port&0xff);

      result = Curl_nbftpsendf(conn, "%s %s", mode[fcmd], tmp);
      if(result)
        return result;
      break;
    }
  }

  /* store which command was sent */
  ftpc->count1 = fcmd;

  /* we set the secondary socket variable to this for now, it is only so that
     the cleanup function will close it in case we fail before the true
     secondary stuff is made */
  if(CURL_SOCKET_BAD != conn->sock[SECONDARYSOCKET])
    sclose(conn->sock[SECONDARYSOCKET]);
  conn->sock[SECONDARYSOCKET] = portsock;

  /* this tcpconnect assignment below is a hackish work-around to make the
     multi interface with active FTP work - as it will not wait for a
     (passive) connect in Curl_is_connected().

     The *proper* fix is to make sure that the active connection from the
     server is done in a non-blocking way. Currently, it is still BLOCKING.
  */
  conn->bits.tcpconnect = TRUE;

  state(conn, FTP_PORT);
  return result;
}

static CURLcode ftp_state_use_pasv(struct connectdata *conn)
{
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result = CURLE_OK;
  /*
    Here's the excecutive summary on what to do:

    PASV is RFC959, expect:
    227 Entering Passive Mode (a1,a2,a3,a4,p1,p2)

    LPSV is RFC1639, expect:
    228 Entering Long Passive Mode (4,4,a1,a2,a3,a4,2,p1,p2)

    EPSV is RFC2428, expect:
    229 Entering Extended Passive Mode (|||port|)

  */

  static const char mode[][5] = { "EPSV", "PASV" };
  int modeoff;

#ifdef PF_INET6
  if(!conn->bits.ftp_use_epsv && conn->bits.ipv6)
    /* EPSV is disabled but we are connected to a IPv6 host, so we ignore the
       request and enable EPSV again! */
    conn->bits.ftp_use_epsv = TRUE;
#endif

  modeoff = conn->bits.ftp_use_epsv?0:1;

  result = Curl_nbftpsendf(conn, "%s", mode[modeoff]);
  if(result)
    return result;

  ftpc->count1 = modeoff;
  state(conn, FTP_PASV);
  infof(conn->data, "Connect data stream passively\n");

  return result;
}

/* REST is the last command in the chain of commands when a "head"-like
   request is made. Thus, if an actual transfer is to be made this is where
   we take off for real. */
static CURLcode ftp_state_post_rest(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct SessionHandle *data = conn->data;

  if(ftp->transfer != FTPTRANSFER_BODY) {
    /* doesn't transfer any data */

    /* still possibly do PRE QUOTE jobs */
    state(conn, FTP_RETR_PREQUOTE);
    result = ftp_state_quote(conn, TRUE, FTP_RETR_PREQUOTE);
  }
  else if(data->set.ftp_use_port) {
    /* We have chosen to use the PORT (or similar) command */
    result = ftp_state_use_port(conn, EPRT);
  }
  else {
    /* We have chosen (this is default) to use the PASV (or similar) command */
    result = ftp_state_use_pasv(conn);
  }
  return result;
}

static CURLcode ftp_state_post_size(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if((ftp->transfer != FTPTRANSFER_BODY) && ftpc->file) {
    /* if a "head"-like request is being made (on a file) */

    /* Determine if server can respond to REST command and therefore
       whether it supports range */
    NBFTPSENDF(conn, "REST %d", 0);

    state(conn, FTP_REST);
  }
  else
    result = ftp_state_post_rest(conn);

  return result;
}

static CURLcode ftp_state_post_type(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if((ftp->transfer == FTPTRANSFER_INFO) && ftpc->file) {
    /* if a "head"-like request is being made (on a file) */

    /* we know ftpc->file is a valid pointer to a file name */
    NBFTPSENDF(conn, "SIZE %s", ftpc->file);

    state(conn, FTP_SIZE);
  }
  else
    result = ftp_state_post_size(conn);

  return result;
}

static CURLcode ftp_state_post_listtype(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;

  /* If this output is to be machine-parsed, the NLST command might be better
     to use, since the LIST command output is not specified or standard in any
     way. It has turned out that the NLST list output is not the same on all
     servers either... */

  /*
     if FTPFILE_NOCWD was specified, we are currently in
     the user's home directory, so we should add the path
     as argument for the LIST / NLST / or custom command.
     Whether the server will support this, is uncertain.

     The other ftp_filemethods will CWD into dir/dir/ first and
     then just do LIST (in that case: nothing to do here)
  */
  char *cmd,*lstArg,*slashPos;

  lstArg = NULL;
  if((data->set.ftp_filemethod == FTPFILE_NOCWD) &&
     data->state.path &&
     data->state.path[0] &&
     strchr(data->state.path,'/')) {

    lstArg = strdup(data->state.path);
    if(!lstArg)
      return CURLE_OUT_OF_MEMORY;

    /* Check if path does not end with /, as then we cut off the file part */
    if(lstArg[strlen(lstArg) - 1] != '/')  {

      /* chop off the file part if format is dir/dir/file */
      slashPos = strrchr(lstArg,'/');
      if(slashPos)
        *(slashPos+1) = '\0';
    }
  }

  cmd = aprintf( "%s%s%s",
                 data->set.str[STRING_CUSTOMREQUEST]?
                 data->set.str[STRING_CUSTOMREQUEST]:
                 (data->set.ftp_list_only?"NLST":"LIST"),
                 lstArg? " ": "",
                 lstArg? lstArg: "" );

  if(!cmd) {
    if(lstArg)
      free(lstArg);
    return CURLE_OUT_OF_MEMORY;
  }

  NBFTPSENDF(conn, "%s",cmd);

  if(lstArg)
    free(lstArg);

  free(cmd);

  state(conn, FTP_LIST);

  return result;
}

static CURLcode ftp_state_post_retrtype(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

  /* We've sent the TYPE, now we must send the list of prequote strings */

  result = ftp_state_quote(conn, TRUE, FTP_RETR_PREQUOTE);

  return result;
}

static CURLcode ftp_state_post_stortype(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

  /* We've sent the TYPE, now we must send the list of prequote strings */

  result = ftp_state_quote(conn, TRUE, FTP_STOR_PREQUOTE);

  return result;
}

static CURLcode ftp_state_post_mdtm(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  /* If we have selected NOBODY and HEADER, it means that we only want file
     information. Which in FTP can't be much more than the file size and
     date. */
  if(data->set.opt_no_body && ftpc->file &&
     ftp_need_type(conn, data->set.prefer_ascii)) {
    /* The SIZE command is _not_ RFC 959 specified, and therefor many servers
       may not support it! It is however the only way we have to get a file's
       size! */

    ftp->transfer = FTPTRANSFER_INFO;
    /* this means no actual transfer will be made */

    /* Some servers return different sizes for different modes, and thus we
       must set the proper type before we check the size */
    result = ftp_nb_type(conn, data->set.prefer_ascii, FTP_TYPE);
    if(result)
      return result;
  }
  else
    result = ftp_state_post_type(conn);

  return result;
}

/* This is called after the CWD commands have been done in the beginning of
   the DO phase */
static CURLcode ftp_state_post_cwd(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  /* Requested time of file or time-depended transfer? */
  if((data->set.get_filetime || data->set.timecondition) && ftpc->file) {

    /* we have requested to get the modified-time of the file, this is a white
       spot as the MDTM is not mentioned in RFC959 */
    NBFTPSENDF(conn, "MDTM %s", ftpc->file);

    state(conn, FTP_MDTM);
  }
  else
    result = ftp_state_post_mdtm(conn);

  return result;
}


/* This is called after the TYPE and possible quote commands have been sent */
static CURLcode ftp_state_ul_setup(struct connectdata *conn,
                                   bool sizechecked)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  int seekerr = CURL_SEEKFUNC_OK;

  if((data->state.resume_from && !sizechecked) ||
     ((data->state.resume_from > 0) && sizechecked)) {
    /* we're about to continue the uploading of a file */
    /* 1. get already existing file's size. We use the SIZE command for this
       which may not exist in the server!  The SIZE command is not in
       RFC959. */

    /* 2. This used to set REST. But since we can do append, we
       don't another ftp command. We just skip the source file
       offset and then we APPEND the rest on the file instead */

    /* 3. pass file-size number of bytes in the source file */
    /* 4. lower the infilesize counter */
    /* => transfer as usual */

    if(data->state.resume_from < 0 ) {
      /* Got no given size to start from, figure it out */
      NBFTPSENDF(conn, "SIZE %s", ftpc->file);
      state(conn, FTP_STOR_SIZE);
      return result;
    }

    /* enable append */
    data->set.ftp_append = TRUE;

    /* Let's read off the proper amount of bytes from the input. */
    if(conn->seek_func) {
      seekerr = conn->seek_func(conn->seek_client, data->state.resume_from,
                                SEEK_SET);
    }

    if(seekerr != CURL_SEEKFUNC_OK) {
      if(seekerr != CURL_SEEKFUNC_CANTSEEK) {
        failf(data, "Could not seek stream");
        return CURLE_FTP_COULDNT_USE_REST;
      }
      /* seekerr == CURL_SEEKFUNC_CANTSEEK (can't seek to offset) */
      else {
        curl_off_t passed=0;
        do {
          curl_off_t readthisamountnow = (data->state.resume_from - passed);
          curl_off_t actuallyread;

          if(readthisamountnow > BUFSIZE)
            readthisamountnow = BUFSIZE;

          actuallyread = (curl_off_t)
            conn->fread_func(data->state.buffer, 1, (size_t)readthisamountnow,
                             conn->fread_in);

          passed += actuallyread;
          if((actuallyread <= 0) || (actuallyread > readthisamountnow)) {
            /* this checks for greater-than only to make sure that the
               CURL_READFUNC_ABORT return code still aborts */
            failf(data, "Failed to read data");
            return CURLE_FTP_COULDNT_USE_REST;
          }
        } while(passed < data->state.resume_from);
      }
    }
    /* now, decrease the size of the read */
    if(data->set.infilesize>0) {
      data->set.infilesize -= data->state.resume_from;

      if(data->set.infilesize <= 0) {
        infof(data, "File already completely uploaded\n");

        /* no data to transfer */
        result=Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);

        /* Set ->transfer so that we won't get any error in
         * ftp_done() because we didn't transfer anything! */
        ftp->transfer = FTPTRANSFER_NONE;

        state(conn, FTP_STOP);
        return CURLE_OK;
      }
    }
    /* we've passed, proceed as normal */
  } /* resume_from */

  NBFTPSENDF(conn, data->set.ftp_append?"APPE %s":"STOR %s",
             ftpc->file);

  state(conn, FTP_STOR);

  return result;
}

static CURLcode ftp_state_quote(struct connectdata *conn,
                                bool init,
                                ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  bool quote=FALSE;
  struct curl_slist *item;

  switch(instate) {
  case FTP_QUOTE:
  default:
    item = data->set.quote;
    break;
  case FTP_RETR_PREQUOTE:
  case FTP_STOR_PREQUOTE:
    item = data->set.prequote;
    break;
  case FTP_POSTQUOTE:
    item = data->set.postquote;
    break;
  }

  if(init)
    ftpc->count1 = 0;
  else
    ftpc->count1++;

  if(item) {
    int i = 0;

    /* Skip count1 items in the linked list */
    while((i< ftpc->count1) && item) {
      item = item->next;
      i++;
    }
    if(item) {
      NBFTPSENDF(conn, "%s", item->data);
      state(conn, instate);
      quote = TRUE;
    }
  }

  if(!quote) {
    /* No more quote to send, continue to ... */
    switch(instate) {
    case FTP_QUOTE:
    default:
      result = ftp_state_cwd(conn);
      break;
    case FTP_RETR_PREQUOTE:
      if(ftp->transfer != FTPTRANSFER_BODY)
        state(conn, FTP_STOP);
      else {
        NBFTPSENDF(conn, "SIZE %s", ftpc->file);
        state(conn, FTP_RETR_SIZE);
      }
      break;
    case FTP_STOR_PREQUOTE:
      result = ftp_state_ul_setup(conn, FALSE);
      break;
    case FTP_POSTQUOTE:
      break;
    }
  }

  return result;
}

static CURLcode ftp_state_pasv_resp(struct connectdata *conn,
                                    int ftpcode)
{
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result;
  struct SessionHandle *data=conn->data;
  Curl_addrinfo *conninfo;
  struct Curl_dns_entry *addr=NULL;
  int rc;
  unsigned short connectport; /* the local port connect() should use! */
  unsigned short newport=0; /* remote port */
  bool connected;

  /* newhost must be able to hold a full IP-style address in ASCII, which
     in the IPv6 case means 5*8-1 = 39 letters */
#define NEWHOST_BUFSIZE 48
  char newhost[NEWHOST_BUFSIZE];
  char *str=&data->state.buffer[4];  /* start on the first letter */

  if((ftpc->count1 == 0) &&
     (ftpcode == 229)) {
    /* positive EPSV response */
    char *ptr = strchr(str, '(');
    if(ptr) {
      unsigned int num;
      char separator[4];
      ptr++;
      if(5  == sscanf(ptr, "%c%c%c%u%c",
                      &separator[0],
                      &separator[1],
                      &separator[2],
                      &num,
                      &separator[3])) {
        const char sep1 = separator[0];
        int i;

        /* The four separators should be identical, or else this is an oddly
           formatted reply and we bail out immediately. */
        for(i=1; i<4; i++) {
          if(separator[i] != sep1) {
            ptr=NULL; /* set to NULL to signal error */
            break;
          }
        }
        if(ptr) {
          newport = (unsigned short)(num & 0xffff);

          if(conn->bits.tunnel_proxy ||
             data->set.proxytype == CURLPROXY_SOCKS5 ||
             data->set.proxytype == CURLPROXY_SOCKS5_HOSTNAME ||
             data->set.proxytype == CURLPROXY_SOCKS4 ||
             data->set.proxytype == CURLPROXY_SOCKS4A)
            /* proxy tunnel -> use other host info because ip_addr_str is the
               proxy address not the ftp host */
            snprintf(newhost, sizeof(newhost), "%s", conn->host.name);
          else
            /* use the same IP we are already connected to */
            snprintf(newhost, NEWHOST_BUFSIZE, "%s", conn->ip_addr_str);
        }
      }
      else
        ptr=NULL;
    }
    if(!ptr) {
      failf(data, "Weirdly formatted EPSV reply");
      return CURLE_FTP_WEIRD_PASV_REPLY;
    }
  }
  else if((ftpc->count1 == 1) &&
          (ftpcode == 227)) {
    /* positive PASV response */
    int ip[4];
    int port[2];

    /*
     * Scan for a sequence of six comma-separated numbers and use them as
     * IP+port indicators.
     *
     * Found reply-strings include:
     * "227 Entering Passive Mode (127,0,0,1,4,51)"
     * "227 Data transfer will passively listen to 127,0,0,1,4,51"
     * "227 Entering passive mode. 127,0,0,1,4,51"
     */
    while(*str) {
      if(6 == sscanf(str, "%d,%d,%d,%d,%d,%d",
                      &ip[0], &ip[1], &ip[2], &ip[3],
                      &port[0], &port[1]))
        break;
      str++;
    }

    if(!*str) {
      failf(data, "Couldn't interpret the 227-response");
      return CURLE_FTP_WEIRD_227_FORMAT;
    }

    /* we got OK from server */
    if(data->set.ftp_skip_ip) {
      /* told to ignore the remotely given IP but instead use the one we used
         for the control connection */
      infof(data, "Skips %d.%d.%d.%d for data connection, uses %s instead\n",
            ip[0], ip[1], ip[2], ip[3],
            conn->ip_addr_str);
      if(conn->bits.tunnel_proxy ||
          data->set.proxytype == CURLPROXY_SOCKS5 ||
          data->set.proxytype == CURLPROXY_SOCKS5_HOSTNAME ||
          data->set.proxytype == CURLPROXY_SOCKS4 ||
          data->set.proxytype == CURLPROXY_SOCKS4A)
        /* proxy tunnel -> use other host info because ip_addr_str is the
           proxy address not the ftp host */
        snprintf(newhost, sizeof(newhost), "%s", conn->host.name);
      else
        snprintf(newhost, sizeof(newhost), "%s", conn->ip_addr_str);
    }
    else
      snprintf(newhost, sizeof(newhost),
               "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    newport = (unsigned short)(((port[0]<<8) + port[1]) & 0xffff);
  }
  else if(ftpc->count1 == 0) {
    /* EPSV failed, move on to PASV */

    /* disable it for next transfer */
    conn->bits.ftp_use_epsv = FALSE;
    infof(data, "disabling EPSV usage\n");

    NBFTPSENDF(conn, "PASV", NULL);
    ftpc->count1++;
    /* remain in the FTP_PASV state */
    return result;
  }
  else {
    failf(data, "Bad PASV/EPSV response: %03d", ftpcode);
    return CURLE_FTP_WEIRD_PASV_REPLY;
  }

  if(data->set.str[STRING_PROXY] && *data->set.str[STRING_PROXY]) {
    /*
     * This is a tunnel through a http proxy and we need to connect to the
     * proxy again here.
     *
     * We don't want to rely on a former host lookup that might've expired
     * now, instead we remake the lookup here and now!
     */
    rc = Curl_resolv(conn, conn->proxy.name, (int)conn->port, &addr);
    if(rc == CURLRESOLV_PENDING)
      /* BLOCKING */
      rc = Curl_wait_for_resolv(conn, &addr);

    connectport =
      (unsigned short)conn->port; /* we connect to the proxy's port */

    if(!addr) {
      failf(data, "Can't resolve proxy host %s:%d",
            conn->proxy.name, connectport);
      return CURLE_FTP_CANT_GET_HOST;
    }
  }
  else {
    /* normal, direct, ftp connection */
    rc = Curl_resolv(conn, newhost, newport, &addr);
    if(rc == CURLRESOLV_PENDING)
      /* BLOCKING */
      rc = Curl_wait_for_resolv(conn, &addr);

    connectport = newport; /* we connect to the remote port */

    if(!addr) {
      failf(data, "Can't resolve new host %s:%d", newhost, connectport);
      return CURLE_FTP_CANT_GET_HOST;
    }
  }

  result = Curl_connecthost(conn,
                            addr,
                            &conn->sock[SECONDARYSOCKET],
                            &conninfo,
                            &connected);

  Curl_resolv_unlock(data, addr); /* we're done using this address */

  if(result && ftpc->count1 == 0 && ftpcode == 229) {
    infof(data, "got positive EPSV response, but can't connect. "
          "Disabling EPSV\n");
    /* disable it for next transfer */
    conn->bits.ftp_use_epsv = FALSE;
    data->state.errorbuf = FALSE; /* allow error message to get rewritten */
    NBFTPSENDF(conn, "PASV", NULL);
    ftpc->count1++;
    /* remain in the FTP_PASV state */
    return result;
 }

  if(result)
    return result;

  conn->bits.tcpconnect = connected; /* simply TRUE or FALSE */

  /*
   * When this is used from the multi interface, this might've returned with
   * the 'connected' set to FALSE and thus we are now awaiting a non-blocking
   * connect to connect and we should not be "hanging" here waiting.
   */

  if(data->set.verbose)
    /* this just dumps information about this second connection */
    ftp_pasv_verbose(conn, conninfo, newhost, connectport);

  switch(data->set.proxytype) {
#ifndef CURL_DISABLE_PROXY
    /* FIX: this MUST wait for a proper connect first if 'connected' is
     * FALSE */
  case CURLPROXY_SOCKS5:
  case CURLPROXY_SOCKS5_HOSTNAME:
    result = Curl_SOCKS5(conn->proxyuser, conn->proxypasswd, newhost, newport,
                         SECONDARYSOCKET, conn);
    break;
  case CURLPROXY_SOCKS4:
    result = Curl_SOCKS4(conn->proxyuser, newhost, newport,
                         SECONDARYSOCKET, conn, FALSE);
    break;
  case CURLPROXY_SOCKS4A:
    result = Curl_SOCKS4(conn->proxyuser, newhost, newport,
                         SECONDARYSOCKET, conn, TRUE);
    break;
#endif /* CURL_DISABLE_PROXY */
  case CURLPROXY_HTTP:
  case CURLPROXY_HTTP_1_0:
    /* do nothing here. handled later. */
    break;
  default:
    failf(data, "unknown proxytype option given");
    result = CURLE_COULDNT_CONNECT;
    break;
  }
#if !defined(CURL_DISABLE_HTTP) && !defined(CURL_DISABLE_PROXY)
  if(conn->bits.tunnel_proxy && conn->bits.httpproxy) {
    /* FIX: this MUST wait for a proper connect first if 'connected' is
     * FALSE */

    /* BLOCKING */
    /* We want "seamless" FTP operations through HTTP proxy tunnel */

    /* Curl_proxyCONNECT is based on a pointer to a struct HTTP at the member
     * conn->proto.http; we want FTP through HTTP and we have to change the
     * member temporarily for connecting to the HTTP proxy. After
     * Curl_proxyCONNECT we have to set back the member to the original struct
     * FTP pointer
     */
    struct HTTP http_proxy;
    struct FTP *ftp_save = data->state.proto.ftp;
    memset(&http_proxy, 0, sizeof(http_proxy));
    data->state.proto.http = &http_proxy;

    result = Curl_proxyCONNECT(conn, SECONDARYSOCKET, newhost, newport);

    data->state.proto.ftp = ftp_save;

    if(CURLE_OK != result)
      return result;
  }
#endif /* !CURL_DISABLE_HTTP && !CURL_DISABLE_PROXY */

  state(conn, FTP_STOP); /* this phase is completed */

  return result;
}

static CURLcode ftp_state_port_resp(struct connectdata *conn,
                                    int ftpcode)
{
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  ftpport fcmd = (ftpport)ftpc->count1;
  CURLcode result = CURLE_OK;

  if(ftpcode != 200) {
    /* the command failed */

    if(EPRT == fcmd) {
      infof(data, "disabling EPRT usage\n");
      conn->bits.ftp_use_eprt = FALSE;
    }
    fcmd++;

    if(fcmd == DONE) {
      failf(data, "Failed to do PORT");
      result = CURLE_FTP_PORT_FAILED;
    }
    else
      /* try next */
      result = ftp_state_use_port(conn, fcmd);
  }
  else {
    infof(data, "Connect data stream actively\n");
    state(conn, FTP_STOP); /* end of DO phase */
  }

  return result;
}

static CURLcode ftp_state_mdtm_resp(struct connectdata *conn,
                                    int ftpcode)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data=conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  switch(ftpcode) {
  case 213:
    {
      /* we got a time. Format should be: "YYYYMMDDHHMMSS[.sss]" where the
         last .sss part is optional and means fractions of a second */
      int year, month, day, hour, minute, second;
      char *buf = data->state.buffer;
      if(6 == sscanf(buf+4, "%04d%02d%02d%02d%02d%02d",
                     &year, &month, &day, &hour, &minute, &second)) {
        /* we have a time, reformat it */
        time_t secs=time(NULL);
        /* using the good old yacc/bison yuck */
        snprintf(buf, sizeof(conn->data->state.buffer),
                 "%04d%02d%02d %02d:%02d:%02d GMT",
                 year, month, day, hour, minute, second);
        /* now, convert this into a time() value: */
        data->info.filetime = (long)curl_getdate(buf, &secs);
      }

#ifdef CURL_FTP_HTTPSTYLE_HEAD
      /* If we asked for a time of the file and we actually got one as well,
         we "emulate" a HTTP-style header in our output. */

      if(data->set.opt_no_body &&
         ftpc->file &&
         data->set.get_filetime &&
         (data->info.filetime>=0) ) {
        struct tm *tm;
        time_t filetime = (time_t)data->info.filetime;
#ifdef HAVE_GMTIME_R
        struct tm buffer;
        tm = (struct tm *)gmtime_r(&filetime, &buffer);
#else
        tm = gmtime(&filetime);
#endif
        /* format: "Tue, 15 Nov 1994 12:45:26" */
        snprintf(buf, BUFSIZE-1,
                 "Last-Modified: %s, %02d %s %4d %02d:%02d:%02d GMT\r\n",
                 Curl_wkday[tm->tm_wday?tm->tm_wday-1:6],
                 tm->tm_mday,
                 Curl_month[tm->tm_mon],
                 tm->tm_year + 1900,
                 tm->tm_hour,
                 tm->tm_min,
                 tm->tm_sec);
        result = Curl_client_write(conn, CLIENTWRITE_BOTH, buf, 0);
        if(result)
          return result;
      } /* end of a ridiculous amount of conditionals */
#endif
    }
    break;
  default:
    infof(data, "unsupported MDTM reply format\n");
    break;
  case 550: /* "No such file or directory" */
    failf(data, "Given file does not exist");
    result = CURLE_FTP_COULDNT_RETR_FILE;
    break;
  }

  if(data->set.timecondition) {
    if((data->info.filetime > 0) && (data->set.timevalue > 0)) {
      switch(data->set.timecondition) {
      case CURL_TIMECOND_IFMODSINCE:
      default:
        if(data->info.filetime <= data->set.timevalue) {
          infof(data, "The requested document is not new enough\n");
          ftp->transfer = FTPTRANSFER_NONE; /* mark this to not transfer data */
          data->info.timecond = TRUE;
          state(conn, FTP_STOP);
          return CURLE_OK;
        }
        break;
      case CURL_TIMECOND_IFUNMODSINCE:
        if(data->info.filetime > data->set.timevalue) {
          infof(data, "The requested document is not old enough\n");
          ftp->transfer = FTPTRANSFER_NONE; /* mark this to not transfer data */
          data->info.timecond = TRUE;
          state(conn, FTP_STOP);
          return CURLE_OK;
        }
        break;
      } /* switch */
    }
    else {
      infof(data, "Skipping time comparison\n");
    }
  }

  if(!result)
    result = ftp_state_post_mdtm(conn);

  return result;
}

static CURLcode ftp_state_type_resp(struct connectdata *conn,
                                    int ftpcode,
                                    ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data=conn->data;

  if(ftpcode/100 != 2) {
    /* "sasserftpd" and "(u)r(x)bot ftpd" both responds with 226 after a
       successful 'TYPE I'. While that is not as RFC959 says, it is still a
       positive response code and we allow that. */
    failf(data, "Couldn't set desired mode");
    return CURLE_FTP_COULDNT_SET_TYPE;
  }
  if(ftpcode != 200)
    infof(data, "Got a %03d response code instead of the assumed 200\n",
          ftpcode);

  if(instate == FTP_TYPE)
    result = ftp_state_post_type(conn);
  else if(instate == FTP_LIST_TYPE)
    result = ftp_state_post_listtype(conn);
  else if(instate == FTP_RETR_TYPE)
    result = ftp_state_post_retrtype(conn);
  else if(instate == FTP_STOR_TYPE)
    result = ftp_state_post_stortype(conn);

  return result;
}

static CURLcode ftp_state_post_retr_size(struct connectdata *conn,
                                         curl_off_t filesize)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data=conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if(data->set.max_filesize && (filesize > data->set.max_filesize)) {
    failf(data, "Maximum file size exceeded");
    return CURLE_FILESIZE_EXCEEDED;
  }
  ftp->downloadsize = filesize;

  if(data->state.resume_from) {
    /* We always (attempt to) get the size of downloads, so it is done before
       this even when not doing resumes. */
    if(filesize == -1) {
      infof(data, "ftp server doesn't support SIZE\n");
      /* We couldn't get the size and therefore we can't know if there really
         is a part of the file left to get, although the server will just
         close the connection when we start the connection so it won't cause
         us any harm, just not make us exit as nicely. */
    }
    else {
      /* We got a file size report, so we check that there actually is a
         part of the file left to get, or else we go home.  */
      if(data->state.resume_from< 0) {
        /* We're supposed to download the last abs(from) bytes */
        if(filesize < -data->state.resume_from) {
          failf(data, "Offset (%" FORMAT_OFF_T
                ") was beyond file size (%" FORMAT_OFF_T ")",
                data->state.resume_from, filesize);
          return CURLE_BAD_DOWNLOAD_RESUME;
        }
        /* convert to size to download */
        ftp->downloadsize = -data->state.resume_from;
        /* download from where? */
        data->state.resume_from = filesize - ftp->downloadsize;
      }
      else {
        if(filesize < data->state.resume_from) {
          failf(data, "Offset (%" FORMAT_OFF_T
                ") was beyond file size (%" FORMAT_OFF_T ")",
                data->state.resume_from, filesize);
          return CURLE_BAD_DOWNLOAD_RESUME;
        }
        /* Now store the number of bytes we are expected to download */
        ftp->downloadsize = filesize-data->state.resume_from;
      }
    }

    if(ftp->downloadsize == 0) {
      /* no data to transfer */
      result = Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);
      infof(data, "File already completely downloaded\n");

      /* Set ->transfer so that we won't get any error in ftp_done()
       * because we didn't transfer the any file */
      ftp->transfer = FTPTRANSFER_NONE;
      state(conn, FTP_STOP);
      return CURLE_OK;
    }

    /* Set resume file transfer offset */
    infof(data, "Instructs server to resume from offset %" FORMAT_OFF_T
          "\n", data->state.resume_from);

    NBFTPSENDF(conn, "REST %" FORMAT_OFF_T, data->state.resume_from);

    state(conn, FTP_RETR_REST);

  }
  else {
    /* no resume */
    NBFTPSENDF(conn, "RETR %s", ftpc->file);
    state(conn, FTP_RETR);
  }

  return result;
}

static CURLcode ftp_state_size_resp(struct connectdata *conn,
                                    int ftpcode,
                                    ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data=conn->data;
  curl_off_t filesize;
  char *buf = data->state.buffer;

  /* get the size from the ascii string: */
  filesize = (ftpcode == 213)?curlx_strtoofft(buf+4, NULL, 0):-1;

  if(instate == FTP_SIZE) {
#ifdef CURL_FTP_HTTPSTYLE_HEAD
    if(-1 != filesize) {
      snprintf(buf, sizeof(data->state.buffer),
               "Content-Length: %" FORMAT_OFF_T "\r\n", filesize);
      result = Curl_client_write(conn, CLIENTWRITE_BOTH, buf, 0);
      if(result)
        return result;
    }
#endif
    Curl_pgrsSetDownloadSize(data, filesize);
    result = ftp_state_post_size(conn);
  }
  else if(instate == FTP_RETR_SIZE) {
    Curl_pgrsSetDownloadSize(data, filesize);
    result = ftp_state_post_retr_size(conn, filesize);
  }
  else if(instate == FTP_STOR_SIZE) {
    data->state.resume_from = filesize;
    result = ftp_state_ul_setup(conn, TRUE);
  }

  return result;
}

static CURLcode ftp_state_rest_resp(struct connectdata *conn,
                                    int ftpcode,
                                    ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  switch(instate) {
  case FTP_REST:
  default:
#ifdef CURL_FTP_HTTPSTYLE_HEAD
    if(ftpcode == 350) {
      char buffer[24]= { "Accept-ranges: bytes\r\n" };
      result = Curl_client_write(conn, CLIENTWRITE_BOTH, buffer, 0);
      if(result)
        return result;
    }
#endif
    result = ftp_state_post_rest(conn);
    break;

  case FTP_RETR_REST:
    if(ftpcode != 350) {
      failf(conn->data, "Couldn't use REST");
      result = CURLE_FTP_COULDNT_USE_REST;
    }
    else {
      NBFTPSENDF(conn, "RETR %s", ftpc->file);
      state(conn, FTP_RETR);
    }
    break;
  }

  return result;
}

static CURLcode ftp_state_stor_resp(struct connectdata *conn,
                                    int ftpcode)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;

  if(ftpcode>=400) {
    failf(data, "Failed FTP upload: %0d", ftpcode);
    /* oops, we never close the sockets! */
    return CURLE_UPLOAD_FAILED;
  }

  if(data->set.ftp_use_port) {
    /* BLOCKING */
    /* PORT means we are now awaiting the server to connect to us. */
    result = AllowServerConnect(conn);
    if( result )
      return result;
  }

  if(conn->ssl[SECONDARYSOCKET].use) {
    /* since we only have a plaintext TCP connection here, we must now
       do the TLS stuff */
    infof(data, "Doing the SSL/TLS handshake on the data stream\n");
    /* BLOCKING */
    result = Curl_ssl_connect(conn, SECONDARYSOCKET);
    if(result)
      return result;
  }

  *(ftp->bytecountp)=0;

  /* When we know we're uploading a specified file, we can get the file
     size prior to the actual upload. */

  Curl_pgrsSetUploadSize(data, data->set.infilesize);

  /* set the SO_SNDBUF for the secondary socket for those who need it */
  Curl_sndbufset(conn->sock[SECONDARYSOCKET]);

  result = Curl_setup_transfer(conn, -1, -1, FALSE, NULL, /* no download */
                               SECONDARYSOCKET, ftp->bytecountp);
  state(conn, FTP_STOP);

  conn->proto.ftpc.pending_resp = TRUE; /* we expect a server response more */

  return result;
}

/* for LIST and RETR responses */
static CURLcode ftp_state_get_resp(struct connectdata *conn,
                                    int ftpcode,
                                    ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  char *buf = data->state.buffer;

  if((ftpcode == 150) || (ftpcode == 125)) {

    /*
      A;
      150 Opening BINARY mode data connection for /etc/passwd (2241
      bytes).  (ok, the file is being transfered)

      B:
      150 Opening ASCII mode data connection for /bin/ls

      C:
      150 ASCII data connection for /bin/ls (137.167.104.91,37445) (0 bytes).

      D:
      150 Opening ASCII mode data connection for /linux/fisk/kpanelrc (0.0.0.0,0) (545 bytes).

      E:
      125 Data connection already open; Transfer starting. */

    curl_off_t size=-1; /* default unknown size */


    /*
     * It appears that there are FTP-servers that return size 0 for files when
     * SIZE is used on the file while being in BINARY mode. To work around
     * that (stupid) behavior, we attempt to parse the RETR response even if
     * the SIZE returned size zero.
     *
     * Debugging help from Salvatore Sorrentino on February 26, 2003.
     */

    if((instate != FTP_LIST) &&
       !data->set.prefer_ascii &&
       (ftp->downloadsize < 1)) {
      /*
       * It seems directory listings either don't show the size or very
       * often uses size 0 anyway. ASCII transfers may very well turn out
       * that the transfered amount of data is not the same as this line
       * tells, why using this number in those cases only confuses us.
       *
       * Example D above makes this parsing a little tricky */
      char *bytes;
      bytes=strstr(buf, " bytes");
      if(bytes--) {
        long in=(long)(bytes-buf);
        /* this is a hint there is size information in there! ;-) */
        while(--in) {
          /* scan for the left parenthesis and break there */
          if('(' == *bytes)
            break;
          /* skip only digits */
          if(!ISDIGIT(*bytes)) {
            bytes=NULL;
            break;
          }
          /* one more estep backwards */
          bytes--;
        }
        /* if we have nothing but digits: */
        if(bytes++) {
          /* get the number! */
          size = curlx_strtoofft(bytes, NULL, 0);
        }
      }
    }
    else if(ftp->downloadsize > -1)
      size = ftp->downloadsize;

    if(data->set.ftp_use_port) {
      /* BLOCKING */
      result = AllowServerConnect(conn);
      if( result )
        return result;
    }

    if(conn->ssl[SECONDARYSOCKET].use) {
      /* since we only have a plaintext TCP connection here, we must now
         do the TLS stuff */
      infof(data, "Doing the SSL/TLS handshake on the data stream\n");
      result = Curl_ssl_connect(conn, SECONDARYSOCKET);
      if(result)
        return result;
    }

    if(size > data->req.maxdownload && data->req.maxdownload > 0)
      size = data->req.size = data->req.maxdownload;

    infof(data, "Maxdownload = %" FORMAT_OFF_T "\n", data->req.maxdownload);

    if(instate != FTP_LIST)
      infof(data, "Getting file with size: %" FORMAT_OFF_T "\n", size);

    /* FTP download: */
    result=Curl_setup_transfer(conn, SECONDARYSOCKET, size, FALSE,
                               ftp->bytecountp,
                               -1, NULL); /* no upload here */
    if(result)
      return result;

    conn->proto.ftpc.pending_resp = TRUE; /* we expect a server response more */
    state(conn, FTP_STOP);
  }
  else {
    if((instate == FTP_LIST) && (ftpcode == 450)) {
      /* simply no matching files in the dir listing */
      ftp->transfer = FTPTRANSFER_NONE; /* don't download anything */
      state(conn, FTP_STOP); /* this phase is over */
    }
    else {
      failf(data, "RETR response: %03d", ftpcode);
      return instate == FTP_RETR && ftpcode == 550? CURLE_REMOTE_FILE_NOT_FOUND:
                                                    CURLE_FTP_COULDNT_RETR_FILE;
    }
  }

  return result;
}

/* after USER, PASS and ACCT */
static CURLcode ftp_state_loggedin(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

#ifdef HAVE_KRB4
  if(conn->data->set.krb) {
    /* We may need to issue a KAUTH here to have access to the files
     * do it if user supplied a password
     */
    if(conn->passwd && *conn->passwd) {
      /* BLOCKING */
      result = Curl_krb_kauth(conn);
      if(result)
        return result;
    }
  }
#endif
  if(conn->ssl[FIRSTSOCKET].use) {
    /* PBSZ = PROTECTION BUFFER SIZE.

    The 'draft-murray-auth-ftp-ssl' (draft 12, page 7) says:

    Specifically, the PROT command MUST be preceded by a PBSZ
    command and a PBSZ command MUST be preceded by a successful
    security data exchange (the TLS negotiation in this case)

    ... (and on page 8):

    Thus the PBSZ command must still be issued, but must have a
    parameter of '0' to indicate that no buffering is taking place
    and the data connection should not be encapsulated.
    */
    NBFTPSENDF(conn, "PBSZ %d", 0);
    state(conn, FTP_PBSZ);
  }
  else {
    result = ftp_state_pwd(conn);
  }
  return result;
}

/* for USER and PASS responses */
static CURLcode ftp_state_user_resp(struct connectdata *conn,
                                    int ftpcode,
                                    ftpstate instate)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  (void)instate; /* no use for this yet */

  /* some need password anyway, and others just return 2xx ignored */
  if((ftpcode == 331) && (ftpc->state == FTP_USER)) {
    /* 331 Password required for ...
       (the server requires to send the user's password too) */
    NBFTPSENDF(conn, "PASS %s", ftp->passwd?ftp->passwd:"");
    state(conn, FTP_PASS);
  }
  else if(ftpcode/100 == 2) {
    /* 230 User ... logged in.
       (the user logged in with or without password) */
    result = ftp_state_loggedin(conn);
  }
  else if(ftpcode == 332) {
    if(data->set.str[STRING_FTP_ACCOUNT]) {
      NBFTPSENDF(conn, "ACCT %s", data->set.str[STRING_FTP_ACCOUNT]);
      state(conn, FTP_ACCT);
    }
    else {
      failf(data, "ACCT requested but none available");
      result = CURLE_LOGIN_DENIED;
    }
  }
  else {
    /* All other response codes, like:

    530 User ... access denied
    (the server denies to log the specified user) */

    if(conn->data->set.str[STRING_FTP_ALTERNATIVE_TO_USER] &&
        !conn->data->state.ftp_trying_alternative) {
      /* Ok, USER failed.  Let's try the supplied command. */
      NBFTPSENDF(conn, "%s",
                 conn->data->set.str[STRING_FTP_ALTERNATIVE_TO_USER]);
      conn->data->state.ftp_trying_alternative = TRUE;
      state(conn, FTP_USER);
      result = CURLE_OK;
    }
    else {
      failf(data, "Access denied: %03d", ftpcode);
      result = CURLE_LOGIN_DENIED;
    }
  }
  return result;
}

/* for ACCT response */
static CURLcode ftp_state_acct_resp(struct connectdata *conn,
                                    int ftpcode)
{
  CURLcode result = CURLE_OK;
  struct SessionHandle *data = conn->data;
  if(ftpcode != 230) {
    failf(data, "ACCT rejected by server: %03d", ftpcode);
    result = CURLE_FTP_WEIRD_PASS_REPLY; /* FIX */
  }
  else
    result = ftp_state_loggedin(conn);

  return result;
}


static CURLcode ftp_statemach_act(struct connectdata *conn)
{
  CURLcode result;
  curl_socket_t sock = conn->sock[FIRSTSOCKET];
  struct SessionHandle *data=conn->data;
  int ftpcode;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  static const char ftpauth[][4]  = { "SSL", "TLS" };
  size_t nread = 0;

  if(ftpc->sendleft) {
    /* we have a piece of a command still left to send */
    ssize_t written;
    result = Curl_write(conn, sock, ftpc->sendthis + ftpc->sendsize -
                        ftpc->sendleft, ftpc->sendleft, &written);
    if(result)
      return result;

    if(written != (ssize_t)ftpc->sendleft) {
      /* only a fraction was sent */
      ftpc->sendleft -= written;
    }
    else {
      free(ftpc->sendthis);
      ftpc->sendthis=NULL;
      ftpc->sendleft = ftpc->sendsize = 0;
      ftpc->response = Curl_tvnow();
    }
    return CURLE_OK;
  }

  /* we read a piece of response */
  result = ftp_readresp(sock, conn, &ftpcode, &nread);
  if(result)
    return result;

  if(ftpcode) {
    /* we have now received a full FTP server response */
    switch(ftpc->state) {
    case FTP_WAIT220:
      if(ftpcode != 220) {
        failf(data, "Got a %03d ftp-server response when 220 was expected",
              ftpcode);
        return CURLE_FTP_WEIRD_SERVER_REPLY;
      }

      /* We have received a 220 response fine, now we proceed. */
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
      if(data->set.krb) {
        /* If not anonymous login, try a secure login. Note that this
           procedure is still BLOCKING. */

        Curl_sec_request_prot(conn, "private");
        /* We set private first as default, in case the line below fails to
           set a valid level */
        Curl_sec_request_prot(conn, data->set.str[STRING_KRB_LEVEL]);

        if(Curl_sec_login(conn) != 0)
          infof(data, "Logging in with password in cleartext!\n");
        else
          infof(data, "Authentication successful\n");
      }
#endif

      if(data->set.ftp_ssl && !conn->ssl[FIRSTSOCKET].use) {
        /* We don't have a SSL/TLS connection yet, but FTPS is
           requested. Try a FTPS connection now */

        ftpc->count3=0;
        switch(data->set.ftpsslauth) {
        case CURLFTPAUTH_DEFAULT:
        case CURLFTPAUTH_SSL:
          ftpc->count2 = 1; /* add one to get next */
          ftpc->count1 = 0;
          break;
        case CURLFTPAUTH_TLS:
          ftpc->count2 = -1; /* subtract one to get next */
          ftpc->count1 = 1;
          break;
        default:
          failf(data, "unsupported parameter to CURLOPT_FTPSSLAUTH: %d",
                data->set.ftpsslauth);
          return CURLE_FAILED_INIT; /* we don't know what to do */
        }
        NBFTPSENDF(conn, "AUTH %s", ftpauth[ftpc->count1]);
        state(conn, FTP_AUTH);
      }
      else {
        result = ftp_state_user(conn);
        if(result)
          return result;
      }

      break;

    case FTP_AUTH:
      /* we have gotten the response to a previous AUTH command */

      /* RFC2228 (page 5) says:
       *
       * If the server is willing to accept the named security mechanism,
       * and does not require any security data, it must respond with
       * reply code 234/334.
       */

      if((ftpcode == 234) || (ftpcode == 334)) {
        /* Curl_ssl_connect is BLOCKING */
        result = Curl_ssl_connect(conn, FIRSTSOCKET);
        if(CURLE_OK == result) {
          conn->protocol |= PROT_FTPS;
          conn->ssl[SECONDARYSOCKET].use = FALSE; /* clear-text data */
          result = ftp_state_user(conn);
        }
      }
      else if(ftpc->count3 < 1) {
        ftpc->count3++;
        ftpc->count1 += ftpc->count2; /* get next attempt */
        result = Curl_nbftpsendf(conn, "AUTH %s", ftpauth[ftpc->count1]);
        /* remain in this same state */
      }
      else {
        if(data->set.ftp_ssl > CURLUSESSL_TRY)
          /* we failed and CURLUSESSL_CONTROL or CURLUSESSL_ALL is set */
          result = CURLE_USE_SSL_FAILED;
        else
          /* ignore the failure and continue */
          result = ftp_state_user(conn);
      }

      if(result)
        return result;
      break;

    case FTP_USER:
    case FTP_PASS:
      result = ftp_state_user_resp(conn, ftpcode, ftpc->state);
      break;

    case FTP_ACCT:
      result = ftp_state_acct_resp(conn, ftpcode);
      break;

    case FTP_PBSZ:
      NBFTPSENDF(conn, "PROT %c",
                 data->set.ftp_ssl == CURLUSESSL_CONTROL ? 'C' : 'P');
      state(conn, FTP_PROT);

      break;

    case FTP_PROT:
      if(ftpcode/100 == 2)
        /* We have enabled SSL for the data connection! */
        conn->ssl[SECONDARYSOCKET].use =
          (bool)(data->set.ftp_ssl != CURLUSESSL_CONTROL);
      /* FTP servers typically responds with 500 if they decide to reject
         our 'P' request */
      else if(data->set.ftp_ssl > CURLUSESSL_CONTROL)
        /* we failed and bails out */
        return CURLE_USE_SSL_FAILED;

      if(data->set.ftp_ccc) {
        /* CCC - Clear Command Channel
         */
        NBFTPSENDF(conn, "CCC", NULL);
        state(conn, FTP_CCC);
      }
      else {
        result = ftp_state_pwd(conn);
        if(result)
          return result;
      }
      break;

    case FTP_CCC:
      if(ftpcode < 500) {
        /* First shut down the SSL layer (note: this call will block) */
        result = Curl_ssl_shutdown(conn, FIRSTSOCKET);

        if(result) {
          failf(conn->data, "Failed to clear the command channel (CCC)");
          return result;
        }
      }

      /* Then continue as normal */
      result = ftp_state_pwd(conn);
      if(result)
        return result;
      break;

    case FTP_PWD:
      if(ftpcode == 257) {
        char *ptr=&data->state.buffer[4];  /* start on the first letter */
        char *dir;
        char *store;

        dir = malloc(nread + 1);
        if(!dir)
          return CURLE_OUT_OF_MEMORY;

        /* Reply format is like
           257<space>"<directory-name>"<space><commentary> and the RFC959
           says

           The directory name can contain any character; embedded
           double-quotes should be escaped by double-quotes (the
           "quote-doubling" convention).
        */
        if('\"' == *ptr) {
          /* it started good */
          ptr++;
          for (store = dir; *ptr;) {
            if('\"' == *ptr) {
              if('\"' == ptr[1]) {
                /* "quote-doubling" */
                *store = ptr[1];
                ptr++;
              }
              else {
                /* end of path */
                *store = '\0'; /* zero terminate */
                break; /* get out of this loop */
              }
            }
            else
              *store = *ptr;
            store++;
            ptr++;
          }
          if(ftpc->entrypath)
            free(ftpc->entrypath);
          ftpc->entrypath =dir; /* remember this */
          infof(data, "Entry path is '%s'\n", ftpc->entrypath);
          /* also save it where getinfo can access it: */
          data->state.most_recent_ftp_entrypath = ftpc->entrypath;

          /* If the path name does not look like an absolute path (i.e.: it
             does not start with a '/'), we probably need some server-dependent
             adjustments. For example, this is the case when connecting to
             an OS400 FTP server: this server supports two name syntaxes,
             the default one being incompatible with standard pathes. In
             addition, this server switches automatically to the regular path
             syntax when one is encountered in a command: this results in
             having an entrypath in the wrong syntax when later used in CWD.
               The method used here is to check the server OS: we do it only
             if the path name looks strange to minimize overhead on other
             systems. */

          if(!ftpc->server_os && ftpc->entrypath[0] != '/') {
            NBFTPSENDF(conn, "SYST", NULL);
            state(conn, FTP_SYST);
            break;
          }
        }
        else {
          /* couldn't get the path */
          free(dir);
          infof(data, "Failed to figure out path\n");
        }
      }
      state(conn, FTP_STOP); /* we are done with the CONNECT phase! */
      DEBUGF(infof(data, "protocol connect phase DONE\n"));
      break;

    case FTP_SYST:
      if(ftpcode == 215) {
        char *ptr=&data->state.buffer[4];  /* start on the first letter */
        char *os;
        char *store;

        os = malloc(nread + 1);
        if(!os)
          return CURLE_OUT_OF_MEMORY;

        /* Reply format is like
           215<space><OS-name><space><commentary>
        */
        while (*ptr == ' ')
          ptr++;
        for (store = os; *ptr && *ptr != ' ';)
          *store++ = *ptr++;
        *store = '\0'; /* zero terminate */
        ftpc->server_os = os;

        /* Check for special servers here. */

        if(strequal(ftpc->server_os, "OS/400")) {
          /* Force OS400 name format 1. */
          NBFTPSENDF(conn, "SITE NAMEFMT 1", NULL);
          state(conn, FTP_NAMEFMT);
          break;
        }
      else {
        /* Nothing special for the target server. */
       }
      }
      else {
        /* Cannot identify server OS. Continue anyway and cross fingers. */
      }

      state(conn, FTP_STOP); /* we are done with the CONNECT phase! */
      DEBUGF(infof(data, "protocol connect phase DONE\n"));
      break;

    case FTP_NAMEFMT:
      if(ftpcode == 250) {
        /* Name format change successful: reload initial path. */
        ftp_state_pwd(conn);
        break;
      }

      state(conn, FTP_STOP); /* we are done with the CONNECT phase! */
      DEBUGF(infof(data, "protocol connect phase DONE\n"));
      break;

    case FTP_QUOTE:
    case FTP_POSTQUOTE:
    case FTP_RETR_PREQUOTE:
    case FTP_STOR_PREQUOTE:
      if(ftpcode >= 400) {
        failf(conn->data, "QUOT command failed with %03d", ftpcode);
        return CURLE_QUOTE_ERROR;
      }
      result = ftp_state_quote(conn, FALSE, ftpc->state);
      if(result)
        return result;

      break;

    case FTP_CWD:
      if(ftpcode/100 != 2) {
        /* failure to CWD there */
        if(conn->data->set.ftp_create_missing_dirs &&
           ftpc->count1 && !ftpc->count2) {
          /* try making it */
          ftpc->count2++; /* counter to prevent CWD-MKD loops */
          NBFTPSENDF(conn, "MKD %s", ftpc->dirs[ftpc->count1 - 1]);
          state(conn, FTP_MKD);
        }
        else {
          /* return failure */
          failf(data, "Server denied you to change to the given directory");
          ftpc->cwdfail = TRUE; /* don't remember this path as we failed
                                   to enter it */
          return CURLE_REMOTE_ACCESS_DENIED;
        }
      }
      else {
        /* success */
        ftpc->count2=0;
        if(++ftpc->count1 <= ftpc->dirdepth) {
          /* send next CWD */
          NBFTPSENDF(conn, "CWD %s", ftpc->dirs[ftpc->count1 - 1]);
        }
        else {
          result = ftp_state_post_cwd(conn);
          if(result)
            return result;
        }
      }
      break;

    case FTP_MKD:
      if((ftpcode/100 != 2) && !ftpc->count3--) {
        /* failure to MKD the dir */
        failf(data, "Failed to MKD dir: %03d", ftpcode);
        return CURLE_REMOTE_ACCESS_DENIED;
      }
      state(conn, FTP_CWD);
      /* send CWD */
      NBFTPSENDF(conn, "CWD %s", ftpc->dirs[ftpc->count1 - 1]);
      break;

    case FTP_MDTM:
      result = ftp_state_mdtm_resp(conn, ftpcode);
      break;

    case FTP_TYPE:
    case FTP_LIST_TYPE:
    case FTP_RETR_TYPE:
    case FTP_STOR_TYPE:
      result = ftp_state_type_resp(conn, ftpcode, ftpc->state);
      break;

    case FTP_SIZE:
    case FTP_RETR_SIZE:
    case FTP_STOR_SIZE:
      result = ftp_state_size_resp(conn, ftpcode, ftpc->state);
      break;

    case FTP_REST:
    case FTP_RETR_REST:
      result = ftp_state_rest_resp(conn, ftpcode, ftpc->state);
      break;

    case FTP_PASV:
      result = ftp_state_pasv_resp(conn, ftpcode);
      break;

    case FTP_PORT:
      result = ftp_state_port_resp(conn, ftpcode);
      break;

    case FTP_LIST:
    case FTP_RETR:
      result = ftp_state_get_resp(conn, ftpcode, ftpc->state);
      break;

    case FTP_STOR:
      result = ftp_state_stor_resp(conn, ftpcode);
      break;

    case FTP_QUIT:
      /* fallthrough, just stop! */
    default:
      /* internal error */
      state(conn, FTP_STOP);
      break;
    }
  } /* if(ftpcode) */

  return result;
}

/* Returns timeout in ms. 0 or negative number means the timeout has already
   triggered */
static long ftp_state_timeout(struct connectdata *conn)
{
  struct SessionHandle *data=conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  long timeout_ms=360000; /* in milliseconds */

  if(data->set.ftp_response_timeout )
    /* if CURLOPT_FTP_RESPONSE_TIMEOUT is set, use that to determine remaining
       time.  Also, use ftp->response because FTP_RESPONSE_TIMEOUT is supposed
       to govern the response for any given ftp response, not for the time
       from connect to the given ftp response. */
    timeout_ms = data->set.ftp_response_timeout - /* timeout time */
      Curl_tvdiff(Curl_tvnow(), ftpc->response); /* spent time */
  else if(data->set.timeout)
    /* if timeout is requested, find out how much remaining time we have */
    timeout_ms = data->set.timeout - /* timeout time */
      Curl_tvdiff(Curl_tvnow(), conn->now); /* spent time */
  else
    /* Without a requested timeout, we only wait 'response_time' seconds for
       the full response to arrive before we bail out */
    timeout_ms = ftpc->response_time -
      Curl_tvdiff(Curl_tvnow(), ftpc->response); /* spent time */

  return timeout_ms;
}


/* called repeatedly until done from multi.c */
static CURLcode ftp_multi_statemach(struct connectdata *conn,
                                         bool *done)
{
  curl_socket_t sock = conn->sock[FIRSTSOCKET];
  int rc;
  struct SessionHandle *data=conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result = CURLE_OK;
  long timeout_ms = ftp_state_timeout(conn);

  *done = FALSE; /* default to not done yet */

  if(timeout_ms <= 0) {
    failf(data, "FTP response timeout");
    return CURLE_OPERATION_TIMEDOUT;
  }

  rc = Curl_socket_ready(ftpc->sendleft?CURL_SOCKET_BAD:sock, /* reading */
                         ftpc->sendleft?sock:CURL_SOCKET_BAD, /* writing */
                         0);

  if(rc == -1) {
    failf(data, "select/poll error");
    return CURLE_OUT_OF_MEMORY;
  }
  else if(rc != 0) {
    result = ftp_statemach_act(conn);
  }
  /* if rc == 0, then select() timed out */

  /* Check for the state outside of the Curl_socket_ready() return code checks
     since at times we are in fact already in this state when this function
     gets called. */
  *done = (bool)(ftpc->state == FTP_STOP);

  return result;
}

static CURLcode ftp_easy_statemach(struct connectdata *conn)
{
  curl_socket_t sock = conn->sock[FIRSTSOCKET];
  int rc;
  struct SessionHandle *data=conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result = CURLE_OK;

  while(ftpc->state != FTP_STOP) {
    long timeout_ms = ftp_state_timeout(conn);

    if(timeout_ms <=0 ) {
      failf(data, "FTP response timeout");
      return CURLE_OPERATION_TIMEDOUT; /* already too little time */
    }

    rc = Curl_socket_ready(ftpc->sendleft?CURL_SOCKET_BAD:sock, /* reading */
                           ftpc->sendleft?sock:CURL_SOCKET_BAD, /* writing */
                           (int)timeout_ms);

    if(rc == -1) {
      failf(data, "select/poll error");
      return CURLE_OUT_OF_MEMORY;
    }
    else if(rc == 0) {
      result = CURLE_OPERATION_TIMEDOUT;
      break;
    }
    else {
      result = ftp_statemach_act(conn);
      if(result)
        break;
    }
  }

  return result;
}

/*
 * Allocate and initialize the struct FTP for the current SessionHandle.  If
 * need be.
 */
static CURLcode ftp_init(struct connectdata *conn)
{
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  if(!ftp) {
    ftp = data->state.proto.ftp = calloc(sizeof(struct FTP), 1);
    if(!ftp)
      return CURLE_OUT_OF_MEMORY;
  }

  /* get some initial data into the ftp struct */
  ftp->bytecountp = &data->req.bytecount;

  /* No need to duplicate user+password, the connectdata struct won't change
     during a session, but we re-init them here since on subsequent inits
     since the conn struct may have changed or been replaced.
  */
  ftp->user = conn->user;
  ftp->passwd = conn->passwd;
  if(TRUE == isBadFtpString(ftp->user))
    return CURLE_URL_MALFORMAT;
  if(TRUE == isBadFtpString(ftp->passwd))
    return CURLE_URL_MALFORMAT;

  return CURLE_OK;
}

/*
 * ftp_connect() should do everything that is to be considered a part of
 * the connection phase.
 *
 * The variable 'done' points to will be TRUE if the protocol-layer connect
 * phase is done when this function returns, or FALSE is not. When called as
 * a part of the easy interface, it will always be TRUE.
 */
static CURLcode ftp_connect(struct connectdata *conn,
                                 bool *done) /* see description above */
{
  CURLcode result;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  struct SessionHandle *data=conn->data;

  *done = FALSE; /* default to not done yet */

  /* If there already is a protocol-specific struct allocated for this
     sessionhandle, deal with it */
  Curl_reset_reqproto(conn);

  result = ftp_init(conn);
  if(CURLE_OK != result)
    return result;

  /* We always support persistant connections on ftp */
  conn->bits.close = FALSE;

  ftpc->response_time = RESP_TIMEOUT; /* set default response time-out */

#if !defined(CURL_DISABLE_HTTP) && !defined(CURL_DISABLE_PROXY)
  if(conn->bits.tunnel_proxy && conn->bits.httpproxy) {
    /* for FTP over HTTP proxy */
    struct HTTP http_proxy;
    struct FTP *ftp_save;

    /* BLOCKING */
    /* We want "seamless" FTP operations through HTTP proxy tunnel */

    /* Curl_proxyCONNECT is based on a pointer to a struct HTTP at the member
     * conn->proto.http; we want FTP through HTTP and we have to change the
     * member temporarily for connecting to the HTTP proxy. After
     * Curl_proxyCONNECT we have to set back the member to the original struct
     * FTP pointer
     */
    ftp_save = data->state.proto.ftp;
    memset(&http_proxy, 0, sizeof(http_proxy));
    data->state.proto.http = &http_proxy;

    result = Curl_proxyCONNECT(conn, FIRSTSOCKET,
                               conn->host.name, conn->remote_port);

    data->state.proto.ftp = ftp_save;

    if(CURLE_OK != result)
      return result;
  }
#endif /* !CURL_DISABLE_HTTP && !CURL_DISABLE_PROXY */

  if(conn->protocol & PROT_FTPS) {
    /* BLOCKING */
    /* FTPS is simply ftp with SSL for the control channel */
    /* now, perform the SSL initialization for this socket */
    result = Curl_ssl_connect(conn, FIRSTSOCKET);
    if(result)
      return result;
  }

  /* When we connect, we start in the state where we await the 220
     response */
  ftp_respinit(conn); /* init the response reader stuff */
  state(conn, FTP_WAIT220);
  ftpc->response = Curl_tvnow(); /* start response time-out now! */

  if(data->state.used_interface == Curl_if_multi)
    result = ftp_multi_statemach(conn, done);
  else {
    result = ftp_easy_statemach(conn);
    if(!result)
      *done = TRUE;
  }

  return result;
}

/***********************************************************************
 *
 * ftp_done()
 *
 * The DONE function. This does what needs to be done after a single DO has
 * performed.
 *
 * Input argument is already checked for validity.
 */
static CURLcode ftp_done(struct connectdata *conn, CURLcode status,
                              bool premature)
{
  struct SessionHandle *data = conn->data;
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  ssize_t nread;
  int ftpcode;
  CURLcode result=CURLE_OK;
  bool was_ctl_valid = ftpc->ctl_valid;
  char *path;
  const char *path_to_use = data->state.path;

  if(!ftp)
    /* When the easy handle is removed from the multi while libcurl is still
     * trying to resolve the host name, it seems that the ftp struct is not
     * yet initialized, but the removal action calls Curl_done() which calls
     * this function. So we simply return success if no ftp pointer is set.
     */
    return CURLE_OK;

  switch(status) {
  case CURLE_BAD_DOWNLOAD_RESUME:
  case CURLE_FTP_WEIRD_PASV_REPLY:
  case CURLE_FTP_PORT_FAILED:
  case CURLE_FTP_COULDNT_SET_TYPE:
  case CURLE_FTP_COULDNT_RETR_FILE:
  case CURLE_UPLOAD_FAILED:
  case CURLE_REMOTE_ACCESS_DENIED:
  case CURLE_FILESIZE_EXCEEDED:
  case CURLE_REMOTE_FILE_NOT_FOUND:
  case CURLE_WRITE_ERROR:
    /* the connection stays alive fine even though this happened */
    /* fall-through */
  case CURLE_OK: /* doesn't affect the control connection's status */
    if(!premature) {
      ftpc->ctl_valid = was_ctl_valid;
      break;
    }
    /* until we cope better with prematurely ended requests, let them
     * fallback as if in complete failure */
  default:       /* by default, an error means the control connection is
                    wedged and should not be used anymore */
    ftpc->ctl_valid = FALSE;
    ftpc->cwdfail = TRUE; /* set this TRUE to prevent us to remember the
                             current path, as this connection is going */
    conn->bits.close = TRUE; /* marked for closure */
    result = status;      /* use the already set error code */
    break;
  }

  /* now store a copy of the directory we are in */
  if(ftpc->prevpath)
    free(ftpc->prevpath);

  /* get the "raw" path */
  path = curl_easy_unescape(data, path_to_use, 0, NULL);
  if(!path) {
    /* out of memory, but we can limp along anyway (and should try to
     * since we're in the out of memory cleanup path) */
    ftpc->prevpath = NULL; /* no path */

  } else {
    size_t flen = ftpc->file?strlen(ftpc->file):0; /* file is "raw" already */
    size_t dlen = strlen(path)-flen;
    if(!ftpc->cwdfail) {
      if(dlen && (data->set.ftp_filemethod != FTPFILE_NOCWD)) {
        ftpc->prevpath = path;
        if(flen)
          /* if 'path' is not the whole string */
          ftpc->prevpath[dlen]=0; /* terminate */
      }
      else {
        /* we never changed dir */
        ftpc->prevpath=strdup("");
        free(path);
      }
      if(ftpc->prevpath)
        infof(data, "Remembering we are in dir \"%s\"\n", ftpc->prevpath);
    }
    else {
      ftpc->prevpath = NULL; /* no path */
      free(path);
    }
  }
  /* free the dir tree and file parts */
  freedirs(ftpc);

#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  Curl_sec_fflush_fd(conn, conn->sock[SECONDARYSOCKET]);
#endif

  /* shut down the socket to inform the server we're done */

#ifdef _WIN32_WCE
  shutdown(conn->sock[SECONDARYSOCKET],2);  /* SD_BOTH */
#endif

  if(conn->sock[SECONDARYSOCKET] != CURL_SOCKET_BAD) {
    if(conn->ssl[SECONDARYSOCKET].use) {
      /* The secondary socket is using SSL so we must close down that part first
         before we close the socket for real */
      Curl_ssl_close(conn, SECONDARYSOCKET);

      /* Note that we keep "use" set to TRUE since that (next) connection is
         still requested to use SSL */
    }
    if(CURL_SOCKET_BAD != conn->sock[SECONDARYSOCKET]) {
      sclose(conn->sock[SECONDARYSOCKET]);
      conn->sock[SECONDARYSOCKET] = CURL_SOCKET_BAD;
    }
  }

  if((ftp->transfer == FTPTRANSFER_BODY) && ftpc->ctl_valid &&
     ftpc->pending_resp && !premature) {
    /*
     * Let's see what the server says about the transfer we just performed,
     * but lower the timeout as sometimes this connection has died while the
     * data has been transfered. This happens when doing through NATs etc that
     * abandon old silent connections.
     */
    long old_time = ftpc->response_time;

    ftpc->response_time = 60*1000; /* give it only a minute for now */

    result = Curl_GetFTPResponse(&nread, conn, &ftpcode);

    ftpc->response_time = old_time; /* set this back to previous value */

    if(!nread && (CURLE_OPERATION_TIMEDOUT == result)) {
      failf(data, "control connection looks dead");
      ftpc->ctl_valid = FALSE; /* mark control connection as bad */
      conn->bits.close = TRUE; /* mark for closure */
    }

    if(result)
      return result;

    if(!ftpc->dont_check) {
      /* 226 Transfer complete, 250 Requested file action okay, completed. */
      if((ftpcode != 226) && (ftpcode != 250)) {
        failf(data, "server did not report OK, got %d", ftpcode);
        result = CURLE_PARTIAL_FILE;
      }
    }
  }

  if(result || premature)
    /* the response code from the transfer showed an error already so no
       use checking further */
    ;
  else if(data->set.upload) {
    if((-1 != data->set.infilesize) &&
       (data->set.infilesize != *ftp->bytecountp) &&
       !data->set.crlf &&
       (ftp->transfer == FTPTRANSFER_BODY)) {
      failf(data, "Uploaded unaligned file size (%" FORMAT_OFF_T
            " out of %" FORMAT_OFF_T " bytes)",
            *ftp->bytecountp, data->set.infilesize);
      result = CURLE_PARTIAL_FILE;
    }
  }
  else {
    if((-1 != data->req.size) &&
       (data->req.size != *ftp->bytecountp) &&
#ifdef CURL_DO_LINEEND_CONV
       /* Most FTP servers don't adjust their file SIZE response for CRLFs, so
        * we'll check to see if the discrepancy can be explained by the number
        * of CRLFs we've changed to LFs.
        */
       ((data->req.size + data->state.crlf_conversions) !=
        *ftp->bytecountp) &&
#endif /* CURL_DO_LINEEND_CONV */
       (data->req.maxdownload != *ftp->bytecountp)) {
      failf(data, "Received only partial file: %" FORMAT_OFF_T " bytes",
            *ftp->bytecountp);
      result = CURLE_PARTIAL_FILE;
    }
    else if(!ftpc->dont_check &&
            !*ftp->bytecountp &&
            (data->req.size>0)) {
      failf(data, "No data was received!");
      result = CURLE_FTP_COULDNT_RETR_FILE;
    }
  }

  /* clear these for next connection */
  ftp->transfer = FTPTRANSFER_BODY;
  ftpc->dont_check = FALSE;

  /* Send any post-transfer QUOTE strings? */
  if(!status && !result && !premature && data->set.postquote)
    result = ftp_sendquote(conn, data->set.postquote);

  return result;
}

/***********************************************************************
 *
 * ftp_sendquote()
 *
 * Where a 'quote' means a list of custom commands to send to the server.
 * The quote list is passed as an argument.
 *
 * BLOCKING
 */

static
CURLcode ftp_sendquote(struct connectdata *conn, struct curl_slist *quote)
{
  struct curl_slist *item;
  ssize_t nread;
  int ftpcode;
  CURLcode result;

  item = quote;
  while(item) {
    if(item->data) {
      char *cmd = item->data;
      bool acceptfail = FALSE;

      /* if a command starts with an asterisk, which a legal FTP command never
         can, the command will be allowed to fail without it causing any
         aborts or cancels etc. It will cause libcurl to act as if the command
         is successful, whatever the server reponds. */

      if(cmd[0] == '*') {
        cmd++;
        acceptfail = TRUE;
      }

      FTPSENDF(conn, "%s", cmd);

      result = Curl_GetFTPResponse(&nread, conn, &ftpcode);
      if(result)
        return result;

      if(!acceptfail && (ftpcode >= 400)) {
        failf(conn->data, "QUOT string not accepted: %s", cmd);
        return CURLE_QUOTE_ERROR;
      }
    }

    item = item->next;
  }

  return CURLE_OK;
}

/***********************************************************************
 *
 * ftp_need_type()
 *
 * Returns TRUE if we in the current situation should send TYPE
 */
static int ftp_need_type(struct connectdata *conn,
                         bool ascii_wanted)
{
  return conn->proto.ftpc.transfertype != (ascii_wanted?'A':'I');
}

/***********************************************************************
 *
 * ftp_nb_type()
 *
 * Set TYPE. We only deal with ASCII or BINARY so this function
 * sets one of them.
 * If the transfer type is not sent, simulate on OK response in newstate
 */
static CURLcode ftp_nb_type(struct connectdata *conn,
                            bool ascii, ftpstate newstate)
{
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result;
  char want = (char)(ascii?'A':'I');

  if(ftpc->transfertype == want) {
    state(conn, newstate);
    return ftp_state_type_resp(conn, 200, newstate);
  }

  NBFTPSENDF(conn, "TYPE %c", want);
  state(conn, newstate);

  /* keep track of our current transfer type */
  ftpc->transfertype = want;
  return CURLE_OK;
}

/***************************************************************************
 *
 * ftp_pasv_verbose()
 *
 * This function only outputs some informationals about this second connection
 * when we've issued a PASV command before and thus we have connected to a
 * possibly new IP address.
 *
 */
#ifndef CURL_DISABLE_VERBOSE_STRINGS
static void
ftp_pasv_verbose(struct connectdata *conn,
                 Curl_addrinfo *ai,
                 char *newhost, /* ascii version */
                 int port)
{
  char buf[256];
  Curl_printable_address(ai, buf, sizeof(buf));
  infof(conn->data, "Connecting to %s (%s) port %d\n", newhost, buf, port);
}
#endif

/*
  Check if this is a range download, and if so, set the internal variables
  properly.
 */

static CURLcode ftp_range(struct connectdata *conn)
{
  curl_off_t from, to;
  curl_off_t totalsize=-1;
  char *ptr;
  char *ptr2;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if(data->state.use_range && data->state.range) {
    from=curlx_strtoofft(data->state.range, &ptr, 0);
    while(ptr && *ptr && (ISSPACE(*ptr) || (*ptr=='-')))
      ptr++;
    to=curlx_strtoofft(ptr, &ptr2, 0);
    if(ptr == ptr2) {
      /* we didn't get any digit */
      to=-1;
    }
    if((-1 == to) && (from>=0)) {
      /* X - */
      data->state.resume_from = from;
      DEBUGF(infof(conn->data, "FTP RANGE %" FORMAT_OFF_T " to end of file\n",
                   from));
    }
    else if(from < 0) {
      /* -Y */
      totalsize = -from;
      data->req.maxdownload = -from;
      data->state.resume_from = from;
      DEBUGF(infof(conn->data, "FTP RANGE the last %" FORMAT_OFF_T " bytes\n",
                   totalsize));
    }
    else {
      /* X-Y */
      totalsize = to-from;
      data->req.maxdownload = totalsize+1; /* include last byte */
      data->state.resume_from = from;
      DEBUGF(infof(conn->data, "FTP RANGE from %" FORMAT_OFF_T
                   " getting %" FORMAT_OFF_T " bytes\n",
                   from, data->req.maxdownload));
    }
    DEBUGF(infof(conn->data, "range-download from %" FORMAT_OFF_T
                 " to %" FORMAT_OFF_T ", totally %" FORMAT_OFF_T " bytes\n",
                 from, to, data->req.maxdownload));
    ftpc->dont_check = TRUE; /* dont check for successful transfer */
  }
  else
    data->req.maxdownload = -1;
  return CURLE_OK;
}


/*
 * ftp_nextconnect()
 *
 * This function shall be called when the second FTP (data) connection is
 * connected.
 */

static CURLcode ftp_nextconnect(struct connectdata *conn)
{
  struct SessionHandle *data=conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  CURLcode result = CURLE_OK;

  /* the ftp struct is inited in ftp_connect() */
  struct FTP *ftp = data->state.proto.ftp;

  DEBUGF(infof(data, "DO-MORE phase starts\n"));

  if(ftp->transfer <= FTPTRANSFER_INFO) {
    /* a transfer is about to take place, or if not a file name was given
       so we'll do a SIZE on it later and then we need the right TYPE first */

    if(data->set.upload) {
      result = ftp_nb_type(conn, data->set.prefer_ascii, FTP_STOR_TYPE);
      if(result)
        return result;
    }
    else {
      /* download */
      ftp->downloadsize = -1; /* unknown as of yet */

      result = ftp_range(conn);
      if(result)
        ;
      else if(data->set.ftp_list_only || !ftpc->file) {
        /* The specified path ends with a slash, and therefore we think this
           is a directory that is requested, use LIST. But before that we
           need to set ASCII transfer mode. */

        /* But only if a body transfer was requested. */
        if(ftp->transfer == FTPTRANSFER_BODY) {
          result = ftp_nb_type(conn, TRUE, FTP_LIST_TYPE);
          if(result)
            return result;
        }
        /* otherwise just fall through */
      }
      else {
        result = ftp_nb_type(conn, data->set.prefer_ascii, FTP_RETR_TYPE);
        if(result)
          return result;
      }
    }
    result = ftp_easy_statemach(conn);
  }

  if((result == CURLE_OK) && (ftp->transfer != FTPTRANSFER_BODY))
    /* no data to transfer. FIX: it feels like a kludge to have this here
       too! */
    result = Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);

  /* end of transfer */
  DEBUGF(infof(data, "DO-MORE phase ends with %d\n", result));

  return result;
}



/***********************************************************************
 *
 * ftp_perform()
 *
 * This is the actual DO function for FTP. Get a file/directory according to
 * the options previously setup.
 */

static
CURLcode ftp_perform(struct connectdata *conn,
                     bool *connected,  /* connect status after PASV / PORT */
                     bool *dophase_done)
{
  /* this is FTP and no proxy */
  CURLcode result=CURLE_OK;

  DEBUGF(infof(conn->data, "DO phase starts\n"));

  if(conn->data->set.opt_no_body) {
    /* requested no body means no transfer... */
    struct FTP *ftp = conn->data->state.proto.ftp;
    ftp->transfer = FTPTRANSFER_INFO;
  }


  *dophase_done = FALSE; /* not done yet */

  /* start the first command in the DO phase */
  result = ftp_state_quote(conn, TRUE, FTP_QUOTE);
  if(result)
    return result;

  /* run the state-machine */
  if(conn->data->state.used_interface == Curl_if_multi)
    result = ftp_multi_statemach(conn, dophase_done);
  else {
    result = ftp_easy_statemach(conn);
    *dophase_done = TRUE; /* with the easy interface we are done here */
  }
  *connected = conn->bits.tcpconnect;

  if(*dophase_done)
    DEBUGF(infof(conn->data, "DO phase is complete\n"));

  return result;
}

/***********************************************************************
 *
 * ftp_do()
 *
 * This function is registered as 'curl_do' function. It decodes the path
 * parts etc as a wrapper to the actual DO function (ftp_perform).
 *
 * The input argument is already checked for validity.
 */
static CURLcode ftp_do(struct connectdata *conn, bool *done)
{
  CURLcode retcode = CURLE_OK;

  *done = FALSE; /* default to false */

  /*
    Since connections can be re-used between SessionHandles, this might be a
    connection already existing but on a fresh SessionHandle struct so we must
    make sure we have a good 'struct FTP' to play with. For new connections,
    the struct FTP is allocated and setup in the ftp_connect() function.
  */
  Curl_reset_reqproto(conn);
  retcode = ftp_init(conn);
  if(retcode)
    return retcode;

  retcode = ftp_parse_url_path(conn);
  if(retcode)
    return retcode;

  retcode = ftp_regular_transfer(conn, done);

  return retcode;
}

/***********************************************************************
 *
 * Curl_(nb)ftpsendf()
 *
 * Sends the formated string as a ftp command to a ftp server
 *
 * NOTE: we build the command in a fixed-length buffer, which sets length
 * restrictions on the command!
 *
 * The "nb" version is made to Never Block.
 */
CURLcode Curl_nbftpsendf(struct connectdata *conn,
                       const char *fmt, ...)
{
  ssize_t bytes_written;
/* may still not be big enough for some krb5 tokens */
#define SBUF_SIZE 1024
  char s[SBUF_SIZE];
  size_t write_len;
  char *sptr=s;
  CURLcode res = CURLE_OK;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  enum protection_level data_sec = conn->data_prot;
#endif

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(s, SBUF_SIZE-3, fmt, ap);
  va_end(ap);

  strcat(s, "\r\n"); /* append a trailing CRLF */

  bytes_written=0;
  write_len = strlen(s);

  ftp_respinit(conn);

#ifdef CURL_DOES_CONVERSIONS
  res = Curl_convert_to_network(data, s, write_len);
  /* Curl_convert_to_network calls failf if unsuccessful */
  if(res != CURLE_OK) {
    return res;
  }
#endif /* CURL_DOES_CONVERSIONS */

#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  conn->data_prot = prot_cmd;
#endif
  res = Curl_write(conn, conn->sock[FIRSTSOCKET], sptr, write_len,
                   &bytes_written);
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  conn->data_prot = data_sec;
#endif

  if(CURLE_OK != res)
    return res;

  if(conn->data->set.verbose)
    Curl_debug(conn->data, CURLINFO_HEADER_OUT,
               sptr, (size_t)bytes_written, conn);

  if(bytes_written != (ssize_t)write_len) {
    /* the whole chunk was not sent, store the rest of the data */
    write_len -= bytes_written;
    sptr += bytes_written;
    ftpc->sendthis = malloc(write_len);
    if(ftpc->sendthis) {
      memcpy(ftpc->sendthis, sptr, write_len);
      ftpc->sendsize = ftpc->sendleft = write_len;
    }
    else {
      failf(data, "out of memory");
      res = CURLE_OUT_OF_MEMORY;
    }
  }
  else
    ftpc->response = Curl_tvnow();

  return res;
}

CURLcode Curl_ftpsendf(struct connectdata *conn,
                       const char *fmt, ...)
{
  ssize_t bytes_written;
  char s[SBUF_SIZE];
  size_t write_len;
  char *sptr=s;
  CURLcode res = CURLE_OK;
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  enum protection_level data_sec = conn->data_prot;
#endif

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(s, SBUF_SIZE-3, fmt, ap);
  va_end(ap);

  strcat(s, "\r\n"); /* append a trailing CRLF */

  bytes_written=0;
  write_len = strlen(s);

#ifdef CURL_DOES_CONVERSIONS
  res = Curl_convert_to_network(conn->data, s, write_len);
  /* Curl_convert_to_network calls failf if unsuccessful */
  if(res != CURLE_OK) {
    return(res);
  }
#endif /* CURL_DOES_CONVERSIONS */

  while(1) {
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
    conn->data_prot = prot_cmd;
#endif
    res = Curl_write(conn, conn->sock[FIRSTSOCKET], sptr, write_len,
                     &bytes_written);
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
    conn->data_prot = data_sec;
#endif

    if(CURLE_OK != res)
      break;

    if(conn->data->set.verbose)
      Curl_debug(conn->data, CURLINFO_HEADER_OUT,
                 sptr, (size_t)bytes_written, conn);

    if(bytes_written != (ssize_t)write_len) {
      write_len -= bytes_written;
      sptr += bytes_written;
    }
    else
      break;
  }

  return res;
}

/***********************************************************************
 *
 * ftp_quit()
 *
 * This should be called before calling sclose() on an ftp control connection
 * (not data connections). We should then wait for the response from the
 * server before returning. The calling code should then try to close the
 * connection.
 *
 */
static CURLcode ftp_quit(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

  if(conn->proto.ftpc.ctl_valid) {
    NBFTPSENDF(conn, "QUIT", NULL);
    state(conn, FTP_QUIT);

    result = ftp_easy_statemach(conn);
  }

  return result;
}

/***********************************************************************
 *
 * ftp_disconnect()
 *
 * Disconnect from an FTP server. Cleanup protocol-specific per-connection
 * resources. BLOCKING.
 */
static CURLcode ftp_disconnect(struct connectdata *conn)
{
  struct ftp_conn *ftpc= &conn->proto.ftpc;

  /* We cannot send quit unconditionally. If this connection is stale or
     bad in any way, sending quit and waiting around here will make the
     disconnect wait in vain and cause more problems than we need to.

     ftp_quit() will check the state of ftp->ctl_valid. If it's ok it
     will try to send the QUIT command, otherwise it will just return.
  */

  /* The FTP session may or may not have been allocated/setup at this point! */
  (void)ftp_quit(conn); /* ignore errors on the QUIT */

  if(ftpc->entrypath) {
    struct SessionHandle *data = conn->data;
    if(data->state.most_recent_ftp_entrypath == ftpc->entrypath) {
      data->state.most_recent_ftp_entrypath = NULL;
    }
    free(ftpc->entrypath);
    ftpc->entrypath = NULL;
  }
  if(ftpc->cache) {
    free(ftpc->cache);
    ftpc->cache = NULL;
  }
  freedirs(ftpc);
  if(ftpc->prevpath) {
    free(ftpc->prevpath);
    ftpc->prevpath = NULL;
  }
  if(ftpc->server_os) {
    free(ftpc->server_os);
    ftpc->server_os = NULL;
  }

  return CURLE_OK;
}

/***********************************************************************
 *
 * ftp_parse_url_path()
 *
 * Parse the URL path into separate path components.
 *
 */
static
CURLcode ftp_parse_url_path(struct connectdata *conn)
{
  struct SessionHandle *data = conn->data;
  /* the ftp struct is already inited in ftp_connect() */
  struct FTP *ftp = data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  const char *slash_pos;  /* position of the first '/' char in curpos */
  const char *path_to_use = data->state.path;
  const char *cur_pos;
  const char *filename = NULL;

  cur_pos = path_to_use; /* current position in path. point at the begin
                            of next path component */

  ftpc->ctl_valid = FALSE;
  ftpc->cwdfail = FALSE;

  switch(data->set.ftp_filemethod) {
  case FTPFILE_NOCWD:
    /* fastest, but less standard-compliant */

    /*
      The best time to check whether the path is a file or directory is right
      here. so:

      the first condition in the if() right here, is there just in case
      someone decides to set path to NULL one day
   */
    if(data->state.path &&
       data->state.path[0] &&
       (data->state.path[strlen(data->state.path) - 1] != '/') )
      filename = data->state.path;  /* this is a full file path */
      /*
        ftpc->file is not used anywhere other than for operations on a file.
        In other words, never for directory operations.
        So we can safely leave filename as NULL here and use it as a
        argument in dir/file decisions.
      */
    break;

  case FTPFILE_SINGLECWD:
    /* get the last slash */
    if(!path_to_use[0]) {
      /* no dir, no file */
      ftpc->dirdepth = 0;
      break;
    }
    slash_pos=strrchr(cur_pos, '/');
    if(slash_pos || !*cur_pos) {
      ftpc->dirs = calloc(1, sizeof(ftpc->dirs[0]));
      if(!ftpc->dirs)
        return CURLE_OUT_OF_MEMORY;

      ftpc->dirs[0] = curl_easy_unescape(conn->data, slash_pos ? cur_pos : "/",
                                         slash_pos?(int)(slash_pos-cur_pos):1,
                                         NULL);
      if(!ftpc->dirs[0]) {
        freedirs(ftpc);
        return CURLE_OUT_OF_MEMORY;
      }
      ftpc->dirdepth = 1; /* we consider it to be a single dir */
      filename = slash_pos ? slash_pos+1 : cur_pos; /* rest is file name */
    }
    else
      filename = cur_pos;  /* this is a file name only */
    break;

  default: /* allow pretty much anything */
  case FTPFILE_MULTICWD:
    ftpc->dirdepth = 0;
    ftpc->diralloc = 5; /* default dir depth to allocate */
    ftpc->dirs = calloc(ftpc->diralloc, sizeof(ftpc->dirs[0]));
    if(!ftpc->dirs)
      return CURLE_OUT_OF_MEMORY;

    /* we have a special case for listing the root dir only */
    if(strequal(path_to_use, "/")) {
      cur_pos++; /* make it point to the zero byte */
      ftpc->dirs[0] = strdup("/");
      ftpc->dirdepth++;
    }
    else {
      /* parse the URL path into separate path components */
      while((slash_pos = strchr(cur_pos, '/')) != NULL) {
        /* 1 or 0 to indicate absolute directory */
        bool absolute_dir = (bool)((cur_pos - data->state.path > 0) &&
                                   (ftpc->dirdepth == 0));

        /* seek out the next path component */
        if(slash_pos-cur_pos) {
          /* we skip empty path components, like "x//y" since the FTP command
             CWD requires a parameter and a non-existant parameter a) doesn't
             work on many servers and b) has no effect on the others. */
          int len = (int)(slash_pos - cur_pos + absolute_dir);
          ftpc->dirs[ftpc->dirdepth] =
            curl_easy_unescape(conn->data, cur_pos - absolute_dir, len, NULL);
          if(!ftpc->dirs[ftpc->dirdepth]) { /* run out of memory ... */
            failf(data, "no memory");
            freedirs(ftpc);
            return CURLE_OUT_OF_MEMORY;
          }
          if(isBadFtpString(ftpc->dirs[ftpc->dirdepth])) {
            free(ftpc->dirs[ftpc->dirdepth]);
            freedirs(ftpc);
            return CURLE_URL_MALFORMAT;
          }
        }
        else {
          cur_pos = slash_pos + 1; /* jump to the rest of the string */
          continue;
        }

        cur_pos = slash_pos + 1; /* jump to the rest of the string */
        if(++ftpc->dirdepth >= ftpc->diralloc) {
          /* enlarge array */
          char *bigger;
          ftpc->diralloc *= 2; /* double the size each time */
          bigger = realloc(ftpc->dirs, ftpc->diralloc * sizeof(ftpc->dirs[0]));
          if(!bigger) {
            freedirs(ftpc);
            return CURLE_OUT_OF_MEMORY;
          }
          ftpc->dirs = (char **)bigger;
        }
      }
    }
    filename = cur_pos;  /* the rest is the file name */
    break;
  } /* switch */

  if(filename && *filename) {
    ftpc->file = curl_easy_unescape(conn->data, filename, 0, NULL);
    if(NULL == ftpc->file) {
      freedirs(ftpc);
      failf(data, "no memory");
      return CURLE_OUT_OF_MEMORY;
    }
    if(isBadFtpString(ftpc->file)) {
      freedirs(ftpc);
      return CURLE_URL_MALFORMAT;
    }
  }
  else
    ftpc->file=NULL; /* instead of point to a zero byte, we make it a NULL
                       pointer */

  if(data->set.upload && !ftpc->file && (ftp->transfer == FTPTRANSFER_BODY)) {
    /* We need a file name when uploading. Return error! */
    failf(data, "Uploading to a URL without a file name!");
    return CURLE_URL_MALFORMAT;
  }

  ftpc->cwddone = FALSE; /* default to not done */

  if(ftpc->prevpath) {
    /* prevpath is "raw" so we convert the input path before we compare the
       strings */
    int dlen;
    char *path = curl_easy_unescape(conn->data, data->state.path, 0, &dlen);
    if(!path) {
      freedirs(ftpc);
      return CURLE_OUT_OF_MEMORY;
    }

    dlen -= ftpc->file?(int)strlen(ftpc->file):0;
    if((dlen == (int)strlen(ftpc->prevpath)) &&
       strnequal(path, ftpc->prevpath, dlen)) {
      infof(data, "Request has same path as previous transfer\n");
      ftpc->cwddone = TRUE;
    }
    free(path);
  }

  return CURLE_OK;
}

/* call this when the DO phase has completed */
static CURLcode ftp_dophase_done(struct connectdata *conn,
                                 bool connected)
{
  CURLcode result = CURLE_OK;
  struct FTP *ftp = conn->data->state.proto.ftp;
  struct ftp_conn *ftpc = &conn->proto.ftpc;

  if(connected)
    result = ftp_nextconnect(conn);

  if(result && (conn->sock[SECONDARYSOCKET] != CURL_SOCKET_BAD)) {
    /* Failure detected, close the second socket if it was created already */
    sclose(conn->sock[SECONDARYSOCKET]);
    conn->sock[SECONDARYSOCKET] = CURL_SOCKET_BAD;
    return result;
  }

  if(ftp->transfer != FTPTRANSFER_BODY)
    /* no data to transfer */
    result=Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);
  else if(!connected)
    /* since we didn't connect now, we want do_more to get called */
    conn->bits.do_more = TRUE;

  ftpc->ctl_valid = TRUE; /* seems good */

  return result;
}

/* called from multi.c while DOing */
static CURLcode ftp_doing(struct connectdata *conn,
                               bool *dophase_done)
{
  CURLcode result;
  result = ftp_multi_statemach(conn, dophase_done);

  if(*dophase_done) {
    result = ftp_dophase_done(conn, FALSE /* not connected */);

    DEBUGF(infof(conn->data, "DO phase is complete\n"));
  }
  return result;
}

/***********************************************************************
 *
 * ftp_regular_transfer()
 *
 * The input argument is already checked for validity.
 *
 * Performs all commands done before a regular transfer between a local and a
 * remote host.
 *
 * ftp->ctl_valid starts out as FALSE, and gets set to TRUE if we reach the
 * ftp_done() function without finding any major problem.
 */
static
CURLcode ftp_regular_transfer(struct connectdata *conn,
                              bool *dophase_done)
{
  CURLcode result=CURLE_OK;
  bool connected=FALSE;
  struct SessionHandle *data = conn->data;
  struct ftp_conn *ftpc = &conn->proto.ftpc;
  data->req.size = -1; /* make sure this is unknown at this point */

  Curl_pgrsSetUploadCounter(data, 0);
  Curl_pgrsSetDownloadCounter(data, 0);
  Curl_pgrsSetUploadSize(data, 0);
  Curl_pgrsSetDownloadSize(data, 0);

  ftpc->ctl_valid = TRUE; /* starts good */

  result = ftp_perform(conn,
                       &connected, /* have we connected after PASV/PORT */
                       dophase_done); /* all commands in the DO-phase done? */

  if(CURLE_OK == result) {

    if(!*dophase_done)
      /* the DO phase has not completed yet */
      return CURLE_OK;

    result = ftp_dophase_done(conn, connected);
    if(result)
      return result;
  }
  else
    freedirs(ftpc);

  return result;
}

static CURLcode ftp_setup_connection(struct connectdata * conn)
{
  struct SessionHandle *data = conn->data;
  char * type;
  char command;

  if(conn->bits.httpproxy && !data->set.tunnel_thru_httpproxy) {
    /* Unless we have asked to tunnel ftp operations through the proxy, we
       switch and use HTTP operations only */
#ifndef CURL_DISABLE_HTTP
    if(conn->handler == &Curl_handler_ftp)
      conn->handler = &Curl_handler_ftp_proxy;
    else {
#ifdef USE_SSL
      conn->handler = &Curl_handler_ftps_proxy;
#else
      failf(data, "FTPS not supported!");
      return CURLE_UNSUPPORTED_PROTOCOL;
#endif
    }
    /*
     * We explicitly mark this connection as persistent here as we're doing
     * FTP over HTTP and thus we accidentally avoid setting this value
     * otherwise.
     */
    conn->bits.close = FALSE;
#else
    failf(data, "FTP over http proxy requires HTTP support built-in!");
    return CURLE_UNSUPPORTED_PROTOCOL;
#endif
  }

  data->state.path++;   /* don't include the initial slash */

  /* FTP URLs support an extension like ";type=<typecode>" that
   * we'll try to get now! */
  type = strstr(data->state.path, ";type=");

  if(!type)
    type = strstr(conn->host.rawalloc, ";type=");

  if(type) {
    *type = 0;                     /* it was in the middle of the hostname */
    command = Curl_raw_toupper(type[6]);

    switch (command) {
    case 'A': /* ASCII mode */
      data->set.prefer_ascii = TRUE;
      break;

    case 'D': /* directory mode */
      data->set.ftp_list_only = TRUE;
      break;

    case 'I': /* binary mode */
    default:
      /* switch off ASCII */
      data->set.prefer_ascii = FALSE;
      break;
    }
  }

  return CURLE_OK;
}

#endif /* CURL_DISABLE_FTP */
