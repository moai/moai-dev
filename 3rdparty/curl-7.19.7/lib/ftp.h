#ifndef __FTP_H
#define __FTP_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2007, Daniel Stenberg, <daniel@haxx.se>, et al.
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
 * $Id: ftp.h,v 1.26 2007-10-12 13:36:38 patrickm Exp $
 ***************************************************************************/

#ifndef CURL_DISABLE_FTP
extern const struct Curl_handler Curl_handler_ftp;

#ifdef USE_SSL
extern const struct Curl_handler Curl_handler_ftps;
#endif

#ifndef CURL_DISABLE_HTTP
extern const struct Curl_handler Curl_handler_ftp_proxy;

# ifdef USE_SSL
extern const struct Curl_handler Curl_handler_ftps_proxy;
# endif
#endif

CURLcode Curl_ftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_nbftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_GetFTPResponse(ssize_t *nread, struct connectdata *conn,
                             int *ftpcode);
#endif /* CURL_DISABLE_FTP */
#endif /* __FTP_H */
