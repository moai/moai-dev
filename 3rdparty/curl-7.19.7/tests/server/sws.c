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
 * $Id: sws.c,v 1.136 2009-09-17 14:02:50 yangtse Exp $
 ***************************************************************************/

/* sws.c: simple (silly?) web server

   This code was originally graciously donated to the project by Juergen
   Wilke. Thanks a bunch!

 */
#include "setup.h" /* portability help from the lib directory */

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h> /* for TCP_NODELAY */
#endif

#define ENABLE_CURLX_PRINTF
/* make the curlx header define all printf() functions to use the curlx_*
   versions instead */
#include "curlx.h" /* from the private lib dir */
#include "getpart.h"
#include "util.h"

/* include memdebug.h last */
#include "memdebug.h"

#if !defined(CURL_SWS_FORK_ENABLED) && defined(HAVE_FORK)
/*
 * The normal sws build for the plain standard curl test suite has no use for
 * fork(), but if you feel wild and crazy and want to setup some more exotic
 * tests. Define this and run...
 */
#define CURL_SWS_FORK_ENABLED
#endif

#ifdef ENABLE_IPV6
static bool use_ipv6 = FALSE;
#endif
static const char *ipv_inuse = "IPv4";

#define REQBUFSIZ 150000
#define REQBUFSIZ_TXT "149999"

static long prevtestno=-1;    /* previous test number we served */
static long prevpartno=-1;    /* previous part number we served */
static bool prevbounce=FALSE; /* instructs the server to increase the part
                                 number for a test in case the identical
                                 testno+partno request shows up again */

#define RCMD_NORMALREQ 0 /* default request, use the tests file normally */
#define RCMD_IDLE      1 /* told to sit idle */
#define RCMD_STREAM    2 /* told to stream */

struct httprequest {
  char reqbuf[REQBUFSIZ]; /* buffer area for the incoming request */
  int checkindex; /* where to start checking of the request */
  int offset;     /* size of the incoming request */
  long testno;     /* test number found in the request */
  long partno;     /* part number found in the request */
  bool open;      /* keep connection open info, as found in the request */
  bool auth_req;  /* authentication required, don't wait for body unless
                     there's an Authorization header */
  bool auth;      /* Authorization header present in the incoming request */
  size_t cl;      /* Content-Length of the incoming request */
  bool digest;    /* Authorization digest header found */
  bool ntlm;      /* Authorization ntlm header found */
  int pipe;       /* if non-zero, expect this many requests to do a "piped"
                     request/response */
  int skip;       /* if non-zero, the server is instructed to not read this
                     many bytes from a PUT/POST request. Ie the client sends N
                     bytes said in Content-Length, but the server only reads N
                     - skip bytes. */
  int rcmd;       /* doing a special command, see defines above */
  int prot_version; /* HTTP version * 10 */
  bool pipelining; /* true if request is pipelined */
};

static int ProcessRequest(struct httprequest *req);
static void storerequest(char *reqbuf, ssize_t totalsize);

#define DEFAULT_PORT 8999

#ifndef DEFAULT_LOGFILE
#define DEFAULT_LOGFILE "log/sws.log"
#endif

const char *serverlogfile = DEFAULT_LOGFILE;

#define SWSVERSION "cURL test suite HTTP server/0.1"

#define REQUEST_DUMP  "log/server.input"
#define RESPONSE_DUMP "log/server.response"

/* very-big-path support */
#define MAXDOCNAMELEN 140000
#define MAXDOCNAMELEN_TXT "139999"

#define REQUEST_KEYWORD_SIZE 256
#define REQUEST_KEYWORD_SIZE_TXT "255"

#define CMD_AUTH_REQUIRED "auth_required"

/* 'idle' means that it will accept the request fine but never respond
   any data. Just keep the connection alive. */
#define CMD_IDLE "idle"

/* 'stream' means to send a never-ending stream of data */
#define CMD_STREAM "stream"

#define END_OF_HEADERS "\r\n\r\n"

enum {
  DOCNUMBER_NOTHING = -7,
  DOCNUMBER_QUIT    = -6,
  DOCNUMBER_BADCONNECT = -5,
  DOCNUMBER_INTERNAL= -4,
  DOCNUMBER_CONNECT = -3,
  DOCNUMBER_WERULEZ = -2,
  DOCNUMBER_404     = -1
};


