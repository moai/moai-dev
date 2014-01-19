/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_X11

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h> /* For INT_MAX */

#include "SDL_x11video.h"
#include "SDL_x11video.h"
#include "SDL_x11touch.h"
#include "SDL_x11xinput2.h"
#include "../../events/SDL_events_c.h"
#include "../../events/SDL_mouse_c.h"
#include "../../events/SDL_touch_c.h"

#include "SDL_timer.h"
#include "SDL_syswm.h"

#include <stdio.h>

typedef struct {
    unsigned char *data;
    int format, count;
    Atom type;
} SDL_x11Prop;

/* Reads property
   Must call XFree on results
 */
static void X11_ReadProperty(SDL_x11Prop *p, Display *disp, Window w, Atom prop)
{
    unsigned char *ret=NULL;
    Atom type;
    int fmt;
    unsigned long count;
    unsigned long bytes_left;
    int bytes_fetch = 0;

    do {
        if (ret != 0) XFree(ret);
        XGetWindowProperty(disp, w, prop, 0, bytes_fetch, False, AnyPropertyType, &type, &fmt, &count, &bytes_left, &ret);
        bytes_fetch += bytes_left;
    } while (bytes_left != 0);

    p->data=ret;
    p->format=fmt;
    p->count=count;
    p->type=type;
}

/* Find text-uri-list in a list of targets and return it's atom
   if available, else return None */
static Atom X11_PickTarget(Display *disp, Atom list[], int list_count)
{
    Atom request = None;
    char *name;
    int i;
    for (i=0; i < list_count && request == None; i++) {
        name = XGetAtomName(disp, list[i]);
        if (strcmp("text/uri-list", name)==0) request = list[i];
        XFree(name);
    }
    return request;
}

/* Wrapper for X11_PickTarget for a maximum of three targets, a special
   case in the Xdnd protocol */
static Atom X11_PickTargetFromAtoms(Display *disp, Atom a0, Atom a1, Atom a2)
{
    int count=0;
    Atom atom[3];
    if (a0 != None) atom[count++] = a0;
    if (a1 != None) atom[count++] = a1;
    if (a2 != None) atom[count++] = a2;
    return X11_PickTarget(disp, atom, count);
}
/*#define DEBUG_XEVENTS*/

struct KeyRepeatCheckData
{
    XEvent *event;
    SDL_bool found;
};

static Bool X11_KeyRepeatCheckIfEvent(Display *display, XEvent *chkev,
    XPointer arg)
{
    struct KeyRepeatCheckData *d = (struct KeyRepeatCheckData *) arg;
    if (chkev->type == KeyPress &&
        chkev->xkey.keycode == d->event->xkey.keycode &&
        chkev->xkey.time - d->event->xkey.time < 2)
        d->found = SDL_TRUE;
    return False;
}

/* Check to see if this is a repeated key.
   (idea shamelessly lifted from GII -- thanks guys! :)
 */
static SDL_bool X11_KeyRepeat(Display *display, XEvent *event)
{
    XEvent dummyev;
    struct KeyRepeatCheckData d;
    d.event = event;
    d.found = SDL_FALSE;
    if (XPending(display))
        XCheckIfEvent(display, &dummyev, X11_KeyRepeatCheckIfEvent,
            (XPointer) &d);
    return d.found;
}

static Bool X11_IsWheelCheckIfEvent(Display *display, XEvent *chkev,
    XPointer arg)
{
    XEvent *event = (XEvent *) arg;
    if (chkev->type == ButtonRelease &&
        chkev->xbutton.button == event->xbutton.button &&
        chkev->xbutton.time == event->xbutton.time)
        return True;
    return False;
}

