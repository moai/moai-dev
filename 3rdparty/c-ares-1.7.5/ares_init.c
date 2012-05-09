
/* Copyright 1998 by the Massachusetts Institute of Technology.
 * Copyright (C) 2007-2011 by Daniel Stenberg
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

#include "ares_setup.h"

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_ARPA_NAMESER_H
#  include <arpa/nameser.h>
#else
#  include "nameser.h"
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#  include <arpa/nameser_compat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef ANDROID
#include <sys/system_properties.h>
#endif

#include "ares.h"
#include "inet_net_pton.h"
#include "ares_library_init.h"
#include "ares_nowarn.h"
#include "ares_platform.h"
#include "ares_inet_ntop.h"
#include "ares_private.h"

#ifdef WATT32
#undef WIN32  /* Redefined in MingW/MSVC headers */
#endif

static int init_by_options(ares_channel channel, const struct ares_options *options,
                           int optmask);
static int init_by_environment(ares_channel channel);
static int init_by_resolv_conf(ares_channel channel);
static int init_by_defaults(ares_channel channel);

#ifndef WATT32
static int config_nameserver(struct server_state **servers, int *nservers,
                             char *str);
#endif
static int set_search(ares_channel channel, const char *str);
static int set_options(ares_channel channel, const char *str);
static const char *try_option(const char *p, const char *q, const char *opt);
static int init_id_key(rc4_key* key,int key_data_len);

#if !defined(WIN32) && !defined(WATT32)
static int sortlist_alloc(struct apattern **sortlist, int *nsort, struct apattern *pat);
static int ip_addr(const char *s, ssize_t len, struct in_addr *addr);
static void natural_mask(struct apattern *pat);
static int config_domain(ares_channel channel, char *str);
static int config_lookup(ares_channel channel, const char *str,
                         const char *bindch, const char *filech);
static int config_sortlist(struct apattern **sortlist, int *nsort,
                           const char *str);
static char *try_config(char *s, const char *opt, char scc);
#endif

#define ARES_CONFIG_CHECK(x) (x->lookups && x->nsort > -1 && \
                             x->nservers > -1 && \
                             x->ndomains > -1 && \
                             x->ndots > -1 && x->timeout > -1 && \
                             x->tries > -1)

int ares_init(ares_channel *channelptr)
{
  return ares_init_options(channelptr, NULL, 0);
}

int ares_init_options(ares_channel *channelptr, struct ares_options *options,
                      int optmask)
{
  ares_channel channel;
  int i;
  int status = ARES_SUCCESS;
  struct timeval now;

#ifdef CURLDEBUG
  const char *env = getenv("CARES_MEMDEBUG");

  if (env)
    curl_memdebug(env);
  env = getenv("CARES_MEMLIMIT");
  if (env) {
    char *endptr;
    long num = strtol(env, &endptr, 10);
    if((endptr != env) && (endptr == env + strlen(env)) && (num > 0))
      curl_memlimit(num);
  }
#endif

  if (ares_library_initialized() != ARES_SUCCESS)
    return ARES_ENOTINITIALIZED;

  channel = malloc(sizeof(struct ares_channeldata));
  if (!channel) {
    *channelptr = NULL;
    return ARES_ENOMEM;
  }

  now = ares__tvnow();

  /* Set everything to distinguished values so we know they haven't
   * been set yet.
   */
  channel->flags = -1;
  channel->timeout = -1;
  channel->tries = -1;
  channel->ndots = -1;
  channel->rotate = -1;
  channel->udp_port = -1;
  channel->tcp_port = -1;
  channel->socket_send_buffer_size = -1;
  channel->socket_receive_buffer_size = -1;
  channel->nservers = -1;
  channel->ndomains = -1;
  channel->nsort = -1;
  channel->tcp_connection_generation = 0;
  channel->lookups = NULL;
  channel->domains = NULL;
  channel->sortlist = NULL;
  channel->servers = NULL;
  channel->sock_state_cb = NULL;
  channel->sock_state_cb_data = NULL;
  channel->sock_create_cb = NULL;
  channel->sock_create_cb_data = NULL;

  channel->last_server = 0;
  channel->last_timeout_processed = (time_t)now.tv_sec;

  memset(&channel->local_dev_name, 0, sizeof(channel->local_dev_name));
  channel->local_ip4 = 0;
  memset(&channel->local_ip6, 0, sizeof(channel->local_ip6));

  /* Initialize our lists of queries */
  ares__init_list_head(&(channel->all_queries));
  for (i = 0; i < ARES_QID_TABLE_SIZE; i++)
    {
      ares__init_list_head(&(channel->queries_by_qid[i]));
    }
  for (i = 0; i < ARES_TIMEOUT_TABLE_SIZE; i++)
    {
      ares__init_list_head(&(channel->queries_by_timeout[i]));
    }

  /* Initialize configuration by each of the four sources, from highest
   * precedence to lowest.
   */

  if (status == ARES_SUCCESS) {
    status = init_by_options(channel, options, optmask);
    if (status != ARES_SUCCESS)
      DEBUGF(fprintf(stderr, "Error: init_by_options failed: %s\n",
                     ares_strerror(status)));
  }
  if (status == ARES_SUCCESS) {
    status = init_by_environment(channel);
    if (status != ARES_SUCCESS)
      DEBUGF(fprintf(stderr, "Error: init_by_environment failed: %s\n",
                     ares_strerror(status)));
  }
  if (status == ARES_SUCCESS) {
    status = init_by_resolv_conf(channel);
    if (status != ARES_SUCCESS)
      DEBUGF(fprintf(stderr, "Error: init_by_resolv_conf failed: %s\n",
                     ares_strerror(status)));
  }

  /*
   * No matter what failed or succeeded, seed defaults to provide
   * useful behavior for things that we missed.
   */
  status = init_by_defaults(channel);
  if (status != ARES_SUCCESS)
    DEBUGF(fprintf(stderr, "Error: init_by_defaults failed: %s\n",
                   ares_strerror(status)));

  /* Generate random key */

  if (status == ARES_SUCCESS) {
    status = init_id_key(&channel->id_key, ARES_ID_KEY_LEN);
    if (status == ARES_SUCCESS)
      channel->next_id = ares__generate_new_id(&channel->id_key);
    else
      DEBUGF(fprintf(stderr, "Error: init_id_key failed: %s\n",
                     ares_strerror(status)));
  }

  if (status != ARES_SUCCESS)
    {
      /* Something failed; clean up memory we may have allocated. */
      if (channel->servers)
        free(channel->servers);
      if (channel->domains)
        {
          for (i = 0; i < channel->ndomains; i++)
            free(channel->domains[i]);
          free(channel->domains);
        }
      if (channel->sortlist)
        free(channel->sortlist);
      if(channel->lookups)
        free(channel->lookups);
      free(channel);
      return status;
    }

  /* Trim to one server if ARES_FLAG_PRIMARY is set. */
  if ((channel->flags & ARES_FLAG_PRIMARY) && channel->nservers > 1)
    channel->nservers = 1;

  ares__init_servers_state(channel);

  *channelptr = channel;
  return ARES_SUCCESS;
}

