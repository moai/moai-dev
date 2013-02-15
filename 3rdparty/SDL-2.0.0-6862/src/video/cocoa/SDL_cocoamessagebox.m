/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

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

#if SDL_VIDEO_DRIVER_COCOA

#if defined(__APPLE__) && defined(__POWERPC__)
#include <altivec.h>
#undef bool
#undef vector
#undef pixel
#endif

#include "SDL_messagebox.h"
#include "SDL_cocoavideo.h"


/* Display a Cocoa message box */
int
Cocoa_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
    Cocoa_RegisterApp();

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSAlert* alert = [[NSAlert alloc] init];

    if (messageboxdata->flags & SDL_MESSAGEBOX_ERROR) {
        [alert setAlertStyle:NSCriticalAlertStyle];
    } else if (messageboxdata->flags & SDL_MESSAGEBOX_WARNING) {
        [alert setAlertStyle:NSWarningAlertStyle];
    } else {
        [alert setAlertStyle:NSInformationalAlertStyle];
    }

    [alert setMessageText:[NSString stringWithUTF8String:messageboxdata->title]];
    [alert setInformativeText:[NSString stringWithUTF8String:messageboxdata->message]];

    const SDL_MessageBoxButtonData *buttons = messageboxdata->buttons;
    int i;
    for (i = 0; i < messageboxdata->numbuttons; ++i) {
        NSButton *button = [alert addButtonWithTitle:[NSString stringWithUTF8String:buttons[i].text]];
        if (buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT) {
            [button setKeyEquivalent:@"\r"];
        } else if (buttons[i].flags & SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT) {
            [button setKeyEquivalent:@"\033"];
        } else {
            [button setKeyEquivalent:@""];
        }
    }

    NSInteger clicked = [alert runModal];
    clicked -= NSAlertFirstButtonReturn;
    *buttonid = buttons[clicked].buttonid;
    [alert release];

    [pool release];

    return 0;
}

#endif /* SDL_VIDEO_DRIVER_COCOA */

/* vi: set ts=4 sw=4 expandtab: */