/* sent as reply to a QUIT */
static const char *docquit =
"HTTP/1.1 200 Goodbye" END_OF_HEADERS;

/* sent as reply to a CONNECT */
static const char *docconnect =
"HTTP/1.1 200 Mighty fine indeed" END_OF_HEADERS;

/* sent as reply to a "bad" CONNECT */
static const char *docbadconnect =
"HTTP/1.1 501 Forbidden you fool" END_OF_HEADERS;

/* send back this on 404 file not found */
static const char *doc404 = "HTTP/1.1 404 Not Found\r\n"
    "Server: " SWSVERSION "\r\n"
    "Connection: close\r\n"
    "Content-Type: text/html"
    END_OF_HEADERS
    "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
    "<HTML><HEAD>\n"
    "<TITLE>404 Not Found</TITLE>\n"
    "</HEAD><BODY>\n"
    "<H1>Not Found</H1>\n"
    "The requested URL was not found on this server.\n"
    "<P><HR><ADDRESS>" SWSVERSION "</ADDRESS>\n" "</BODY></HTML>\n";

#ifndef WIN32
#  if defined(SIGPIPE) && defined(HAVE_SIGNAL)
static volatile int sigpipe;  /* Why? It's not used */
static void sigpipe_handler(int sig)
{
  (void)sig; /* prevent warning */
  sigpipe = 1;
}
#  endif
#endif

