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

SUPPRESS_EMPTY_FILE_WARNING
#ifdef _WIN32

/*=========================================================================*\
* Socket compatibilization module for Win32
* LuaSocket toolkit
*
* The penalty of calling select to avoid busy-wait is only paid when
* the I/O call fail in the first place. 
*
* RCS ID: $Id: wsocket.c,v 1.36 2007/06/11 23:44:54 diego Exp $
\*=========================================================================*/
#include <string.h>

#include "socket.h"

/* WinSock doesn't have a strerror... */
static const char *wstrerror(int err);

/*-------------------------------------------------------------------------*\
* Initializes module 
\*-------------------------------------------------------------------------*/
int socket_open(void) {
    Wzl_sockaddrDATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0); 
    int err = Wzl_sockaddrStartup(wVersionRequested, &wsaData );
    if (err != 0) return 0;
    if ((LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) &&
        (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)) {
        Wzl_sockaddrCleanup();
        return 0; 
    }
    return 1;
}

/*-------------------------------------------------------------------------*\
* Close module 
\*-------------------------------------------------------------------------*/
int socket_close(void) {
    Wzl_sockaddrCleanup();
    return 1;
}

/*-------------------------------------------------------------------------*\
* Wait for readable/writable/connected socket with timeout
\*-------------------------------------------------------------------------*/
#define WAITFD_R        1
#define WAITFD_W        2
#define WAITFD_E        4
#define WAITFD_C        (WAITFD_E|WAITFD_W)

