/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: tftpd.c,v 1.55 2009-09-17 15:33:32 yangtse Exp $
 *
 * Trivial file transfer protocol server.
 *
 * This code includes many modifications by Jim Guyton <guyton@rand-unix>
 *
 * This source file was started based on netkit-tftpd 0.17
 * Heavily modified for curl's test suite
 */

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "setup.h" /* portability help from the lib directory */

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
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
#ifdef HAVE_ARPA_TFTP_H
#include <arpa/tftp.h>
#else
#include "tftp.h"
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_SYS_FILIO_H
/* FIONREAD on Solaris 7 */
#include <sys/filio.h>
#endif

#include <setjmp.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#define ENABLE_CURLX_PRINTF
/* make the curlx header define all printf() functions to use the curlx_*
   versions instead */
#include "curlx.h" /* from the private lib dir */
#include "getpart.h"
#include "util.h"

/* include memdebug.h last */
#include "memdebug.h"

#ifdef ENABLE_IPV6
static bool use_ipv6 = FALSE;
#endif
static const char *ipv_inuse = "IPv4";

struct testcase {
  char *buffer;   /* holds the file data to send to the client */
  size_t bufsize; /* size of the data in buffer */
  char *rptr;     /* read pointer into the buffer */
  size_t rcount;  /* amount of data left to read of the file */
  long num;       /* test case number */
  int ofile;      /* file descriptor for output file when uploading to us */
};

static int synchnet(curl_socket_t);
static struct tftphdr *r_init(void);
static struct tftphdr *w_init(void);
static int readit(struct testcase *test, struct tftphdr **dpp, int convert);
static int writeit(struct testcase *test, struct tftphdr **dpp, int ct,
                   int convert);

#define opcode_RRQ   1
#define opcode_WRQ   2
#define opcode_DATA  3
#define opcode_ACK   4
#define opcode_ERROR 5

#define TIMEOUT         5

#define PKTSIZE SEGSIZE+4

struct formats {
  const char *f_mode;
  int f_convert;
};
static struct formats formata[] = {
  { "netascii",   1 },
  { "octet",      0 },
  { NULL,         0 }
};

static int tftp(struct testcase *test, struct tftphdr *tp, ssize_t size);
static void nak(int error);
static void sendtftp(struct testcase *test, struct formats *pf);
static void recvtftp(struct testcase *test, struct formats *pf);
static int validate_access(struct testcase *test, const char *, int);

static curl_socket_t peer;
static int maxtimeout = 5*TIMEOUT;

static char buf[PKTSIZE];
static char ackbuf[PKTSIZE];
static struct sockaddr_in from;
static curl_socklen_t fromlen;

struct bf {
  int counter;            /* size of data in buffer, or flag */
  char buf[PKTSIZE];      /* room for data packet */
};
static struct bf bfs[2];

                                /* Values for bf.counter  */
#define BF_ALLOC -3             /* alloc'd but not yet filled */
#define BF_FREE  -2             /* free */
/* [-1 .. SEGSIZE] = size of data in the data buffer */

static int nextone;     /* index of next buffer to use */
static int current;     /* index of buffer in use */

                           /* control flags for crlf conversions */
static int newline = 0;    /* fillbuf: in middle of newline expansion */
static int prevchar = -1;  /* putbuf: previous char (cr check) */

static void read_ahead(struct testcase *test,
                       int convert /* if true, convert to ascii */);
static ssize_t write_behind(struct testcase *test, int convert);
static struct tftphdr *rw_init(int);
static struct tftphdr *w_init(void) { return rw_init(0); } /* write-behind */
static struct tftphdr *r_init(void) { return rw_init(1); } /* read-ahead */

static struct tftphdr *
rw_init(int x)              /* init for either read-ahead or write-behind */
{                           /* zero for write-behind, one for read-head */
  newline = 0;            /* init crlf flag */
  prevchar = -1;
  bfs[0].counter =  BF_ALLOC;     /* pass out the first buffer */
  current = 0;
  bfs[1].counter = BF_FREE;
  nextone = x;                    /* ahead or behind? */
  return (struct tftphdr *)bfs[0].buf;
}


/* Have emptied current buffer by sending to net and getting ack.
   Free it and return next buffer filled with data.
 */