static SDL_bool X11_IsWheelEvent(Display * display,XEvent * event,int * ticks)
{
    XEvent relevent;
    if (XPending(display)) {
        /* according to the xlib docs, no specific mouse wheel events exist.
           however, mouse wheel events trigger a button press and a button release
           immediately. thus, checking if the same button was released at the same
           time as it was pressed, should be an adequate hack to derive a mouse
           wheel event. */
        if (XCheckIfEvent(display, &relevent, X11_IsWheelCheckIfEvent,
            (XPointer) event)) {

            /* by default, X11 only knows 5 buttons. on most 3 button + wheel mouse,
               Button4 maps to wheel up, Button5 maps to wheel down. */
            if (event->xbutton.button == Button4) {
                *ticks = 1;
            }
            else if (event->xbutton.button == Button5) {
                *ticks = -1;
            }
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

/* Convert URI to local filename
   return filename if possible, else NULL
*/
static char* X11_URIToLocal(char* uri) {
    char *file = NULL;

    if (memcmp(uri,"file:/",6) == 0) uri += 6;      /* local file? */
    else if (strstr(uri,":/") != NULL) return file; /* wrong scheme */

    SDL_bool local = uri[0] != '/' || ( uri[0] != '\0' && uri[1] == '/' );

    /* got a hostname? */
    if ( !local && uri[0] == '/' && uri[2] != '/' ) {
      char* hostname_end = strchr( uri+1, '/' );
      if ( hostname_end != NULL ) {
          char hostname[ 257 ];
          if ( gethostname( hostname, 255 ) == 0 ) {
            hostname[ 256 ] = '\0';
            if ( memcmp( uri+1, hostname, hostname_end - ( uri+1 )) == 0 ) {
                uri = hostname_end + 1;
                local = SDL_TRUE;
            }
          }
      }
    }
    if ( local ) {
      file = uri;
      if ( uri[1] == '/' ) {
          file++;
      } else {
          file--;
      }
    }
    return file;
}

#if SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS
static void X11_HandleGenericEvent(SDL_VideoData *videodata,XEvent event)
{
    /* event is a union, so cookie == &event, but this is type safe. */
    XGenericEventCookie *cookie = &event.xcookie;
    if (XGetEventData(videodata->display, cookie)) {
        X11_HandleXinput2Event(videodata, cookie);
        XFreeEventData(videodata->display, cookie);
    }
}
#endif /* SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS */


static void
X11_DispatchFocusIn(SDL_WindowData *data)
{
#ifdef DEBUG_XEVENTS
    printf("window %p: Dispatching FocusIn\n", data);
#endif
    SDL_SetKeyboardFocus(data->window);
#ifdef X_HAVE_UTF8_STRING
    if (data->ic) {
        XSetICFocus(data->ic);
    }
#endif
}

static void
X11_DispatchFocusOut(SDL_WindowData *data)
{
#ifdef DEBUG_XEVENTS
    printf("window %p: Dispatching FocusOut\n", data);
#endif
    SDL_SetKeyboardFocus(NULL);
#ifdef X_HAVE_UTF8_STRING
    if (data->ic) {
        XUnsetICFocus(data->ic);
    }
#endif
}

static void
X11_DispatchMapNotify(SDL_WindowData *data)
{
    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_SHOWN, 0, 0);
    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_RESTORED, 0, 0);
}

static void
X11_DispatchUnmapNotify(SDL_WindowData *data)
{
    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_HIDDEN, 0, 0);
    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_MINIMIZED, 0, 0);
}

static void
X11_DispatchEvent(_THIS)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->driverdata;
    Display *display = videodata->display;
    SDL_WindowData *data;
    XEvent xevent;
    int i;

    SDL_zero(xevent);           /* valgrind fix. --ryan. */
    XNextEvent(display, &xevent);

    /* filter events catchs XIM events and sends them to the correct
       handler */
    if (XFilterEvent(&xevent, None) == True) {
#if 0
        printf("Filtered event type = %d display = %d window = %d\n",
               xevent.type, xevent.xany.display, xevent.xany.window);
#endif
        return;
    }

    /* Send a SDL_SYSWMEVENT if the application wants them */
    if (SDL_GetEventState(SDL_SYSWMEVENT) == SDL_ENABLE) {
        SDL_SysWMmsg wmmsg;

        SDL_VERSION(&wmmsg.version);
        wmmsg.subsystem = SDL_SYSWM_X11;
        wmmsg.msg.x11.event = xevent;
        SDL_SendSysWMEvent(&wmmsg);
    }

