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
 * $Id: dict.c,v 1.63 2009-10-08 00:02:32 yangtse Exp $
 ***************************************************************************/

#include "setup.h"

#ifndef CURL_DISABLE_DICT

/* -- WIN32 approved -- */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef WIN32
#include <time.h>
#include <io.h>
#else
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <netdb.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif


#endif

#include "urldata.h"
#include <curl/curl.h>
#include "transfer.h"
#include "sendf.h"

#include "progress.h"
#include "strequal.h"
#include "dict.h"
#include "rawstr.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>

/* The last #include file should be: */
#include "memdebug.h"


/*
 * Forward declarations.
 */

static CURLcode dict_do(struct connectdata *conn, bool *done);

/*
 * DICT protocol handler.
 */

const struct Curl_handler Curl_handler_dict = {
  "DICT",                               /* scheme */
  ZERO_NULL,                            /* setup_connection */
  dict_do,                              /* do_it */
  ZERO_NULL,                            /* done */
  ZERO_NULL,                            /* do_more */
  ZERO_NULL,                            /* connect_it */
  ZERO_NULL,                            /* connecting */
  ZERO_NULL,                            /* doing */
  ZERO_NULL,                            /* proto_getsock */
  ZERO_NULL,                            /* doing_getsock */
  ZERO_NULL,                            /* perform_getsock */
  ZERO_NULL,                            /* disconnect */
  PORT_DICT,                            /* defport */
  PROT_DICT                             /* protocol */
};

static char *unescape_word(struct SessionHandle *data, const char *inputbuff)
{
  char *newp;
  char *dictp;
  char *ptr;
  int len;
  char byte;
  int olen=0;

  newp = curl_easy_unescape(data, inputbuff, 0, &len);
  if(!newp)
    return NULL;

  dictp = malloc(((size_t)len)*2 + 1); /* add one for terminating zero */
  if(dictp) {
    /* According to RFC2229 section 2.2, these letters need to be escaped with
       \[letter] */
    for(ptr = newp;
        (byte = *ptr) != 0;
        ptr++) {
      if((byte <= 32) || (byte == 127) ||
          (byte == '\'') || (byte == '\"') || (byte == '\\')) {
        dictp[olen++] = '\\';
      }
      dictp[olen++] = byte;
    }
    dictp[olen]=0;

    free(newp);
  }
  return dictp;
}

static CURLcode dict_do(struct connectdata *conn, bool *done)
{
  char *word;
  char *eword;
  char *ppath;
  char *database = NULL;
  char *strategy = NULL;
  char *nthdef = NULL; /* This is not part of the protocol, but required
                          by RFC 2229 */
  CURLcode result=CURLE_OK;
  struct SessionHandle *data=conn->data;
  curl_socket_t sockfd = conn->sock[FIRSTSOCKET];

  char *path = data->state.path;
  curl_off_t *bytecount = &data->req.bytecount;

  *done = TRUE; /* unconditionally */

  if(conn->bits.user_passwd) {
    /* AUTH is missing */
  }

  if(Curl_raw_nequal(path, DICT_MATCH, sizeof(DICT_MATCH)-1) ||
      Curl_raw_nequal(path, DICT_MATCH2, sizeof(DICT_MATCH2)-1) ||
      Curl_raw_nequal(path, DICT_MATCH3, sizeof(DICT_MATCH3)-1)) {

    word = strchr(path, ':');
    if(word) {
      word++;
      database = strchr(word, ':');
      if(database) {
        *database++ = (char)0;
        strategy = strchr(database, ':');
        if(strategy) {
          *strategy++ = (char)0;
          nthdef = strchr(strategy, ':');
          if(nthdef) {
            *nthdef = (char)0;
          }
        }
      }
    }

    if((word == NULL) || (*word == (char)0)) {
      infof(data, "lookup word is missing");
      word=(char *)"default";
    }
    if((database == NULL) || (*database == (char)0)) {
      database = (char *)"!";
    }
    if((strategy == NULL) || (*strategy == (char)0)) {
      strategy = (char *)".";
    }

    eword = unescape_word(data, word);
    if(!eword)
      return CURLE_OUT_OF_MEMORY;

    result = Curl_sendf(sockfd, conn,
                        "CLIENT " LIBCURL_NAME " " LIBCURL_VERSION "\r\n"
                        "MATCH "
                        "%s "    /* database */
                        "%s "    /* strategy */
                        "%s\r\n" /* word */
                        "QUIT\r\n",

                        database,
                        strategy,
                        eword
                        );

    free(eword);

    if(result)
      failf(data, "Failed sending DICT request");
    else
      result = Curl_setup_transfer(conn, FIRSTSOCKET, -1, FALSE, bytecount,
                                   -1, NULL); /* no upload */
    if(result)
      return result;
  }
  else if(Curl_raw_nequal(path, DICT_DEFINE, sizeof(DICT_DEFINE)-1) ||
           Curl_raw_nequal(path, DICT_DEFINE2, sizeof(DICT_DEFINE2)-1) ||
           Curl_raw_nequal(path, DICT_DEFINE3, sizeof(DICT_DEFINE3)-1)) {

    word = strchr(path, ':');
    if(word) {
      word++;
      database = strchr(word, ':');
      if(database) {
        *database++ = (char)0;
        nthdef = strchr(database, ':');
        if(nthdef) {
          *nthdef = (char)0;
        }
      }
    }

    if((word == NULL) || (*word == (char)0)) {
      infof(data, "lookup word is missing");
      word=(char *)"default";
    }
    if((database == NULL) || (*database == (char)0)) {
      database = (char *)"!";
    }

    eword = unescape_word(data, word);
    if(!eword)
      return CURLE_OUT_OF_MEMORY;

    result = Curl_sendf(sockfd, conn,
                        "CLIENT " LIBCURL_NAME " " LIBCURL_VERSION "\r\n"
                        "DEFINE "
                        "%s "     /* database */
                        "%s\r\n"  /* word */
                        "QUIT\r\n",
                        database,
                        eword);

    free(eword);

    if(result)
      failf(data, "Failed sending DICT request");
    else
      result = Curl_setup_transfer(conn, FIRSTSOCKET, -1, FALSE, bytecount,
                                   -1, NULL); /* no upload */

    if(result)
      return result;

  }
  else {

    ppath = strchr(path, '/');
    if(ppath) {
      int i;

      ppath++;
      for (i = 0; ppath[i]; i++) {
        if(ppath[i] == ':')
          ppath[i] = ' ';
      }
      result = Curl_sendf(sockfd, conn,
                          "CLIENT " LIBCURL_NAME " " LIBCURL_VERSION "\r\n"
                          "%s\r\n"
                          "QUIT\r\n", ppath);
      if(result)
        failf(data, "Failed sending DICT request");
      else
        result = Curl_setup_transfer(conn, FIRSTSOCKET, -1, FALSE, bytecount,
                                     -1, NULL);
      if(result)
        return result;
    }
  }

  return CURLE_OK;
}
#endif /*CURL_DISABLE_DICT*/