/* ares_dup() duplicates a channel handle with all its options and returns a
   new channel handle */
int ares_dup(ares_channel *dest, ares_channel src)
{
  struct ares_options opts;
  struct ares_addr_node *servers;
  int ipv6_nservers = 0;
  int i, rc;
  int optmask;

  *dest = NULL; /* in case of failure return NULL explicitly */

  /* First get the options supported by the old ares_save_options() function,
     which is most of them */
  rc = ares_save_options(src, &opts, &optmask);
  if(rc)
    return rc;

  /* Then create the new channel with those options */
  rc = ares_init_options(dest, &opts, optmask);

  /* destroy the options copy to not leak any memory */
  ares_destroy_options(&opts);

  if(rc)
    return rc;

  /* Now clone the options that ares_save_options() doesn't support. */
  (*dest)->sock_create_cb      = src->sock_create_cb;
  (*dest)->sock_create_cb_data = src->sock_create_cb_data;

  strncpy((*dest)->local_dev_name, src->local_dev_name, sizeof(src->local_dev_name));
  (*dest)->local_ip4 = src->local_ip4;
  memcpy((*dest)->local_ip6, src->local_ip6, sizeof(src->local_ip6));

  /* Full name server cloning required when not all are IPv4 */
  for (i = 0; i < src->nservers; i++)
    {
      if (src->servers[i].addr.family != AF_INET) {
        ipv6_nservers++;
        break;
      }
    }
  if (ipv6_nservers) {
    rc = ares_get_servers(src, &servers);
    if (rc != ARES_SUCCESS)
      return rc;
    rc = ares_set_servers(*dest, servers);
    ares_free_data(servers);
    if (rc != ARES_SUCCESS)
      return rc;
  }

  return ARES_SUCCESS; /* everything went fine */
}

/* Save options from initialized channel */
int ares_save_options(ares_channel channel, struct ares_options *options,
                      int *optmask)
{
  int i, j;
  int ipv4_nservers = 0;

  /* Zero everything out */
  memset(options, 0, sizeof(struct ares_options));

  if (!ARES_CONFIG_CHECK(channel))
    return ARES_ENODATA;

  /* Traditionally the optmask wasn't saved in the channel struct so it was
     recreated here. ROTATE is the first option that has no struct field of
     its own in the public config struct */
  (*optmask) = (ARES_OPT_FLAGS|ARES_OPT_TRIES|ARES_OPT_NDOTS|
                ARES_OPT_UDP_PORT|ARES_OPT_TCP_PORT|ARES_OPT_SOCK_STATE_CB|
                ARES_OPT_SERVERS|ARES_OPT_DOMAINS|ARES_OPT_LOOKUPS|
                ARES_OPT_SORTLIST|ARES_OPT_TIMEOUTMS) |
    (channel->optmask & ARES_OPT_ROTATE);

  /* Copy easy stuff */
  options->flags   = channel->flags;

  /* We return full millisecond resolution but that's only because we don't
     set the ARES_OPT_TIMEOUT anymore, only the new ARES_OPT_TIMEOUTMS */
  options->timeout = channel->timeout;
  options->tries   = channel->tries;
  options->ndots   = channel->ndots;
  options->udp_port = (unsigned short)channel->udp_port;
  options->tcp_port = (unsigned short)channel->tcp_port;
  options->sock_state_cb     = channel->sock_state_cb;
  options->sock_state_cb_data = channel->sock_state_cb_data;

  /* Copy IPv4 servers */
  if (channel->nservers) {
    for (i = 0; i < channel->nservers; i++)
    {
      if (channel->servers[i].addr.family == AF_INET)
        ipv4_nservers++;
    }
    if (ipv4_nservers) {
      options->servers = malloc(ipv4_nservers * sizeof(struct in_addr));
      if (!options->servers)
        return ARES_ENOMEM;
      for (i = j = 0; i < channel->nservers; i++)
      {
        if (channel->servers[i].addr.family == AF_INET)
          memcpy(&options->servers[j++],
                 &channel->servers[i].addr.addrV4,
                 sizeof(channel->servers[i].addr.addrV4));
      }
    }
  }
  options->nservers = ipv4_nservers;

  /* copy domains */
  if (channel->ndomains) {
    options->domains = malloc(channel->ndomains * sizeof(char *));
    if (!options->domains)
      return ARES_ENOMEM;

    for (i = 0; i < channel->ndomains; i++)
    {
      options->ndomains = i;
      options->domains[i] = strdup(channel->domains[i]);
      if (!options->domains[i])
        return ARES_ENOMEM;
    }
  }
  options->ndomains = channel->ndomains;

  /* copy lookups */
  if (channel->lookups) {
    options->lookups = strdup(channel->lookups);
    if (!options->lookups && channel->lookups)
      return ARES_ENOMEM;
  }

  /* copy sortlist */
  if (channel->nsort) {
    options->sortlist = malloc(channel->nsort * sizeof(struct apattern));
    if (!options->sortlist)
      return ARES_ENOMEM;
    for (i = 0; i < channel->nsort; i++)
      options->sortlist[i] = channel->sortlist[i];
  }
  options->nsort = channel->nsort;

  return ARES_SUCCESS;
}

