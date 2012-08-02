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
 * $Id: tftp.c,v 1.93 2009-08-12 08:19:39 bagder Exp $
 ***************************************************************************/

#include "setup.h"

#ifndef CURL_DISABLE_TFTP
/* -- WIN32 approved -- */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#if defined(WIN32)
#include <time.h>
#include <io.h>
#else
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <netdb.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#endif /* WIN32 */

#include "urldata.h"
#include <curl/curl.h>
#include "transfer.h"
#include "sendf.h"
#include "tftp.h"
#include "progress.h"
#include "connect.h"
#include "strerror.h"
#include "sockaddr.h" /* required for Curl_sockaddr_storage */
#include "url.h"
#include "rawstr.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>

#include "curl_memory.h"
#include "select.h"

/* The last #include file should be: */
#include "memdebug.h"

/* RFC2348 allows the block size to be negotiated */
#define TFTP_BLKSIZE_DEFAULT 512
#define TFTP_BLKSIZE_MIN 8
#define TFTP_BLKSIZE_MAX 65464
#define TFTP_OPTION_BLKSIZE "blksize"
#define TFTP_OPTION_TSIZE "tsize"
#define TFTP_OPTION_INTERVAL "interval"

typedef enum {
  TFTP_MODE_NETASCII=0,
  TFTP_MODE_OCTET
} tftp_mode_t;

typedef enum {
  TFTP_STATE_START=0,
  TFTP_STATE_RX,
  TFTP_STATE_TX,
  TFTP_STATE_FIN
} tftp_state_t;

typedef enum {
  TFTP_EVENT_INIT=0,
  TFTP_EVENT_RRQ = 1,
  TFTP_EVENT_WRQ = 2,
  TFTP_EVENT_DATA = 3,
  TFTP_EVENT_ACK = 4,
  TFTP_EVENT_ERROR = 5,
  TFTP_EVENT_OACK = 6,
  TFTP_EVENT_TIMEOUT
} tftp_event_t;

typedef enum {
  TFTP_ERR_UNDEF=0,
  TFTP_ERR_NOTFOUND,
  TFTP_ERR_PERM,
  TFTP_ERR_DISKFULL,
  TFTP_ERR_ILLEGAL,
  TFTP_ERR_UNKNOWNID,
  TFTP_ERR_EXISTS,
  TFTP_ERR_NOSUCHUSER,  /* This will never be triggered by this code */

  /* The remaining error codes are internal to curl */
  TFTP_ERR_NONE = -100,
  TFTP_ERR_TIMEOUT,
  TFTP_ERR_NORESPONSE
} tftp_error_t;

typedef struct tftp_packet {
  unsigned char *data;
} tftp_packet_t;

typedef struct tftp_state_data {
  tftp_state_t    state;
  tftp_mode_t     mode;
  tftp_error_t    error;
  struct connectdata      *conn;
  curl_socket_t   sockfd;
  int             retries;
  time_t          retry_time;
  time_t          retry_max;
  time_t          start_time;
  time_t          max_time;
  unsigned short  block;
  struct Curl_sockaddr_storage   local_addr;
  struct Curl_sockaddr_storage   remote_addr;
  curl_socklen_t  remote_addrlen;
  ssize_t         rbytes;
  size_t          sbytes;
  size_t          blksize;
  int             requested_blksize;
  tftp_packet_t   rpacket;
  tftp_packet_t   spacket;
} tftp_state_data_t;


/* Forward declarations */
static CURLcode tftp_rx(tftp_state_data_t *state, tftp_event_t event) ;
static CURLcode tftp_tx(tftp_state_data_t *state, tftp_event_t event) ;
static CURLcode tftp_connect(struct connectdata *conn, bool *done);
static CURLcode tftp_disconnect(struct connectdata *conn);
static CURLcode tftp_do(struct connectdata *conn, bool *done);
static CURLcode tftp_done(struct connectdata *conn,
                               CURLcode, bool premature);
static CURLcode tftp_setup_connection(struct connectdata * conn);


/*
 * TFTP protocol handler.
 */

