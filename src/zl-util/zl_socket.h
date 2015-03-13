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

#ifndef ZL_SOCKET_H
#define ZL_SOCKET_H

/*=========================================================================*\
* Socket compatibilization module
* LuaSocket toolkit
*
* BSD Sockets and WinSock are similar, but there are a few irritating
* differences. Also, not all *nix platforms behave the same. This module
* (and the associated usocket.h and wsocket.h) factor these differences and
* creates a interface compatible with the io.h module.
*
* RCS ID: $Id: socket.h,v 1.20 2005/11/20 07:20:23 diego Exp $
\*=========================================================================*/

/*=========================================================================*\
* Platform specific compatibilization
\*=========================================================================*/
#ifdef _WIN32
	#include <zl-common/zl_types.h>
	#include <zl-util/zl_wsocket.h>
#else
	#include <zl-util/zl_usocket.h>
#endif

enum {
    IO_DONE = 0,        /* operation completed successfully */
    IO_TIMEOUT = -1,    /* operation timed out */
    IO_CLOSED = -2,     /* the connection has been closed */
	IO_UNKNOWN = -3
};

typedef struct in_addr zl_inaddr;
typedef struct sockaddr zl_sockaddr;

int		zl_inet_aton				( cc8* cp, zl_inaddr* inp );
int		zl_inet_bind				( zl_socket* ps, const char* address, u16 port );
int		zl_inet_connect				( zl_socket* ps, const char* address, u16 port, double tm );

cc8*	zl_io_strerror				( int err );
int		zl_socket_accept			( zl_socket* ps, zl_socket* pa, zl_sockaddr *addr, socklen_t *addr_len, double tm );
int		zl_socket_bind				( zl_socket* ps, zl_sockaddr* addr, socklen_t addr_len );
int		zl_socket_connect			( zl_socket* ps, zl_sockaddr* addr, socklen_t addr_len, double tm );
int		zl_socket_create			( zl_socket* ps, int domain, int type, int protocol );
void	zl_socket_destroy			( zl_socket* ps );
int		zl_socket_disable_sigpipe	();
int		zl_socket_gethostbyaddr		( const char *addr, socklen_t len, struct hostent **hp );
int		zl_socket_gethostbyname		( const char *addr, struct hostent **hp );
cc8*	zl_socket_hoststrerror		( int err );
cc8*	zl_socket_ioerror			( zl_socket* ps, int err );
int		zl_socket_listen			( zl_socket* ps, int backlog );
int		zl_socket_recv				( zl_socket* ps, char *data, size_t count, size_t *got, double tm );
int		zl_socket_recvfrom			( zl_socket* ps, char *data, size_t count, size_t *got, zl_sockaddr *addr, socklen_t *addr_len, double tm );
int		zl_socket_select			( zl_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, double tm );
int		zl_socket_send				( zl_socket n, const char *data, size_t count, size_t *sent, double tm );
int		zl_socket_sendto			( zl_socket n, const char *data, size_t count, size_t *sent, zl_sockaddr *addr, socklen_t addr_len, double tm );
void	zl_socket_setblocking		( zl_socket* ps );
void	zl_socket_setnonblocking	( zl_socket* ps );
void	zl_socket_shutdown			( zl_socket* ps, int how );
cc8*	zl_socket_strerror			( int err );
int		zl_socket_waitfd			( zl_socket n, int sw, double tm );

#endif
