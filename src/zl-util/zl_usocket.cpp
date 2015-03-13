// LuaSocket 2.0.2 license
// Copyright © 2004-2007 Diego Nehab
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


// modifications Copyright (c) 2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLDeviceTime.h>
#include <zl-util/zl_socket.h>

#ifdef ZL_SOCKET_POLL

	#include <sys/poll.h>

	#define WAITFD_R        POLLIN
	#define WAITFD_W        POLLOUT
	#define WAITFD_C        ( POLLIN | POLLOUT )

#else

	#define WAITFD_R        1
	#define WAITFD_W        2
	#define WAITFD_C        ( WAITFD_R | WAITFD_W )

#endif

typedef struct hostent zl_hostent;

//----------------------------------------------------------------//
int zl_inet_aton ( cc8* cp, zl_inaddr* inp ) {

	#ifndef inet_aton
	
		unsigned int a = 0, b = 0, c = 0, d = 0;
		int n = 0, r;
		unsigned long int addr = 0;
		r = sscanf(cp, "%u.%u.%u.%u%n", &a, &b, &c, &d, &n);
		if (r == 0 || n == 0) return 0;
		cp += n;
		if (*cp) return 0;
		if (a > 255 || b > 255 || c > 255 || d > 255) return 0;
		if (inp) {
			addr += a; addr <<= 8;
			addr += b; addr <<= 8;
			addr += c; addr <<= 8;
			addr += d;
			inp->s_addr = htonl(addr);
		}
		return 1;
	#else
	
		return inet_aton ( cp, inp );
	#endif
}

//----------------------------------------------------------------//
int zl_inet_bind ( zl_socket* ps, cc8* address, unsigned short port ) {

    struct sockaddr_in local;
    int err;
    memset(&local, 0, sizeof(local));
	
    /* address is either wildcard or a valid ip address */
    local.sin_addr.s_addr = htonl ( INADDR_ANY );
    local.sin_port = htons ( port );
    local.sin_family = AF_INET;
	
    if ( strcmp ( address, "*" ) && !inet_aton ( address, &local.sin_addr )) {
        struct hostent *hp = NULL;
        struct in_addr **addr;
        err = zl_socket_gethostbyname ( address, &hp );
        if ( err != IO_DONE) return err;
        addr = ( struct in_addr ** ) hp->h_addr_list;
        memcpy ( &local.sin_addr, *addr, sizeof ( struct in_addr ));
    }
    err = zl_socket_bind(ps, ( zl_sockaddr* )&local, sizeof ( local ));
    if ( err != IO_DONE ) {
		zl_socket_destroy ( ps );
	}
    return err;
}

//----------------------------------------------------------------//
int zl_inet_connect ( zl_socket* ps, cc8* address, unsigned short port, double tm ) {

    struct sockaddr_in remote;
    int err;
    memset ( &remote, 0, sizeof ( remote ));
    remote.sin_family = AF_INET;
    remote.sin_port = htons ( port );
	if ( strcmp ( address, "*" )) {
        if ( !inet_aton ( address, &remote.sin_addr )) {
            zl_hostent* hp = NULL;
            zl_inaddr** addr;
            err = zl_socket_gethostbyname ( address, &hp );
            if (err != IO_DONE) return err;
            addr = ( struct in_addr** ) hp->h_addr_list;
            memcpy ( &remote.sin_addr, *addr, sizeof ( struct in_addr ));
        }
    }
	else {
		remote.sin_family = AF_UNSPEC;
	}
    return zl_socket_connect ( ps, ( zl_sockaddr* )&remote, sizeof ( remote ), tm );
}

//----------------------------------------------------------------//
//int zl_inet_create ( zl_socket* ps, int type ) {
//    return zl_socket_create ( ps, AF_INET, type, 0 );
//}

//----------------------------------------------------------------//
cc8* zl_io_strerror( int err ) {
    switch (err) {
        case IO_DONE: return NULL;
        case IO_CLOSED: return "closed";
        case IO_TIMEOUT: return "timeout";
        default: return "unknown error"; 
    }
}