static int ProcessRequest(struct httprequest *req)
{
  char *line=&req->reqbuf[req->checkindex];
  bool chunked = FALSE;
  static char request[REQUEST_KEYWORD_SIZE];
  static char doc[MAXDOCNAMELEN];
  char logbuf[256];
  int prot_major, prot_minor;
  char *end;
  int error;
  end = strstr(line, END_OF_HEADERS);

  logmsg("ProcessRequest() called");

  /* try to figure out the request characteristics as soon as possible, but
     only once! */
  if((req->testno == DOCNUMBER_NOTHING) &&
     sscanf(line,
            "%" REQUEST_KEYWORD_SIZE_TXT"s %" MAXDOCNAMELEN_TXT "s HTTP/%d.%d",
            request,
            doc,
            &prot_major,
            &prot_minor) == 4) {
    char *ptr;

    req->prot_version = prot_major*10 + prot_minor;

    /* find the last slash */
    ptr = strrchr(doc, '/');

    /* get the number after it */
    if(ptr) {
      FILE *stream;
      char *filename;

      if((strlen(doc) + strlen(request)) < 200)
        sprintf(logbuf, "Got request: %s %s HTTP/%d.%d",
                request, doc, prot_major, prot_minor);
      else
        sprintf(logbuf, "Got a *HUGE* request HTTP/%d.%d",
                prot_major, prot_minor);
      logmsg("%s", logbuf);

      if(!strncmp("/verifiedserver", ptr, 15)) {
        logmsg("Are-we-friendly question received");
        req->testno = DOCNUMBER_WERULEZ;
        return 1; /* done */
      }

      if(!strncmp("/quit", ptr, 5)) {
        logmsg("Request-to-quit received");
        req->testno = DOCNUMBER_QUIT;
        return 1; /* done */
      }

      ptr++; /* skip the slash */

      /* skip all non-numericals following the slash */
      while(*ptr && !ISDIGIT(*ptr))
        ptr++;

      req->testno = strtol(ptr, &ptr, 10);

      if(req->testno > 10000) {
        req->partno = req->testno % 10000;
        req->testno /= 10000;
      }
      else
        req->partno = 0;

      sprintf(logbuf, "Requested test number %ld part %ld",
              req->testno, req->partno);

      logmsg("%s", logbuf);

      filename = test2file(req->testno);

      stream=fopen(filename, "rb");
      if(!stream) {
        error = ERRNO;
        logmsg("fopen() failed with error: %d %s", error, strerror(error));
        logmsg("Error opening file: %s", filename);
        logmsg("Couldn't open test file %ld", req->testno);
        req->open = FALSE; /* closes connection */
        return 1; /* done */
      }
      else {
        char *cmd = NULL;
        size_t cmdsize = 0;
        int num=0;

        /* get the custom server control "commands" */
        cmd = (char *)spitout(stream, "reply", "servercmd", &cmdsize);
        fclose(stream);

        if(cmdsize) {
          logmsg("Found a reply-servercmd section!");

          if(!strncmp(CMD_AUTH_REQUIRED, cmd, strlen(CMD_AUTH_REQUIRED))) {
            logmsg("instructed to require authorization header");
            req->auth_req = TRUE;
          }
          else if(!strncmp(CMD_IDLE, cmd, strlen(CMD_IDLE))) {
            logmsg("instructed to idle");
            req->rcmd = RCMD_IDLE;
            req->open = TRUE;
          }
          else if(!strncmp(CMD_STREAM, cmd, strlen(CMD_STREAM))) {
            logmsg("instructed to stream");
            req->rcmd = RCMD_STREAM;
          }
          else if(1 == sscanf(cmd, "pipe: %d", &num)) {
            logmsg("instructed to allow a pipe size %d", num);
            req->pipe = num-1; /* decrease by one since we don't count the
                                  first request in this number */
          }
          else if(1 == sscanf(cmd, "skip: %d", &num)) {
            logmsg("instructed to skip this number of bytes %d", num);
            req->skip = num;
          }
          else {
            logmsg("funny instruction found: %s", cmd);
          }
          free(cmd);
        }
      }
    }
    else {
      if(sscanf(req->reqbuf, "CONNECT %" MAXDOCNAMELEN_TXT "s HTTP/%d.%d",
                doc, &prot_major, &prot_minor) == 3) {
        sprintf(logbuf, "Received a CONNECT %s HTTP/%d.%d request",
                doc, prot_major, prot_minor);
        logmsg("%s", logbuf);

        if(req->prot_version == 10)
          req->open = FALSE; /* HTTP 1.0 closes connection by default */

        if(!strncmp(doc, "bad", 3))
          /* if the host name starts with bad, we fake an error here */
          req->testno = DOCNUMBER_BADCONNECT;
        else if(!strncmp(doc, "test", 4)) {
          /* if the host name starts with test, the port number used in the
             CONNECT line will be used as test number! */
          char *portp = strchr(doc, ':');
          if(portp)
            req->testno = atoi(portp+1);
          else
            req->testno = DOCNUMBER_CONNECT;
        }
        else
          req->testno = DOCNUMBER_CONNECT;
      }
      else {
        logmsg("Did not find test number in PATH");
        req->testno = DOCNUMBER_404;
      }
    }
  }

  if(!end) {
    /* we don't have a complete request yet! */
    logmsg("ProcessRequest returned without a complete request");
    return 0;
  }
  logmsg("ProcessRequest found a complete request");

  if(req->pipe)
    /* we do have a full set, advance the checkindex to after the end of the
       headers, for the pipelining case mostly */
    req->checkindex += (end - line) + strlen(END_OF_HEADERS);

  /* **** Persistence ****
   *
   * If the request is a HTTP/1.0 one, we close the connection unconditionally
   * when we're done.
   *
   * If the request is a HTTP/1.1 one, we MUST check for a "Connection:"
   * header that might say "close". If it does, we close a connection when
   * this request is processed. Otherwise, we keep the connection alive for X
   * seconds.
   */

  do {
    if((req->cl==0) && curlx_strnequal("Content-Length:", line, 15)) {
      /* If we don't ignore content-length, we read it and we read the whole
         request including the body before we return. If we've been told to
         ignore the content-length, we will return as soon as all headers
         have been received */
      size_t cl = strtol(line+15, &line, 10);
      req->cl = cl - req->skip;

      logmsg("Found Content-Length: %zu in the request", cl);
      if(req->skip)
        logmsg("... but will abort after %zu bytes", req->cl);
      break;
    }
    else if(curlx_strnequal("Transfer-Encoding: chunked", line,
                            strlen("Transfer-Encoding: chunked"))) {
      /* chunked data coming in */
      chunked = TRUE;
    }

    if(chunked) {
      if(strstr(req->reqbuf, "\r\n0\r\n\r\n"))
        /* end of chunks reached */
        return 1; /* done */
      else
        return 0; /* not done */
    }

    line = strchr(line, '\n');
    if(line)
      line++;
  } while(line);

  if(!req->auth && strstr(req->reqbuf, "Authorization:")) {
    req->auth = TRUE; /* Authorization: header present! */
    if(req->auth_req)
      logmsg("Authorization header found, as required");
  }

  if(!req->digest && strstr(req->reqbuf, "Authorization: Digest")) {
    /* If the client is passing this Digest-header, we set the part number
       to 1000. Not only to spice up the complexity of this, but to make
       Digest stuff to work in the test suite. */
    req->partno += 1000;
    req->digest = TRUE; /* header found */
    logmsg("Received Digest request, sending back data %ld", req->partno);
  }
  else if(!req->ntlm &&
          strstr(req->reqbuf, "Authorization: NTLM TlRMTVNTUAAD")) {
    /* If the client is passing this type-3 NTLM header */
    req->partno += 1002;
    req->ntlm = TRUE; /* NTLM found */
    logmsg("Received NTLM type-3, sending back data %ld", req->partno);
    if(req->cl) {
      logmsg("  Expecting %zu POSTed bytes", req->cl);
    }
  }
  else if(!req->ntlm &&
          strstr(req->reqbuf, "Authorization: NTLM TlRMTVNTUAAB")) {
    /* If the client is passing this type-1 NTLM header */
    req->partno += 1001;
    req->ntlm = TRUE; /* NTLM found */
    logmsg("Received NTLM type-1, sending back data %ld", req->partno);
  }
  else if((req->partno >= 1000) && strstr(req->reqbuf, "Authorization: Basic")) {
    /* If the client is passing this Basic-header and the part number is already
       >=1000, we add 1 to the part number.  This allows simple Basic authentication
       negotiation to work in the test suite. */
    req->partno += 1;
    logmsg("Received Basic request, sending back data %ld", req->partno);
  }
  if(strstr(req->reqbuf, "Connection: close"))
    req->open = FALSE; /* close connection after this request */

  if(!req->pipe &&
     req->open &&
     req->prot_version >= 11 &&
     end &&
     req->reqbuf + req->offset > end + strlen(END_OF_HEADERS) &&
     (!strncmp(req->reqbuf, "GET", strlen("GET")) ||
      !strncmp(req->reqbuf, "HEAD", strlen("HEAD")))) {
    /* If we have a persistent connection, HTTP version >= 1.1
       and GET/HEAD request, enable pipelining. */
    req->checkindex = (end - req->reqbuf) + strlen(END_OF_HEADERS);
    req->pipelining = TRUE;
  }

  while(req->pipe) {
    /* scan for more header ends within this chunk */
    line = &req->reqbuf[req->checkindex];
    end = strstr(line, END_OF_HEADERS);
    if(!end)
      break;
    req->checkindex += (end - line) + strlen(END_OF_HEADERS);
    req->pipe--;
  }


  /* If authentication is required and no auth was provided, end now. This
     makes the server NOT wait for PUT/POST data and you can then make the
     test case send a rejection before any such data has been sent. Test case
     154 uses this.*/
  if(req->auth_req && !req->auth)
    return 1;

  if(req->cl > 0) {
    if(req->cl <= req->offset - (end - req->reqbuf) - strlen(END_OF_HEADERS))
      return 1; /* done */
    else
      return 0; /* not complete yet */
  }

  return 1; /* done */
}

