/*
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* Simple program to test the SDL joystick routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

#ifndef SDL_JOYSTICK_DISABLED

#ifdef __IPHONEOS__
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   480
#else
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#endif


static void
DrawRect(SDL_Renderer *r, const int x, const int y, const int w, const int h)
{
    const SDL_Rect area = { x, y, w, h };
    SDL_RenderFillRect(r, &area);
}

static SDL_bool
WatchJoystick(SDL_Joystick * joystick)
{
    SDL_Window *window = NULL;
    SDL_Renderer *screen = NULL;
    const char *name = NULL;
    SDL_bool retval = SDL_FALSE;
    SDL_bool done = SDL_FALSE;
    SDL_Event event;
    int i;

    /* Create a window to display joystick axis position */
    window = SDL_CreateWindow("Joystick Test", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, 0);
    if (window == NULL) {
        fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    screen = SDL_CreateRenderer(window, -1, 0);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return SDL_FALSE;
    }

    SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(screen);
    SDL_RenderPresent(screen);
    SDL_RaiseWindow(window);

    /* Print info about the joystick we are watching */
    name = SDL_JoystickName(joystick);
    printf("Watching joystick %d: (%s)\n", SDL_JoystickInstanceID(joystick),
           name ? name : "Unknown Joystick");
    printf("Joystick has %d axes, %d hats, %d balls, and %d buttons\n",
           SDL_JoystickNumAxes(joystick), SDL_JoystickNumHats(joystick),
           SDL_JoystickNumBalls(joystick), SDL_JoystickNumButtons(joystick));

    /* Loop, getting joystick events! */
    while (!done) {
        /* blank screen, set up for drawing this frame. */
        SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(screen);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_JOYAXISMOTION:
                printf("Joystick %d axis %d value: %d\n",
                       event.jaxis.which,
                       event.jaxis.axis, event.jaxis.value);
                break;
            case SDL_JOYHATMOTION:
                printf("Joystick %d hat %d value:",
                       event.jhat.which, event.jhat.hat);
                if (event.jhat.value == SDL_HAT_CENTERED)
                    printf(" centered");
                if (event.jhat.value & SDL_HAT_UP)
                    printf(" up");
                if (event.jhat.value & SDL_HAT_RIGHT)
                    printf(" right");
                if (event.jhat.value & SDL_HAT_DOWN)
                    printf(" down");
                if (event.jhat.value & SDL_HAT_LEFT)
                    printf(" left");
                printf("\n");
                break;
            case SDL_JOYBALLMOTION:
                printf("Joystick %d ball %d delta: (%d,%d)\n",
                       event.jball.which,
                       event.jball.ball, event.jball.xrel, event.jball.yrel);
                break;
            case SDL_JOYBUTTONDOWN:
                printf("Joystick %d button %d down\n",
                       event.jbutton.which, event.jbutton.button);
                break;
            case SDL_JOYBUTTONUP:
                printf("Joystick %d button %d up\n",
                       event.jbutton.which, event.jbutton.button);
                break;
            case SDL_KEYDOWN:
                if ((event.key.keysym.sym != SDLK_ESCAPE) &&
                    (event.key.keysym.sym != SDLK_AC_BACK)) {
                    break;
                }
                /* Fall through to signal quit */
            case SDL_FINGERDOWN:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_QUIT:
                done = SDL_TRUE;
                break;
            default:
                break;
            }
        }
        /* Update visual joystick state */
        SDL_SetRenderDrawColor(screen, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        for (i = 0; i < SDL_JoystickNumButtons(joystick); ++i) {
            if (SDL_JoystickGetButton(joystick, i) == SDL_PRESSED) {
                DrawRect(screen, (i%20) * 34, SCREEN_HEIGHT - 68 + (i/20) * 34, 32, 32);
            }
        }

        SDL_SetRenderDrawColor(screen, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i = 0; i < SDL_JoystickNumAxes(joystick); ++i) {
            /* Draw the X/Y axis */
            int x, y;
            x = (((int) SDL_JoystickGetAxis(joystick, i)) + 32768);
            x *= SCREEN_WIDTH;
            x /= 65535;
            if (x < 0) {
                x = 0;
            } else if (x > (SCREEN_WIDTH - 16)) {
                x = SCREEN_WIDTH - 16;
            }
            ++i;
            if (i < SDL_JoystickNumAxes(joystick)) {
                y = (((int) SDL_JoystickGetAxis(joystick, i)) + 32768);
            } else {
                y = 32768;
            }
            y *= SCREEN_HEIGHT;
            y /= 65535;
            if (y < 0) {
                y = 0;
            } else if (y > (SCREEN_HEIGHT - 16)) {
                y = SCREEN_HEIGHT - 16;
            }

            DrawRect(screen, x, y, 16, 16);
        }

        SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
        for (i = 0; i < SDL_JoystickNumHats(joystick); ++i) {
            /* Derive the new position */
            int x = SCREEN_WIDTH/2;
            int y = SCREEN_HEIGHT/2;
            const Uint8 hat_pos = SDL_JoystickGetHat(joystick, i);

            if (hat_pos & SDL_HAT_UP) {
                y = 0;
            } else if (hat_pos & SDL_HAT_DOWN) {
                y = SCREEN_HEIGHT-8;
            }

            if (hat_pos & SDL_HAT_LEFT) {
                x = 0;
            } else if (hat_pos & SDL_HAT_RIGHT) {
                x = SCREEN_WIDTH-8;
            }

            DrawRect(screen, x, y, 8, 8);
        }

        SDL_RenderPresent(screen);

        if (SDL_JoystickGetAttached( joystick ) == 0) {
            done = SDL_TRUE;
            retval = SDL_TRUE;  /* keep going, wait for reattach. */
        }
    }

    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    return retval;
}