//----------------------------------------------------------------//
int zl_socket_accept(zl_socket* ps, zl_socket* pa, zl_sockaddr *addr, socklen_t *len, double tm) {
    zl_sockaddr daddr;
    socklen_t dlen = sizeof(daddr);
    if (*ps == SOCKET_INVALID) return IO_CLOSED; 
    if (!addr) addr = &daddr;
    if (!len) len = &dlen;
    for ( ;; ) {
        int err;
        if ((*pa = accept(*ps, addr, len)) != SOCKET_INVALID) return IO_DONE;
        err = errno;
        if (err == EINTR) continue;
        if (err != EAGAIN && err != ECONNABORTED) return err;
        if ((err = zl_socket_waitfd(*ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    /* can't reach here */
    return IO_UNKNOWN;
}

//----------------------------------------------------------------//
int zl_socket_bind(zl_socket* ps, zl_sockaddr *addr, socklen_t len) {
    int err = IO_DONE;
    zl_socket_setblocking(ps);
    if (bind(*ps, addr, len) < 0) err = errno; 
    zl_socket_setnonblocking(ps);
    return err;
}

//----------------------------------------------------------------//
int zl_socket_connect(zl_socket* ps, zl_sockaddr *addr, socklen_t len, double tm) {
    int err;
    /* avoid calling on closed sockets */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* call connect until done or failed without being interrupted */
    do if (connect(*ps, addr, len) == 0) return IO_DONE;
    while ((err = errno) == EINTR);
    /* if connection failed immediately, return error code */
    if (err != EINPROGRESS && err != EAGAIN) return err; 
    /* zero timeout case optimization */
    if ( tm == 0.0 ) return IO_TIMEOUT;
    /* wait until we have the result of the connection attempt or timeout */
    err = zl_socket_waitfd(*ps, WAITFD_C, tm);
    if (err == IO_CLOSED) {
        if (recv(*ps, (char *) &err, 0, 0) == 0) return IO_DONE;
        else return errno;
    } else return err;
}

//----------------------------------------------------------------//
int zl_socket_create ( zl_socket* ps, int domain, int type, int protocol ) {

	zl_socket_disable_sigpipe ();

    *ps = socket(domain, type, protocol);
    if (*ps != SOCKET_INVALID) return IO_DONE; 
    else return errno; 
}

//----------------------------------------------------------------//
void zl_socket_destroy(zl_socket* ps) {
    if (*ps != SOCKET_INVALID) {
        zl_socket_setblocking(ps);
        close(*ps);
        *ps = SOCKET_INVALID;
    }
}

//----------------------------------------------------------------//
int zl_socket_disable_sigpipe () {
    /* instals a handler to ignore sigpipe or it will crash us */
    signal(SIGPIPE, SIG_IGN);
    return 1;
}

//----------------------------------------------------------------//
int zl_socket_gethostbyaddr(cc8*addr, socklen_t len, struct hostent **hp) {
    *hp = gethostbyaddr(addr, len, AF_INET);
    if (*hp) return IO_DONE;
    else if (h_errno) return h_errno;
    else if (errno) return errno;
    else return IO_UNKNOWN;
}

//----------------------------------------------------------------//
int zl_socket_gethostbyname(cc8*addr, struct hostent **hp) {
    *hp = gethostbyname(addr);
    if (*hp) return IO_DONE;
    else if (h_errno) return h_errno;
    else if (errno) return errno;
    else return IO_UNKNOWN;
}

//----------------------------------------------------------------//
cc8* zl_socket_hoststrerror(int err) {
    if (err <= 0) return zl_io_strerror(err);
    switch (err) {
        case HOST_NOT_FOUND: return "host not found";
        default: return hstrerror(err);
    }
}

//----------------------------------------------------------------//
cc8* zl_socket_ioerror(zl_socket* ps, int err) {
    (void) ps;
    return zl_socket_strerror(err);
}

//----------------------------------------------------------------//
int zl_socket_listen(zl_socket* ps, int backlog) {
    int err = IO_DONE; 
    zl_socket_setblocking(ps);
    if (listen(*ps, backlog)) err = errno; 
    zl_socket_setnonblocking(ps);
    return err;
}

//----------------------------------------------------------------//
int zl_socket_recv(zl_socket* ps, char *data, size_t count, size_t *got, double tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        long taken = (long) recv(*ps, data, count, 0);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        err = errno;
        if (taken == 0) return IO_CLOSED;
        if (err == EINTR) continue;
        if (err != EAGAIN) return err; 
        if ((err = zl_socket_waitfd(*ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

//----------------------------------------------------------------//
int zl_socket_recvfrom(zl_socket* ps, char *data, size_t count, size_t *got,
        zl_sockaddr *addr, socklen_t *len, double tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        long taken = (long) recvfrom(*ps, data, count, 0, addr, len);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        err = errno;
        if (taken == 0) return IO_CLOSED;
        if (err == EINTR) continue;
        if (err != EAGAIN) return err; 
        if ((err = zl_socket_waitfd(*ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

//----------------------------------------------------------------//
int zl_socket_select ( zl_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, double tm ) {

	struct timeval* tp = NULL;
	struct timeval tv;

	if ( tm >= 0.0f ) {
		tm += ZLDeviceTime::GetTimeInSeconds ();
		tp = &tv;
	}

	int ret;

    do {
		if ( tp ) {
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			
			double t = tm - ZLDeviceTime::GetTimeInSeconds ();
			if ( t > 0.0 ) {
				tv.tv_sec = ( int )t;
				tv.tv_usec = ( int )(( t - tv.tv_sec ) * 1.0e6 );
			}
		}
        ret = select ( n, rfds, wfds, efds, tp );
    }
	while ( ret < 0 && errno == EINTR );
	
	return ret;
}

//----------------------------------------------------------------//
int zl_socket_send ( zl_socket n, cc8*data, size_t count, size_t *sent, double tm ) {

	return zl_socket_sendto ( n, data, count, sent, 0, 0, tm );
}

//----------------------------------------------------------------//
int zl_socket_sendto ( zl_socket n, cc8*data, size_t count, size_t *sent, zl_sockaddr *addr, socklen_t len, double tm ) {
	
    *sent = 0;
	
    // avoid making system calls on closed sockets
    if ( n == SOCKET_INVALID ) return IO_CLOSED;
	
    // loop until we send something or we give up on error
    for ( ;; ) {
		
		long put = ( long )sendto ( n, data, count, 0, addr, len );
		
		// if we sent anything, we are done
        if ( put > 0 ) {
            *sent = put;
            return IO_DONE;
        }
		
        int err = errno;
		
		if ( put == 0 || err == EPIPE ) return IO_CLOSED; // send can't really return 0, but EPIPE means the connection was closed
        if ( err == EINTR ) continue; // we call was interrupted, just try again
        if ( err != EAGAIN ) return err; // if failed fatal reason, report error
        if (( err = zl_socket_waitfd ( n + 1, WAITFD_W, tm )) != IO_DONE) return err; // wait until we can send something or we timeout
    }

    return IO_UNKNOWN; // can't reach here
}

//----------------------------------------------------------------//
void zl_socket_setblocking(zl_socket* ps) {
    int flags = fcntl(*ps, F_GETFL, 0);
    flags &= (~(O_NONBLOCK));
    fcntl(*ps, F_SETFL, flags);
}

//----------------------------------------------------------------//
void zl_socket_setnonblocking(zl_socket* ps) {
    int flags = fcntl(*ps, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(*ps, F_SETFL, flags);
}

//----------------------------------------------------------------//
void zl_socket_shutdown(zl_socket* ps, int how) {
    zl_socket_setblocking(ps);
    shutdown(*ps, how);
    zl_socket_setnonblocking(ps);
}

//----------------------------------------------------------------//
cc8* zl_socket_strerror(int err) {
    if (err <= 0) return zl_io_strerror(err);
    switch (err) {
		case IO_DONE: return NULL;
        case IO_CLOSED: return "closed";
        case IO_TIMEOUT: return "timeout";
        case EADDRINUSE: return "address already in use";
        case EISCONN: return "already connected";
        case EACCES: return "permission denied";
        case ECONNREFUSED: return "connection refused";
        case ECONNABORTED: return "closed";
        case ECONNRESET: return "closed";
        case ETIMEDOUT: return "timeout";
        default: return strerror(errno);
    }
}

//----------------------------------------------------------------//
int zl_socket_waitfd ( zl_socket n, int sw, double tm ) {

	struct timeval* tp = NULL;
	struct timeval tv;

	if ( tm >= 0.0f ) {
		tm += ZLDeviceTime::GetTimeInSeconds ();
		tp = &tv;
	}
	
	int ret;
	
	fd_set rfds, wfds, *rp, *wp;
	
	do {
	
		// must set bits within loop, because select may have modifed them
		rp = wp = NULL;
		if ( sw & WAITFD_R ) { FD_ZERO ( &rfds ); FD_SET ( n, &rfds ); rp = &rfds; }
		if ( sw & WAITFD_W ) { FD_ZERO ( &wfds ); FD_SET ( n, &wfds ); wp = &wfds; }
		tp = NULL;
		
		if ( tp ) {
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			
			double t = tm - ZLDeviceTime::GetTimeInSeconds ();
			if ( t > 0.0 ) {
				tv.tv_sec = ( int )t;
				tv.tv_usec = ( int )(( t - tv.tv_sec ) * 1.0e6 );
			}
		}
		ret = select ( n, rp, wp, NULL, tp );
	}
	while ( ret == -1 && errno == EINTR );

	if ( ret == -1 ) return errno;
	if ( ret == 0 ) return IO_TIMEOUT;
	if ( sw == WAITFD_C && FD_ISSET ( n, &rfds )) return IO_CLOSED;
	return IO_DONE;
}