/* store the entire request in a file */
static void storerequest(char *reqbuf, ssize_t totalsize)
{
  int res;
  int error = 0;
  ssize_t written;
  ssize_t writeleft;
  FILE *dump;

  if (reqbuf == NULL)
    return;

  if (totalsize == 0)
    return;
  else if (totalsize < 0) {
    logmsg("Invalid size (%zd bytes) for request input. Not written to %s",
           totalsize, REQUEST_DUMP);
    return;
  }

  do {
    dump = fopen(REQUEST_DUMP, "ab");
  } while ((dump == NULL) && ((error = ERRNO) == EINTR));
  if (dump == NULL) {
    logmsg("Error opening file %s error: %d %s",
           REQUEST_DUMP, error, strerror(error));
    logmsg("Failed to write request input to " REQUEST_DUMP);
    return;
  }

  writeleft = totalsize;
  do {
    written = (ssize_t)fwrite((void *) &reqbuf[totalsize-writeleft],
                              1, (size_t)writeleft, dump);
    if (written > 0)
      writeleft -= written;
  } while ((writeleft > 0) && ((error = ERRNO) == EINTR));

  if (writeleft > 0) {
    logmsg("Error writing file %s error: %d %s",
           REQUEST_DUMP, error, strerror(error));
    logmsg("Wrote only (%zd bytes) of (%zd bytes) request input to %s",
           totalsize-writeleft, totalsize, REQUEST_DUMP);
  }

  do {
    res = fclose(dump);
  } while(res && ((error = ERRNO) == EINTR));
  if(res)
    logmsg("Error closing file %s error: %d %s",
           REQUEST_DUMP, error, strerror(error));

  if(!writeleft)
    logmsg("Wrote request (%zd bytes) input to " REQUEST_DUMP,
           totalsize);
}

