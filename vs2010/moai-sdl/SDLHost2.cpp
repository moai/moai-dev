#include "SDLHost2.h"


#ifndef HAS_MOAILUA
#define HAS_MOAILUA
#include <lua-headers/moai_lua.h>
#endif

#ifdef SDLHOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif

#ifdef _WIN32
//#include <glut.h>
#include <FolderWatcher-win.h>
#else
//#include <GLUT/glut.h>
#endif

#ifdef __APPLE__
#include <FolderWatcher-mac.h>
#include <OpenGL/OpenGL.h>
#endif

static bool sDynamicallyReevaluateLuaFiles = false;

static unsigned int s_timerInterval = 0;

#ifndef SDL_STATICWRAPPER_VAR
#define SDL_STATICWRAPPER_VAR
void* CurrentSdlHost2Instance = NULL;
#endif


SdlHost2::SdlHost2(int argc, char** arg)
{
	memset(&m_WindowPos, 0, sizeof(vec2u));
	memset(&m_WindowSize, 0, sizeof(vec2u));
	memset(&m_WindowTitle, 0, WINDOWTITLE_LENGTH);

	m_SDLWindow = NULL;

	m_InputManager = new SdlInputManager();

	makeActive();

	doInit();

	AKURunBytecode ( moai_lua, moai_lua_SIZE );


	// @todo	un-dumb this
	char* lastScript = NULL;

	AKUSetArgv ( arg );

	for ( int i = 1; i < argc; ++i ) {
		char* thisarg = arg [ i ];
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

	double simstep = AKUGetSimStep();

	m_TimerInterval = (unsigned int)(AKUGetSimStep() * 1000.0);
	m_TimerInterval3 = m_TimerInterval * 2;

	m_Counter = 0;

	runGame();
}


SdlHost2::~SdlHost2()
{
	// teardown
	AKUFinalize();
	delete m_InputManager;
}

bool SdlHost2::doInit()
{
	// initialize SDL
	SDL_Init(
		SDL_INIT_VIDEO |
		SDL_INIT_TIMER |
		SDL_INIT_JOYSTICK |
		SDL_INIT_GAMECONTROLLER |
		SDL_INIT_HAPTIC
	);

	// refresh the AKU context
	m_AkuContext = AKUGetContext();
	if(m_AkuContext) {
		AKUDeleteContext(m_AkuContext);
	}
	m_AkuContext = AKUCreateContext();

	//AKUInitMemPool ( 100 * 1024 * 1024 );

	// initialise AKU modules
	// @todo add more AKU things here
	#ifdef SDLHOST_USE_LUAEXT
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();
	#endif

	// set AKU input configuration and reserve AKU input devices
	// @todo	we're probably going to need multiple devices
	m_InputManager->doAKUInit();

	AKUSetFunc_OpenWindow (&SdlHost2::AKUCallbackWrapper_OpenWindowFunc);

	return true;
}

void SdlHost2::runGame()
{
	SDL_Event event;
	unsigned int tick_start = SDL_GetTicks();
	unsigned int tick_end = 0;
	unsigned int tick_delta = 0;
	unsigned int tick_wait = 0;
	bool bGameRunning = true;
	m_DeltaTime = 0.0;
	if(m_SDLWindow != NULL) {
		m_ActiveTimer = SDL_AddTimer(
			m_TimerInterval,
			&SdlHost2::SDLCallbackWrapper_OnTickFunc,//_onTick,
			&m_TimerInterval
		);	
		
		while (bGameRunning)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type) {
				case SDL_KEYDOWN:
					printf("Keypress!\n");
					break;
				case SDL_MOUSEMOTION:
					m_InputManager->inputNotify_onMouseMove(&(event.motion));
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					m_InputManager->inputNotify_onMouseButton(&(event.button));
					break;
				case SDL_USEREVENT:
					ProcessUserEvent(event.user.code);
					break;

				case SDL_CONTROLLERAXISMOTION:
					//printf("axis motion\n");
					m_InputManager->inputNotify_onPadAxisMove(&(event.caxis));
					break;

				case SDL_CONTROLLERBUTTONDOWN:
					printf("button down!\n");
					break;
				case SDL_CONTROLLERBUTTONUP:
					printf("button up!\n");
					break;

				case SDL_QUIT:
					SDL_RemoveTimer(m_ActiveTimer);
					bGameRunning = false;
					break;
				default:
					break;
				}
			};

			// Render as fast as possible, because really, why the fuck not?
			// Jack the barrier, bring the noise.
			AKURender();
			SDL_GL_SwapWindow(m_SDLWindow);


			// Update the clock.
			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;
			m_DeltaTime = ((double)tick_delta) / 1000.0;

			tick_start = tick_end;
		}
	}
}

