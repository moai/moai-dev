#include "AKUCallbacks.h"


/*
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


		sprintf_s(s_WinTitle, 255, "[Moai-SDL] %s", title);

		// bring up a window
		if((sWindow_Main = SDL_CreateWindow(
			s_WinTitle,
			sWinX,
			sWinY,
			sWinWidth,
			sWinHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
			)) == NULL)
		{
			SDL_Quit();
			return;
		}

		sHasWindow = true;
	}

	glcontext = SDL_GL_CreateContext(sWindow_Main);


	AKUDetectGfxContext ();
	AKUSetScreenSize ( sWinWidth, sWinHeight );
	AKUSetViewSize ( sWinWidth, sWinHeight );
}
*/