/* return 0 on success, non-zero on failure */
static int get_request(curl_socket_t sock, struct httprequest *req)
{
  int fail = 0;
  char *reqbuf = req->reqbuf;
  ssize_t got = 0;

  char *pipereq = NULL;
  int pipereq_length = 0;

  if(req->pipelining) {
    pipereq = reqbuf + req->checkindex;
    pipereq_length = req->offset - req->checkindex;
  }

  /*** Init the httprequest structure properly for the upcoming request ***/

  req->checkindex = 0;
  req->offset = 0;
  req->testno = DOCNUMBER_NOTHING;
  req->partno = 0;
  req->open = TRUE;
  req->auth_req = FALSE;
  req->auth = FALSE;
  req->cl = 0;
  req->digest = FALSE;
  req->ntlm = FALSE;
  req->pipe = 0;
  req->skip = 0;
  req->rcmd = RCMD_NORMALREQ;
  req->prot_version = 0;
  req->pipelining = FALSE;

  /*** end of httprequest init ***/

  while (req->offset < REQBUFSIZ-1) {
    if(pipereq_length && pipereq) {
      memmove(reqbuf, pipereq, pipereq_length);
      got = pipereq_length;
      pipereq_length = 0;
    }
    else {
      if(req->skip)
        /* we are instructed to not read the entire thing, so we make sure to only
           read what we're supposed to and NOT read the enire thing the client
           wants to send! */
        got = sread(sock, reqbuf + req->offset, req->cl);
      else
        got = sread(sock, reqbuf + req->offset, REQBUFSIZ-1 - req->offset);
    }
    if (got <= 0) {
      if (got < 0) {
        logmsg("recv() returned error: %d", SOCKERRNO);
        return DOCNUMBER_INTERNAL;
      }
      logmsg("Connection closed by client");
      reqbuf[req->offset] = '\0';

      /* dump the request receivied so far to the external file */
      storerequest(reqbuf, req->offset);
      return DOCNUMBER_INTERNAL;
    }

    logmsg("Read %zd bytes", got);

    req->offset += (int)got;
    reqbuf[req->offset] = '\0';

    if(ProcessRequest(req)) {
      if(req->pipe--) {
        logmsg("Waiting for another piped request");
        continue;
      }
      break;
    }
  }

  if((req->offset == REQBUFSIZ-1) && (got > 0)) {
    logmsg("Request would overflow buffer, closing connection");
    /* dump request received so far to external file anyway */
    reqbuf[REQBUFSIZ-1] = '\0';
    fail = 1;
  }
  else if(req->offset > REQBUFSIZ-1) {
    logmsg("Request buffer overflow, closing connection");
    /* dump request received so far to external file anyway */
    reqbuf[REQBUFSIZ-1] = '\0';
    fail = 1;
  }
  else
    reqbuf[req->offset] = '\0';

  /* dump the request to an external file */
  storerequest(reqbuf, req->pipelining ? req->checkindex : req->offset);

  return fail; /* return 0 on success */
}