static int readit(struct testcase *test, struct tftphdr **dpp,
                  int convert /* if true, convert to ascii */)
{
  struct bf *b;

  bfs[current].counter = BF_FREE; /* free old one */
  current = !current;             /* "incr" current */

  b = &bfs[current];              /* look at new buffer */
  if (b->counter == BF_FREE)      /* if it's empty */
    read_ahead(test, convert);    /* fill it */

  *dpp = (struct tftphdr *)b->buf;        /* set caller's ptr */
  return b->counter;
}

#undef MIN /* some systems have this defined already, some don't */
#define MIN(x,y) ((x)<(y)?(x):(y));

/*
 * fill the input buffer, doing ascii conversions if requested
 * conversions are  lf -> cr,lf  and cr -> cr, nul
 */
static void read_ahead(struct testcase *test,
                       int convert /* if true, convert to ascii */)
{
  int i;
  char *p;
  int c;
  struct bf *b;
  struct tftphdr *dp;

  b = &bfs[nextone];              /* look at "next" buffer */
  if (b->counter != BF_FREE)      /* nop if not free */
    return;
  nextone = !nextone;             /* "incr" next buffer ptr */

  dp = (struct tftphdr *)b->buf;

  if (convert == 0) {
    /* The former file reading code did this:
       b->counter = read(fileno(file), dp->th_data, SEGSIZE); */
    size_t copy_n = MIN(SEGSIZE, test->rcount);
    memcpy(dp->th_data, test->rptr, copy_n);

    /* decrease amount, advance pointer */
    test->rcount -= copy_n;
    test->rptr += copy_n;
    b->counter = (int)copy_n;
    return;
  }

  p = dp->th_data;
  for (i = 0 ; i < SEGSIZE; i++) {
    if (newline) {
      if (prevchar == '\n')
        c = '\n';       /* lf to cr,lf */
      else
        c = '\0';       /* cr to cr,nul */
      newline = 0;
    }
    else {
      if(test->rcount) {
        c=test->rptr[0];
        test->rptr++;
        test->rcount--;
      }
      else
        break;
      if (c == '\n' || c == '\r') {
        prevchar = c;
        c = '\r';
        newline = 1;
      }
    }
    *p++ = (char)c;
  }
  b->counter = (int)(p - dp->th_data);
}

/* Update count associated with the buffer, get new buffer from the queue.
   Calls write_behind only if next buffer not available.
 */
static int writeit(struct testcase *test, struct tftphdr **dpp,
                   int ct, int convert)
{
  bfs[current].counter = ct;      /* set size of data to write */
  current = !current;             /* switch to other buffer */
  if (bfs[current].counter != BF_FREE)     /* if not free */
    write_behind(test, convert);     /* flush it */
  bfs[current].counter = BF_ALLOC;        /* mark as alloc'd */
  *dpp =  (struct tftphdr *)bfs[current].buf;
  return ct;                      /* this is a lie of course */
}

/*
 * Output a buffer to a file, converting from netascii if requested.
 * CR,NUL -> CR  and CR,LF => LF.
 * Note spec is undefined if we get CR as last byte of file or a
 * CR followed by anything else.  In this case we leave it alone.
 */
static ssize_t write_behind(struct testcase *test, int convert)
{
  char *writebuf;
  int count;
  int ct;
  char *p;
  int c;                          /* current character */
  struct bf *b;
  struct tftphdr *dp;

  b = &bfs[nextone];
  if (b->counter < -1)            /* anything to flush? */
    return 0;                     /* just nop if nothing to do */

  if(!test->ofile) {
    char outfile[256];
    snprintf(outfile, sizeof(outfile), "log/upload.%ld", test->num);
    test->ofile=open(outfile, O_CREAT|O_RDWR, 0777);
    if(test->ofile == -1) {
      logmsg("Couldn't create and/or open file %s for upload!", outfile);
      return -1; /* failure! */
    }
  }

  count = b->counter;             /* remember byte count */
  b->counter = BF_FREE;           /* reset flag */
  dp = (struct tftphdr *)b->buf;
  nextone = !nextone;             /* incr for next time */
  writebuf = dp->th_data;

  if (count <= 0)
    return -1;                    /* nak logic? */

  if (convert == 0)
    return write(test->ofile, writebuf, count);

  p = writebuf;
  ct = count;
  while (ct--) {                  /* loop over the buffer */
    c = *p++;                     /* pick up a character */
    if (prevchar == '\r') {       /* if prev char was cr */
      if (c == '\n')              /* if have cr,lf then just */
        lseek(test->ofile, -1, SEEK_CUR); /* smash lf on top of the cr */
      else
        if (c == '\0')            /* if have cr,nul then */
          goto skipit;            /* just skip over the putc */
      /* else just fall through and allow it */
    }
    /* formerly
       putc(c, file); */
    write(test->ofile, &c, 1);
    skipit:
    prevchar = c;
  }
  return count;
}