int
main(int argc, char *argv[])
{
    const char *name;
    int i;
    SDL_Joystick *joystick;

    /* Initialize SDL (Note: video is required to start event loop) */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Print information about the joysticks */
    printf("There are %d joysticks attached\n", SDL_NumJoysticks());
    for (i = 0; i < SDL_NumJoysticks(); ++i) {
        name = SDL_JoystickNameForIndex(i);
        printf("Joystick %d: %s\n", i, name ? name : "Unknown Joystick");
        joystick = SDL_JoystickOpen(i);
        if (joystick == NULL) {
            fprintf(stderr, "SDL_JoystickOpen(%d) failed: %s\n", i,
                    SDL_GetError());
        } else {
            char guid[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick),
                                      guid, sizeof (guid));
            printf("       axes: %d\n", SDL_JoystickNumAxes(joystick));
            printf("      balls: %d\n", SDL_JoystickNumBalls(joystick));
            printf("       hats: %d\n", SDL_JoystickNumHats(joystick));
            printf("    buttons: %d\n", SDL_JoystickNumButtons(joystick));
            printf("instance id: %d\n", SDL_JoystickInstanceID(joystick));
            printf("       guid: %s\n", guid);
            SDL_JoystickClose(joystick);
        }
    }

#ifdef ANDROID
    if (SDL_NumJoysticks() > 0) {
#else
    if (argv[1]) {
#endif
        SDL_bool reportederror = SDL_FALSE;
        SDL_bool keepGoing = SDL_TRUE;
        SDL_Event event;
#ifdef ANDROID
        joystick = SDL_JoystickOpen(0);
#else
        joystick = SDL_JoystickOpen(atoi(argv[1]));
#endif
        while ( keepGoing ) {
            if (joystick == NULL) {
                if ( !reportederror ) {
                    printf("Couldn't open joystick %d: %s\n", atoi(argv[1]), SDL_GetError());
                    keepGoing = SDL_FALSE;
                    reportederror = SDL_TRUE;
                }
            } else {
                reportederror = SDL_FALSE;
                keepGoing = WatchJoystick(joystick);
                SDL_JoystickClose(joystick);
            }

            joystick = NULL;
            if (keepGoing) {
                printf("Waiting for attach\n");
            }
            while (keepGoing) {
                SDL_WaitEvent(&event);
                if ((event.type == SDL_QUIT) || (event.type == SDL_FINGERDOWN)
                    || (event.type == SDL_MOUSEBUTTONDOWN)) {
                    keepGoing = SDL_FALSE;
                } else if (event.type == SDL_JOYDEVICEADDED) {
                    joystick = SDL_JoystickOpen(atoi(argv[1]));
                    break;
                }
            }
        }
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

#ifdef ANDROID
    exit(0);
#else
    return 0;
#endif
}

#else

int
main(int argc, char *argv[])
{
    fprintf(stderr, "SDL compiled without Joystick support.\n");
    exit(1);
}

#endif
