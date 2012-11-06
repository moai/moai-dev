/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: lib543.c,v 1.3 2009-04-14 12:53:54 yangtse Exp $
 *
 * Based on Alex Fishman's bug report on September 30, 2007
 */

#include "setup.h"
#include "test.h"

#include "memdebug.h"

int test(char *URL)
{
  unsigned char a[] = {0x9c, 0x26, 0x4b, 0x3d, 0x49, 0x4, 0xa1, 0x1,
                       0xe0, 0xd8, 0x7c,  0x20, 0xb7, 0xef, 0x53, 0x29, 0xfa,
                       0x1d, 0x57, 0xe1};

  CURL* easy  = curl_easy_init();
  int asize = (int)sizeof(a);
  char* s = curl_easy_escape(easy, (char*)a, asize);
  (void)URL;

  printf("%s\n", s);

  curl_free(s);
  curl_easy_cleanup(easy);

  return 0;
}