const struct Curl_handler Curl_handler_tftp = {
  "TFTP",                               /* scheme */
  tftp_setup_connection,                /* setup_connection */
  tftp_do,                              /* do_it */
  tftp_done,                            /* done */
  ZERO_NULL,                            /* do_more */
  tftp_connect,                         /* connect_it */
  ZERO_NULL,                            /* connecting */
  ZERO_NULL,                            /* doing */
  ZERO_NULL,                            /* proto_getsock */
  ZERO_NULL,                            /* doing_getsock */
  ZERO_NULL,                            /* perform_getsock */
  tftp_disconnect,                      /* disconnect */
  PORT_TFTP,                            /* defport */
  PROT_TFTP                             /* protocol */
};


/**********************************************************
 *
 * tftp_set_timeouts -
 *
 * Set timeouts based on state machine state.
 * Use user provided connect timeouts until DATA or ACK
 * packet is received, then use user-provided transfer timeouts
 *
 *
 **********************************************************/
static CURLcode tftp_set_timeouts(tftp_state_data_t *state)
{
  time_t maxtime, timeout;
  long timeout_ms;
  bool start = (bool)(state->state == TFTP_STATE_START);

  time(&state->start_time);

  /* Compute drop-dead time */
  timeout_ms = Curl_timeleft(state->conn, NULL, start);

  if(timeout_ms < 0) {
    /* time-out, bail out, go home */
    failf(state->conn->data, "Connection time-out");
    return CURLE_OPERATION_TIMEDOUT;
  }

  if(start) {

    maxtime = (time_t)(timeout_ms + 500) / 1000;
    state->max_time = state->start_time+maxtime;

    /* Set per-block timeout to total */
    timeout = maxtime ;

    /* Average restart after 5 seconds */
    state->retry_max = timeout/5;

    if(state->retry_max < 1)
      /* avoid division by zero below */
      state->retry_max = 1;

    /* Compute the re-start interval to suit the timeout */
    state->retry_time = timeout/state->retry_max;
    if(state->retry_time<1)
      state->retry_time=1;

  }
  else {
    if(timeout_ms > 0)
      maxtime = (time_t)(timeout_ms + 500) / 1000;
    else
      maxtime = 3600;

    state->max_time = state->start_time+maxtime;

    /* Set per-block timeout to 10% of total */
    timeout = maxtime/10 ;

    /* Average reposting an ACK after 15 seconds */
    state->retry_max = timeout/15;
  }
  /* But bound the total number  */
  if(state->retry_max<3)
    state->retry_max=3;

  if(state->retry_max>50)
    state->retry_max=50;

  /* Compute the re-ACK interval to suit the timeout */
  state->retry_time = timeout/state->retry_max;
  if(state->retry_time<1)
    state->retry_time=1;

  infof(state->conn->data,
        "set timeouts for state %d; Total %d, retry %d maxtry %d\n",
        state->state, (state->max_time-state->start_time),
        state->retry_time, state->retry_max);

  return CURLE_OK;
}

/**********************************************************
 *
 * tftp_set_send_first
 *
 * Event handler for the START state
 *
 **********************************************************/

static void setpacketevent(tftp_packet_t *packet, unsigned short num)
{
  packet->data[0] = (unsigned char)(num >> 8);
  packet->data[1] = (unsigned char)(num & 0xff);
}


static void setpacketblock(tftp_packet_t *packet, unsigned short num)
{
  packet->data[2] = (unsigned char)(num >> 8);
  packet->data[3] = (unsigned char)(num & 0xff);
}

static unsigned short getrpacketevent(const tftp_packet_t *packet)
{
  return (unsigned short)((packet->data[0] << 8) | packet->data[1]);
}

static unsigned short getrpacketblock(const tftp_packet_t *packet)
{
  return (unsigned short)((packet->data[2] << 8) | packet->data[3]);
}

static size_t Curl_strnlen(const char *string, size_t maxlen)
{
  const char *end = memchr (string, '\0', maxlen);
  return end ? (size_t) (end - string) : maxlen;
}

static const char *tftp_option_get(const char *buf, size_t len,
                                   const char **option, const char **value)
{
  size_t loc;

  loc = Curl_strnlen( buf, len );
  loc++; /* NULL term */

  if (loc >= len)
    return NULL;
  *option = buf;

  loc += Curl_strnlen( buf+loc, len-loc );
  loc++; /* NULL term */

  if (loc > len)
    return NULL;
  *value = &buf[strlen(*option) + 1];

  return &buf[loc];
}

