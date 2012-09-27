/*****************************************************************************
  *                                  _   _ ____  _
  *  Project                     ___| | | |  _ \| |
  *                             / __| | | | |_) | |
  *                            | (__| |_| |  _ <| |___
  *                             \___|\___/|_| \_\_____|
  *
  * $Id: lib539.c,v 1.3 2008-09-22 17:20:29 danf Exp $
  */

#include "test.h"

#include "memdebug.h"

int test(char *URL)
{
   CURLcode res;
   CURL *curl;
   char *newURL;
   struct curl_slist *slist;

   if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
     fprintf(stderr, "curl_global_init() failed\n");
     return TEST_ERR_MAJOR_BAD;
   }

   if ((curl = curl_easy_init()) == NULL) {
     fprintf(stderr, "curl_easy_init() failed\n");
     curl_global_cleanup();
     return TEST_ERR_MAJOR_BAD;
   }

   /*
    * Begin with cURL set to use a single CWD to the URL's directory.
    */
   curl_easy_setopt(curl, CURLOPT_URL, URL);
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, (long) CURLFTPMETHOD_SINGLECWD);

   res = curl_easy_perform(curl);

   /*
    * Change the FTP_FILEMETHOD option to use full paths rather than a CWD
    * command.  Alter the URL's path a bit, appending a "./".  Use an innocuous
    * QUOTE command, after which cURL will CWD to ftp_conn->entrypath and then
    * (on the next call to ftp_statemach_act) find a non-zero ftpconn->dirdepth
    * even though no directories are stored in the ftpconn->dirs array (after a
    * call to freedirs).
    */
   newURL = malloc(strlen(URL) + 3);
   if (newURL == NULL) {
     curl_easy_cleanup(curl);
     curl_global_cleanup();
     return TEST_ERR_MAJOR_BAD;
   }
   newURL = strcat(strcpy(newURL, URL), "./");

   slist = curl_slist_append (NULL, "SYST");
   if (slist == NULL) {
     free(newURL);
     curl_easy_cleanup(curl);
     curl_global_cleanup();
     return TEST_ERR_MAJOR_BAD;
   }

   curl_easy_setopt(curl, CURLOPT_URL, newURL);
   curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, (long) CURLFTPMETHOD_NOCWD);
   curl_easy_setopt(curl, CURLOPT_QUOTE, slist);

   res = curl_easy_perform(curl);

   curl_slist_free_all(slist);
   free(newURL);
   curl_easy_cleanup(curl);
   curl_global_cleanup();

   return (int)res;
}