/* When an error has occurred, it is possible that the two sides are out of
 * synch.  Ie: that what I think is the other side's response to packet N is
 * really their response to packet N-1.
 *
 * So, to try to prevent that, we flush all the input queued up for us on the
 * network connection on our host.
 *
 * We return the number of packets we flushed (mostly for reporting when trace
 * is active).
 */

static int synchnet(curl_socket_t f /* socket to flush */)
{

#if defined(HAVE_IOCTLSOCKET)
  unsigned long i;
#else
  int i;
#endif
  int j = 0;
  char rbuf[PKTSIZE];
  struct sockaddr_in fromaddr;
  curl_socklen_t fromaddrlen;

  while (1) {
#if defined(HAVE_IOCTLSOCKET)
    (void) ioctlsocket(f, FIONREAD, &i);
#else
    (void) ioctl(f, FIONREAD, &i);
#endif
    if (i) {
      j++;
      fromaddrlen = sizeof(fromaddr);
      (void)recvfrom(f, rbuf, sizeof(rbuf), 0,
                     (struct sockaddr *)&fromaddr, &fromaddrlen);
    }
    else
      break;
  }
  return j;
}

#if defined(HAVE_ALARM) && defined(SIGALRM)
/*
 * Like signal(), but with well-defined semantics.
 */
static void mysignal(int sig, void (*handler)(int))
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handler;
  sigaction(sig, &sa, NULL);
}
#endif

#ifndef DEFAULT_LOGFILE
#define DEFAULT_LOGFILE "log/tftpd.log"
#endif

#define DEFAULT_PORT 8999 /* UDP */
const char *serverlogfile = DEFAULT_LOGFILE;

#define REQUEST_DUMP  "log/server.input"