/* returns -1 on failure */
static int send_doc(curl_socket_t sock, struct httprequest *req)
{
  ssize_t written;
  size_t count;
  const char *buffer;
  char *ptr=NULL;
  FILE *stream;
  char *cmd=NULL;
  size_t cmdsize=0;
  FILE *dump;
  bool persistant = TRUE;
  bool sendfailure = FALSE;
  size_t responsesize;
  int error = 0;
  int res;

  static char weare[256];

  char partbuf[80]="data";

  logmsg("Send response number %ld part %ld", req->testno, req->partno);

  switch(req->rcmd) {
  default:
  case RCMD_NORMALREQ:
    break; /* continue with business as usual */
  case RCMD_STREAM:
#define STREAMTHIS "a string to stream 01234567890\n"
    count = strlen(STREAMTHIS);
    while(1) {
      written = swrite(sock, STREAMTHIS, count);
      if(written != (ssize_t)count) {
        logmsg("Stopped streaming");
        break;
      }
    }
    return -1;
  case RCMD_IDLE:
    /* Do nothing. Sit idle. Pretend it rains. */
    return 0;
  }

  req->open = FALSE;

  if(req->testno < 0) {
    size_t msglen;
    char msgbuf[64];

    switch(req->testno) {
    case DOCNUMBER_QUIT:
      logmsg("Replying to QUIT");
      buffer = docquit;
      break;
    case DOCNUMBER_WERULEZ:
      /* we got a "friends?" question, reply back that we sure are */
      logmsg("Identifying ourselves as friends");
      sprintf(msgbuf, "WE ROOLZ: %ld\r\n", (long)getpid());
      msglen = strlen(msgbuf);
      sprintf(weare, "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n%s",
              msglen, msgbuf);
      buffer = weare;
      break;
    case DOCNUMBER_INTERNAL:
      logmsg("Bailing out due to internal error");
      return -1;
    case DOCNUMBER_CONNECT:
      logmsg("Replying to CONNECT");
      buffer = docconnect;
      break;
    case DOCNUMBER_BADCONNECT:
      logmsg("Replying to a bad CONNECT");
      buffer = docbadconnect;
      break;
    case DOCNUMBER_404:
    default:
      logmsg("Replying to with a 404");
      buffer = doc404;
      break;
    }
    ptr = NULL;
    stream=NULL;

    count = strlen(buffer);
  }
  else {
    char *filename = test2file(req->testno);

    if(0 != req->partno)
      sprintf(partbuf, "data%ld", req->partno);

    stream=fopen(filename, "rb");
    if(!stream) {
      error = ERRNO;
      logmsg("fopen() failed with error: %d %s", error, strerror(error));
      logmsg("Error opening file: %s", filename);
      logmsg("Couldn't open test file");
      return 0;
    }
    else {
      buffer = spitout(stream, "reply", partbuf, &count);
      ptr = (char *)buffer;
      fclose(stream);
    }

    /* re-open the same file again */
    stream=fopen(filename, "rb");
    if(!stream) {
      error = ERRNO;
      logmsg("fopen() failed with error: %d %s", error, strerror(error));
      logmsg("Error opening file: %s", filename);
      logmsg("Couldn't open test file");
      return 0;
    }
    else {
      /* get the custom server control "commands" */
      cmd = (char *)spitout(stream, "reply", "postcmd", &cmdsize);
      fclose(stream);
    }
  }

  dump = fopen(RESPONSE_DUMP, "ab"); /* b is for windows-preparing */
  if(!dump) {
    error = ERRNO;
    logmsg("fopen() failed with error: %d %s", error, strerror(error));
    logmsg("Error opening file: %s", RESPONSE_DUMP);
    logmsg("couldn't create logfile: " RESPONSE_DUMP);
    return -1;
  }

  /* If the word 'swsclose' is present anywhere in the reply chunk, the
     connection will be closed after the data has been sent to the requesting
     client... */
  if(strstr(buffer, "swsclose") || !count) {
    persistant = FALSE;
    logmsg("connection close instruction \"swsclose\" found in response");
  }
  if(strstr(buffer, "swsbounce")) {
    prevbounce = TRUE;
    logmsg("enable \"swsbounce\" in the next request");
  }
  else
    prevbounce = FALSE;


  responsesize = count;
  do {
    /* Ok, we send no more than 200 bytes at a time, just to make sure that
       larger chunks are split up so that the client will need to do multiple
       recv() calls to get it and thus we exercise that code better */
    size_t num = count;
    if(num > 200)
      num = 200;
    written = swrite(sock, buffer, num);
    if (written < 0) {
      sendfailure = TRUE;
      break;
    }
    else {
      logmsg("Sent off %zd bytes", written);
    }
    /* write to file as well */
    fwrite(buffer, 1, written, dump);

    count -= written;
    buffer += written;
  } while(count>0);

  do {
    res = fclose(dump);
  } while(res && ((error = ERRNO) == EINTR));
  if(res)
    logmsg("Error closing file %s error: %d %s",
           RESPONSE_DUMP, error, strerror(error));

  if(sendfailure) {
    logmsg("Sending response failed. Only (%zu bytes) of (%zu bytes) were sent",
           responsesize-count, responsesize);
    if(ptr)
      free(ptr);
    if(cmd)
      free(cmd);
    return -1;
  }

  logmsg("Response sent (%zu bytes) and written to " RESPONSE_DUMP,
         responsesize);

  if(ptr)
    free(ptr);

  if(cmdsize > 0 ) {
    char command[32];
    int num;
    ptr=cmd;
    do {
      if(2 == sscanf(ptr, "%31s %d", command, &num)) {
        if(!strcmp("wait", command)) {
          logmsg("Told to sleep for %d seconds", num);
          sleep(num); /* wait this many seconds */
        }
        else
          logmsg("Unknown command in reply command section");
      }
      ptr = strchr(ptr, '\n');
      if(ptr)
        ptr++;
      else
        ptr = NULL;
    } while(ptr && *ptr);
  }
  if(cmd)
    free(cmd);

  req->open = persistant;

  prevtestno = req->testno;
  prevpartno = req->partno;

  return 0;
}


