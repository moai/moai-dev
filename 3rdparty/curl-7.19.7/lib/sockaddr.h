#ifndef __SOCKADDR_H
#define __SOCKADDR_H
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
 * $Id: sockaddr.h,v 1.5 2009-05-18 12:25:45 yangtse Exp $
 ***************************************************************************/

#include "setup.h"

#ifdef HAVE_STRUCT_SOCKADDR_STORAGE
struct Curl_sockaddr_storage {
  struct sockaddr_storage buffer;
};
#else
struct Curl_sockaddr_storage {
  char buffer[256];   /* this should be big enough to fit a lot */
};
#endif

#endif /* __SOCKADDR_H */