#if SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS
    if(xevent.type == GenericEvent) {
        X11_HandleGenericEvent(videodata,xevent);
        return;
    }
#endif

#if 0
    printf("type = %d display = %d window = %d\n",
           xevent.type, xevent.xany.display, xevent.xany.window);
#endif

    data = NULL;
    if (videodata && videodata->windowlist) {
        for (i = 0; i < videodata->numwindows; ++i) {
            if ((videodata->windowlist[i] != NULL) &&
                (videodata->windowlist[i]->xwindow == xevent.xany.window)) {
                data = videodata->windowlist[i];
                break;
            }
        }
    }
    if (!data) {
        return;
    }

    switch (xevent.type) {

        /* Gaining mouse coverage? */
    case EnterNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: EnterNotify! (%d,%d,%d)\n", data,
                   xevent.xcrossing.x,
                   xevent.xcrossing.y,
                   xevent.xcrossing.mode);
            if (xevent.xcrossing.mode == NotifyGrab)
                printf("Mode: NotifyGrab\n");
            if (xevent.xcrossing.mode == NotifyUngrab)
                printf("Mode: NotifyUngrab\n");
#endif
            SDL_SetMouseFocus(data->window);

            if (!SDL_GetMouse()->relative_mode) {
                SDL_SendMouseMotion(data->window, 0, 0, xevent.xcrossing.x, xevent.xcrossing.y);
            }
        }
        break;
        /* Losing mouse coverage? */
    case LeaveNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: LeaveNotify! (%d,%d,%d)\n", data,
                   xevent.xcrossing.x,
                   xevent.xcrossing.y,
                   xevent.xcrossing.mode);
            if (xevent.xcrossing.mode == NotifyGrab)
                printf("Mode: NotifyGrab\n");
            if (xevent.xcrossing.mode == NotifyUngrab)
                printf("Mode: NotifyUngrab\n");
#endif
            if (!SDL_GetMouse()->relative_mode) {
                SDL_SendMouseMotion(data->window, 0, 0, xevent.xcrossing.x, xevent.xcrossing.y);
            }

            if (xevent.xcrossing.mode != NotifyGrab &&
                xevent.xcrossing.mode != NotifyUngrab &&
                xevent.xcrossing.detail != NotifyInferior) {
                SDL_SetMouseFocus(NULL);
            }
        }
        break;

        /* Gaining input focus? */
    case FocusIn:{
            if (xevent.xfocus.detail == NotifyInferior) {
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusIn (NotifierInferior, ignoring)\n", data);
#endif
                break;
            }
#ifdef DEBUG_XEVENTS
            printf("window %p: FocusIn!\n", data);
#endif
            if (data->pending_focus == PENDING_FOCUS_OUT &&
                data->window == SDL_GetKeyboardFocus()) {
                /* We want to reset the keyboard here, because we may have
                   missed keyboard messages after our previous FocusOut.
                 */
                /* Actually, if we do this we clear the ALT key on Unity
                   because it briefly takes focus for their dashboard.

                   I think it's better to think the ALT key is held down
                   when it's not, then always lose the ALT modifier on Unity.
                 */
                /*SDL_ResetKeyboard();*/
            }
            data->pending_focus = PENDING_FOCUS_IN;
            data->pending_focus_time = SDL_GetTicks() + PENDING_FOCUS_IN_TIME;
        }
        break;

        /* Losing input focus? */
    case FocusOut:{
            if (xevent.xfocus.detail == NotifyInferior) {
                /* We still have focus if a child gets focus */
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusOut (NotifierInferior, ignoring)\n", data);
#endif
                break;
            }
#ifdef DEBUG_XEVENTS
            printf("window %p: FocusOut!\n", data);
#endif
            data->pending_focus = PENDING_FOCUS_OUT;
            data->pending_focus_time = SDL_GetTicks() + PENDING_FOCUS_OUT_TIME;
        }
        break;

        /* Generated upon EnterWindow and FocusIn */
    case KeymapNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: KeymapNotify!\n", data);
