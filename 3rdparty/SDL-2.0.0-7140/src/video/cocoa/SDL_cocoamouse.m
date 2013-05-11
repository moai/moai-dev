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

#if SDL_VIDEO_DRIVER_COCOA

#include "SDL_assert.h"
#include "SDL_events.h"
#include "SDL_cocoamouse.h"

#include "../../events/SDL_mouse_c.h"


static SDL_Cursor *
Cocoa_CreateDefaultCursor()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSCursor *nscursor;
    SDL_Cursor *cursor = NULL;

    nscursor = [NSCursor arrowCursor];

    if (nscursor) {
        cursor = SDL_calloc(1, sizeof(*cursor));
        if (cursor) {
            cursor->driverdata = nscursor;
            [nscursor retain];
        }
    }

    [pool release];

    return cursor;
}

static SDL_Cursor *
Cocoa_CreateCursor(SDL_Surface * surface, int hot_x, int hot_y)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSImage *nsimage;
    NSCursor *nscursor = NULL;
    SDL_Cursor *cursor = NULL;

    nsimage = Cocoa_CreateImage(surface);
    if (nsimage) {
        nscursor = [[NSCursor alloc] initWithImage: nsimage hotSpot: NSMakePoint(hot_x, hot_y)];
    }

    if (nscursor) {
        cursor = SDL_calloc(1, sizeof(*cursor));
        if (cursor) {
            cursor->driverdata = nscursor;
        }
    }

    [pool release];

    return cursor;
}

static SDL_Cursor *
Cocoa_CreateSystemCursor(SDL_SystemCursor id)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSCursor *nscursor = NULL;
    SDL_Cursor *cursor = NULL;

    switch(id)
    {
    case SDL_SYSTEM_CURSOR_ARROW:
        nscursor = [NSCursor arrowCursor];
        break;
    case SDL_SYSTEM_CURSOR_IBEAM:
        nscursor = [NSCursor IBeamCursor];
        break;
    case SDL_SYSTEM_CURSOR_WAIT:
        nscursor = [NSCursor arrowCursor];
        break;
    case SDL_SYSTEM_CURSOR_CROSSHAIR:
        nscursor = [NSCursor crosshairCursor];
        break;
    case SDL_SYSTEM_CURSOR_WAITARROW:
        nscursor = [NSCursor arrowCursor];
        break;
    case SDL_SYSTEM_CURSOR_SIZENWSE:
    case SDL_SYSTEM_CURSOR_SIZENESW:
        nscursor = [NSCursor closedHandCursor];
        break;
    case SDL_SYSTEM_CURSOR_SIZEWE:
        nscursor = [NSCursor resizeLeftRightCursor];
        break;
    case SDL_SYSTEM_CURSOR_SIZENS:
        nscursor = [NSCursor resizeUpDownCursor];
        break;
    case SDL_SYSTEM_CURSOR_SIZEALL:
        nscursor = [NSCursor closedHandCursor];
        break;
    case SDL_SYSTEM_CURSOR_NO:
        nscursor = [NSCursor operationNotAllowedCursor];
        break;
    case SDL_SYSTEM_CURSOR_HAND:
        nscursor = [NSCursor pointingHandCursor];
        break;
    default:
        SDL_assert(!"Unknown system cursor");
        return NULL;
    }

    if (nscursor) {
        cursor = SDL_calloc(1, sizeof(*cursor));
        if (cursor) {
            // We'll free it later, so retain it here
            [nscursor retain];
            cursor->driverdata = nscursor;
        }
    }

    [pool release];

    return cursor;
}

static void
Cocoa_FreeCursor(SDL_Cursor * cursor)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSCursor *nscursor = (NSCursor *)cursor->driverdata;

    [nscursor release];
    SDL_free(cursor);

    [pool release];
}

static int
Cocoa_ShowCursor(SDL_Cursor * cursor)
{
	/* We need to track the previous state because hide and unhide calls need to
	 * be matched, but ShowCursor calls don't.
	 */
	static SDL_bool isShown = SDL_TRUE;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    if (cursor) {
        NSCursor *nscursor = (NSCursor *)cursor->driverdata;

        /* We're possibly executing from an event handler where this operation
         * is unsupported. This will execute it in the main Cocoa event loop
         * after this returns.
         */
        [nscursor performSelectorOnMainThread:@selector(set)
                                   withObject:nil
                                waitUntilDone:NO];

		if (!isShown) {
			[NSCursor unhide];
			isShown = SDL_TRUE;
		}
	} else if (isShown) {
		[NSCursor hide];
		isShown = SDL_FALSE;
    }

    [pool release];

    return 0;
}

