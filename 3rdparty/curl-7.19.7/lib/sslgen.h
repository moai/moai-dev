#ifndef __SSLGEN_H
#define __SSLGEN_H
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
 * $Id: sslgen.h,v 1.18 2009-05-04 21:57:14 bagder Exp $
 ***************************************************************************/

bool Curl_ssl_config_matches(struct ssl_config_data* data,
                             struct ssl_config_data* needle);
bool Curl_clone_ssl_config(struct ssl_config_data* source,
                           struct ssl_config_data* dest);
void Curl_free_ssl_config(struct ssl_config_data* sslc);

#ifdef USE_SSL
int Curl_ssl_init(void);
void Curl_ssl_cleanup(void);
CURLcode Curl_ssl_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_connect_nonblocking(struct connectdata *conn,
                                      int sockindex,
                                      bool *done);
/* tell the SSL stuff to close down all open information regarding
   connections (and thus session ID caching etc) */
void Curl_ssl_close_all(struct SessionHandle *data);
void Curl_ssl_close(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_shutdown(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_set_engine(struct SessionHandle *data, const char *engine);
/* Sets engine as default for all SSL operations */
CURLcode Curl_ssl_set_engine_default(struct SessionHandle *data);
struct curl_slist *Curl_ssl_engines_list(struct SessionHandle *data);
ssize_t Curl_ssl_send(struct connectdata *conn,
                      int sockindex,
                      const void *mem,
                      size_t len);
ssize_t Curl_ssl_recv(struct connectdata *conn, /* connection data */
                      int sockindex,            /* socketindex */
                      char *mem,                /* store read data here */
                      size_t len);              /* max amount to read */
/* init the SSL session ID cache */
CURLcode Curl_ssl_initsessions(struct SessionHandle *, long);
size_t Curl_ssl_version(char *buffer, size_t size);
bool Curl_ssl_data_pending(const struct connectdata *conn,
                           int connindex);
int Curl_ssl_check_cxn(struct connectdata *conn);
void Curl_ssl_free_certinfo(struct SessionHandle *data);

/* Functions to be used by SSL library adaptation functions */

/* extract a session ID */
int Curl_ssl_getsessionid(struct connectdata *conn,
                          void **ssl_sessionid,
                          size_t *idsize) /* set 0 if unknown */;
/* add a new session ID */
CURLcode Curl_ssl_addsessionid(struct connectdata *conn,
                               void *ssl_sessionid,
                               size_t idsize);
/* delete a session from the cache */
void Curl_ssl_delsessionid(struct connectdata *conn, void *ssl_sessionid);

#define SSL_SHUTDOWN_TIMEOUT 10000 /* ms */

#else
/* When SSL support is not present, just define away these function calls */
#define Curl_ssl_init() 1
#define Curl_ssl_cleanup() do { } while (0)
#define Curl_ssl_connect(x,y) CURLE_FAILED_INIT
#define Curl_ssl_close_all(x)
#define Curl_ssl_close(x,y)
#define Curl_ssl_shutdown(x,y) CURLE_FAILED_INIT
#define Curl_ssl_set_engine(x,y) CURLE_FAILED_INIT
#define Curl_ssl_set_engine_default(x) CURLE_FAILED_INIT
#define Curl_ssl_engines_list(x) NULL
#define Curl_ssl_send(a,b,c,d) -1
#define Curl_ssl_recv(a,b,c,d) -1
#define Curl_ssl_initsessions(x,y) CURLE_OK
#define Curl_ssl_version(x,y) 0
#define Curl_ssl_data_pending(x,y) 0
#define Curl_ssl_check_cxn(x) 0
#define Curl_ssl_free_certinfo(x)

#endif

#endif /* USE_SSL */
