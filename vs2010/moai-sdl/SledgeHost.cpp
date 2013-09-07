#include "stdafx.h"
#include "SledgeHost.h"

#include <uslscore/USFileSys.h>

#ifndef HAS_MOAILUA
#define HAS_MOAILUA
#include <lua-headers/moai_lua.h>
#endif

#define SLEDGE_HOST_USE_LUAEXT

#ifdef SLEDGE_HOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif

#include <aku/AKU-untz.h>

#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
#include <aku/AKU-audiosampler.h>
#endif

#ifdef WIN32
//#include <glut.h>
#include <FolderWatcher-win.h>
#else
//#include <GLUT/glut.h>
#endif

#ifdef __APPLE__
#include <FolderWatcher-mac.h>
#include <OpenGL/OpenGL.h>
#include <moaiext-macosx/SFSAkuInit.h>
#endif

static bool sDynamicallyReevaluateLuaFiles = false;

static u32 s_timerInterval = 0;

#ifndef SDL_STATICWRAPPER_VAR
#define SDL_STATICWRAPPER_VAR
void* CurrentSledgeHostInstance = NULL;
#endif


SledgeHost::SledgeHost(s32 argc, char** argv):
m_LastScript(NULL),
m_SDLWindow(NULL)
{
	memset(&m_WindowPos, 0, sizeof(vec2<u32>));
	memset(&m_WindowSize, 0, sizeof(vec2<u32>));
	memset(&m_WindowTitle, 0, WINDOWTITLE_LENGTH);

	m_InputManager = new SledgeInputManager();

	MakeActive();


	bool bHasValidScript = false;
	switch (argc)
	{
	case 1:
		bHasValidScript = DoDefaultScriptCheck();
		break;
	default:
		bHasValidScript = DoProcessArgs(argc, argv);
		break;
	}

	if(!bHasValidScript)
	{
		printf("Error!\n");
		return;
	}

	// Detect desired different CWD and switch if necessary.
	int scr = 0;
	if(m_LastScript != NULL)
	{
#ifdef _DEBUG
		printf("script path given: %s\n", m_LastScript);
#endif
#if defined(_WIN32) || defined(_WIN64)
		scr = winhostext_SetWorkingDirectory(m_LastScript);
#endif
	}

	// Initialize MOAI, SDL et al.
	DoSystemInit();

	// Run core MOAI Lua.
	AKURunBytecode ( moai_lua, moai_lua_SIZE );

	// Register Sledge modules with MOAI.
	REGISTER_LUA_CLASS ( SledgeCore );
	REGISTER_LUA_CLASS ( SledgeInputHandler );
	SledgeInputHandler::SetManager(m_InputManager);
	REGISTER_LUA_CLASS ( SledgeGraphicsHandler );

	// Initialize the timer intervals.
	m_TimerInterval = (u32)(AKUGetSimStep() * 1000.0);
	m_TimerInterval3 = m_TimerInterval * 2;
	m_Counter = 0;

	// Finally, run the designated script.
	AKUSetArgv ( argv );
	if(m_LastScript != NULL)
	{
#if defined(_WIN32) || defined(_WIN64)
		if(scr > 0)
		{
			AKURunScript(&(m_LastScript[strlen(m_LastScript) - scr]));
		} else {
			AKURunScript(m_LastScript);
		}
#else 
		AKURunScript(lastScript);
#endif

		if ( sDynamicallyReevaluateLuaFiles ) {
#if defined(_WIN32) || defined(_WIN64)
			winhostext_WatchFolder ( m_LastScript );
#elif __APPLE__
			//		FIXME: FWWatch isn't linkable at the moment. need to fix this.
			//		FWWatchFolder( lastScript );
#endif
		}
	}



	//RunGame();
}


SledgeHost::~SledgeHost()
{
	DoSystemTeardown();
	// teardown
	//AKUFinalize();
	//delete m_InputManager;
}

