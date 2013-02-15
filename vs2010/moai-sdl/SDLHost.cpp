#include "SDLHost.h"

#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>

#ifndef UNUSED
#define UNUSED(p) (( void )p)
#endif

#ifdef SDLHOST_USE_DEBUGGER
#include <aku/AKU-debugger.h>
#endif

#ifdef SDLHOST_USE_FMOD
#include <aku/AKU-fmod-ex.h>//<AKU-fmod-ex.h>//<aku/AKU-fmod.h>
#endif

#ifdef SDLHOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif

#ifdef SDLHOST_USE_UNTZ
#include <aku/AKU-untz.h>
#endif

#ifdef SDLHOST_USE_AUDIOSAMPLER
#include <aku/AKU-audiosampler.h>
#endif

#ifdef SDLHOST_USE_PARTICLE_PRESETS
#include <ParticlePresets.h>
#endif

#ifdef _WIN32
#include <glut.h>
//#include <FolderWatcher-win.h>
#else
#include <GLUT/glut.h>
#endif



static bool sHasWindow = false;
static bool sExitFullscreen = false;
static bool sDynamicallyReevaluateLuaFiles = false;
static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;




static void _cleanup() {
	AKUFinalize ();
	/*
	if ( sDynamicallyReevaluateLuaFiles ) {
#ifdef _WIN32
		winhostext_CleanUp ();
#elif __APPLE__
		FWStopAll ();
#endif
	}
	*/
	
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


unsigned int _onTick(unsigned int millisec)
{
	printf("boo/n");

	return 0;
}

int SdlHost(int argc, char** arg)
{
	// @todo	maybe we should use AKUInitMemPool here
	atexit(_cleanup);

	// init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	SdlRefreshContext();

	// @todo	un-dumb this
	char* lastScript = NULL;
	for (int i = 1; i < argc; ++i)
	{
		char* thisarg = arg[i];
		if ( strcmp( thisarg, "-e" ) == 0 ) {
			sDynamicallyReevaluateLuaFiles = true;
		}
		else if ( strcmp( thisarg, "-s" ) == 0 && ++i < argc ) {
			char* script = arg [ i ];
			AKURunString ( script );
		}
		else {
			AKURunScript ( thisarg );
			lastScript = thisarg;
		}
	}

	/*
	if ( lastScript && sDynamicallyReevaluateLuaFiles ) {
#ifdef _WIN32
		winhostext_WatchFolder ( lastScript );
#elif __APPLE__
		FWWatchFolder( lastScript );
#endif
	}
	*/

	unsigned int timerInterval =
		(unsigned int)(AKUGetSimStep() * 1000.0);
	
	// sdl-style loop
	SDL_Event event;
	unsigned int tick_start = SDL_GetTicks();
	unsigned int tick_end = 0;
	unsigned int tick_delta = 0;
	float dt = 0.0f;
	bool bGameRunning = true;
	if(sHasWindow)
	{
		SDL_SetTimer(timerInterval, _onTick);
		while(bGameRunning)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type) {
				case SDL_KEYDOWN:
					printf("Keypress!\n");
					break;
				case SDL_QUIT:
					bGameRunning = false;
					break;
				default:
					//printf("Unhandled event.\n");
					break;
				}
			};

			// Update the clock.
			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;

			// Do on-tick events.
			//_onTick(tick_delta);

			dt = (float)(tick_delta) * 0.001f;/// 1000.0f;
			tick_start = tick_end;
		}
	}


	return 0;
}

void SdlRefreshContext()
{
	AKUContextID context = AKUGetContext();
	if(context) {
		AKUDeleteContext(context);
	}
	AKUCreateContext();

	#ifdef SDLHOST_USE_FMOD
	//AKUFmodExInit();
	#endif

	#ifdef SDLHOST_USE_LUAEXT
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();
	#endif

	#ifdef SDLHOST_USE_UNTZ
	AKUUntzInit();
	#endif

	#ifdef SDLHOST_USE_AUDIOSAMPLER
	AKUAudioSamplerInit();	
	#endif

	#ifdef SDLHOST_USE_PARTICLE_PRESETS
	//ParticlePresets();
	#endif

	AKUSetInputConfigurationName ( "AKUSdl" );

	//AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	//AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );


	AKURunBytecode ( moai_lua, moai_lua_SIZE );
}