static CURLcode tftp_parse_option_ack(tftp_state_data_t *state,
                                      const char *ptr, int len)
{
  const char *tmp = ptr;
  struct SessionHandle *data = state->conn->data;

  /* if OACK doesn't contain blksize option, the default (512) must be used */
  state->blksize = TFTP_BLKSIZE_DEFAULT;

  while (tmp < ptr + len) {
    const char *option, *value;

    tmp = tftp_option_get(tmp, ptr + len - tmp, &option, &value);
    if(tmp == NULL) {
      failf(data, "Malformed ACK packet, rejecting");
      return CURLE_TFTP_ILLEGAL;
    }

    infof(data, "got option=(%s) value=(%s)\n", option, value);

    if(checkprefix(option, TFTP_OPTION_BLKSIZE)) {
      int blksize;

      blksize = (int)strtol( value, NULL, 10 );

      if(!blksize) {
        failf(data, "invalid blocksize value in OACK packet");
        return CURLE_TFTP_ILLEGAL;
      }
      else if(blksize > TFTP_BLKSIZE_MAX) {
        failf(data, "%s (%d)", "blksize is larger than max supported",
                TFTP_BLKSIZE_MAX);
        return CURLE_TFTP_ILLEGAL;
      }
      else if(blksize < TFTP_BLKSIZE_MIN) {
        failf(data, "%s (%d)", "blksize is smaller than min supported",
                TFTP_BLKSIZE_MIN);
        return CURLE_TFTP_ILLEGAL;
      }
      else if (blksize > state->requested_blksize) {
        /* could realloc pkt buffers here, but the spec doesn't call out
         * support for the server requesting a bigger blksize than the client
         * requests */
        failf(data, "%s (%d)",
                "server requested blksize larger than allocated", blksize);
        return CURLE_TFTP_ILLEGAL;
      }

      state->blksize = blksize;
      infof(data, "%s (%d) %s (%d)\n", "blksize parsed from OACK",
        state->blksize, "requested", state->requested_blksize);
    }
    else if(checkprefix(option, TFTP_OPTION_TSIZE)) {
      long tsize = 0;

      tsize = strtol( value, NULL, 10 );
      if(!tsize) {
        failf(data, "invalid tsize value in OACK packet");
        return CURLE_TFTP_ILLEGAL;
      }
      Curl_pgrsSetDownloadSize(data, tsize);
      infof(data, "%s (%d)\n", "tsize parsed from OACK", tsize);
    }
  }

  return CURLE_OK;
}

static size_t tftp_option_add(tftp_state_data_t *state, size_t csize,
                              char *buf, const char *option)
{
  if( ( strlen(option) + csize + 1U ) > state->blksize )
        return 0;
  strcpy(buf, option);
  return( strlen(option) + 1 );
}

static CURLcode tftp_connect_for_tx(tftp_state_data_t *state, tftp_event_t event)
{
  CURLcode res;
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  struct SessionHandle *data = state->conn->data;

  infof(data, "%s\n", "Connected for transmit");
#endif
  state->state = TFTP_STATE_TX;
  res = tftp_set_timeouts(state);
  if(res != CURLE_OK)
    return(res);
  return tftp_tx(state, event);
}

static CURLcode tftp_connect_for_rx(tftp_state_data_t *state, tftp_event_t event)
{
  CURLcode res;
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  struct SessionHandle *data = state->conn->data;

  infof(data, "%s\n", "Connected for receive");
#endif
  state->state = TFTP_STATE_RX;
  res = tftp_set_timeouts(state);
  if(res != CURLE_OK)
    return(res);
  return tftp_rx(state, event);
}

