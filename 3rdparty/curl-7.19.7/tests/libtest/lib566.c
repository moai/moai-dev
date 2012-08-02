/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: lib566.c,v 1.2 2009-10-31 18:51:51 bagder Exp $
 */

#include "test.h"

#include "memdebug.h"

int test(char *URL)
{
  CURLcode res;
  CURL *curl;

  double content_length = 3;

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
  curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

  res = curl_easy_perform(curl);

  if(!res) {
    FILE *moo;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                            &content_length);
    moo = fopen(libtest_arg2, "wb");
    if(moo) {
      fprintf(moo, "CL: %.0f\n", content_length);
      fclose(moo);
    }
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return (int)res;
}

