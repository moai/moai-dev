#include "SDLHost.h"


#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>

static bool sHasWindow = false;
static bool sExitFullscreen = false;
static bool sDynamicallyReevaluateLuaFiles = false;
static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;




static void _cleanup() {
	
}

#pragma region AKUCallbacks
// forward definitions
void _AKUOpenWindowFunc(const char* title, int width, int height);

// declarations
void _AKUOpenWindowFunc(const char* title, int width, int height)
{
	sWinX = 180;
	sWinY = 100;
	sWinWidth = width;
	sWinHeight = height;

	if(!sHasWindow)
	{
		// configure SDL's GL settings.
		// @todo	assess whether this is actually a good idea
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// bring up a window
		SDL_Surface* screen;
		if((screen = SDL_SetVideoMode(
				sWinWidth,
				sWinHeight,
				32,
				SDL_OPENGL)
			) == NULL)
		{
			SDL_Quit();
			return;
		}
		s_screens.push_back(screen);

		// set window position
		// @todo	there's got to be a better way to do this. SDL2
		//			seems to have a smarter mechanism
		char str_windowpos[255];
		sprintf_s(
			str_windowpos,
			255,
			"SDL_VIDEO_WINDOW_POS=%d,%d",
			sWinX,
			sWinY			
		);
		SDL_putenv(str_windowpos);

		// set window title
		// @todo	add app-defined icon?
		SDL_WM_SetCaption(title, 0);

		sHasWindow = true;
	}

}


#pragma endregion

int SdlHost(int argc, char** arg)
{
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);

	return 0;
}

void SdlRefreshContext()
{

}