static CURLcode tftp_send_first(tftp_state_data_t *state, tftp_event_t event)
{
  size_t sbytes;
  const char *mode = "octet";
  char *filename;
  char buf[64];
  struct SessionHandle *data = state->conn->data;
  CURLcode res = CURLE_OK;

  /* Set ascii mode if -B flag was used */
  if(data->set.prefer_ascii)
    mode = "netascii";

  switch(event) {

  case TFTP_EVENT_INIT:    /* Send the first packet out */
  case TFTP_EVENT_TIMEOUT: /* Resend the first packet out */
    /* Increment the retry counter, quit if over the limit */
    state->retries++;
    if(state->retries>state->retry_max) {
      state->error = TFTP_ERR_NORESPONSE;
      state->state = TFTP_STATE_FIN;
      return res;
    }

    if(data->set.upload) {
      /* If we are uploading, send an WRQ */
      setpacketevent(&state->spacket, TFTP_EVENT_WRQ);
      state->conn->data->req.upload_fromhere =
        (char *)state->spacket.data+4;
      if(data->set.infilesize != -1)
        Curl_pgrsSetUploadSize(data, data->set.infilesize);
    }
    else {
      /* If we are downloading, send an RRQ */
      setpacketevent(&state->spacket, TFTP_EVENT_RRQ);
    }
    /* As RFC3617 describes the separator slash is not actually part of the
       file name so we skip the always-present first letter of the path
       string. */
    filename = curl_easy_unescape(data, &state->conn->data->state.path[1], 0,
                                  NULL);
    if(!filename)
      return CURLE_OUT_OF_MEMORY;

    snprintf((char *)state->spacket.data+2,
             state->blksize,
             "%s%c%s%c", filename, '\0',  mode, '\0');
    sbytes = 4 + strlen(filename) + strlen(mode);

    /* add tsize option */
    if(data->set.upload && (data->set.infilesize != -1))
      snprintf( buf, sizeof(buf), "%" FORMAT_OFF_T, data->set.infilesize );
    else
      strcpy(buf, "0"); /* the destination is large enough */

    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes,
                              TFTP_OPTION_TSIZE);
    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes, buf);
    /* add blksize option */
    snprintf( buf, sizeof(buf), "%d", state->requested_blksize );
    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes,
                              TFTP_OPTION_BLKSIZE);
    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes, buf );
    /* add timeout option */
    snprintf( buf, sizeof(buf), "%d", state->retry_time );
    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes,
                              TFTP_OPTION_INTERVAL);
    sbytes += tftp_option_add(state, sbytes,
                              (char *)state->spacket.data+sbytes, buf );

    if (sendto(state->sockfd, (void *)state->spacket.data,
               sbytes, 0,
               state->conn->ip_addr->ai_addr,
               state->conn->ip_addr->ai_addrlen) < 0) {
      failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
    }
    Curl_safefree(filename);
    break;

  case TFTP_EVENT_OACK:
    if(data->set.upload) {
      res = tftp_connect_for_tx(state, event);
    }
    else {
      res = tftp_connect_for_rx(state, event);
    }
    break;

  case TFTP_EVENT_ACK: /* Connected for transmit */
    res = tftp_connect_for_tx(state, event);
    break;

  case TFTP_EVENT_DATA: /* Connected for receive */
    res = tftp_connect_for_rx(state, event);
    break;

  case TFTP_EVENT_ERROR:
    state->state = TFTP_STATE_FIN;
    break;

  default:
    failf(state->conn->data, "tftp_send_first: internal error");
    break;
  }
  return res;
}

/**********************************************************
 *
 * tftp_rx
 *
 * Event handler for the RX state
 *
 **********************************************************/
