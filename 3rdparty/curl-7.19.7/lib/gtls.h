#ifndef __GTLS_H
#define __GTLS_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2008, Daniel Stenberg, <daniel@haxx.se>, et al.
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
 * $Id: gtls.h,v 1.10 2009-02-12 20:48:43 danf Exp $
 ***************************************************************************/

#ifdef USE_GNUTLS

int Curl_gtls_init(void);
int Curl_gtls_cleanup(void);
CURLcode Curl_gtls_connect(struct connectdata *conn, int sockindex);

/* tell GnuTLS to close down all open information regarding connections (and
   thus session ID caching etc) */
void Curl_gtls_close_all(struct SessionHandle *data);

 /* close a SSL connection */
void Curl_gtls_close(struct connectdata *conn, int sockindex);

/* return number of sent (non-SSL) bytes */
ssize_t Curl_gtls_send(struct connectdata *conn, int sockindex,
                       const void *mem, size_t len);
ssize_t Curl_gtls_recv(struct connectdata *conn, /* connection data */
                       int num,                  /* socketindex */
                       char *buf,                /* store read data here */
                       size_t buffersize,        /* max amount to read */
                       bool *wouldblock);
void Curl_gtls_session_free(void *ptr);
size_t Curl_gtls_version(char *buffer, size_t size);
int Curl_gtls_shutdown(struct connectdata *conn, int sockindex);
int Curl_gtls_seed(struct SessionHandle *data);

/* API setup for GnuTLS */
#define curlssl_init Curl_gtls_init
#define curlssl_cleanup Curl_gtls_cleanup
#define curlssl_connect Curl_gtls_connect
#define curlssl_session_free(x)  Curl_gtls_session_free(x)
#define curlssl_close_all Curl_gtls_close_all
#define curlssl_close Curl_gtls_close
#define curlssl_shutdown(x,y) Curl_gtls_shutdown(x,y)
#define curlssl_set_engine(x,y) (x=x, y=y, CURLE_FAILED_INIT)
#define curlssl_set_engine_default(x) (x=x, CURLE_FAILED_INIT)
#define curlssl_engines_list(x) (x=x, (struct curl_slist *)NULL)
#define curlssl_send Curl_gtls_send
#define curlssl_recv Curl_gtls_recv
#define curlssl_version Curl_gtls_version
#define curlssl_check_cxn(x) (x=x, -1)
#define curlssl_data_pending(x,y) (x=x, y=y, 0)

#endif /* USE_GNUTLS */
#endif