static void
Cocoa_WarpMouse(SDL_Window * window, int x, int y)
{
    SDL_Mouse *mouse = SDL_GetMouse();
    CGPoint point;

    point.x = (float)window->x + x;
    point.y = (float)window->y + y;

    {
        /* This makes Cocoa_HandleMouseEvent ignore this delta in the next
         * movement event.
         */
        SDL_MouseData *driverdata = (SDL_MouseData*)mouse->driverdata;
        NSPoint location =  [NSEvent mouseLocation];
        driverdata->deltaXOffset = location.x - point.x;
        driverdata->deltaYOffset = point.y - location.y;
    }

    /* According to the docs, this was deprecated in 10.6, but it's still
     * around. The substitute requires a CGEventSource, but I'm not entirely
     * sure how we'd procure the right one for this event.
     */
    CGSetLocalEventsSuppressionInterval(0.0);
    CGWarpMouseCursorPosition(point);
    CGSetLocalEventsSuppressionInterval(0.25);

    /* CGWarpMouseCursorPosition doesn't generate a window event, unlike our
     * other implementations' APIs.
     */
    SDL_SendMouseMotion(mouse->focus, mouse->mouseID, 0, x, y);
}

static int
Cocoa_SetRelativeMouseMode(SDL_bool enabled)
{
    CGError result;

    if (enabled) {
        result = CGAssociateMouseAndMouseCursorPosition(NO);
    } else {
        result = CGAssociateMouseAndMouseCursorPosition(YES);
    }
    if (result != kCGErrorSuccess) {
        return SDL_SetError("CGAssociateMouseAndMouseCursorPosition() failed");
    }
    return 0;
}

void
Cocoa_InitMouse(_THIS)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    mouse->driverdata = SDL_calloc(1, sizeof(SDL_MouseData));

    mouse->CreateCursor = Cocoa_CreateCursor;
    mouse->CreateSystemCursor = Cocoa_CreateSystemCursor;
    mouse->ShowCursor = Cocoa_ShowCursor;
    mouse->FreeCursor = Cocoa_FreeCursor;
    mouse->WarpMouse = Cocoa_WarpMouse;
    mouse->SetRelativeMouseMode = Cocoa_SetRelativeMouseMode;

    SDL_SetDefaultCursor(Cocoa_CreateDefaultCursor());
}

void
Cocoa_HandleMouseEvent(_THIS, NSEvent *event)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    if (mouse->relative_mode &&
        ([event type] == NSMouseMoved ||
         [event type] == NSLeftMouseDragged ||
         [event type] == NSRightMouseDragged ||
         [event type] == NSOtherMouseDragged)) {
        SDL_MouseData *driverdata = (SDL_MouseData*)mouse->driverdata;
        float x = [event deltaX] + driverdata->deltaXOffset;
        float y = [event deltaY] + driverdata->deltaYOffset;
        driverdata->deltaXOffset = driverdata->deltaYOffset = 0;

        SDL_SendMouseMotion(mouse->focus, mouse->mouseID, 1, (int)x, (int)y);
    }
}

void
Cocoa_HandleMouseWheel(SDL_Window *window, NSEvent *event)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    float x = [event deltaX];
    float y = [event deltaY];

    if (x > 0) {
        x += 0.9f;
    } else if (x < 0) {
        x -= 0.9f;
    }
    if (y > 0) {
        y += 0.9f;
    } else if (y < 0) {
        y -= 0.9f;
    }
    SDL_SendMouseWheel(window, mouse->mouseID, (int)x, (int)y);
}

void
Cocoa_QuitMouse(_THIS)
{
    SDL_Mouse *mouse = SDL_GetMouse();
    if (mouse) {
        SDL_free(mouse->driverdata);
    }
}

#endif /* SDL_VIDEO_DRIVER_COCOA */

/* vi: set ts=4 sw=4 expandtab: */