static int init_by_options(ares_channel channel,
                           const struct ares_options *options,
                           int optmask)
{
  int i;

  /* Easy stuff. */
  if ((optmask & ARES_OPT_FLAGS) && channel->flags == -1)
    channel->flags = options->flags;
  if ((optmask & ARES_OPT_TIMEOUTMS) && channel->timeout == -1)
    channel->timeout = options->timeout;
  else if ((optmask & ARES_OPT_TIMEOUT) && channel->timeout == -1)
    channel->timeout = options->timeout * 1000;
  if ((optmask & ARES_OPT_TRIES) && channel->tries == -1)
    channel->tries = options->tries;
  if ((optmask & ARES_OPT_NDOTS) && channel->ndots == -1)
    channel->ndots = options->ndots;
  if ((optmask & ARES_OPT_ROTATE) && channel->rotate == -1)
    channel->rotate = 1;
  if ((optmask & ARES_OPT_UDP_PORT) && channel->udp_port == -1)
    channel->udp_port = options->udp_port;
  if ((optmask & ARES_OPT_TCP_PORT) && channel->tcp_port == -1)
    channel->tcp_port = options->tcp_port;
  if ((optmask & ARES_OPT_SOCK_STATE_CB) && channel->sock_state_cb == NULL)
    {
      channel->sock_state_cb = options->sock_state_cb;
      channel->sock_state_cb_data = options->sock_state_cb_data;
    }
  if ((optmask & ARES_OPT_SOCK_SNDBUF)
      && channel->socket_send_buffer_size == -1)
    channel->socket_send_buffer_size = options->socket_send_buffer_size;
  if ((optmask & ARES_OPT_SOCK_RCVBUF)
      && channel->socket_receive_buffer_size == -1)
    channel->socket_receive_buffer_size = options->socket_receive_buffer_size;

  /* Copy the IPv4 servers, if given. */
  if ((optmask & ARES_OPT_SERVERS) && channel->nservers == -1)
    {
      /* Avoid zero size allocations at any cost */
      if (options->nservers > 0)
        {
          channel->servers =
            malloc(options->nservers * sizeof(struct server_state));
          if (!channel->servers)
            return ARES_ENOMEM;
          for (i = 0; i < options->nservers; i++)
            {
              channel->servers[i].addr.family = AF_INET;
              memcpy(&channel->servers[i].addr.addrV4,
                     &options->servers[i],
                     sizeof(channel->servers[i].addr.addrV4));
            }
        }
      channel->nservers = options->nservers;
    }

  /* Copy the domains, if given.  Keep channel->ndomains consistent so
   * we can clean up in case of error.
   */
  if ((optmask & ARES_OPT_DOMAINS) && channel->ndomains == -1)
    {
      /* Avoid zero size allocations at any cost */
      if (options->ndomains > 0)
      {
        channel->domains = malloc(options->ndomains * sizeof(char *));
        if (!channel->domains)
          return ARES_ENOMEM;
        for (i = 0; i < options->ndomains; i++)
          {
            channel->ndomains = i;
            channel->domains[i] = strdup(options->domains[i]);
            if (!channel->domains[i])
              return ARES_ENOMEM;
          }
      }
      channel->ndomains = options->ndomains;
    }

  /* Set lookups, if given. */
  if ((optmask & ARES_OPT_LOOKUPS) && !channel->lookups)
    {
      channel->lookups = strdup(options->lookups);
      if (!channel->lookups)
        return ARES_ENOMEM;
    }

  /* copy sortlist */
  if ((optmask & ARES_OPT_SORTLIST) && (channel->nsort == -1) &&
      (options->nsort>0)) {
    channel->sortlist = malloc(options->nsort * sizeof(struct apattern));
    if (!channel->sortlist)
      return ARES_ENOMEM;
    for (i = 0; i < options->nsort; i++)
      channel->sortlist[i] = options->sortlist[i];
    channel->nsort = options->nsort;
  }

  channel->optmask = optmask;

  return ARES_SUCCESS;
}

static int init_by_environment(ares_channel channel)
{
  const char *localdomain, *res_options;
  int status;

  localdomain = getenv("LOCALDOMAIN");
  if (localdomain && channel->ndomains == -1)
    {
      status = set_search(channel, localdomain);
      if (status != ARES_SUCCESS)
        return status;
    }

  res_options = getenv("RES_OPTIONS");
  if (res_options)
    {
      status = set_options(channel, res_options);
      if (status != ARES_SUCCESS)
        return status;
    }

  return ARES_SUCCESS;
}

#ifdef WIN32
/*
 * Warning: returns a dynamically allocated buffer, the user MUST
 * use free() if the function returns 1
 */
static int get_res_nt(HKEY hKey, const char *subkey, char **obuf)
{
  /* Test for the size we need */
  DWORD size = 0;
  int result;

  result = RegQueryValueEx(hKey, subkey, 0, NULL, NULL, &size);
  if ((result != ERROR_SUCCESS && result != ERROR_MORE_DATA) || !size)
    return 0;
  *obuf = malloc(size+1);
  if (!*obuf)
    return 0;

  if (RegQueryValueEx(hKey, subkey, 0, NULL,
                      (LPBYTE)*obuf, &size) != ERROR_SUCCESS)
  {
    free(*obuf);
    return 0;
  }
  if (size == 1)
  {
    free(*obuf);
    return 0;
  }
  return 1;
}

