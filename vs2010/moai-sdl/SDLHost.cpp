#include "SDLHost.h"

#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#ifndef HAS_MOAILUA
#define HAS_MOAILUA
#include <lua-headers/moai_lua.h>
#endif

#ifndef UNUSED
#define UNUSED(p) (( void )p)
#endif

#ifdef SDLHOST_USE_DEBUGGER
#include <aku/AKU-debugger.h>
#endif

#ifdef SDLHOST_USE_FMOD
//#include //<aku/AKU-fmod-ex.h>//<aku/AKU-fmod.h>
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
#include <FolderWatcher-win.h>
#else
#include <GLUT/glut.h>
#endif


SDL_Window* sWindow_Main;
SDL_GLContext glcontext;

static bool sHasWindow = false;
static bool sExitFullscreen = false;
static bool sDynamicallyReevaluateLuaFiles = false;
static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;

static char s_WinTitle[255];
static char s_WinTitleActual[255];

static unsigned int s_timerInterval;
static unsigned int s_timerInterval2;

static int s_JoystickCount = 0;

SDL_TimerID s_activeTimer = NULL;



static void _cleanup() {

	AKUFinalize ();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(sWindow_Main);	
	SDL_Quit();
	
	if ( sDynamicallyReevaluateLuaFiles ) {
#ifdef _WIN32
		winhostext_CleanUp ();
#elif __APPLE__
		FWStopAll ();
#endif

	}
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

#pragma endregion

#pragma region inputFuncs
// @todo	Add some input functions here.
void _input_onMouseButton(SDL_MouseButtonEvent* p_event)
{
	// @ todo	Add keyboard modifier support.
	switch (p_event->button)		
	{
	case SDL_BUTTON_LEFT:
		AKUEnqueueButtonEvent(
			SDLInputDeviceID::DEVICE,
			SDLInputDeviceSensorID::MOUSE_LEFT,
			(p_event->state == SDL_PRESSED)
		);
		break;
	case SDL_BUTTON_MIDDLE:
		AKUEnqueueButtonEvent(
			SDLInputDeviceID::DEVICE,
			SDLInputDeviceSensorID::MOUSE_MIDDLE,
			(p_event->state == SDL_PRESSED)
			);
		break;
	case SDL_BUTTON_RIGHT:
		AKUEnqueueButtonEvent(
			SDLInputDeviceID::DEVICE,
			SDLInputDeviceSensorID::MOUSE_RIGHT,
			(p_event->state == SDL_PRESSED)
			);
		break;
	}
}

void _input_onMouseMove(SDL_MouseMotionEvent* p_event)
{
	AKUEnqueuePointerEvent (
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::POINTER,
		p_event->x,
		p_event->y
	);	
}
#pragma endregion


unsigned int _onTick(unsigned int millisec, void* param)
{
	UNUSED ( millisec );
	//printf("boo\n");

	// re-register the timer
	s_timerInterval2 =
		(unsigned int)(AKUGetSimStep() * 1000.0);
	//	SDL_SetTimer(s_timerInterval, _onTick);

	s_activeTimer = SDL_AddTimer(s_timerInterval2, _onTick, &s_timerInterval2);

	AKUUpdate ();
	//AKURender();

	if ( sDynamicallyReevaluateLuaFiles ) {		
#ifdef _WIN32
		winhostext_Query ();
#elif __APPLE__
		FWReloadChangedLuaFiles ();
#endif
	}

	SDL_SetWindowTitle(sWindow_Main, s_WinTitleActual);
	//SDL_GL_SwapBuffers();

	return 0;
}


int SdlHost(int argc, char** arg)
{
	// @todo	maybe we should use AKUInitMemPool here
	atexit(_cleanup);

	// init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

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

	
	if ( lastScript && sDynamicallyReevaluateLuaFiles ) {
#ifdef _WIN32
		winhostext_WatchFolder ( lastScript );
#elif __APPLE__
		FWWatchFolder( lastScript );
#endif
	}
	

	s_timerInterval = (unsigned int)(AKUGetSimStep() * 1000.0);
	
	// sdl-style loop
	SDL_Event event;
	unsigned int tick_start = SDL_GetTicks();
	unsigned int tick_end = 0;
	unsigned int tick_delta = 0;
	unsigned int tick_wait = 0;
	double dt = 0.0f;
	bool bGameRunning = true;
	if(sHasWindow)
	{
		s_activeTimer = SDL_AddTimer(s_timerInterval, _onTick, &s_timerInterval);
		while(bGameRunning)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type) {
				case SDL_KEYDOWN:
					printf("Keypress!\n");
					break;
				case SDL_MOUSEMOTION:
					_input_onMouseMove(&(event.motion));
					//printf("Mousemotion!\n");
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					_input_onMouseButton(&(event.button));
					//printf("mouse button event\n");
					break;
				case SDL_QUIT:
					SDL_RemoveTimer(s_activeTimer);
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

			unsigned int tick_temp = 17 - tick_delta;
			if(tick_temp > 0)
				tick_wait = tick_temp;
			
			SDL_Delay(20);  // if we don't wait for long enough, akurender will
							// fuck EVERYTHING.
			AKURender();
			SDL_GL_SwapWindow(sWindow_Main);
			//SDL_GL_SwapBuffers();

			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;

			// Do on-tick events.
			//SDL_WM_SetCaption(s_WinTitleActual, 0);
			//_onTick(tick_delta);

			dt = 1000.0 / ((double)tick_delta);/// 1000.0f;

			sprintf_s(s_WinTitleActual, 255, "%s fps: %0.2f [%d]", s_WinTitle, dt, tick_delta);

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
	AKUFmodLoad();
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
	ParticlePresets();
	#endif

	AKUSetInputConfigurationName ( "AKUSDL" );

	AKUReserveInputDevices(1);

	AKUSetInputDevice(
		SDLInputDeviceID::DEVICE,
		"device"
	);

	AKUReserveInputDeviceSensors(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::TOTAL
	);
	/*
	AKUSetInputDeviceActive(
		SDLInputDeviceID::DEVICE,
		true
	);
	*/

	AKUSetInputDeviceKeyboard(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::KEYBOARD,
		"keyboard"
		);
	AKUSetInputDevicePointer(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::POINTER,
		"pointer"
	);
	AKUSetInputDeviceButton(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::MOUSE_LEFT,
		"mouseLeft"
	);
	AKUSetInputDeviceButton(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::MOUSE_MIDDLE,
		"mouseMiddle"
	);
	AKUSetInputDeviceButton(
		SDLInputDeviceID::DEVICE,
		SDLInputDeviceSensorID::MOUSE_RIGHT,
		"mouseRight"
	);


	// 2013/02/15
	// @todo poll controllers here?
	/*
	s_JoystickCount = SDL_NumJoysticks();
	printf("joysticks connected: %d\n", s_JoystickCount);
	for (int i = 0; i < s_JoystickCount; ++i)
	{
		SDL_Joystick* thisStick = SDL_JoystickOpen(i);
		int num_axes = SDL_JoystickNumAxes(thisStick);
		int num_btn = SDL_JoystickNumButtons(thisStick);
		int num_balls = SDL_JoystickNumBalls(thisStick);

		//bool isGamepad = SDL_

		printf(
			"joystick %d:\n\taxes:\t\t%d\n\tbuttons:\t%d\n\tballs:\t\t%d\n",
			i,
			num_axes,
			num_btn,
			num_balls
		);

		SDL_JoystickClose(thisStick);
	}
	*/

	//AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	//AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );


	AKURunBytecode ( moai_lua, moai_lua_SIZE );
}