int main(int argc, char *argv[])
{
  struct sockaddr_in me;
#ifdef ENABLE_IPV6
  struct sockaddr_in6 me6;
#endif /* ENABLE_IPV6 */
  curl_socket_t sock, msgsock;
  int flag;
  unsigned short port = DEFAULT_PORT;
  char *pidname= (char *)".http.pid";
  struct httprequest req;
  int rc;
  int arg=1;
#ifdef CURL_SWS_FORK_ENABLED
  bool use_fork = FALSE;
#endif

  while(argc>arg) {
    if(!strcmp("--version", argv[arg])) {
      printf("sws IPv4%s"
#ifdef CURL_SWS_FORK_ENABLED
             " FORK"
#endif
             "\n"
             ,
#ifdef ENABLE_IPV6
             "/IPv6"
#else
             ""
#endif
             );
      return 0;
    }
    else if(!strcmp("--pidfile", argv[arg])) {
      arg++;
      if(argc>arg)
        pidname = argv[arg++];
    }
    else if(!strcmp("--ipv6", argv[arg])) {
#ifdef ENABLE_IPV6
      ipv_inuse = "IPv6";
      use_ipv6 = TRUE;
#endif
      arg++;
    }
#ifdef CURL_SWS_FORK_ENABLED
    else if(!strcmp("--fork", argv[arg])) {
      use_fork=TRUE;
      arg++;
    }
#endif
    else if(argc>arg) {

      if(atoi(argv[arg]))
        port = (unsigned short)atoi(argv[arg++]);

      if(argc>arg)
        path = argv[arg++];
    }
  }

#ifdef WIN32
  win32_init();
  atexit(win32_cleanup);
#else

#ifdef SIGPIPE
#ifdef HAVE_SIGNAL
  signal(SIGPIPE, sigpipe_handler);
#endif
#ifdef HAVE_SIGINTERRUPT
  siginterrupt(SIGPIPE, 1);
#endif
#endif
#endif

#ifdef ENABLE_IPV6
  if(!use_ipv6)
#endif
    sock = socket(AF_INET, SOCK_STREAM, 0);
#ifdef ENABLE_IPV6
  else
    sock = socket(AF_INET6, SOCK_STREAM, 0);
#endif

  if (CURL_SOCKET_BAD == sock) {
    logmsg("Error opening socket: %d", SOCKERRNO);
    return 1;
  }

  flag = 1;
  if (0 != setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
            (void *) &flag, sizeof(flag))) {
    logmsg("setsockopt(SO_REUSEADDR) failed: %d", SOCKERRNO);
    sclose(sock);
    return 1;
  }

