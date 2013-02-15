#ifndef SDLHOST
#define SDLHOST

#include <vector>


#include <SDL.h>
#define SDL_main main
//#undef main



int SdlHost (int argc, char** arg);
void SdlRefreshContext();

#endif