static int get_res_interfaces_nt(HKEY hKey, const char *subkey, char **obuf)
{
  char enumbuf[39]; /* GUIDs are 38 chars + 1 for NULL */
  DWORD enum_size = 39;
  int idx = 0;
  HKEY hVal;

  while (RegEnumKeyEx(hKey, idx++, enumbuf, &enum_size, 0,
                      NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS)
  {
    int rc;

    enum_size = 39;
    if (RegOpenKeyEx(hKey, enumbuf, 0, KEY_QUERY_VALUE, &hVal) !=
        ERROR_SUCCESS)
      continue;
    rc = get_res_nt(hVal, subkey, obuf);
      RegCloseKey(hVal);
    if (rc)
      return 1;
    }
  return 0;
}

/**
 * The desired output for this method is that we set "ret_buf" to
 * something like:
 *
 * 192.168.0.1,dns01.my.domain,fe80::200:f8ff:fe21:67cf
 *
 * The only ordering requirement is that primary servers are listed
 * before secondary. There is no requirement that IPv4 addresses should
 * necessarily be before IPv6.
 *
 * Note that ret_size should ideally be big enough to hold around
 * 2-3 IPv4 and 2-3 IPv6 addresses.
 *
 * Finally, we need to return the total number of DNS servers located.
 */
static int get_iphlpapi_dns_info (char *ret_buf, size_t ret_size)
{
  const size_t  ipv4_size = INET_ADDRSTRLEN  + 1;  /* +1 for ',' at end */
  const size_t  ipv6_size = INET6_ADDRSTRLEN + 12; /* +12 for "%0123456789," at end */
  size_t        left = ret_size;
  char         *ret  = ret_buf;
  int           count = 0;

  /* Use the GetAdaptersAddresses method if it's available, otherwise
     fall back to GetNetworkParams. */
  if (ares_fpGetAdaptersAddresses != ZERO_NULL)
  {
    const ULONG            working_buf_size = 15000;
    IP_ADAPTER_ADDRESSES   *pFirstEntry = NULL;
    IP_ADAPTER_ADDRESSES   *pEntry = NULL;
    ULONG                  bufSize = 0;
    ULONG                  result = 0;

    /* According to MSDN, the recommended way to do this is to use a temporary
       buffer of 15K, to "dramatically reduce the chance that the GetAdaptersAddresses
       method returns ERROR_BUFFER_OVERFLOW" */
    pFirstEntry  = ( IP_ADAPTER_ADDRESSES * ) malloc( working_buf_size );
    bufSize = working_buf_size;
    if( !pFirstEntry )
      return 0;

    /* Call the method one time */
    result = ( *ares_fpGetAdaptersAddresses )( AF_UNSPEC, 0, 0, pFirstEntry, &bufSize );
    if( result == ERROR_BUFFER_OVERFLOW )
    {
      /* Reallocate, bufSize should now be set to the required size */
      pFirstEntry = ( IP_ADAPTER_ADDRESSES * ) realloc( pFirstEntry, bufSize );
      if( !pFirstEntry )
        return 0;

      /* Call the method a second time */
      result = ( *ares_fpGetAdaptersAddresses )( AF_UNSPEC, 0, 0, pFirstEntry, &bufSize );
      if( result == ERROR_BUFFER_OVERFLOW )
      {
        /* Reallocate, bufSize should now be set to the required size */
        pFirstEntry = ( IP_ADAPTER_ADDRESSES * ) realloc( pFirstEntry, bufSize );
        if( !pFirstEntry )
          return 0;

        /* Call the method a third time. The maximum number of times we're going to do
           this is 3. Three shall be the number thou shalt count, and the number of the
           counting shall be three.  Five is right out. */
        result = ( *ares_fpGetAdaptersAddresses )( AF_UNSPEC, 0, 0, pFirstEntry, &bufSize );
      }
    }

    /* Check the current result for failure */
    if( result != ERROR_SUCCESS )
    {
      free( pFirstEntry );
      return 0;
    }

    /* process the results */
    for( pEntry = pFirstEntry ; pEntry != NULL ; pEntry = pEntry->Next )
    {
      IP_ADAPTER_DNS_SERVER_ADDRESS* pDNSAddr = pEntry->FirstDnsServerAddress;
      for( ; pDNSAddr != NULL ; pDNSAddr = pDNSAddr->Next )
      {
        struct sockaddr *pGenericAddr = pDNSAddr->Address.lpSockaddr;
        size_t stringlen = 0;

        if( pGenericAddr->sa_family == AF_INET && left > ipv4_size )
        {
          /* Handle the v4 case */
          struct sockaddr_in *pIPv4Addr = ( struct sockaddr_in * ) pGenericAddr;
          ares_inet_ntop( AF_INET, &pIPv4Addr->sin_addr, ret, ipv4_size - 1 ); /* -1 for comma */

          /* Append a comma to the end, THEN NULL. Should be OK because we
             already tested the size at the top of the if statement. */
          stringlen = strlen( ret );
          ret[ stringlen ] = ',';
          ret[ stringlen + 1 ] = '\0';
          ret += stringlen + 1;
          left -= ret - ret_buf;
          ++count;
        }
        else if( pGenericAddr->sa_family == AF_INET6 && left > ipv6_size )
        {
          /* Handle the v6 case */
          struct sockaddr_in6 *pIPv6Addr = ( struct sockaddr_in6 * ) pGenericAddr;
          ares_inet_ntop( AF_INET6, &pIPv6Addr->sin6_addr, ret, ipv6_size - 1 ); /* -1 for comma */

          /* Append a comma to the end, THEN NULL. Should be OK because we
             already tested the size at the top of the if statement. */
          stringlen = strlen( ret );
          ret[ stringlen ] = ',';
          ret[ stringlen + 1 ] = '\0';
          ret += stringlen + 1;
          left -= ret - ret_buf;
          ++count;

          /* NB on Windows this also returns stuff in the fec0::/10 range,
             seems to be hard-coded somehow. Do we need to ignore them? */
        }
      }
    }

    if( pFirstEntry )
      free( pFirstEntry );
    if (ret > ret_buf)
      ret[-1] = '\0';
    return count;
  }
  else
  {
    FIXED_INFO    *fi, *newfi;
    DWORD          size = sizeof (*fi);
    IP_ADDR_STRING *ipAddr;
    int            i;
    int            debug  = 0;
    HRESULT        res;

    fi = malloc(size);
    if (!fi)
      return 0;

    res = (*ares_fpGetNetworkParams) (fi, &size);
    if ((res != ERROR_BUFFER_OVERFLOW) && (res != ERROR_SUCCESS))
      goto quit;

    newfi = realloc(fi, size);
    if (!newfi)
      goto quit;

    fi = newfi;
    res = (*ares_fpGetNetworkParams) (fi, &size);
    if (res != ERROR_SUCCESS)
      goto quit;

    if (debug)
    {
      printf ("Host Name: %s\n", fi->HostName);
      printf ("Domain Name: %s\n", fi->DomainName);
      printf ("DNS Servers:\n"
              "    %s (primary)\n", fi->DnsServerList.IpAddress.String);
    }
    if (strlen(fi->DnsServerList.IpAddress.String) > 0 &&
        inet_addr(fi->DnsServerList.IpAddress.String) != INADDR_NONE &&
        left > ipv4_size)
    {
      ret += sprintf (ret, "%s,", fi->DnsServerList.IpAddress.String);
      left -= ret - ret_buf;
      ++count;
    }

    for (i = 0, ipAddr = fi->DnsServerList.Next; ipAddr && left > ipv4_size;
         ipAddr = ipAddr->Next, i++)
    {
      if (inet_addr(ipAddr->IpAddress.String) != INADDR_NONE)
      {
         ret += sprintf (ret, "%s,", ipAddr->IpAddress.String);
         left -= ret - ret_buf;
         ++count;
      }
      if (debug)
         printf ("    %s (secondary %d)\n", ipAddr->IpAddress.String, i+1);
    }

quit:
    if (fi)
      free(fi);

    if (debug && left <= ipv4_size)
      printf ("Too many nameservers. Truncating to %d addressess", count);
    if (ret > ret_buf)
      ret[-1] = '\0';
    return count;
  }
}
#endif

static int init_by_resolv_conf(ares_channel channel)
{
#ifndef WATT32
  char *line = NULL;
#endif
  int status = -1, nservers = 0, nsort = 0;
  struct server_state *servers = NULL;
  struct apattern *sortlist = NULL;

#ifdef WIN32

    /*
  NameServer info via IPHLPAPI (IP helper API):
    GetNetworkParams() should be the trusted source for this.
    Available in Win-98/2000 and later. If that fail, fall-back to
    registry information.

  NameServer Registry:

   On Windows 9X, the DNS server can be found in:
HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\VxD\MSTCP\NameServer

        On Windows NT/2000/XP/2003:
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\NameServer
        or
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\DhcpNameServer
        or
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\{AdapterID}\
NameServer
        or
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\{AdapterID}\
DhcpNameServer
   */

  HKEY mykey;
  HKEY subkey;
  DWORD data_type;
  DWORD bytes;
  DWORD result;
  char  buf[512];
  win_platform platform;

  if (channel->nservers > -1)  /* don't override ARES_OPT_SERVER */
     return ARES_SUCCESS;

  if (get_iphlpapi_dns_info(buf,sizeof(buf)) > 0)
  {
    status = config_nameserver(&servers, &nservers, buf);
    if (status == ARES_SUCCESS)
      goto okay;
  }

  platform = ares__getplatform();

  if (platform == WIN_NT)
  {
    if (RegOpenKeyEx(
          HKEY_LOCAL_MACHINE, WIN_NS_NT_KEY, 0,
          KEY_READ, &mykey
          ) == ERROR_SUCCESS)
    {
      RegOpenKeyEx(mykey, "Interfaces", 0,
                   KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS, &subkey);
      if (get_res_nt(mykey, NAMESERVER, &line))
      {
        status = config_nameserver(&servers, &nservers, line);
        free(line);
      }
      else if (get_res_nt(mykey, DHCPNAMESERVER, &line))
      {
        status = config_nameserver(&servers, &nservers, line);
        free(line);
      }
      /* Try the interfaces */
      else if (get_res_interfaces_nt(subkey, NAMESERVER, &line))
      {
        status = config_nameserver(&servers, &nservers, line);
        free(line);
      }
      else if (get_res_interfaces_nt(subkey, DHCPNAMESERVER, &line))
      {
        status = config_nameserver(&servers, &nservers, line);
        free(line);
      }
      RegCloseKey(subkey);
      RegCloseKey(mykey);
    }
  }
  else if (platform == WIN_9X)
  {
    if (RegOpenKeyEx(
          HKEY_LOCAL_MACHINE, WIN_NS_9X, 0,
          KEY_READ, &mykey
          ) == ERROR_SUCCESS)
    {
      if ((result = RegQueryValueEx(
             mykey, NAMESERVER, NULL, &data_type,
             NULL, &bytes
             )
            ) == ERROR_SUCCESS ||
          result == ERROR_MORE_DATA)
      {
        if (bytes)
        {
          line = malloc(bytes+1);
          if (RegQueryValueEx(mykey, NAMESERVER, NULL, &data_type,
                              (unsigned char *)line, &bytes) ==
              ERROR_SUCCESS)
          {
            status = config_nameserver(&servers, &nservers, line);
          }
          free(line);
        }
      }
    }
    RegCloseKey(mykey);
  }

  if (status == ARES_SUCCESS)
    status = ARES_EOF;
  else
    /* Catch the case when all the above checks fail (which happens when there
       is no network card or the cable is unplugged) */
    status = ARES_EFILE;

#elif defined(__riscos__)

  /* Under RISC OS, name servers are listed in the
     system variable Inet$Resolvers, space separated. */

  line = getenv("Inet$Resolvers");
  status = ARES_EOF;
  if (line) {
    char *resolvers = strdup(line), *pos, *space;

    if (!resolvers)
      return ARES_ENOMEM;

    pos = resolvers;
    do {
      space = strchr(pos, ' ');
      if (space)
        *space = '\0';
      status = config_nameserver(&servers, &nservers, pos);
      if (status != ARES_SUCCESS)
        break;
      pos = space + 1;
    } while (space);

    if (status == ARES_SUCCESS)
      status = ARES_EOF;

    free(resolvers);
  }

#elif defined(WATT32)
  int i;

  sock_init();
  for (i = 0; def_nameservers[i]; i++)
      ;
  if (i == 0)
    return ARES_SUCCESS; /* use localhost DNS server */

  nservers = i;
  servers = calloc(i, sizeof(struct server_state));
  if (!servers)
     return ARES_ENOMEM;

  for (i = 0; def_nameservers[i]; i++)
  {
    servers[i].addr.addrV4.s_addr = htonl(def_nameservers[i]);
    servers[i].addr.family = AF_INET;
  }
  status = ARES_EOF;

#elif defined(ANDROID)
  char value[PROP_VALUE_MAX]="";
  __system_property_get("net.dns1", value);
  status = config_nameserver(&servers, &nservers, value);
  if (status == ARES_SUCCESS)
    status = ARES_EOF;
#else
  {
    char *p;
    FILE *fp;
    size_t linesize;
    int error;

    /* Don't read resolv.conf and friends if we don't have to */
    if (ARES_CONFIG_CHECK(channel))
        return ARES_SUCCESS;

    fp = fopen(PATH_RESOLV_CONF, "r");
    if (fp) {
      while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
      {
        if ((p = try_config(line, "domain", ';')))
          status = config_domain(channel, p);
        else if ((p = try_config(line, "lookup", ';')) && !channel->lookups)
          status = config_lookup(channel, p, "bind", "file");
        else if ((p = try_config(line, "search", ';')))
          status = set_search(channel, p);
        else if ((p = try_config(line, "nameserver", ';')) &&
                 channel->nservers == -1)
          status = config_nameserver(&servers, &nservers, p);
        else if ((p = try_config(line, "sortlist", ';')) &&
                 channel->nsort == -1)
          status = config_sortlist(&sortlist, &nsort, p);
        else if ((p = try_config(line, "options", ';')))
          status = set_options(channel, p);
        else
          status = ARES_SUCCESS;
        if (status != ARES_SUCCESS)
          break;
      }
      fclose(fp);
    }
    else {
      error = ERRNO;
      switch(error) {
      case ENOENT:
      case ESRCH:
        status = ARES_EOF;
        break;
      default:
        DEBUGF(fprintf(stderr, "fopen() failed with error: %d %s\n",
                       error, strerror(error)));
        DEBUGF(fprintf(stderr, "Error opening file: %s\n", PATH_RESOLV_CONF));
        status = ARES_EFILE;
      }
    }

    if ((status == ARES_EOF) && (!channel->lookups)) {
      /* Many systems (Solaris, Linux, BSD's) use nsswitch.conf */
      fp = fopen("/etc/nsswitch.conf", "r");
      if (fp) {
        while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
        {
          if ((p = try_config(line, "hosts:", '\0')) && !channel->lookups)
            /* ignore errors */
            (void)config_lookup(channel, p, "dns", "files");
        }
        fclose(fp);
      }
      else {
        error = ERRNO;
        switch(error) {
        case ENOENT:
        case ESRCH:
          status = ARES_EOF;
          break;
        default:
          DEBUGF(fprintf(stderr, "fopen() failed with error: %d %s\n",
                         error, strerror(error)));
          DEBUGF(fprintf(stderr, "Error opening file: %s\n", "/etc/nsswitch.conf"));
          status = ARES_EFILE;
        }
      }
    }

    if ((status == ARES_EOF) && (!channel->lookups)) {
      /* Linux / GNU libc 2.x and possibly others have host.conf */
      fp = fopen("/etc/host.conf", "r");
      if (fp) {
        while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
        {
          if ((p = try_config(line, "order", '\0')) && !channel->lookups)
            /* ignore errors */
            (void)config_lookup(channel, p, "bind", "hosts");
        }
        fclose(fp);
      }
      else {
        error = ERRNO;
        switch(error) {
        case ENOENT:
        case ESRCH:
          status = ARES_EOF;
          break;
        default:
          DEBUGF(fprintf(stderr, "fopen() failed with error: %d %s\n",
                         error, strerror(error)));
          DEBUGF(fprintf(stderr, "Error opening file: %s\n", "/etc/host.conf"));
          status = ARES_EFILE;
        }
      }
    }

    if ((status == ARES_EOF) && (!channel->lookups)) {
      /* Tru64 uses /etc/svc.conf */
      fp = fopen("/etc/svc.conf", "r");
      if (fp) {
        while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
        {
          if ((p = try_config(line, "hosts=", '\0')) && !channel->lookups)
            /* ignore errors */
            (void)config_lookup(channel, p, "bind", "local");
        }
        fclose(fp);
      }
      else {
        error = ERRNO;
        switch(error) {
        case ENOENT:
        case ESRCH:
          status = ARES_EOF;
          break;
        default:
          DEBUGF(fprintf(stderr, "fopen() failed with error: %d %s\n",
                         error, strerror(error)));
          DEBUGF(fprintf(stderr, "Error opening file: %s\n", "/etc/svc.conf"));
          status = ARES_EFILE;
        }
      }
    }

    if(line)
      free(line);
  }

#endif

  /* Handle errors. */
  if (status != ARES_EOF)
    {
      if (servers != NULL)
        free(servers);
      if (sortlist != NULL)
        free(sortlist);
      return status;
    }

  /* If we got any name server entries, fill them in. */
#ifdef WIN32
okay:
#endif
  if (servers)
    {
      channel->servers = servers;
      channel->nservers = nservers;
    }
  else if ( ares_get_default_dns_addr())
    {
      servers = malloc ( sizeof(struct server_state));
      channel->servers = servers;

      channel->servers[0].addr.family = AF_INET;
      channel->servers[0].addr.addrV4.s_addr = htonl( ares_get_default_dns_addr());
      channel->nservers = 1;
    }

  /* If we got any sortlist entries, fill them in. */
  if (sortlist)
    {
      channel->sortlist = sortlist;
      channel->nsort = nsort;
    }

  return ARES_SUCCESS;
}

static int init_by_defaults(ares_channel channel)
{
  char *hostname = NULL;
  int rc = ARES_SUCCESS;
#ifdef HAVE_GETHOSTNAME
  char *dot;
#endif

  if (channel->flags == -1)
    channel->flags = 0;
  if (channel->timeout == -1)
    channel->timeout = DEFAULT_TIMEOUT;
  if (channel->tries == -1)
    channel->tries = DEFAULT_TRIES;
  if (channel->ndots == -1)
    channel->ndots = 1;
  if (channel->rotate == -1)
    channel->rotate = 0;
  if (channel->udp_port == -1)
    channel->udp_port = htons(NAMESERVER_PORT);
  if (channel->tcp_port == -1)
    channel->tcp_port = htons(NAMESERVER_PORT);

  if (channel->nservers == -1) {
    /* If nobody specified servers, try a local named. */
    channel->servers = malloc(sizeof(struct server_state));
    if (!channel->servers) {
      rc = ARES_ENOMEM;
      goto error;
    }
    channel->servers[0].addr.family = AF_INET;
    channel->servers[0].addr.addrV4.s_addr = htonl(INADDR_LOOPBACK);
    channel->nservers = 1;
  }

#if defined(USE_WINSOCK)
#define toolong(x) (x == -1) &&  (SOCKERRNO == WSAEFAULT)
#elif defined(ENAMETOOLONG)
#define toolong(x) (x == -1) && ((SOCKERRNO == ENAMETOOLONG) || \
                                 (SOCKERRNO == EINVAL))
#else
#define toolong(x) (x == -1) &&  (SOCKERRNO == EINVAL)
#endif

  if (channel->ndomains == -1) {
    /* Derive a default domain search list from the kernel hostname,
     * or set it to empty if the hostname isn't helpful.
     */
    size_t len = 64;
    int res;
    channel->ndomains = 0; /* default to none */

#ifdef HAVE_GETHOSTNAME
    hostname = malloc(len);
    if(!hostname) {
      rc = ARES_ENOMEM;
      goto error;
    }

    do {
      res = gethostname(hostname, len);

      if(toolong(res)) {
        char *p;
        len *= 2;
        p = realloc(hostname, len);
        if(!p) {
          rc = ARES_ENOMEM;
          goto error;
        }
        hostname = p;
        continue;
      }
      else if(res) {
        rc = ARES_EBADNAME;
        goto error;
      }

    } while(0);

    dot = strchr(hostname, '.');
    if (dot) {
      /* a dot was found */
      channel->domains = malloc(sizeof(char *));
      if (!channel->domains) {
        rc = ARES_ENOMEM;
        goto error;
      }
      channel->domains[0] = strdup(dot + 1);
      if (!channel->domains[0]) {
        rc = ARES_ENOMEM;
        goto error;
      }
      channel->ndomains = 1;
    }
#endif
  }

  if (channel->nsort == -1) {
    channel->sortlist = NULL;
    channel->nsort = 0;
  }

  if (!channel->lookups) {
    channel->lookups = strdup("fb");
    if (!channel->lookups)
      rc = ARES_ENOMEM;
  }

  error:
  if(rc) {
    if(channel->servers)
      free(channel->servers);

    if(channel->domains && channel->domains[0])
      free(channel->domains[0]);
    if(channel->domains)
      free(channel->domains);
    if(channel->lookups)
      free(channel->lookups);
  }

  if(hostname)
    free(hostname);

  return rc;
}