int socket_waitfd(zl_socket* ps, int sw, p_timeout tm) {
    int ret;
    fd_set rfds, wfds, efds, *rp = NULL, *wp = NULL, *ep = NULL;
    struct timeval tv, *tp = NULL;
    double t;
    if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
    if (sw & WAITFD_R) { 
        FD_ZERO(&rfds); 
		FD_SET(*ps, &rfds);
        rp = &rfds; 
    }
    if (sw & WAITFD_W) { FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
    if (sw & WAITFD_C) { FD_ZERO(&efds); FD_SET(*ps, &efds); ep = &efds; }
    if ((t = timeout_get(tm)) >= 0.0) {
        tv.tv_sec = (int) t;
        tv.tv_usec = (int) ((t-tv.tv_sec)*1.0e6);
        tp = &tv;
    }
    ret = select(0, rp, wp, ep, tp);
    if (ret == -1) return Wzl_sockaddrGetLastError();
    if (ret == 0) return IO_TIMEOUT;
    if (sw == WAITFD_C && FD_ISSET(*ps, &efds)) return IO_CLOSED;
    return IO_DONE;
}

/*-------------------------------------------------------------------------*\
* Select with int timeout in ms
\*-------------------------------------------------------------------------*/
int socket_select(zl_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, 
        p_timeout tm) {
    struct timeval tv; 
    double t = timeout_get(tm);
    tv.tv_sec = (int) t;
    tv.tv_usec = (int) ((t - tv.tv_sec) * 1.0e6);
    if (n <= 0) {
        Sleep((DWORD) (1000*t));
        return 0;
    } else return select(0, rfds, wfds, efds, t >= 0.0? &tv: NULL);
}

/*-------------------------------------------------------------------------*\
* Close and inutilize socket
\*-------------------------------------------------------------------------*/
void socket_destroy(zl_socket* ps) {
    if (*ps != SOCKET_INVALID) {
        socket_setblocking(ps); /* close can take a long time on WIN32 */
        closesocket(*ps);
        *ps = SOCKET_INVALID;
    }
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
void socket_shutdown(zl_socket* ps, int how) {
    socket_setblocking(ps);
    shutdown(*ps, how);
    socket_setnonblocking(ps);
}

/*-------------------------------------------------------------------------*\
* Creates and sets up a socket
\*-------------------------------------------------------------------------*/
int socket_create(zl_socket* ps, int domain, int type, int protocol) {
    *ps = socket(domain, type, protocol);
    if (*ps != SOCKET_INVALID) return IO_DONE;
    else return Wzl_sockaddrGetLastError();
}

/*-------------------------------------------------------------------------*\
* Connects or returns error message
\*-------------------------------------------------------------------------*/
int socket_connect(zl_socket* ps, zl_sockaddr *addr, socklen_t len, p_timeout tm) {
    int err;
    /* don't call on closed socket */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* ask system to connect */
    if (connect(*ps, addr, len) == 0) return IO_DONE;
    /* make sure the system is trying to connect */
    err = Wzl_sockaddrGetLastError();
    if (err != Wzl_sockaddrEWOULDBLOCK && err != Wzl_sockaddrEINPROGRESS) return err;
    /* zero timeout case optimization */
    if (timeout_iszero(tm)) return IO_TIMEOUT;
    /* we wait until something happens */
    err = socket_waitfd(ps, WAITFD_C, tm);
    if (err == IO_CLOSED) {
        int len = sizeof(err);
        /* give windows time to set the error (yes, disgusting) */
        Sleep(10);
        /* find out why we failed */
        getsockopt(*ps, SOL_SOCKET, SO_ERROR, (char *)&err, &len); 
        /* we KNOW there was an error. if 'why' is 0, we will return
        * "unknown error", but it's not really our fault */
        return err > 0? err: IO_UNKNOWN; 
    } else return err;

}

/*-------------------------------------------------------------------------*\
* Binds or returns error message
\*-------------------------------------------------------------------------*/
int socket_bind(zl_socket* ps, zl_sockaddr *addr, socklen_t len) {
    int err = IO_DONE;
    socket_setblocking(ps);
    if (bind(*ps, addr, len) < 0) err = Wzl_sockaddrGetLastError();
    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
int socket_listen(zl_socket* ps, int backlog) {
    int err = IO_DONE;
    socket_setblocking(ps);
    if (listen(*ps, backlog) < 0) err = Wzl_sockaddrGetLastError();
    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* Accept with timeout
\*-------------------------------------------------------------------------*/
int socket_accept(zl_socket* ps, zl_socket* pa, zl_sockaddr *addr, socklen_t *len, 
        p_timeout tm) {
    zl_sockaddr daddr;
    socklen_t dlen = sizeof(daddr);
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    if (!addr) addr = &daddr;
    if (!len) len = &dlen;
    for ( ;; ) {
        int err;
        /* try to get client socket */
        if ((*pa = accept(*ps, addr, len)) != SOCKET_INVALID) return IO_DONE;
        /* find out why we failed */
        err = Wzl_sockaddrGetLastError(); 
        /* if we failed because there was no connectoin, keep trying */
        if (err != Wzl_sockaddrEWOULDBLOCK && err != Wzl_sockaddrECONNABORTED) return err;
        /* call select to avoid busy wait */
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    } 
    /* can't reach here */
    return IO_UNKNOWN; 
}

/*-------------------------------------------------------------------------*\
* Send with timeout
* On windows, if you try to send 10MB, the OS will buffer EVERYTHING 
* this can take an awful lot of time and we will end up blocked. 
* Therefore, whoever calls this function should not pass a huge buffer.
\*-------------------------------------------------------------------------*/
int socket_send(zl_socket* ps, const char *data, size_t count, 
        size_t *sent, p_timeout tm)
{
    int err;
    *sent = 0;
    /* avoid making system calls on closed sockets */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* loop until we send something or we give up on error */
    for ( ;; ) {
        /* try to send something */
		int put = send(*ps, data, (int) count, 0);
        /* if we sent something, we are done */
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        /* deal with failure */
        err = Wzl_sockaddrGetLastError(); 
        /* we can only proceed if there was no serious error */
        if (err != Wzl_sockaddrEWOULDBLOCK) return err;
        /* avoid busy wait */
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    } 
    /* can't reach here */
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Sendto with timeout
\*-------------------------------------------------------------------------*/
int socket_sendto(zl_socket* ps, const char *data, size_t count, size_t *sent, 
        zl_sockaddr *addr, socklen_t len, p_timeout tm)
{
    int err;
    *sent = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int put = sendto(*ps, data, (int) count, 0, addr, len);
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        err = Wzl_sockaddrGetLastError(); 
        if (err != Wzl_sockaddrEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    } 
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Receive with timeout
\*-------------------------------------------------------------------------*/
int socket_recv(zl_socket* ps, char *data, size_t count, size_t *got, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int taken = recv(*ps, data, (int) count, 0);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        if (taken == 0) return IO_CLOSED;
        err = Wzl_sockaddrGetLastError();
        if (err != Wzl_sockaddrEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Recvfrom with timeout
\*-------------------------------------------------------------------------*/
int socket_recvfrom(zl_socket* ps, char *data, size_t count, size_t *got, 
        zl_sockaddr *addr, socklen_t *len, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int taken = recvfrom(*ps, data, (int) count, 0, addr, len);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        if (taken == 0) return IO_CLOSED;
        err = Wzl_sockaddrGetLastError();
        if (err != Wzl_sockaddrEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Put socket into blocking mode
\*-------------------------------------------------------------------------*/
void socket_setblocking(zl_socket* ps) {
    u_long argp = 0;
    ioctlsocket(*ps, FIONBIO, &argp);
}

/*-------------------------------------------------------------------------*\
* Put socket into non-blocking mode
\*-------------------------------------------------------------------------*/
void socket_setnonblocking(zl_socket* ps) {
    u_long argp = 1;
    ioctlsocket(*ps, FIONBIO, &argp);
}

/*-------------------------------------------------------------------------*\
* DNS helpers 
\*-------------------------------------------------------------------------*/
int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp) {
    *hp = gethostbyaddr(addr, len, AF_INET);
    if (*hp) return IO_DONE;
    else return Wzl_sockaddrGetLastError();
}

int socket_gethostbyname(const char *addr, struct hostent **hp) {
    *hp = gethostbyname(addr);
    if (*hp) return IO_DONE;
    else return  Wzl_sockaddrGetLastError();
}

/*-------------------------------------------------------------------------*\
* Error translation functions
\*-------------------------------------------------------------------------*/
const char *socket_hoststrerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case Wzl_sockaddrHOST_NOT_FOUND: return "host not found";
        default: return wstrerror(err); 
    }
}

const char *socket_strerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case Wzl_sockaddrEADDRINUSE: return "address already in use";
        case Wzl_sockaddrECONNREFUSED: return "connection refused";
        case Wzl_sockaddrEISCONN: return "already connected";
        case Wzl_sockaddrEACCES: return "permission denied";
        case Wzl_sockaddrECONNABORTED: return "closed";
        case Wzl_sockaddrECONNRESET: return "closed";
        case Wzl_sockaddrETIMEDOUT: return "timeout";
        default: return wstrerror(err);
    }
}

const char *socket_ioerror(zl_socket* ps, int err) {
	(void) ps;
	return socket_strerror(err);
}

static const char *wstrerror(int err) {
    switch (err) {
        case Wzl_sockaddrEINTR: return "Interrupted function call";
        case Wzl_sockaddrEACCES: return "Permission denied";
        case Wzl_sockaddrEFAULT: return "Bad address";
        case Wzl_sockaddrEINVAL: return "Invalid argument";
        case Wzl_sockaddrEMFILE: return "Too many open files";
        case Wzl_sockaddrEWOULDBLOCK: return "Resource temporarily unavailable";
        case Wzl_sockaddrEINPROGRESS: return "Operation now in progress";
        case Wzl_sockaddrEALREADY: return "Operation already in progress";
        case Wzl_sockaddrENOTSOCK: return "Socket operation on nonsocket";
        case Wzl_sockaddrEDESTADDRREQ: return "Destination address required";
        case Wzl_sockaddrEMSGSIZE: return "Message too long";
        case Wzl_sockaddrEPROTOTYPE: return "Protocol wrong type for socket";
        case Wzl_sockaddrENOPROTOOPT: return "Bad protocol option";
        case Wzl_sockaddrEPROTONOSUPPORT: return "Protocol not supported";
        case Wzl_sockaddrESOCKTNOSUPPORT: return "Socket type not supported";
        case Wzl_sockaddrEOPNOTSUPP: return "Operation not supported";
        case Wzl_sockaddrEPFNOSUPPORT: return "Protocol family not supported";
        case Wzl_sockaddrEAFNOSUPPORT: 
            return "Address family not supported by protocol family"; 
        case Wzl_sockaddrEADDRINUSE: return "Address already in use";
        case Wzl_sockaddrEADDRNOTAVAIL: return "Cannot assign requested address";
        case Wzl_sockaddrENETDOWN: return "Network is down";
        case Wzl_sockaddrENETUNREACH: return "Network is unreachable";
        case Wzl_sockaddrENETRESET: return "Network dropped connection on reset";
        case Wzl_sockaddrECONNABORTED: return "Software caused connection abort";
        case Wzl_sockaddrECONNRESET: return "Connection reset by peer";
        case Wzl_sockaddrENOBUFS: return "No buffer space available";
        case Wzl_sockaddrEISCONN: return "Socket is already connected";
        case Wzl_sockaddrENOTCONN: return "Socket is not connected";
        case Wzl_sockaddrESHUTDOWN: return "Cannot send after socket shutdown";
        case Wzl_sockaddrETIMEDOUT: return "Connection timed out";
        case Wzl_sockaddrECONNREFUSED: return "Connection refused";
        case Wzl_sockaddrEHOSTDOWN: return "Host is down";
        case Wzl_sockaddrEHOSTUNREACH: return "No route to host";
        case Wzl_sockaddrEPROCLIM: return "Too many processes";
        case Wzl_sockaddrSYSNOTREADY: return "Network subsystem is unavailable";
        case Wzl_sockaddrVERNOTSUPPORTED: return "Winsock.dll version out of range";
        case Wzl_sockaddrNOTINITIALISED: 
            return "Successful Wzl_sockaddrStartup not yet performed";
        case Wzl_sockaddrEDISCON: return "Graceful shutdown in progress";
        case Wzl_sockaddrHOST_NOT_FOUND: return "Host not found";
        case Wzl_sockaddrTRY_AGAIN: return "Nonauthoritative host not found";
        case Wzl_sockaddrNO_RECOVERY: return "Nonrecoverable name lookup error"; 
        case Wzl_sockaddrNO_DATA: return "Valid name, no data record of requested type";
        default: return "Unknown error";
    }
}

#endif
