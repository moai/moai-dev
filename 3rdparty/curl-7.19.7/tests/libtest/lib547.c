/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: lib547.c,v 1.7 2009-05-08 02:14:50 yangtse Exp $
 *
 * argv1 = URL
 * argv2 = proxy
 * argv3 = proxyuser:password
 */

#include "test.h"

#include "memdebug.h"

#ifdef CURL_DOES_CONVERSIONS
   /* ASCII representation with escape sequences for non-ASCII platforms */
#  define UPLOADTHIS "\x74\x68\x69\x73\x20\x69\x73\x20\x74\x68\x65\x20\x62" \
                     "\x6c\x75\x72\x62\x20\x77\x65\x20\x77\x61\x6e\x74\x20" \
                     "\x74\x6f\x20\x75\x70\x6c\x6f\x61\x64\x0a"
#else
#  define UPLOADTHIS "this is the blurb we want to upload\n"
#endif

#ifndef LIB548
static size_t readcallback(void  *ptr,
                           size_t size,
                           size_t nmemb,
                           void *clientp)
{
  int *counter = (int *)clientp;

  if(*counter) {
    /* only do this once and then require a clearing of this */
    fprintf(stderr, "READ ALREADY DONE!\n");
    return 0;
  }
  (*counter)++; /* bump */

  if(size * nmemb > strlen(UPLOADTHIS)) {
    fprintf(stderr, "READ!\n");
    strcpy(ptr, UPLOADTHIS);
    return strlen(UPLOADTHIS);
  }
  fprintf(stderr, "READ NOT FINE!\n");
  return 0;
}
static curlioerr ioctlcallback(CURL *handle,
                               int cmd,
                               void *clientp)
{
  int *counter = (int *)clientp;
  (void)handle; /* unused */
  if(cmd == CURLIOCMD_RESTARTREAD) {
    fprintf(stderr, "REWIND!\n");
    *counter = 0; /* clear counter to make the read callback restart */
  }
  return CURLIOE_OK;
}



#endif

int test(char *URL)
{
  CURLcode res;
  CURL *curl;
#ifndef LIB548
  int counter=0;
#endif

  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return TEST_ERR_MAJOR_BAD;
  }

  if ((curl = curl_easy_init()) == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
    curl_global_cleanup();
    return TEST_ERR_MAJOR_BAD;
  }

  curl_easy_setopt(curl, CURLOPT_URL, URL);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
#ifdef LIB548
  /* set the data to POST with a mere pointer to a zero-terminated string */
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, UPLOADTHIS);
#else
  /* 547 style, which means reading the POST data from a callback */
  curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, ioctlcallback);
  curl_easy_setopt(curl, CURLOPT_IOCTLDATA, &counter);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, readcallback);
  curl_easy_setopt(curl, CURLOPT_READDATA, &counter);
  /* We CANNOT do the POST fine without setting the size (or choose chunked)! */
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(UPLOADTHIS));
#endif
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_PROXY, libtest_arg2);
  curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, libtest_arg3);
  curl_easy_setopt(curl, CURLOPT_PROXYAUTH,
                   (long) (CURLAUTH_NTLM | CURLAUTH_DIGEST | CURLAUTH_BASIC) );

  res = curl_easy_perform(curl);

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return (int)res;
}