#if !defined(WIN32) && !defined(WATT32)
static int config_domain(ares_channel channel, char *str)
{
  char *q;

  /* Set a single search domain. */
  q = str;
  while (*q && !ISSPACE(*q))
    q++;
  *q = '\0';
  return set_search(channel, str);
}

#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER == 910) && \
    defined(__OPTIMIZE__) && defined(__unix__) &&  defined(__i386__)
  /* workaround icc 9.1 optimizer issue */
# define vqualifier volatile
#else
# define vqualifier
#endif

static int config_lookup(ares_channel channel, const char *str,
                         const char *bindch, const char *filech)
{
  char lookups[3], *l;
  const char *vqualifier p;

  /* Set the lookup order.  Only the first letter of each work
   * is relevant, and it has to be "b" for DNS or "f" for the
   * host file.  Ignore everything else.
   */
  l = lookups;
  p = str;
  while (*p)
    {
      if ((*p == *bindch || *p == *filech) && l < lookups + 2) {
        if (*p == *bindch) *l++ = 'b';
        else *l++ = 'f';
      }
      while (*p && !ISSPACE(*p) && (*p != ','))
        p++;
      while (*p && (ISSPACE(*p) || (*p == ',')))
        p++;
    }
  *l = '\0';
  channel->lookups = strdup(lookups);
  return (channel->lookups) ? ARES_SUCCESS : ARES_ENOMEM;
}
#endif  /* !WIN32 & !WATT32 */

