/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: lib562.c,v 1.1 2009-04-30 09:02:39 bagder Exp $
 */

#include "setup.h" /* struct_stat etc. */
#include "test.h"

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "memdebug.h"

/*
 * From "KNOWN_BUGS" April 2009:

 59. If the CURLOPT_PORT option is used on an FTP URL like
 "ftp://example.com/file;type=A" the ";type=A" is stripped off.

 */

int test(char *URL)
{
  CURL *curl;
  CURLcode res = CURLE_OK;

  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return TEST_ERR_MAJOR_BAD;
  }

  /* get a curl handle */
  if ((curl = curl_easy_init()) == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
    curl_global_cleanup();
    return TEST_ERR_MAJOR_BAD;
  }

  /* enable verbose */
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  /* set port number */
  curl_easy_setopt(curl, CURLOPT_PORT, atoi(libtest_arg2) );

  /* specify target */
  curl_easy_setopt(curl,CURLOPT_URL, URL);

  /* Now run off and do what you've been told! */
  res = curl_easy_perform(curl);

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return res;
}
