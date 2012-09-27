/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: lib514.c,v 1.5 2008-09-20 04:26:57 yangtse Exp $
 */

#include "test.h"

#include "memdebug.h"

int test(char *URL)
{
  CURL *curl;
  CURLcode res=CURLE_OK;

  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return TEST_ERR_MAJOR_BAD;
  }

  if ((curl = curl_easy_init()) == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
    curl_global_cleanup();
    return TEST_ERR_MAJOR_BAD;
  }

  /* First set the URL that is about to receive our POST. */
  curl_easy_setopt(curl, CURLOPT_URL, URL);

  /* Based on a bug report by Niels van Tongeren on June 29, 2004:

  A weird situation occurs when request 1 is a POST request and the request
  2 is a HEAD request. For the POST request we set the CURLOPT_POSTFIELDS,
  CURLOPT_POSTFIELDSIZE and CURLOPT_POST options. For the HEAD request we
  set the CURLOPT_NOBODY option to '1'.

  */

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "moo");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 3L);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  /* this is where transfer 1 would take place, but skip that and change
     options right away instead */

  curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); /* show verbose for debug */
  curl_easy_setopt(curl, CURLOPT_HEADER, 1L); /* include header */

  /* Now, we should be making a fine HEAD request */

  /* Perform the request 2, res will get the return code */
  res = curl_easy_perform(curl);

  /* always cleanup */
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return (int)res;
}
