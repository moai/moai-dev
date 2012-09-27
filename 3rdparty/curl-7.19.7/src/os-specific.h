#ifndef HEADER_CURL_OS_SPECIFIC_H
#define HEADER_CURL_OS_SPECIFIC_H
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
 * $Id: os-specific.h,v 1.2 2009-06-10 18:02:11 yangtse Exp $
 ***************************************************************************/

#ifdef __VMS

int is_vms_shell(void);
void vms_special_exit(int code, int vms_show);

#undef exit
#define exit(__code) vms_special_exit((__code), (0))

#define  VMS_STS(c,f,e,s) (((c&0xF)<<28)|((f&0xFFF)<<16)|((e&0x1FFF)<3)|(s&7))
#define  VMSSTS_HIDE  VMS_STS(1,0,0,0)

#endif /* __VMS */

#endif /* HEADER_CURL_OS_SPECIFIC_H */