#ifndef WATT32
static int config_nameserver(struct server_state **servers, int *nservers,
                             char *str)
{
  struct ares_addr host;
  struct server_state *newserv;
  char *p, *txtaddr;
  /* On Windows, there may be more than one nameserver specified in the same
   * registry key, so we parse input as a space or comma seperated list.
   */
  for (p = str; p;)
    {
      /* Skip whitespace and commas. */
      while (*p && (ISSPACE(*p) || (*p == ',')))
        p++;
      if (!*p)
        /* No more input, done. */
        break;

      /* Pointer to start of IPv4 or IPv6 address part. */
      txtaddr = p;

      /* Advance past this address. */
      while (*p && !ISSPACE(*p) && (*p != ','))
        p++;
      if (*p)
        /* Null terminate this address. */
        *p++ = '\0';
      else
        /* Reached end of input, done when this address is processed. */
        p = NULL;

      /* Convert textual address to binary format. */
      if (ares_inet_pton(AF_INET, txtaddr, &host.addrV4) == 1)
        host.family = AF_INET;
      else if (ares_inet_pton(AF_INET6, txtaddr, &host.addrV6) == 1)
        host.family = AF_INET6;
      else
        continue;

      /* Resize servers state array. */
      newserv = realloc(*servers, (*nservers + 1) *
                        sizeof(struct server_state));
      if (!newserv)
        return ARES_ENOMEM;

      /* Store address data. */
      newserv[*nservers].addr.family = host.family;
      if (host.family == AF_INET)
        memcpy(&newserv[*nservers].addr.addrV4, &host.addrV4,
               sizeof(host.addrV4));
      else
        memcpy(&newserv[*nservers].addr.addrV6, &host.addrV6,
               sizeof(host.addrV6));

      /* Update arguments. */
      *servers = newserv;
      *nservers += 1;
    }

  return ARES_SUCCESS;
}