void SdlHost2::makeActive()
{
	CurrentSdlHost2Instance = (void*)this;
}

void SdlHost2::AKUCallback_OpenWindowFunc
	(const char* title, int width, int height)
{
	m_WindowPos.x = 180;
	m_WindowPos.y = 100;

	m_WindowSize.x = width;
	m_WindowSize.y = height;
	
	// Set up a window if we don't have one yet.
	if(m_SDLWindow == NULL)
	{
		// configure SDL's GL settings.
		// @todo	assess whether this is actually a good idea
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		sprintf_s(m_WindowTitleBase, WINDOWTITLE_LENGTH, "[Moai-SDL] %s", title);

		// bring up the window
		if((m_SDLWindow = SDL_CreateWindow(
			m_WindowTitleBase,
			m_WindowPos.x,
			m_WindowPos.y,
			m_WindowSize.x,
			m_WindowSize.y,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
			)) == NULL)
		{
			SDL_Quit();
			return;
		}

		// create the gl context
		m_SDLGLContext = SDL_GL_CreateContext(m_SDLWindow);

		// tell AKU about what we just did
		AKUDetectGfxContext ();
		AKUSetScreenSize (
			m_WindowSize.x,
			m_WindowSize.y
		);		
		AKUSetViewSize (
			m_WindowSize.x,
			m_WindowSize.y
		);		
	}


}

void SdlHost2::AKUCallbackWrapper_OpenWindowFunc
	(const char* title, int width, int height)
{
	SdlHost2* obj = (SdlHost2*)CurrentSdlHost2Instance;

	obj->AKUCallback_OpenWindowFunc(title, width, height);
}

void SdlHost2::_doAkuUpdate()
{
	AKUUpdate();

	if ( sDynamicallyReevaluateLuaFiles ) {		
#ifdef _WIN32
		winhostext_Query ();
#elif __APPLE__
		FWReloadChangedLuaFiles ();
#endif
	}
}

void SdlHost2::_doAkuRender()
{
	AKURender();
	SDL_GL_SwapWindow(m_SDLWindow);
}

void SdlHost2::ProcessUserEvent(int p_type)
{
	switch (p_type)
	{
	case SDLUserEventType::UET_RENDER:
		_doAkuRender();
		break;
	case SDLUserEventType::UET_UPDATE:
		_doAkuUpdate();
		break;
	default:
		break;
	}
}

unsigned int SdlHost2::SDLCallback_OnTickFunc(unsigned int millisec, void* param)
{
	UNUSED (millisec);

	// re-register the timer
	m_TimerInterval2 =
		(unsigned int)(AKUGetSimStep() * 1000.0);
	m_ActiveTimer = SDL_AddTimer(
		m_TimerInterval2,
		&SdlHost2::SDLCallbackWrapper_OnTickFunc,//_onTick,
		&m_TimerInterval2
		);

	// use SDL_PushEvent to enqueue AKU_Update because AKU for whatever reason
	// isn't threadsafe; this SDL callback runs on a separate thread so we
	// can't do it here.
	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = SDLUserEventType::UET_UPDATE;
	SDL_PushEvent(&event);

	return 0;
}

unsigned int SdlHost2::SDLCallbackWrapper_OnTickFunc(unsigned int millisec, void* param)
{
	SdlHost2* obj = (SdlHost2*)CurrentSdlHost2Instance;
	return obj->SDLCallback_OnTickFunc(millisec, param);
}