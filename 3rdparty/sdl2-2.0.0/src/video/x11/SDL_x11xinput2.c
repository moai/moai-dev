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

#include "SDL_x11video.h"
#include "SDL_x11xinput2.h"
#include "../../events/SDL_mouse_c.h"
#include "../../events/SDL_touch_c.h"

#define MAX_AXIS 16

#if SDL_VIDEO_DRIVER_X11_XINPUT2
static int xinput2_initialized = 0;

#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
static int xinput2_multitouch_supported = 0;
#endif

/* Opcode returned XQueryExtension
 * It will be used in event processing
 * to know that the event came from
 * this extension */
static int xinput2_opcode;

static void parse_valuators(const double *input_values,unsigned char *mask,int mask_len,
                            double *output_values,int output_values_len) {
    int i = 0,z = 0;
    int top = mask_len * 8;
    if (top > MAX_AXIS)
        top = MAX_AXIS;

    SDL_memset(output_values,0,output_values_len * sizeof(double));
    for (; i < top && z < output_values_len; i++) {
        if (XIMaskIsSet(mask, i)) {
            const int value = (int) *input_values;
            output_values[z] = value;
            input_values++;
        }
        z++;
    }
}
#endif /* SDL_VIDEO_DRIVER_X11_XINPUT2 */

void
X11_InitXinput2(_THIS)
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

    XIEventMask eventmask;
    unsigned char mask[3] = { 0,0,0 };
    int event, err;
    int major = 2, minor = 0;
    int outmajor,outminor;
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
    minor = 2;
#endif
    /*
    * Initialize XInput 2
    * According to http://who-t.blogspot.com/2009/05/xi2-recipes-part-1.html its better
    * to inform Xserver what version of Xinput we support.The server will store the version we support.
    * "As XI2 progresses it becomes important that you use this call as the server may treat the client
    * differently depending on the supported version".
    *
    * FIXME:event and err are not needed but if not passed XQueryExtension returns SegmentationFault
    */
    if (!SDL_X11_HAVE_XINPUT2 ||
        !XQueryExtension(data->display, "XInputExtension", &xinput2_opcode, &event, &err)) {
        return;
    }

    outmajor = major;
    outminor = minor;
    if (XIQueryVersion(data->display, &outmajor, &outminor) != Success) {
        return;
    }

    /*Check supported version*/
    if(outmajor * 1000 + outminor < major * 1000 + minor) {
        /*X server does not support the version we want*/
        return;
    }
    xinput2_initialized = 1;
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
    /*XInput 2.2*/
    if(outmajor * 1000 + outminor >= major * 1000 + minor) {
        xinput2_multitouch_supported = 1;
    }
#endif

    /*Enable  Raw motion events for this display*/
    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = sizeof(mask);
    eventmask.mask = mask;

    XISetMask(mask, XI_RawMotion);

    if (XISelectEvents(data->display,DefaultRootWindow(data->display),&eventmask,1) != Success) {
        return;
    }
#endif
}

int
X11_HandleXinput2Event(SDL_VideoData *videodata,XGenericEventCookie *cookie)
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2
    if(cookie->extension != xinput2_opcode) {
        return 0;
    }
    switch(cookie->evtype) {
        case XI_RawMotion: {
            const XIRawEvent *rawev = (const XIRawEvent*)cookie->data;
            SDL_Mouse *mouse = SDL_GetMouse();
            double relative_cords[2];

            if (!mouse->relative_mode) {
                return 0;
            }

            parse_valuators(rawev->raw_values,rawev->valuators.mask,
                            rawev->valuators.mask_len,relative_cords,2);
            SDL_SendMouseMotion(mouse->focus,mouse->mouseID,1,(int)relative_cords[0],(int)relative_cords[1]);
            return 1;
            }
            break;
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
        case XI_TouchBegin: {
            const XIDeviceEvent *xev = (const XIDeviceEvent *) cookie->data;
            SDL_SendTouch(xev->sourceid,xev->detail,
                      SDL_TRUE, xev->event_x, xev->event_y, 1.0);
            return 1;
            }
            break;
        case XI_TouchEnd: {
            const XIDeviceEvent *xev = (const XIDeviceEvent *) cookie->data;
            SDL_SendTouch(xev->sourceid,xev->detail,
                      SDL_FALSE, xev->event_x, xev->event_y, 1.0);
            return 1;
            }
            break;
        case XI_TouchUpdate: {
            const XIDeviceEvent *xev = (const XIDeviceEvent *) cookie->data;
            SDL_SendTouchMotion(xev->sourceid,xev->detail,
                                xev->event_x, xev->event_y, 1.0);
            return 1;
            }
            break;
#endif
    }
#endif
    return 0;
}

void
X11_InitXinput2Multitouch(_THIS)
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;
    XIDeviceInfo *info;
    int ndevices,i,j;
    info = XIQueryDevice(data->display, XIAllMasterDevices, &ndevices);

    for (i = 0; i < ndevices; i++) {
        XIDeviceInfo *dev = &info[i];
        for (j = 0; j < dev->num_classes; j++) {
            SDL_TouchID touchId;
            XIAnyClassInfo *class = dev->classes[j];
            XITouchClassInfo *t = (XITouchClassInfo*)class;

            /*Only touch devices*/
            if (class->type != XITouchClass)
                continue;

            touchId = t->sourceid;
            if (!SDL_GetTouch(touchId)) {
                SDL_AddTouch(touchId, dev->name);
            }
        }
    }
    XIFreeDeviceInfo(info);
#endif
}

void
X11_Xinput2SelectTouch(_THIS, SDL_Window *window)
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
    if (!X11_Xinput2IsMultitouchSupported()) {
        return;
    }

    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;
    XIEventMask eventmask;
    unsigned char mask[3] = { 0,0,0 };
    SDL_WindowData *window_data = (SDL_WindowData*)window->driverdata;

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = sizeof(mask);
    eventmask.mask = mask;

    XISetMask(mask, XI_TouchBegin);
    XISetMask(mask, XI_TouchUpdate);
    XISetMask(mask, XI_TouchEnd);

    XISelectEvents(data->display,window_data->xwindow,&eventmask,1);
#endif
}


int
X11_Xinput2IsInitialized()
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2
    return xinput2_initialized;
#else
    return 0;
#endif
}

int
X11_Xinput2IsMultitouchSupported()
{
#if SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH
    return xinput2_initialized && xinput2_multitouch_supported;
#else
    return 0;
#endif
}

#endif /* SDL_VIDEO_DRIVER_X11 */

/* vi: set ts=4 sw=4 expandtab: */