bool SledgeHost::DoSystemInit()
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

	// initialise AKU modules
	// @todo add more AKU things here
	#ifdef SLEDGE_HOST_USE_LUAEXT
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();
	#endif

	AKUUntzInit();

	#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
	AKUAudioSamplerInit();
	#endif

	// set AKU input configuration and reserve AKU input devices 
	m_InputManager->doAKUInit();

	// get hardware environment keys into MOAI
#if defined(_WIN32) || defined(_WIN64)
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	SledgeCore::LoadInfoXML( "info.xml", &environment );
#elif defined(__APPLE__)
	[SFSAkuInit MoaiTypesInit];
	[SFSAkuInit MoaiEnvironmentInit];
#endif
	
	AKUSetFunc_OpenWindow (&SledgeHost::AKUCallbackWrapper_OpenWindowFunc);

	return true;
}

/**
 * @fn	void SledgeHost::DoSystemTeardown(void)
 *
 * @brief	Releases any system resources used.
 *
 * @author	Jetha Chan
 * @date	9/7/2013
 */
void SledgeHost::DoSystemTeardown(void)
{
	AKUFinalize();
	SDL_Quit();
	delete m_InputManager;
}

/**
 * @fn	void SledgeHost::RunGame()
 *
 * @brief	Actually run the damned game.
 *
 * @author	Jetha Chan
 * @date	9/7/2013
 */
void SledgeHost::RunGame()
{
	SDL_Event event;
	u32 tick_start = SDL_GetTicks();
	u32 tick_end = 0;
	u32 tick_delta = 0;
	u32 tick_wait = 0;
	bool bGameRunning = true;
	m_DeltaTime = 0.0;
	if(m_SDLWindow != NULL) {
		m_ActiveTimer = SDL_AddTimer(
			m_TimerInterval,
			&SledgeHost::SDLCallbackWrapper_OnTickFunc,//_onTick,
			&m_TimerInterval
		);	
		
		while (bGameRunning)
		{
			m_InputManager->blankKeybFlags();

			while(SDL_PollEvent(&event))
			{
				switch(event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_F4 && (event.key.keysym.mod == KMOD_LALT || event.key.keysym.mod == KMOD_RALT))
					{
						bGameRunning = false;						
					} else {
						m_InputManager->inputNotify_onKeyDown(&(event.key));
					}

					break;

				case SDL_MOUSEMOTION:
					m_InputManager->inputNotify_onMouseMove(&(event.motion));
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					m_InputManager->inputNotify_onMouseButton(&(event.button));
					break;
				case SDL_USEREVENT:
					//Render updates not being received and updates not tied to render causes stuttering
					//ProcessUserEvent(event.user.code);
					break;

				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					break;

				case SDL_QUIT:
					SDL_RemoveTimer(m_ActiveTimer);
					bGameRunning = false;
					break;
				default:
					break;
				}
			};

			AKUUpdate();
			AKURender();
			SDL_GL_SwapWindow(m_SDLWindow);

			m_InputManager->doOnTick();

			// Update the clock.
			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;
			m_DeltaTime = ((double)tick_delta) / 1000.0;
			//printf("Tick: %i - %f\n", tick_delta, m_DeltaTime);
			tick_start = tick_end;
		}
	}
}

/**
 * @fn	void SledgeHost::MakeActive()
 *
 * @brief	Make this class instance the active one for the purposes of
 * 			AKU callbacks.
 *
 * @author	Jetha Chan
 * @date	9/7/2013
 */
void SledgeHost::MakeActive()
{
	CurrentSledgeHostInstance = (void*)this;
}