#ifdef ENABLE_IPV6
  if(!use_ipv6) {
#endif
    memset(&me, 0, sizeof(me));
    me.sin_family = AF_INET;
    me.sin_addr.s_addr = INADDR_ANY;
    me.sin_port = htons(port);
    rc = bind(sock, (struct sockaddr *) &me, sizeof(me));
#ifdef ENABLE_IPV6
  }
  else {
    memset(&me6, 0, sizeof(me6));
    me6.sin6_family = AF_INET6;
    me6.sin6_addr = in6addr_any;
    me6.sin6_port = htons(port);
    rc = bind(sock, (struct sockaddr *) &me6, sizeof(me6));
  }
#endif /* ENABLE_IPV6 */
  if(0 != rc) {
    logmsg("Error binding socket: %d", SOCKERRNO);
    sclose(sock);
    return 1;
  }

  if(!write_pidfile(pidname)) {
    sclose(sock);
    return 1;
  }

  logmsg("Running %s version on port %d", ipv_inuse, (int)port);

  /* start accepting connections */
  rc = listen(sock, 5);
  if(0 != rc) {
    logmsg("listen() failed with error: %d", SOCKERRNO);
    sclose(sock);
    return 1;
  }

  while (1) {
    msgsock = accept(sock, NULL, NULL);

    if (CURL_SOCKET_BAD == msgsock) {
      printf("MAJOR ERROR: accept() failed with error: %d\n", SOCKERRNO);
      break;
    }

    set_advisor_read_lock(SERVERLOGS_LOCK);

#ifdef CURL_SWS_FORK_ENABLED
    if(use_fork) {
      /* The fork enabled version just forks off the child and don't care
         about it anymore, so don't assume otherwise. Beware and don't do
         this at home. */
      rc = fork();
      if(-1 == rc) {
        printf("MAJOR ERROR: fork() failed!\n");
        break;
      }
    }
    else
      /* not a fork, just set rc so the following proceeds nicely */
      rc = 0;
    /* 0 is returned to the child */
    if(0 == rc) {
#endif
    logmsg("====> Client connect");

#ifdef TCP_NODELAY
    /*
     * Disable the Nagle algorithm to make it easier to send out a large
     * response in many small segments to torture the clients more.
     */
    flag = 1;
    if (setsockopt(msgsock, IPPROTO_TCP, TCP_NODELAY,
                   (void *)&flag, sizeof(flag)) == -1) {
      logmsg("====> TCP_NODELAY failed");
    }
#endif

    /* initialization of httprequest struct is done in get_request(), but due
       to pipelining treatment the pipelining struct field must be initialized
       previously to FALSE every time a new connection arrives. */

    req.pipelining = FALSE;

    do {
      if(get_request(msgsock, &req))
        /* non-zero means error, break out of loop */
        break;

      if(prevbounce) {
        /* bounce treatment requested */
        if((req.testno == prevtestno) &&
           (req.partno == prevpartno)) {
          req.partno++;
          logmsg("BOUNCE part number to %ld", req.partno);
        }
        else {
          prevbounce = FALSE;
          prevtestno = -1;
          prevpartno = -1;
        }
      }

      send_doc(msgsock, &req);

      if((req.testno < 0) && (req.testno != DOCNUMBER_CONNECT)) {
        logmsg("special request received, no persistency");
        break;
      }
      if(!req.open) {
        logmsg("instructed to close connection after server-reply");
        break;
      }

      if(req.open)
        logmsg("=> persistant connection request ended, awaits new request");
      /* if we got a CONNECT, loop and get another request as well! */
    } while(req.open || (req.testno == DOCNUMBER_CONNECT));

    logmsg("====> Client disconnect");
    sclose(msgsock);

    clear_advisor_read_lock(SERVERLOGS_LOCK);

    if (req.testno == DOCNUMBER_QUIT)
      break;
#ifdef CURL_SWS_FORK_ENABLED
    }
#endif
  }

  sclose(sock);

  clear_advisor_read_lock(SERVERLOGS_LOCK);

  return 0;
}