#endif
            /* FIXME:
               X11_SetKeyboardState(SDL_Display, xevent.xkeymap.key_vector);
             */
        }
        break;

        /* Has the keyboard layout changed? */
    case MappingNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: MappingNotify!\n", data);
#endif
            X11_UpdateKeymap(_this);
        }
        break;

        /* Key press? */
    case KeyPress:{
            KeyCode keycode = xevent.xkey.keycode;
            KeySym keysym = NoSymbol;
            char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
            Status status = 0;

#ifdef DEBUG_XEVENTS
            printf("window %p: KeyPress (X11 keycode = 0x%X)\n", data, xevent.xkey.keycode);
#endif
            SDL_SendKeyboardKey(SDL_PRESSED, videodata->key_layout[keycode]);
#if 1
            if (videodata->key_layout[keycode] == SDL_SCANCODE_UNKNOWN) {
                int min_keycode, max_keycode;
                XDisplayKeycodes(display, &min_keycode, &max_keycode);
#if SDL_VIDEO_DRIVER_X11_HAS_XKBKEYCODETOKEYSYM
                keysym = XkbKeycodeToKeysym(display, keycode, 0, 0);
#else
                keysym = XKeycodeToKeysym(display, keycode, 0);
#endif
                fprintf(stderr,
                        "The key you just pressed is not recognized by SDL. To help get this fixed, please report this to the SDL mailing list <sdl@libsdl.org> X11 KeyCode %d (%d), X11 KeySym 0x%lX (%s).\n",
                        keycode, keycode - min_keycode, keysym,
                        XKeysymToString(keysym));
            }
#endif
            /* */
            SDL_zero(text);
#ifdef X_HAVE_UTF8_STRING
            if (data->ic) {
                Xutf8LookupString(data->ic, &xevent.xkey, text, sizeof(text),
                                  &keysym, &status);
            }
#else
            XLookupString(&xevent.xkey, text, sizeof(text), &keysym, NULL);
#endif
            if (*text) {
                SDL_SendKeyboardText(text);
            }
        }
        break;

        /* Key release? */
    case KeyRelease:{
            KeyCode keycode = xevent.xkey.keycode;

#ifdef DEBUG_XEVENTS
            printf("window %p: KeyRelease (X11 keycode = 0x%X)\n", data, xevent.xkey.keycode);
#endif
            if (X11_KeyRepeat(display, &xevent)) {
                /* We're about to get a repeated key down, ignore the key up */
                break;
            }
            SDL_SendKeyboardKey(SDL_RELEASED, videodata->key_layout[keycode]);
        }
        break;

        /* Have we been iconified? */
    case UnmapNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: UnmapNotify!\n", data);
#endif
            X11_DispatchUnmapNotify(data);
        }
        break;

        /* Have we been restored? */
    case MapNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: MapNotify!\n", data);
#endif
            X11_DispatchMapNotify(data);
        }
        break;

        /* Have we been resized or moved? */
    case ConfigureNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: ConfigureNotify! (position: %d,%d, size: %dx%d)\n", data,
                   xevent.xconfigure.x, xevent.xconfigure.y,
                   xevent.xconfigure.width, xevent.xconfigure.height);