void SledgeHost::AKUCallback_OpenWindowFunc
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

		// bring up the window
		if((m_SDLWindow = SDL_CreateWindow(
			title,
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
		
		SDL_SetWindowTitle(m_SDLWindow, title);

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

		int vsync = SDL_GL_SetSwapInterval(1);
		if (vsync == 0) {
			printf("VSync enabled!\n");
		} else {
			printf("VSync not enableable!\n");
		}
	}

	SledgeGraphicsHandler::SetWindow(m_SDLWindow);


	// 2013/06/17: Get additional environment info
	SledgeCore::GetAdditionalHWInfo( &(MOAIEnvironment::Get()) );

}

void SledgeHost::AKUCallbackWrapper_OpenWindowFunc
	(const char* title, int width, int height)
{
	SledgeHost* obj = (SledgeHost*)CurrentSledgeHostInstance;

	obj->AKUCallback_OpenWindowFunc(title, width, height);
}

void SledgeHost::_doAkuUpdate()
{
	AKUUpdate();

	if ( sDynamicallyReevaluateLuaFiles ) {		
#ifdef _WIN32
		winhostext_Query ();
#elif __APPLE__
//		FWReloadChangedLuaFiles ();
#endif
	}
}

void SledgeHost::_doAkuRender()
{
	AKURender();
	SDL_GL_SwapWindow(m_SDLWindow);
}

void SledgeHost::ProcessUserEvent(int p_type)
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

u32 SledgeHost::SDLCallback_OnTickFunc(u32 millisec, void* param)
{
	UNUSED (millisec);

	// re-register the timer
	m_TimerInterval2 =
		(u32)(AKUGetSimStep() * 1000.0);
	m_ActiveTimer = SDL_AddTimer(
		m_TimerInterval2,
		&SledgeHost::SDLCallbackWrapper_OnTickFunc,//_onTick,
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

u32 SledgeHost::SDLCallbackWrapper_OnTickFunc(u32 millisec, void* param)
{
	SledgeHost* obj = (SledgeHost*)CurrentSledgeHostInstance;
	return obj->SDLCallback_OnTickFunc(millisec, param);
}

/**
 * @fn	bool SledgeHost::DoDefaultScriptCheck()
 *
 * @brief	Check for default script existence, and set the first one found
 * 			to be loaded.
 *
 * @author	Jetha Chan
 * @date	9/7/2013
 *
 * @return	TRUE if a default script is found, FALSE if not.
 */
bool SledgeHost::DoDefaultScriptCheck()
{
	s32 foundfileIdx = -1;
	// try and find a default script
	for (s32 idx = 0; idx < SLEDGE_NAMESPACE::DefaultScriptLocationCount; ++idx)
	{
#ifdef _DEBUG
		printf("%d: %s...", idx, SLEDGE_NAMESPACE::DefaultScriptLocations[idx]);
#endif
		if(USFileSys::CheckFileExists(SLEDGE_NAMESPACE::DefaultScriptLocations[idx]))
		{
			m_LastScript = SLEDGE_NAMESPACE::DefaultScriptLocations[idx];
			foundfileIdx = idx;
#ifdef _DEBUG
			printf("\tfound\n");
#endif
			break;
		} else {
#ifdef _DEBUG
			printf("\tnot found\n");
#endif
		}
	}

	return foundfileIdx != -1;
}

/**
 * @fn	bool SledgeHost::DoProcessArgs(s32 argc, char*argv)
 *
 * @brief	Processes the command-line arguments passed to the host.
 *
 * @author	Jetha Chan
 * @date	9/7/2013
 *
 * @param	argc			The number of command-line arguments.
 * @param [in,out]	argv	The command-line arguments.
 *
 * @return	true if it succeeds, false if it fails.
 */
bool SledgeHost::DoProcessArgs(s32 argc, char** argv)
{
	bool bValidArgs = false;

	for (s32 idx = 1; idx < argc; ++idx)
	{
		char* thisarg = argv[idx];

		// -e				enables dynamic re-evaluation of Lua files
		if ( strcmp( thisarg, "-e" ) == 0 ) {
			m_bDoLuaDynamicReeval = true;
		}
		// -s [Lua string]	executes a given Lua string
		else if ( strcmp( thisarg, "-s" ) == 0 && ++idx < argc ) {
			char* script = argv [ idx ];
			AKURunString ( script );
			bValidArgs = true;
			break;
		}
		// (anything else)	interpreted as the location of a script to load
		else {
			m_LastScript = thisarg;
			bValidArgs = true;
			break;
		}
	}

	return bValidArgs;
}

