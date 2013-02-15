#ifndef SDLHOST
#define SDLHOST

#include <vector>


#include <SDL.h>
#define SDL_main main
//#undef main

static std::vector<SDL_Surface*> s_screens;

int SdlHost (int argc, char** arg);
void SdlRefreshContext();

#endif