#endif
            if (xevent.xconfigure.x != data->last_xconfigure.x ||
                xevent.xconfigure.y != data->last_xconfigure.y) {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_MOVED,
                                    xevent.xconfigure.x, xevent.xconfigure.y);
            }
            if (xevent.xconfigure.width != data->last_xconfigure.width ||
                xevent.xconfigure.height != data->last_xconfigure.height) {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_RESIZED,
                                    xevent.xconfigure.width,
                                    xevent.xconfigure.height);
            }
            data->last_xconfigure = xevent.xconfigure;
        }
        break;

        /* Have we been requested to quit (or another client message?) */
    case ClientMessage:{

            int xdnd_version=0;

            if (xevent.xclient.message_type == videodata->XdndEnter) {
                SDL_bool use_list = xevent.xclient.data.l[1] & 1;
                data->xdnd_source = xevent.xclient.data.l[0];
                xdnd_version = ( xevent.xclient.data.l[1] >> 24);
                if (use_list) {
                    /* fetch conversion targets */
                    SDL_x11Prop p;
                    X11_ReadProperty(&p, display, data->xdnd_source, videodata->XdndTypeList);
                    /* pick one */
                    data->xdnd_req = X11_PickTarget(display, (Atom*)p.data, p.count);
                    XFree(p.data);
                } else {
                    /* pick from list of three */
                    data->xdnd_req = X11_PickTargetFromAtoms(display, xevent.xclient.data.l[2], xevent.xclient.data.l[3], xevent.xclient.data.l[4]);
                }
            }
            else if (xevent.xclient.message_type == videodata->XdndPosition) {

                /* reply with status */
                XClientMessageEvent m;
                memset(&m, 0, sizeof(XClientMessageEvent));
                m.type = ClientMessage;
                m.display = xevent.xclient.display;
                m.window = xevent.xclient.data.l[0];
                m.message_type = videodata->XdndStatus;
                m.format=32;
                m.data.l[0] = data->xwindow;
                m.data.l[1] = (data->xdnd_req != None);
                m.data.l[2] = 0; /* specify an empty rectangle */
                m.data.l[3] = 0;
                m.data.l[4] = videodata->XdndActionCopy; /* we only accept copying anyway */

                XSendEvent(display, xevent.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                XFlush(display);
            }
            else if(xevent.xclient.message_type == videodata->XdndDrop) {
                if (data->xdnd_req == None) {
                    /* say again - not interested! */
                    XClientMessageEvent m;
                    memset(&m, 0, sizeof(XClientMessageEvent));
                    m.type = ClientMessage;
                    m.display = xevent.xclient.display;
                    m.window = xevent.xclient.data.l[0];
                    m.message_type = videodata->XdndFinished;
                    m.format=32;
                    m.data.l[0] = data->xwindow;
                    m.data.l[1] = 0;
                    m.data.l[2] = None; /* fail! */
                    XSendEvent(display, xevent.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                } else {
                    /* convert */
                    if(xdnd_version >= 1) {
                        XConvertSelection(display, videodata->XdndSelection, data->xdnd_req, videodata->PRIMARY, data->xwindow, xevent.xclient.data.l[2]);
                    } else {
                        XConvertSelection(display, videodata->XdndSelection, data->xdnd_req, videodata->PRIMARY, data->xwindow, CurrentTime);
                    }
                }
            }
            else if ((xevent.xclient.message_type == videodata->WM_PROTOCOLS) &&
                (xevent.xclient.format == 32) &&
                (xevent.xclient.data.l[0] == videodata->_NET_WM_PING)) {
                Window root = DefaultRootWindow(display);

#ifdef DEBUG_XEVENTS
                printf("window %p: _NET_WM_PING\n", data);
#endif
                xevent.xclient.window = root;
                XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
                break;
            }

            else if ((xevent.xclient.message_type == videodata->WM_PROTOCOLS) &&
                (xevent.xclient.format == 32) &&
                (xevent.xclient.data.l[0] == videodata->WM_DELETE_WINDOW)) {

#ifdef DEBUG_XEVENTS
                printf("window %p: WM_DELETE_WINDOW\n", data);
#endif
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_CLOSE, 0, 0);
                break;
            }
        }
        break;

        /* Do we need to refresh ourselves? */
    case Expose:{
#ifdef DEBUG_XEVENTS
            printf("window %p: Expose (count = %d)\n", data, xevent.xexpose.count);
#endif
            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_EXPOSED, 0, 0);
        }
        break;

    case MotionNotify:{
            SDL_Mouse *mouse = SDL_GetMouse();
            if(!mouse->relative_mode) {
#ifdef DEBUG_MOTION
                printf("window %p: X11 motion: %d,%d\n", xevent.xmotion.x, xevent.xmotion.y);
#endif

                SDL_SendMouseMotion(data->window, 0, 0, xevent.xmotion.x, xevent.xmotion.y);
            }
        }
        break;

    case ButtonPress:{
            int ticks = 0;
            if (X11_IsWheelEvent(display,&xevent,&ticks)) {
                SDL_SendMouseWheel(data->window, 0, 0, ticks);
            } else {
                SDL_SendMouseButton(data->window, 0, SDL_PRESSED, xevent.xbutton.button);
            }
        }
        break;

    case ButtonRelease:{
            SDL_SendMouseButton(data->window, 0, SDL_RELEASED, xevent.xbutton.button);
        }
        break;

    case PropertyNotify:{
#ifdef DEBUG_XEVENTS
            unsigned char *propdata;
            int status, real_format;
            Atom real_type;
            unsigned long items_read, items_left, i;

            char *name = XGetAtomName(display, xevent.xproperty.atom);
            if (name) {
                printf("window %p: PropertyNotify: %s %s\n", data, name, (xevent.xproperty.state == PropertyDelete) ? "deleted" : "changed");
                XFree(name);
            }

            status = XGetWindowProperty(display, data->xwindow, xevent.xproperty.atom, 0L, 8192L, False, AnyPropertyType, &real_type, &real_format, &items_read, &items_left, &propdata);
            if (status == Success && items_read > 0) {
                if (real_type == XA_INTEGER) {
                    int *values = (int *)propdata;

                    printf("{");
                    for (i = 0; i < items_read; i++) {
                        printf(" %d", values[i]);
                    }
                    printf(" }\n");
                } else if (real_type == XA_CARDINAL) {
                    if (real_format == 32) {
                        Uint32 *values = (Uint32 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    } else if (real_format == 16) {
                        Uint16 *values = (Uint16 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    } else if (real_format == 8) {
                        Uint8 *values = (Uint8 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    }
                } else if (real_type == XA_STRING ||
                           real_type == videodata->UTF8_STRING) {
                    printf("{ \"%s\" }\n", propdata);
                } else if (real_type == XA_ATOM) {
                    Atom *atoms = (Atom *)propdata;

                    printf("{");
                    for (i = 0; i < items_read; i++) {
                        char *name = XGetAtomName(display, atoms[i]);
                        if (name) {
                            printf(" %s", name);
                            XFree(name);
                        }
                    }
                    printf(" }\n");
                } else {
                    char *name = XGetAtomName(display, real_type);
                    printf("Unknown type: %ld (%s)\n", real_type, name ? name : "UNKNOWN");
                    if (name) {
                        XFree(name);
                    }
                }
            }
            if (status == Success) {
                XFree(propdata);
            }
#endif /* DEBUG_XEVENTS */

            if (xevent.xproperty.atom == data->videodata->_NET_WM_STATE) {
                /* Get the new state from the window manager.
                   Compositing window managers can alter visibility of windows
                   without ever mapping / unmapping them, so we handle that here,
                   because they use the NETWM protocol to notify us of changes.
                 */
                Uint32 flags = X11_GetNetWMState(_this, xevent.xproperty.window);
                if ((flags^data->window->flags) & SDL_WINDOW_HIDDEN) {
                    if (flags & SDL_WINDOW_HIDDEN) {
                        X11_DispatchUnmapNotify(data);
                    } else {
                        X11_DispatchMapNotify(data);
                    }
                }
            }
        }
        break;

    /* Copy the selection from XA_CUT_BUFFER0 to the requested property */
    case SelectionRequest: {
            XSelectionRequestEvent *req;
            XEvent sevent;
            int seln_format;
            unsigned long nbytes;
            unsigned long overflow;
            unsigned char *seln_data;

            req = &xevent.xselectionrequest;
#ifdef DEBUG_XEVENTS
            printf("window %p: SelectionRequest (requestor = %ld, target = %ld)\n", data,
                req->requestor, req->target);
#endif

            SDL_zero(sevent);
            sevent.xany.type = SelectionNotify;
            sevent.xselection.selection = req->selection;
            sevent.xselection.target = None;
            sevent.xselection.property = None;
            sevent.xselection.requestor = req->requestor;
            sevent.xselection.time = req->time;
            if (XGetWindowProperty(display, DefaultRootWindow(display),
                    XA_CUT_BUFFER0, 0, INT_MAX/4, False, req->target,
                    &sevent.xselection.target, &seln_format, &nbytes,
                    &overflow, &seln_data) == Success) {
                Atom XA_TARGETS = XInternAtom(display, "TARGETS", 0);
                if (sevent.xselection.target == req->target) {
                    XChangeProperty(display, req->requestor, req->property,
                        sevent.xselection.target, seln_format, PropModeReplace,
                        seln_data, nbytes);
                    sevent.xselection.property = req->property;
                } else if (XA_TARGETS == req->target) {
                    Atom SupportedFormats[] = { sevent.xselection.target, XA_TARGETS };
                    XChangeProperty(display, req->requestor, req->property,
                        XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)SupportedFormats,
                        sizeof(SupportedFormats)/sizeof(*SupportedFormats));
                    sevent.xselection.property = req->property;
                }
                XFree(seln_data);
            }
            XSendEvent(display, req->requestor, False, 0, &sevent);
            XSync(display, False);
        }
        break;

    case SelectionNotify: {
#ifdef DEBUG_XEVENTS
            printf("window %p: SelectionNotify (requestor = %ld, target = %ld)\n", data,
                xevent.xselection.requestor, xevent.xselection.target);
#endif
            Atom target = xevent.xselection.target;
            if (target == data->xdnd_req) {
                /* read data */
                SDL_x11Prop p;
                X11_ReadProperty(&p, display, data->xwindow, videodata->PRIMARY);

                if (p.format == 8) {
                    SDL_bool expect_lf = SDL_FALSE;
                    char *start = NULL;
                    char *scan = (char*)p.data;
                    char *fn;
                    char *uri;
                    int length = 0;
                    while (p.count--) {
                        if (!expect_lf) {
                            if (*scan == 0x0D) {
                                expect_lf = SDL_TRUE;
                            }
                            if (start == NULL) {
                                start = scan;
                                length = 0;
                            }
                            length++;
                        } else {
                            if (*scan == 0x0A && length > 0) {
                                uri = SDL_malloc(length--);
                                SDL_memcpy(uri, start, length);
                                uri[length] = '\0';
                                fn = X11_URIToLocal(uri);
                                if (fn) {
                                    SDL_SendDropFile(fn);
                                }
                                SDL_free(uri);
                            }
                            expect_lf = SDL_FALSE;
                            start = NULL;
                        }
                        scan++;
                    }
                }

                XFree(p.data);

                /* send reply */
                XClientMessageEvent m;
                SDL_memset(&m, 0, sizeof(XClientMessageEvent));
                m.type = ClientMessage;
                m.display = display;
                m.window = data->xdnd_source;
                m.message_type = videodata->XdndFinished;
                m.format = 32;
                m.data.l[0] = data->xwindow;
                m.data.l[1] = 1;
                m.data.l[2] = videodata->XdndActionCopy;
                XSendEvent(display, data->xdnd_source, False, NoEventMask, (XEvent*)&m);

                XSync(display, False);

            } else {
                videodata->selection_waiting = SDL_FALSE;
            }
        }
        break;

    default:{
#ifdef DEBUG_XEVENTS
            printf("window %p: Unhandled event %d\n", data, xevent.type);
#endif
        }
        break;
    }
}

