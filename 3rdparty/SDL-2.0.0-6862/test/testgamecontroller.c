/*
  Copyright (C) 1997-2011 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* Simple program to test the SDL game controller routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

#ifndef SDL_JOYSTICK_DISABLED

#ifdef __IPHONEOS__
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	480
#else
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#endif

#define MAX_NUM_AXES 6
#define MAX_NUM_HATS 2

static SDL_bool s_ForceQuit = SDL_FALSE;

static void
DrawRect(SDL_Renderer *r, const int x, const int y, const int w, const int h)
{
    const SDL_Rect area = { x, y, w, h };
    SDL_RenderFillRect(r, &area);
}

void
WatchGameController(SDL_GameController * gamecontroller)
{
    SDL_Window *window = NULL;
    SDL_Renderer *screen = NULL;
    const char *name = NULL;
    int done = 0;
    SDL_Event event;
    int i;

    /* Create a window to display controller axis position */
    window = SDL_CreateWindow("Game Controller Test", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_CreateRenderer(window, -1, 0);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return;
    }

    SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(screen);
    SDL_RenderPresent(screen);
	SDL_RaiseWindow(window);

    /* Print info about the controller we are watching */
    name = SDL_GameControllerName(gamecontroller);
    printf("Watching controller %s\n",  name ? name : "Unknown Controller");
    
    /* Loop, getting controller events! */
    while (!done) {
        /* blank screen, set up for drawing this frame. */
        SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(screen);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_CONTROLLERAXISMOTION:
                printf("Controller %d axis %d value: %d\n",
                       event.caxis.which,
                       event.caxis.axis, event.caxis.value);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                printf("Controller %d button %d down\n",
                       event.cbutton.which, event.cbutton.button);
                break;
            case SDL_CONTROLLERBUTTONUP:
                printf("Controller %d button %d up\n",
                       event.cbutton.which, event.cbutton.button);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym != SDLK_ESCAPE) {
                    break;
                }
                /* Fall through to signal quit */
            case SDL_QUIT:
                done = 1;
				s_ForceQuit = SDL_TRUE;
                break;
            default:
                break;
            }
        }
        /* Update visual controller state */
        SDL_SetRenderDrawColor(screen, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        for (i = 0; i <SDL_CONTROLLER_BUTTON_MAX; ++i) {
            if (SDL_GameControllerGetButton(gamecontroller, i) == SDL_PRESSED) {
                DrawRect(screen, i * 34, SCREEN_HEIGHT - 34, 32, 32);
            }
        }

        SDL_SetRenderDrawColor(screen, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i = 0; i < SDL_CONTROLLER_AXIS_MAX / 2; ++i) {
            /* Draw the X/Y axis */
            int x, y;
            x = (((int) SDL_GameControllerGetAxis(gamecontroller, i * 2 + 0)) + 32768);
            x *= SCREEN_WIDTH;
            x /= 65535;
            if (x < 0) {
                x = 0;
            } else if (x > (SCREEN_WIDTH - 16)) {
                x = SCREEN_WIDTH - 16;
            }
            y = (((int) SDL_GameControllerGetAxis(gamecontroller, i * 2 + 1)) + 32768);
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

        SDL_RenderPresent(screen);
		
		if ( !done )
			done = SDL_GameControllerGetAttached( gamecontroller ) == 0;
    }

    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
}

int
main(int argc, char *argv[])
{
    const char *name;
    int i;
	int nController = 0;
    SDL_GameController *gamecontroller;

	SDL_SetHint( SDL_HINT_GAMECONTROLLERCONFIG, "341a3608000000000000504944564944,Aferglow PS3 Controller,a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7" );
    /* Initialize SDL (Note: video is required to start event loop) */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER ) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Print information about the controller */
    for (i = 0; i < SDL_NumJoysticks(); ++i) {
		if ( SDL_IsGameController(i) )
		{
			nController++;
			name = SDL_GameControllerNameForIndex(i);
			printf("Game Controller %d: %s\n", i, name ? name : "Unknown Controller");
		}
    }
	printf("There are %d game controllers attached\n", nController);

    if (argv[1]) {
		int nreportederror = 0;
		SDL_Event event;
		gamecontroller = SDL_GameControllerOpen(atoi(argv[1]));
		while ( s_ForceQuit == SDL_FALSE ) {
			if (gamecontroller == NULL) {
				if ( nreportederror == 0 ) {
					printf("Couldn't open joystick %d: %s\n", atoi(argv[1]), SDL_GetError());
					nreportederror = 1;
				}
			} else {
				nreportederror = 0;
				WatchGameController(gamecontroller);
				SDL_GameControllerClose(gamecontroller);
			}
			
			gamecontroller = NULL;
			SDL_WaitEvent( &event );
			if ( event.type == SDL_JOYDEVICEADDED )
				gamecontroller = SDL_GameControllerOpen(atoi(argv[1]));
		}
	}
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER );

    return (0);
}

#else

int
main(int argc, char *argv[])
{
    fprintf(stderr, "SDL compiled without Joystick support.\n");
    exit(1);
}

#endif