static CURLcode tftp_rx(tftp_state_data_t *state, tftp_event_t event)
{
  int rblock;
  struct SessionHandle *data = state->conn->data;

  switch(event) {

  case TFTP_EVENT_DATA:
    /* Is this the block we expect? */
    rblock = getrpacketblock(&state->rpacket);
    if((state->block+1) != rblock) {
      /* No, log it, up the retry count and fail if over the limit */
      infof(data,
            "Received unexpected DATA packet block %d\n", rblock);
      state->retries++;
      if(state->retries>state->retry_max) {
        failf(data, "tftp_rx: giving up waiting for block %d",
              state->block+1);
        return CURLE_TFTP_ILLEGAL;
      }
    }
    /* This is the expected block.  Reset counters and ACK it. */
    state->block = (unsigned short)rblock;
    state->retries = 0;
    setpacketevent(&state->spacket, TFTP_EVENT_ACK);
    setpacketblock(&state->spacket, state->block);
    if(sendto(state->sockfd, (void *)state->spacket.data,
              4, SEND_4TH_ARG,
              (struct sockaddr *)&state->remote_addr,
              state->remote_addrlen) < 0) {
      failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
      return CURLE_SEND_ERROR;
    }

    /* Check if completed (That is, a less than full packet is received) */
    if(state->rbytes < (ssize_t)state->blksize+4){
      state->state = TFTP_STATE_FIN;
    }
    else {
      state->state = TFTP_STATE_RX;
    }
    break;

  case TFTP_EVENT_OACK:
    /* ACK option acknowledgement so we can move on to data */
    state->block = 0;
    state->retries = 0;
    setpacketevent(&state->spacket, TFTP_EVENT_ACK);
    setpacketblock(&state->spacket, state->block);
    if(sendto(state->sockfd, (void *)state->spacket.data,
              4, SEND_4TH_ARG,
              (struct sockaddr *)&state->remote_addr,
              state->remote_addrlen) < 0) {
      failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
      return CURLE_SEND_ERROR;
    }

    /* we're ready to RX data */
    state->state = TFTP_STATE_RX;
    break;

  case TFTP_EVENT_TIMEOUT:
    /* Increment the retry count and fail if over the limit */
    state->retries++;
    infof(data,
          "Timeout waiting for block %d ACK.  Retries = %d\n", state->retries);
    if(state->retries > state->retry_max) {
      state->error = TFTP_ERR_TIMEOUT;
      state->state = TFTP_STATE_FIN;
    }
    else {
      /* Resend the previous ACK and check all sbytes were sent */
      if(sendto(state->sockfd, (void *)state->spacket.data,
                4, SEND_4TH_ARG,
                (struct sockaddr *)&state->remote_addr,
                state->remote_addrlen) < 0) {
        failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
        return CURLE_SEND_ERROR;
      }
    }
    break;

  case TFTP_EVENT_ERROR:
    state->state = TFTP_STATE_FIN;
    break;

  default:
    failf(data, "%s", "tftp_rx: internal error");
    return CURLE_TFTP_ILLEGAL; /* not really the perfect return code for
                                  this */
  }
  return CURLE_OK;
}

/**********************************************************
 *
 * tftp_tx
 *
 * Event handler for the TX state
 *
 **********************************************************/
static CURLcode tftp_tx(tftp_state_data_t *state, tftp_event_t event)
{
  struct SessionHandle *data = state->conn->data;
  int rblock;
  int readcount;
  CURLcode res = CURLE_OK;
  struct SingleRequest *k = &data->req;

  switch(event) {

  case TFTP_EVENT_ACK:
    /* Ack the packet */
    rblock = getrpacketblock(&state->rpacket);

    if(rblock != state->block) {
      /* This isn't the expected block.  Log it and up the retry counter */
      infof(data, "Received ACK for block %d, expecting %d\n",
            rblock, state->block);
      state->retries++;
      /* Bail out if over the maximum */
      if(state->retries>state->retry_max) {
        failf(data, "tftp_tx: giving up waiting for block %d ack",
              state->block);
        res = CURLE_SEND_ERROR;
      }
      else {
        /* Re-send the data packet and check all sbytes were sent */
        if(sendto(state->sockfd, (void *)&state->spacket,
                  4+state->sbytes, SEND_4TH_ARG,
                  (struct sockaddr *)&state->remote_addr,
                  state->remote_addrlen) < 0) {
          failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
          res = CURLE_SEND_ERROR;
        }
      }
      return res;
    }
    /* fall-through */
  case TFTP_EVENT_OACK:
    /* This is the expected packet.  Reset the counters and send the next
       block */
    state->block++;
    state->retries = 0;
    setpacketevent(&state->spacket, TFTP_EVENT_DATA);
    setpacketblock(&state->spacket, state->block);
    if(state->block > 1 && state->sbytes < state->blksize) {
      state->state = TFTP_STATE_FIN;
      return CURLE_OK;
    }
    res = Curl_fillreadbuffer(state->conn, (int)state->blksize, &readcount);
    state->sbytes = readcount;
    if(res)
      return res;
    /* Send the data packet and check all sbytes were sent */
    if(sendto(state->sockfd, (void *)state->spacket.data,
              4+state->sbytes, SEND_4TH_ARG,
              (struct sockaddr *)&state->remote_addr,
              state->remote_addrlen) < 0) {
      failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
      return CURLE_SEND_ERROR;
    }
    /* Update the progress meter */
    k->writebytecount += state->sbytes;
    Curl_pgrsSetUploadCounter(data, k->writebytecount);
    break;

  case TFTP_EVENT_TIMEOUT:
    /* Increment the retry counter and log the timeout */
    state->retries++;
    infof(data, "Timeout waiting for block %d ACK. "
          " Retries = %d\n", state->retries);
    /* Decide if we've had enough */
    if(state->retries > state->retry_max) {
      state->error = TFTP_ERR_TIMEOUT;
      state->state = TFTP_STATE_FIN;
    }
    else {
      /* Re-send the data packet and check all sbytes were sent */
      if(sendto(state->sockfd, (void *)state->spacket.data,
                4+state->sbytes, SEND_4TH_ARG,
                (struct sockaddr *)&state->remote_addr,
                state->remote_addrlen) < 0) {
        failf(data, "%s", Curl_strerror(state->conn, SOCKERRNO));
        return CURLE_SEND_ERROR;
      }
      /* since this was a re-send, we remain at the still byte position */
      Curl_pgrsSetUploadCounter(data, k->writebytecount);
    }
    break;

  case TFTP_EVENT_ERROR:
    state->state = TFTP_STATE_FIN;
    break;

  default:
    failf(data, "%s", "tftp_tx: internal error");
    break;
  }

  return res;
}