static void
X11_HandleFocusChanges(_THIS)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->driverdata;
    int i;

    if (videodata && videodata->windowlist) {
        for (i = 0; i < videodata->numwindows; ++i) {
            SDL_WindowData *data = videodata->windowlist[i];
            if (data && data->pending_focus != PENDING_FOCUS_NONE) {
                Uint32 now = SDL_GetTicks();
                if ( (int)(data->pending_focus_time-now) <= 0 ) {
                    if ( data->pending_focus == PENDING_FOCUS_IN ) {
                        X11_DispatchFocusIn(data);
                    } else {
                        X11_DispatchFocusOut(data);
                    }
                    data->pending_focus = PENDING_FOCUS_NONE;
                }
            }
        }
    }
}
/* Ack!  XPending() actually performs a blocking read if no events available */
static int
X11_Pending(Display * display)
{
    /* Flush the display connection and look to see if events are queued */
    XFlush(display);
    if (XEventsQueued(display, QueuedAlready)) {
        return (1);
    }

    /* More drastic measures are required -- see if X is ready to talk */
    {
        static struct timeval zero_time;        /* static == 0 */
        int x11_fd;
        fd_set fdset;

        x11_fd = ConnectionNumber(display);
        FD_ZERO(&fdset);
        FD_SET(x11_fd, &fdset);
        if (select(x11_fd + 1, &fdset, NULL, NULL, &zero_time) == 1) {
            return (XPending(display));
        }
    }

    /* Oh well, nothing is ready .. */
    return (0);
}