#ifndef WIN32
static int config_sortlist(struct apattern **sortlist, int *nsort,
                           const char *str)
{
  struct apattern pat;
  const char *q;

  /* Add sortlist entries. */
  while (*str && *str != ';')
    {
      int bits;
      char ipbuf[16], ipbufpfx[32];
      /* Find just the IP */
      q = str;
      while (*q && *q != '/' && *q != ';' && !ISSPACE(*q))
        q++;
      memcpy(ipbuf, str, q-str);
      ipbuf[q-str] = '\0';
      /* Find the prefix */
      if (*q == '/')
        {
          const char *str2 = q+1;
          while (*q && *q != ';' && !ISSPACE(*q))
            q++;
          memcpy(ipbufpfx, str, q-str);
          ipbufpfx[q-str] = '\0';
          str = str2;
        }
      else
        ipbufpfx[0] = '\0';
      /* Lets see if it is CIDR */
      /* First we'll try IPv6 */
      if ((bits = ares_inet_net_pton(AF_INET6, ipbufpfx[0] ? ipbufpfx : ipbuf,
                                     &pat.addrV6,
                                     sizeof(pat.addrV6))) > 0)
        {
          pat.type = PATTERN_CIDR;
          pat.mask.bits = (unsigned short)bits;
          pat.family = AF_INET6;
          if (!sortlist_alloc(sortlist, nsort, &pat))
            return ARES_ENOMEM;
        }
      else if (ipbufpfx[0] &&
               (bits = ares_inet_net_pton(AF_INET, ipbufpfx, &pat.addrV4,
                                          sizeof(pat.addrV4))) > 0)
        {
          pat.type = PATTERN_CIDR;
          pat.mask.bits = (unsigned short)bits;
          pat.family = AF_INET;
          if (!sortlist_alloc(sortlist, nsort, &pat))
            return ARES_ENOMEM;
        }
      /* See if it is just a regular IP */
      else if (ip_addr(ipbuf, q-str, &pat.addrV4) == 0)
        {
          if (ipbufpfx[0])
            {
              memcpy(ipbuf, str, q-str);
              ipbuf[q-str] = '\0';
              if (ip_addr(ipbuf, q-str, &pat.mask.addr4) != 0)
                natural_mask(&pat);
            }
          else
            natural_mask(&pat);
          pat.family = AF_INET;
          pat.type = PATTERN_MASK;
          if (!sortlist_alloc(sortlist, nsort, &pat))
            return ARES_ENOMEM;
        }
      else
        {
          while (*q && *q != ';' && !ISSPACE(*q))
            q++;
        }
      str = q;
      while (ISSPACE(*str))
        str++;
    }

  return ARES_SUCCESS;
}
#endif  /* !WIN32 */
#endif  /* !WATT32 */

static int set_search(ares_channel channel, const char *str)
{
  int n;
  const char *p, *q;

  if(channel->ndomains != -1) {
    /* if we already have some domains present, free them first */
    for(n=0; n < channel->ndomains; n++)
      free(channel->domains[n]);
    free(channel->domains);
    channel->domains = NULL;
    channel->ndomains = -1;
  }

  /* Count the domains given. */
  n = 0;
  p = str;
  while (*p)
    {
      while (*p && !ISSPACE(*p))
        p++;
      while (ISSPACE(*p))
        p++;
      n++;
    }

  if (!n)
    {
      channel->ndomains = 0;
      return ARES_SUCCESS;
    }

  channel->domains = malloc(n * sizeof(char *));
  if (!channel->domains)
    return ARES_ENOMEM;

  /* Now copy the domains. */
  n = 0;
  p = str;
  while (*p)
    {
      channel->ndomains = n;
      q = p;
      while (*q && !ISSPACE(*q))
        q++;
      channel->domains[n] = malloc(q - p + 1);
      if (!channel->domains[n])
        return ARES_ENOMEM;
      memcpy(channel->domains[n], p, q - p);
      channel->domains[n][q - p] = 0;
      p = q;
      while (ISSPACE(*p))
        p++;
      n++;
    }
  channel->ndomains = n;

  return ARES_SUCCESS;
}

static int set_options(ares_channel channel, const char *str)
{
  const char *p, *q, *val;

  p = str;
  while (*p)
    {
      q = p;
      while (*q && !ISSPACE(*q))
        q++;
      val = try_option(p, q, "ndots:");
      if (val && channel->ndots == -1)
        channel->ndots = aresx_sltosi(strtol(val, NULL, 10));
      val = try_option(p, q, "retrans:");
      if (val && channel->timeout == -1)
        channel->timeout = aresx_sltosi(strtol(val, NULL, 10));
      val = try_option(p, q, "retry:");
      if (val && channel->tries == -1)
        channel->tries = aresx_sltosi(strtol(val, NULL, 10));
      val = try_option(p, q, "rotate");
      if (val && channel->rotate == -1)
        channel->rotate = 1;
      p = q;
      while (ISSPACE(*p))
        p++;
    }

  return ARES_SUCCESS;
}

static const char *try_option(const char *p, const char *q, const char *opt)
{
  size_t len = strlen(opt);
  return ((size_t)(q - p) >= len && !strncmp(p, opt, len)) ? &p[len] : NULL;
}