/**********************************************************
 *
 * tftp_state_machine
 *
 * The tftp state machine event dispatcher
 *
 **********************************************************/
static CURLcode tftp_state_machine(tftp_state_data_t *state,
                                   tftp_event_t event)
{
  CURLcode res = CURLE_OK;
  struct SessionHandle *data = state->conn->data;
  switch(state->state) {
  case TFTP_STATE_START:
    DEBUGF(infof(data, "TFTP_STATE_START\n"));
    res = tftp_send_first(state, event);
    break;
  case TFTP_STATE_RX:
    DEBUGF(infof(data, "TFTP_STATE_RX\n"));
    res = tftp_rx(state, event);
    break;
  case TFTP_STATE_TX:
    DEBUGF(infof(data, "TFTP_STATE_TX\n"));
    res = tftp_tx(state, event);
    break;
  case TFTP_STATE_FIN:
    infof(data, "%s\n", "TFTP finished");
    break;
  default:
    DEBUGF(infof(data, "STATE: %d\n", state->state));
    failf(data, "%s", "Internal state machine error");
    res = CURLE_TFTP_ILLEGAL;
    break;
  }
  return res;
}

/**********************************************************
 *
 * tftp_disconnect
 *
 * The disconnect callback
 *
 **********************************************************/
static CURLcode tftp_disconnect(struct connectdata *conn)
{
  tftp_state_data_t *state = conn->proto.tftpc;

  /* done, free dynamically allocated pkt buffers */
  if(state) {
    Curl_safefree(state->rpacket.data);
    Curl_safefree(state->spacket.data);
    free(state);
  }

  return CURLE_OK;
}

/**********************************************************
 *
 * tftp_connect
 *
 * The connect callback
 *
 **********************************************************/
static CURLcode tftp_connect(struct connectdata *conn, bool *done)
{
  CURLcode code;
  tftp_state_data_t *state;
  int blksize, rc;

  blksize = TFTP_BLKSIZE_DEFAULT;

  /* If there already is a protocol-specific struct allocated for this
     sessionhandle, deal with it */
  Curl_reset_reqproto(conn);

  state = conn->proto.tftpc = calloc(sizeof(tftp_state_data_t), 1);
  if(!state)
    return CURLE_OUT_OF_MEMORY;

  /* alloc pkt buffers based on specified blksize */
  if(conn->data->set.tftp_blksize) {
    blksize = (int)conn->data->set.tftp_blksize;
    if(blksize > TFTP_BLKSIZE_MAX || blksize < TFTP_BLKSIZE_MIN )
      return CURLE_TFTP_ILLEGAL;
  }

  if(!state->rpacket.data) {
    state->rpacket.data = calloc(1, blksize + 2 + 2);

    if(!state->rpacket.data)
      return CURLE_OUT_OF_MEMORY;
  }

  if(!state->spacket.data) {
    state->spacket.data = calloc(1, blksize + 2 + 2);

    if(!state->spacket.data)
      return CURLE_OUT_OF_MEMORY;
  }

  conn->bits.close = TRUE; /* we don't keep TFTP connections up bascially
                              because there's none or very little gain for UDP
                           */

  state->conn = conn;
  state->sockfd = state->conn->sock[FIRSTSOCKET];
  state->state = TFTP_STATE_START;
  state->error = TFTP_ERR_NONE;
  state->blksize = TFTP_BLKSIZE_DEFAULT;
  state->requested_blksize = blksize;

  ((struct sockaddr *)&state->local_addr)->sa_family =
    (unsigned short)(conn->ip_addr->ai_family);

  tftp_set_timeouts(state);

  if(!conn->bits.bound) {
    /* If not already bound, bind to any interface, random UDP port. If it is
     * reused or a custom local port was desired, this has already been done!
     *
     * We once used the size of the local_addr struct as the third argument for
     * bind() to better work with IPv6 or whatever size the struct could have,
     * but we learned that at least Tru64, AIX and IRIX *requires* the size of
     * that argument to match the exact size of a 'sockaddr_in' struct when
     * running IPv4-only.
     *
     * Therefore we use the size from the address we connected to, which we
     * assume uses the same IP version and thus hopefully this works for both
     * IPv4 and IPv6...
     */
    rc = bind(state->sockfd, (struct sockaddr *)&state->local_addr,
              conn->ip_addr->ai_addrlen);
    if(rc) {
      failf(conn->data, "bind() failed; %s",
            Curl_strerror(conn, SOCKERRNO));
      return CURLE_COULDNT_CONNECT;
    }
    conn->bits.bound = TRUE;
  }

  Curl_pgrsStartNow(conn->data);

  *done = TRUE;
  code = CURLE_OK;
  return(code);
}