/* !!! FIXME: this should be exposed in a header, or something. */
int SDL_GetNumTouch(void);
void SDL_dbus_screensaver_tickle(_THIS);

void
X11_PumpEvents(_THIS)
{
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

    /* Update activity every 30 seconds to prevent screensaver */
    if (_this->suspend_screensaver) {
        Uint32 now = SDL_GetTicks();
        if (!data->screensaver_activity ||
            (int) (now - data->screensaver_activity) >= 30000) {
            XResetScreenSaver(data->display);

            #if SDL_USE_LIBDBUS
            SDL_dbus_screensaver_tickle(_this);
            #endif

            data->screensaver_activity = now;
        }
    }

    /* Keep processing pending events */
    while (X11_Pending(data->display)) {
        X11_DispatchEvent(_this);
    }

    /* FIXME: Only need to do this when there are pending focus changes */
    X11_HandleFocusChanges(_this);
}


void
X11_SuspendScreenSaver(_THIS)
{
#if SDL_VIDEO_DRIVER_X11_XSCRNSAVER
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;
    int dummy;
    int major_version, minor_version;

    if (SDL_X11_HAVE_XSS) {
        /* XScreenSaverSuspend was introduced in MIT-SCREEN-SAVER 1.1 */
        if (!XScreenSaverQueryExtension(data->display, &dummy, &dummy) ||
            !XScreenSaverQueryVersion(data->display,
                                      &major_version, &minor_version) ||
            major_version < 1 || (major_version == 1 && minor_version < 1)) {
            return;
        }

        XScreenSaverSuspend(data->display, _this->suspend_screensaver);
        XResetScreenSaver(data->display);
    }
#endif

#if SDL_USE_LIBDBUS
    if (_this->suspend_screensaver) {
        SDL_dbus_screensaver_tickle(_this);
    }
#endif
}

#endif /* SDL_VIDEO_DRIVER_X11 */

/* vi: set ts=4 sw=4 expandtab: */
