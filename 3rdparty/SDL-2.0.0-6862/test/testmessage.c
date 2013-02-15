/*
  Copyright (C) 1997-2011 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* Simple test of the SDL MessageBox API*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "SDL.h"
#include "SDL_thread.h"

static int alive = 0;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    SDL_Quit();
    exit(rc);
}

int
main(int argc, char *argv[])
{
    int success;

    /* Load the SDL library */
    if (SDL_Init(0) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return (1);
    }

    success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Simple MessageBox",
                "This is a simple error MessageBox",
                NULL);
    if (success == -1) {
        printf("Error Presenting MessageBox: %s\n", SDL_GetError());
        quit(1);
    }

    success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Simple MessageBox",
                "This is a simple MessageBox with a newline:\r\nHello world!",
                NULL);
    if (success == -1) {
        printf("Error Presenting MessageBox: %s\n", SDL_GetError());
        quit(1);
    }

    /* Google says this is Traditional Chinese for "beef with broccoli" */
    success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "UTF-8 Simple MessageBox",
                "Unicode text: '牛肉西蘭花' ...",
                NULL);
    if (success == -1) {
        printf("Error Presenting MessageBox: %s\n", SDL_GetError());
        quit(1);
    }

    /* Google says this is Traditional Chinese for "beef with broccoli" */
    success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "UTF-8 Simple MessageBox",
                "Unicode text and newline:\r\n'牛肉西蘭花'\n'牛肉西蘭花'",
                NULL);
    if (success == -1) {
        printf("Error Presenting MessageBox: %s\n", SDL_GetError());
        quit(1);
    }

    {
        const SDL_MessageBoxButtonData buttons[] = {
            {
                SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
                0,
                "OK"
            },{
                SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
                1,
                "Cancel"
            },
        };

        SDL_MessageBoxData data = {
            SDL_MESSAGEBOX_INFORMATION,
            NULL, // no parent window
            "Custom MessageBox",
            "This is a custom messagebox",
            2,
            buttons,
            NULL // Default color scheme
        };

        int button = -1;
        success = SDL_ShowMessageBox(&data, &button);
        if (success == -1) {
            printf("Error Presenting MessageBox: %s\n", SDL_GetError());
            quit(2);
        }
        printf("Pressed button: %d, %s\n", button, button == 1 ? "Cancel" : "OK");
    }

    SDL_Quit();
    return (0);
}