/**********************************************************
 *
 * tftp_done
 *
 * The done callback
 *
 **********************************************************/
static CURLcode tftp_done(struct connectdata *conn, CURLcode status,
                               bool premature)
{
  (void)status; /* unused */
  (void)premature; /* not used */

  Curl_pgrsDone(conn);

  return CURLE_OK;
}


/**********************************************************
 *
 * tftp
 *
 * The do callback
 *
 * This callback handles the entire TFTP transfer
 *
 **********************************************************/

static CURLcode tftp_do(struct connectdata *conn, bool *done)
{
  struct SessionHandle  *data = conn->data;
  tftp_state_data_t     *state;
  tftp_event_t          event;
  CURLcode              code;
  int                   rc;
  struct Curl_sockaddr_storage fromaddr;
  curl_socklen_t        fromlen;
  int                   check_time = 0;
  struct SingleRequest *k = &data->req;

  *done = TRUE;

  /*
    Since connections can be re-used between SessionHandles, this might be a
    connection already existing but on a fresh SessionHandle struct so we must
    make sure we have a good 'struct TFTP' to play with. For new connections,
    the struct TFTP is allocated and setup in the tftp_connect() function.
  */
  Curl_reset_reqproto(conn);

  if(!conn->proto.tftpc) {
    code = tftp_connect(conn, done);
    if(code)
      return code;
  }
  state = (tftp_state_data_t *)conn->proto.tftpc;

  /* Run the TFTP State Machine */
  for(code=tftp_state_machine(state, TFTP_EVENT_INIT);
      (state->state != TFTP_STATE_FIN) && (code == CURLE_OK);
      code=tftp_state_machine(state, event) ) {

    /* Wait until ready to read or timeout occurs */
    rc=Curl_socket_ready(state->sockfd, CURL_SOCKET_BAD,
                         (int)(state->retry_time * 1000));

    if(rc == -1) {
      /* bail out */
      int error = SOCKERRNO;
      failf(data, "%s", Curl_strerror(conn, error));
      event = TFTP_EVENT_ERROR;
    }
    else if(rc==0) {
      /* A timeout occured */
      event = TFTP_EVENT_TIMEOUT;

      /* Force a look at transfer timeouts */
      check_time = 0;

    }
    else {

      /* Receive the packet */
      fromlen = sizeof(fromaddr);
      state->rbytes = (ssize_t)recvfrom(state->sockfd,
                                        (void *)state->rpacket.data,
                                        state->blksize+4,
                                        0,
                                        (struct sockaddr *)&fromaddr,
                                        &fromlen);
      if(state->remote_addrlen==0) {
        memcpy(&state->remote_addr, &fromaddr, fromlen);
        state->remote_addrlen = fromlen;
      }

      /* Sanity check packet length */
      if(state->rbytes < 4) {
        failf(data, "Received too short packet");
        /* Not a timeout, but how best to handle it? */
        event = TFTP_EVENT_TIMEOUT;
      }
      else {

        /* The event is given by the TFTP packet time */
        event = (tftp_event_t)getrpacketevent(&state->rpacket);

        switch(event) {
        case TFTP_EVENT_DATA:
          /* Don't pass to the client empty or retransmitted packets */
          if(state->rbytes > 4 &&
              ((state->block+1) == getrpacketblock(&state->rpacket))) {
            code = Curl_client_write(conn, CLIENTWRITE_BODY,
                                     (char *)state->rpacket.data+4,
                                     state->rbytes-4);
            if(code)
              return code;
            k->bytecount += state->rbytes-4;
            Curl_pgrsSetDownloadCounter(data, (curl_off_t) k->bytecount);
          }
          break;
        case TFTP_EVENT_ERROR:
          state->error = (tftp_error_t)getrpacketblock(&state->rpacket);
          infof(data, "%s\n", (const char *)state->rpacket.data+4);
          break;
        case TFTP_EVENT_ACK:
          break;
        case TFTP_EVENT_OACK:
          code = tftp_parse_option_ack(state,
                                       (const char *)state->rpacket.data+2,
                                       (int)state->rbytes-2);
          if(code)
            return code;
          break;
        case TFTP_EVENT_RRQ:
        case TFTP_EVENT_WRQ:
        default:
          failf(data, "%s", "Internal error: Unexpected packet");
          break;
        }

        /* Update the progress meter */
        if(Curl_pgrsUpdate(conn))
          return CURLE_ABORTED_BY_CALLBACK;
      }
    }

    /* Check for transfer timeout every 10 blocks, or after timeout */
    if(check_time%10==0) {
      time_t current;
      time(&current);
      if(current>state->max_time) {
        DEBUGF(infof(data, "timeout: %d > %d\n",
                     current, state->max_time));
        state->error = TFTP_ERR_TIMEOUT;
        state->state = TFTP_STATE_FIN;
      }
    }

  }
  if(code)
    return code;

  /* Tell curl we're done */
  code = Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);
  if(code)
    return code;

  /* If we have encountered an error */
  if(state->error != TFTP_ERR_NONE) {

    /* Translate internal error codes to curl error codes */
    switch(state->error) {
    case TFTP_ERR_NOTFOUND:
      code = CURLE_TFTP_NOTFOUND;
      break;
    case TFTP_ERR_PERM:
      code = CURLE_TFTP_PERM;
      break;
    case TFTP_ERR_DISKFULL:
      code = CURLE_REMOTE_DISK_FULL;
      break;
    case TFTP_ERR_UNDEF:
    case TFTP_ERR_ILLEGAL:
      code = CURLE_TFTP_ILLEGAL;
      break;
    case TFTP_ERR_UNKNOWNID:
      code = CURLE_TFTP_UNKNOWNID;
      break;
    case TFTP_ERR_EXISTS:
      code = CURLE_REMOTE_FILE_EXISTS;
      break;
    case TFTP_ERR_NOSUCHUSER:
      code = CURLE_TFTP_NOSUCHUSER;
      break;
    case TFTP_ERR_TIMEOUT:
      code = CURLE_OPERATION_TIMEDOUT;
      break;
    case TFTP_ERR_NORESPONSE:
      code = CURLE_COULDNT_CONNECT;
      break;
    default:
      code= CURLE_ABORTED_BY_CALLBACK;
      break;
    }
  }
  else
    code = CURLE_OK;
  return code;
}

static CURLcode tftp_setup_connection(struct connectdata * conn)
{
  struct SessionHandle *data = conn->data;
  char * type;
  char command;

  conn->socktype = SOCK_DGRAM;   /* UDP datagram based */

  /* TFTP URLs support an extension like ";mode=<typecode>" that
   * we'll try to get now! */
  type = strstr(data->state.path, ";mode=");

  if(!type)
    type = strstr(conn->host.rawalloc, ";mode=");

  if(type) {
    *type = 0;                   /* it was in the middle of the hostname */
    command = Curl_raw_toupper(type[6]);

    switch (command) {
    case 'A': /* ASCII mode */
    case 'N': /* NETASCII mode */
      data->set.prefer_ascii = TRUE;
      break;

    case 'O': /* octet mode */
    case 'I': /* binary mode */
    default:
      /* switch off ASCII */
      data->set.prefer_ascii = FALSE;
      break;
    }
  }

  return CURLE_OK;
}
#endif