#if !defined(WIN32) && !defined(WATT32)
static char *try_config(char *s, const char *opt, char scc)
{
  size_t len;
  char *p;
  char *q;

  if (!s || !opt)
    /* no line or no option */
    return NULL;

  /* Hash '#' character is always used as primary comment char, additionally
     a not-NUL secondary comment char will be considered when specified. */

  /* trim line comment */
  p = s;
  if(scc)
    while (*p && (*p != '#') && (*p != scc))
      p++;
  else
    while (*p && (*p != '#'))
      p++;
  *p = '\0';

  /* trim trailing whitespace */
  q = p - 1;
  while ((q >= s) && ISSPACE(*q))
    q--;
  *++q = '\0';

  /* skip leading whitespace */
  p = s;
  while (*p && ISSPACE(*p))
    p++;

  if (!*p)
    /* empty line */
    return NULL;

  if ((len = strlen(opt)) == 0)
    /* empty option */
    return NULL;

  if (strncmp(p, opt, len) != 0)
    /* line and option do not match */
    return NULL;

  /* skip over given option name */
  p += len;

  if (!*p)
    /* no option value */
    return NULL;

  if ((opt[len-1] != ':') && (opt[len-1] != '=') && !ISSPACE(*p))
    /* whitespace between option name and value is mandatory
       for given option names which do not end with ':' or '=' */
    return NULL;

  /* skip over whitespace */
  while (*p && ISSPACE(*p))
    p++;

  if (!*p)
    /* no option value */
    return NULL;

  /* return pointer to option value */
  return p;
}

static int sortlist_alloc(struct apattern **sortlist, int *nsort,
                          struct apattern *pat)
{
  struct apattern *newsort;
  newsort = realloc(*sortlist, (*nsort + 1) * sizeof(struct apattern));
  if (!newsort)
    return 0;
  newsort[*nsort] = *pat;
  *sortlist = newsort;
  (*nsort)++;
  return 1;
}

static int ip_addr(const char *ipbuf, ssize_t len, struct in_addr *addr)
{

  /* Four octets and three periods yields at most 15 characters. */
  if (len > 15)
    return -1;

  addr->s_addr = inet_addr(ipbuf);
  if (addr->s_addr == INADDR_NONE && strcmp(ipbuf, "255.255.255.255") != 0)
    return -1;
  return 0;
}

static void natural_mask(struct apattern *pat)
{
  struct in_addr addr;

  /* Store a host-byte-order copy of pat in a struct in_addr.  Icky,
   * but portable.
   */
  addr.s_addr = ntohl(pat->addrV4.s_addr);

  /* This is out of date in the CIDR world, but some people might
   * still rely on it.
   */
  if (IN_CLASSA(addr.s_addr))
    pat->mask.addr4.s_addr = htonl(IN_CLASSA_NET);
  else if (IN_CLASSB(addr.s_addr))
    pat->mask.addr4.s_addr = htonl(IN_CLASSB_NET);
  else
    pat->mask.addr4.s_addr = htonl(IN_CLASSC_NET);
}
#endif /* !WIN32 && !WATT32 */

/* initialize an rc4 key. If possible a cryptographically secure random key
   is generated using a suitable function (for example win32's RtlGenRandom as
   described in
   http://blogs.msdn.com/michael_howard/archive/2005/01/14/353379.aspx
   otherwise the code defaults to cross-platform albeit less secure mechanism
   using rand
*/
static void randomize_key(unsigned char* key,int key_data_len)
{
  int randomized = 0;
  int counter=0;
#ifdef WIN32
  BOOLEAN res;
  if (ares_fpSystemFunction036)
    {
      res = (*ares_fpSystemFunction036) (key, key_data_len);
      if (res)
        randomized = 1;
    }
#else /* !WIN32 */
#ifdef RANDOM_FILE
  FILE *f = fopen(RANDOM_FILE, "rb");
  if(f) {
    counter = aresx_uztosi(fread(key, 1, key_data_len, f));
    fclose(f);
  }
#endif
#endif /* WIN32 */

  if ( !randomized ) {
    for (;counter<key_data_len;counter++)
      key[counter]=(unsigned char)(rand() % 256);
  }
}

static int init_id_key(rc4_key* key,int key_data_len)
{
  unsigned char index1;
  unsigned char index2;
  unsigned char* state;
  short counter;
  unsigned char *key_data_ptr = 0;

  key_data_ptr = calloc(1,key_data_len);
  if (!key_data_ptr)
    return ARES_ENOMEM;

  state = &key->state[0];
  for(counter = 0; counter < 256; counter++)
    /* unnecessary AND but it keeps some compilers happier */
    state[counter] = (unsigned char)(counter & 0xff);
  randomize_key(key->state,key_data_len);
  key->x = 0;
  key->y = 0;
  index1 = 0;
  index2 = 0;
  for(counter = 0; counter < 256; counter++)
  {
    index2 = (unsigned char)((key_data_ptr[index1] + state[counter] +
                              index2) % 256);
    ARES_SWAP_BYTE(&state[counter], &state[index2]);

    index1 = (unsigned char)((index1 + 1) % key_data_len);
  }
  free(key_data_ptr);
  return ARES_SUCCESS;
}

unsigned short ares__generate_new_id(rc4_key* key)
{
  unsigned short r=0;
  ares__rc4(key, (unsigned char *)&r, sizeof(r));
  return r;
}

void ares_set_local_ip4(ares_channel channel, unsigned int local_ip)
{
  channel->local_ip4 = local_ip;
}

/* local_ip6 should be 16 bytes in length */
void ares_set_local_ip6(ares_channel channel,
                        const unsigned char* local_ip6)
{
  memcpy(&channel->local_ip6, local_ip6, sizeof(channel->local_ip6));
}

/* local_dev_name should be null terminated. */
void ares_set_local_dev(ares_channel channel,
                        const char* local_dev_name)
{
  strncpy(channel->local_dev_name, local_dev_name,
          sizeof(channel->local_dev_name));
  channel->local_dev_name[sizeof(channel->local_dev_name) - 1] = 0;
}


void ares_set_socket_callback(ares_channel channel,
                              ares_sock_create_callback cb,
                              void *data)
{
  channel->sock_create_cb = cb;
  channel->sock_create_cb_data = data;
}

void ares__init_servers_state(ares_channel channel)
{
  struct server_state *server;
  int i;

  for (i = 0; i < channel->nservers; i++)
    {
      server = &channel->servers[i];
      server->udp_socket = ARES_SOCKET_BAD;
      server->tcp_socket = ARES_SOCKET_BAD;
      server->tcp_connection_generation = ++channel->tcp_connection_generation;
      server->tcp_lenbuf_pos = 0;
      server->tcp_buffer_pos = 0;
      server->tcp_buffer = NULL;
      server->tcp_length = 0;
      server->qhead = NULL;
      server->qtail = NULL;
      ares__init_list_head(&server->queries_to_server);
      server->channel = channel;
      server->is_broken = 0;
    }
}