int main(int argc, char **argv)
{
  struct sockaddr_in me;
#ifdef ENABLE_IPV6
  struct sockaddr_in6 me6;
#endif /* ENABLE_IPV6 */

  struct tftphdr *tp;
  ssize_t n = 0;
  int arg = 1;
  char *pidname= (char *)".tftpd.pid";
  unsigned short port = DEFAULT_PORT;
  curl_socket_t sock;
  int flag;
  int rc;
  struct testcase test;
  int result = 0;

  while(argc>arg) {
    if(!strcmp("--version", argv[arg])) {
      printf("tftpd IPv4%s\n",
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
#endif

#ifdef ENABLE_IPV6
  if(!use_ipv6)
#endif
    sock = socket(AF_INET, SOCK_DGRAM, 0);
#ifdef ENABLE_IPV6
  else
    sock = socket(AF_INET6, SOCK_DGRAM, 0);
#endif

  if(CURL_SOCKET_BAD == sock) {
    perror("opening stream socket");
    logmsg("Error opening socket");
    return 1;
  }

  flag = 1;
  if (setsockopt
      (sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &flag,
       sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEADDR)");
  }

#ifdef ENABLE_IPV6
  if(!use_ipv6) {
#endif
    me.sin_family = AF_INET;
    me.sin_addr.s_addr = INADDR_ANY;
    me.sin_port = htons(port);
    rc = bind(sock, (struct sockaddr *) &me, sizeof(me));
#ifdef ENABLE_IPV6
  }
  else {
    memset(&me6, 0, sizeof(struct sockaddr_in6));
    me6.sin6_family = AF_INET6;
    me6.sin6_addr = in6addr_any;
    me6.sin6_port = htons(port);
    rc = bind(sock, (struct sockaddr *) &me6, sizeof(me6));
  }
#endif /* ENABLE_IPV6 */
  if(rc < 0) {
    perror("binding stream socket");
    logmsg("Error binding socket");
    sclose(sock);
    return 1;
  }

  if(!write_pidfile(pidname)) {
    sclose(sock);
    return 1;
  }

  logmsg("Running %s version on port UDP/%d", ipv_inuse, (int)port);

  do {
    fromlen = sizeof(from);
    n = (ssize_t)recvfrom(sock, buf, sizeof(buf), 0,
                          (struct sockaddr *)&from, &fromlen);
    if (n < 0) {
      logmsg("recvfrom");
      result = 3;
      break;
    }

    set_advisor_read_lock(SERVERLOGS_LOCK);

    from.sin_family = AF_INET;

    peer = socket(AF_INET, SOCK_DGRAM, 0);
    if(CURL_SOCKET_BAD == peer) {
      logmsg("socket");
      result = 2;
      break;
    }

    if (connect(peer, (struct sockaddr *)&from, sizeof(from)) < 0) {
      logmsg("connect: fail");
      result = 1;
      break;
    }
    maxtimeout = 5*TIMEOUT;

    tp = (struct tftphdr *)buf;
    tp->th_opcode = ntohs(tp->th_opcode);
    if (tp->th_opcode == opcode_RRQ || tp->th_opcode == opcode_WRQ) {
      memset(&test, 0, sizeof(test));
      if (tftp(&test, tp, n) < 0)
        break;
      if(test.buffer)
        free(test.buffer);
    }
    sclose(peer);

    clear_advisor_read_lock(SERVERLOGS_LOCK);

    logmsg("end of one transfer");

  } while(1);

  clear_advisor_read_lock(SERVERLOGS_LOCK);

  return result;
}

/*
 * Handle initial connection protocol.
 */
static int tftp(struct testcase *test, struct tftphdr *tp, ssize_t size)
{
  char *cp;
  int first = 1, ecode;
  struct formats *pf;
  char *filename, *mode = NULL;
  int error;
  FILE *server;

  /* Open request dump file. */
  server = fopen(REQUEST_DUMP, "ab");
  if(!server) {
    error = ERRNO;
    logmsg("fopen() failed with error: %d %s", error, strerror(error));
    logmsg("Error opening file: %s", REQUEST_DUMP);
    return -1;
  }

  /* store input protocol */
  fprintf(server, "opcode: %x\n", tp->th_opcode);

  cp = (char *)&tp->th_stuff;
  filename = cp;
again:
  while (cp < buf + size) {
    if (*cp == '\0')
      break;
    cp++;
  }
  if (*cp) {
    nak(EBADOP);
    fclose(server);
    return 3;
  }
  if (first) {
    mode = ++cp;
    first = 0;
    goto again;
  }
  /* store input protocol */
  fprintf(server, "filename: %s\n", filename);

  for (cp = mode; cp && *cp; cp++)
    if(ISUPPER(*cp))
      *cp = (char)tolower((int)*cp);

  /* store input protocol */
  fprintf(server, "mode: %s\n", mode);
  fclose(server);

  for (pf = formata; pf->f_mode; pf++)
    if (strcmp(pf->f_mode, mode) == 0)
      break;
  if (!pf->f_mode) {
    nak(EBADOP);
    return 2;
  }
  ecode = validate_access(test, filename, tp->th_opcode);
  if (ecode) {
    nak(ecode);
    return 1;
  }
  if (tp->th_opcode == opcode_WRQ)
    recvtftp(test, pf);
  else
    sendtftp(test, pf);

  return 0;
}

/*
 * Validate file access.
 */
static int validate_access(struct testcase *test,
                           const char *filename, int mode)
{
  char *ptr;
  long testno, partno;
  int error;
  char partbuf[80]="data";

  logmsg("trying to get file: %s mode %x", filename, mode);

  if(!strncmp("verifiedserver", filename, 14)) {
    char weare[128];
    size_t count = sprintf(weare, "WE ROOLZ: %ld\r\n", (long)getpid());

    logmsg("Are-we-friendly question received");
    test->buffer = strdup(weare);
    test->rptr = test->buffer; /* set read pointer */
    test->bufsize = count;    /* set total count */
    test->rcount = count;     /* set data left to read */
    return 0; /* fine */
  }

  /* find the last slash */
  ptr = strrchr(filename, '/');

  if(ptr) {
    char *file;

    ptr++; /* skip the slash */

    /* skip all non-numericals following the slash */
    while(*ptr && !ISDIGIT(*ptr))
      ptr++;

    /* get the number */
    testno = strtol(ptr, &ptr, 10);

    if(testno > 10000) {
      partno = testno % 10000;
      testno /= 10000;
    }
    else
      partno = 0;


    logmsg("requested test number %ld part %ld", testno, partno);

    test->num = testno;

    file = test2file(testno);

    if(0 != partno)
      sprintf(partbuf, "data%ld", partno);

    if(file) {
      FILE *stream=fopen(file, "rb");
      if(!stream) {
        error = ERRNO;
        logmsg("fopen() failed with error: %d %s", error, strerror(error));
        logmsg("Error opening file: %s", file);
        logmsg("Couldn't open test file: %s", file);
        return EACCESS;
      }
      else {
        size_t count;
        test->buffer = (char *)spitout(stream, "reply", partbuf, &count);
        fclose(stream);
        if(test->buffer) {
          test->rptr = test->buffer; /* set read pointer */
          test->bufsize = count;    /* set total count */
          test->rcount = count;     /* set data left to read */
        }
        else
          return EACCESS;
      }

    }
    else
      return EACCESS;
  }
  else {
    logmsg("no slash found in path");
    return EACCESS; /* failure */
  }

  logmsg("file opened and all is good");
  return 0;
}

static int timeout;
#ifdef HAVE_SIGSETJMP
static sigjmp_buf timeoutbuf;
#endif

#if defined(HAVE_ALARM) && defined(SIGALRM)
static int rexmtval = TIMEOUT;

static void timer(int signum)
{
  (void)signum;

  logmsg("alarm!");

  timeout += rexmtval;
  if(timeout >= maxtimeout) {
    clear_advisor_read_lock(SERVERLOGS_LOCK);
    exit(1);
  }
#ifdef HAVE_SIGSETJMP
  siglongjmp(timeoutbuf, 1);
#endif
}

static void justtimeout(int signum)
{
  (void)signum;
}
#endif  /* HAVE_ALARM && SIGALRM */

static unsigned short sendblock;
static struct tftphdr *sdp;
static struct tftphdr *sap; /* ack packet */
/*
 * Send the requested file.
 */
static void sendtftp(struct testcase *test, struct formats *pf)
{
  int size;
  ssize_t n;
  sendblock = 1;
#if defined(HAVE_ALARM) && defined(SIGALRM)
  mysignal(SIGALRM, timer);
#endif
  sdp = r_init();
  sap = (struct tftphdr *)ackbuf;
  do {
    size = readit(test, &sdp, pf->f_convert);
    if (size < 0) {
      nak(ERRNO + 100);
      return;
    }
    sdp->th_opcode = htons((u_short)opcode_DATA);
    sdp->th_block = htons((u_short)sendblock);
    timeout = 0;
#ifdef HAVE_SIGSETJMP
    (void) sigsetjmp(timeoutbuf, 1);
#endif
    send_data:
    if (swrite(peer, sdp, size + 4) != size + 4) {
      logmsg("write");
      return;
    }
    read_ahead(test, pf->f_convert);
    for ( ; ; ) {
#ifdef HAVE_ALARM
      alarm(rexmtval);        /* read the ack */
#endif
      n = sread(peer, ackbuf, sizeof (ackbuf));
#ifdef HAVE_ALARM
      alarm(0);
#endif
      if (n < 0) {
        logmsg("read: fail");
        return;
      }
      sap->th_opcode = ntohs((u_short)sap->th_opcode);
      sap->th_block = ntohs((u_short)sap->th_block);

      if (sap->th_opcode == opcode_ERROR) {
        logmsg("got ERROR");
        return;
      }

      if (sap->th_opcode == opcode_ACK) {
        if (sap->th_block == sendblock) {
          break;
        }
        /* Re-synchronize with the other side */
        (void) synchnet(peer);
        if (sap->th_block == (sendblock-1)) {
          goto send_data;
        }
      }

    }
    sendblock++;
  } while (size == SEGSIZE);
}


static unsigned short recvblock;
static struct tftphdr *rdp;
static struct tftphdr *rap; /* ack buffer */
/*
 * Receive a file.
 */
static void recvtftp(struct testcase *test, struct formats *pf)
{
  ssize_t n, size;
  recvblock = 0;
#if defined(HAVE_ALARM) && defined(SIGALRM)
  mysignal(SIGALRM, timer);
#endif
  rdp = w_init();
  rap = (struct tftphdr *)ackbuf;
  do {
    timeout = 0;
    rap->th_opcode = htons((u_short)opcode_ACK);
    rap->th_block = htons((u_short)recvblock);
    recvblock++;
#ifdef HAVE_SIGSETJMP
    (void) sigsetjmp(timeoutbuf, 1);
#endif
send_ack:
    if (swrite(peer, ackbuf, 4) != 4) {
      logmsg("write: fail\n");
      goto abort;
    }
    write_behind(test, pf->f_convert);
    for ( ; ; ) {
#ifdef HAVE_ALARM
      alarm(rexmtval);
#endif
      n = sread(peer, rdp, PKTSIZE);
#ifdef HAVE_ALARM
      alarm(0);
#endif
      if (n < 0) {                       /* really? */
        logmsg("read: fail\n");
        goto abort;
      }
      rdp->th_opcode = ntohs((u_short)rdp->th_opcode);
      rdp->th_block = ntohs((u_short)rdp->th_block);
      if (rdp->th_opcode == opcode_ERROR)
        goto abort;
      if (rdp->th_opcode == opcode_DATA) {
        if (rdp->th_block == recvblock) {
          break;                         /* normal */
        }
        /* Re-synchronize with the other side */
        (void) synchnet(peer);
        if (rdp->th_block == (recvblock-1))
          goto send_ack;                 /* rexmit */
      }
    }

    size = writeit(test, &rdp, (int)(n - 4), pf->f_convert);
    if (size != (n-4)) {                 /* ahem */
      if (size < 0)
        nak(ERRNO + 100);
      else
        nak(ENOSPACE);
      goto abort;
    }
  } while (size == SEGSIZE);
  write_behind(test, pf->f_convert);

  rap->th_opcode = htons((u_short)opcode_ACK);  /* send the "final" ack */
  rap->th_block = htons((u_short)recvblock);
  (void) swrite(peer, ackbuf, 4);
#if defined(HAVE_ALARM) && defined(SIGALRM)
  mysignal(SIGALRM, justtimeout);        /* just abort read on timeout */
  alarm(rexmtval);
#endif
  n = sread(peer, buf, sizeof(buf));     /* normally times out and quits */
#ifdef HAVE_ALARM
  alarm(0);
#endif
  if (n >= 4 &&                          /* if read some data */
      rdp->th_opcode == opcode_DATA &&   /* and got a data block */
      recvblock == rdp->th_block) {      /* then my last ack was lost */
    (void) swrite(peer, ackbuf, 4);      /* resend final ack */
  }
abort:
  return;
}

struct errmsg {
  int e_code;
  const char *e_msg;
};
static struct errmsg errmsgs[] = {
  { EUNDEF,       "Undefined error code" },
  { ENOTFOUND,    "File not found" },
  { EACCESS,      "Access violation" },
  { ENOSPACE,     "Disk full or allocation exceeded" },
  { EBADOP,       "Illegal TFTP operation" },
  { EBADID,       "Unknown transfer ID" },
  { EEXISTS,      "File already exists" },
  { ENOUSER,      "No such user" },
  { -1,           0 }
};

/*
 * Send a nak packet (error message).  Error code passed in is one of the
 * standard TFTP codes, or a UNIX errno offset by 100.
 */
static void nak(int error)
{
  struct tftphdr *tp;
  int length;
  struct errmsg *pe;

  tp = (struct tftphdr *)buf;
  tp->th_opcode = htons((u_short)opcode_ERROR);
  tp->th_code = htons((u_short)error);
  for (pe = errmsgs; pe->e_code >= 0; pe++)
    if (pe->e_code == error)
      break;
  if (pe->e_code < 0) {
    pe->e_msg = strerror(error - 100);
    tp->th_code = EUNDEF;   /* set 'undef' errorcode */
  }
  strcpy(tp->th_msg, pe->e_msg);
  length = (int)strlen(pe->e_msg);
  tp->th_msg[length] = '\0';
  length += 5;
  if (swrite(peer, buf, length) != length)
    logmsg("nak: fail\n");
}
