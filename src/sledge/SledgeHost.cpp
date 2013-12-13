#include "stdafx.h"
#include "SledgeHost.h"
#include <zl-vfs/ZLVfsFileSystem.h>

#ifdef __APPLE__
#include <moai-macosx/SFSAkuInit.h>
#endif

void* SledgeHost::s_CurrentSledgeHostInstance = NULL;

/**
 * Constructor.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param	argc			The number of parameters in the parameter array.
 * @param [in,out]	argv	An array of C-string parameters, the first of
 *							which should be the	command executed to invoke
 *							the program.
 */
SledgeHost::SledgeHost(s32 argc, char** argv):
m_LastScript(NULL),
m_StatusCode(SLEDGE_NAMESPACE::SFS_OK),
m_SDLWindow(NULL),
m_SDLGLContext(NULL),
m_InputManager(NULL)
{
	memset(&m_WindowPos, 0, sizeof(vec2<u32>));
	memset(&m_WindowSize, 0, sizeof(vec2<u32>));
	memset(&m_WindowTitle, 0, SLEDGE_NAMESPACE::WINDOWTITLE_LENGTH);
	
	// Make this SledgeHost instance "active" for the purposes of callbacks.
	MakeActive();
	
	ZLVfsFileSystem::Get().Init();
	
	// Load a Lua script:
	switch (argc)
	{
	case 1:
		// a) when we don't have any command-line arguments, try the default
		// scripts.
		if(!DoDefaultScriptCheck())		
			m_StatusCode = SLEDGE_NAMESPACE::SFS_ERR_NOSCRIPT;
		break;
	default:
		// b) when we do have command-line arguments, see what we can do.
		if(!DoProcessArgs(argc, argv))
			m_StatusCode = SLEDGE_NAMESPACE::SFS_ERR_NOSCRIPT;
		break;
	}

	// Early out if we don't have a script to run.
	if(m_StatusCode == SLEDGE_NAMESPACE::SFS_ERR_NOSCRIPT)
		return;
	

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

	// Instantiate an input manager.
	m_InputManager = new SledgeInputManager();

	// Initialize MOAI, SDL et al.
	DoSystemInit();

	// Run core MOAI Lua.
	//AKURunBytecode ( moai_lua, moai_lua_SIZE );
	AKURunData (moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED);

	// Register Sledge modules with MOAI.
	REGISTER_LUA_CLASS ( SledgeCore );
	REGISTER_LUA_CLASS ( SledgeInputWrapper );
	//REGISTER_LUA_CLASS ( SledgeInput );
	SledgeInputWrapper::SetManager(m_InputManager);
	REGISTER_LUA_CLASS ( SledgeGraphicsHandler );
	
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
		AKURunScript(m_LastScript);
#endif

		if ( m_bDoLuaDynamicReeval ) {
#if defined(_WIN32) || defined(_WIN64)
			winhostext_WatchFolder ( m_LastScript );
#elif __APPLE__
			//		FIXME: FWWatch isn't linkable at the moment. need to fix this.
			//		FWWatchFolder( lastScript );
#endif
		}
	}
}

SledgeHost::~SledgeHost()
{
	DoSystemTeardown();
}


/**
 * Returns status code to check against flags.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @return	The status code.
 */
s32 SledgeHost::CheckStatus()
{
	return m_StatusCode;
}

/**
 * Make this class instance the active one for the purposes of AKU callbacks.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 */
void SledgeHost::MakeActive()
{
	s_CurrentSledgeHostInstance = (void*)this;
}

/**
 * Runs the game.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 */
void SledgeHost::RunGame()
{
	SDL_Event event;
	u32 tick_start = SDL_GetTicks();
	u32 tick_end = 0;
	u32 tick_delta = 0;
	u32 tick_wait = 0;
	bool bGameRunning = true;
	if(m_SDLWindow != NULL) {
		
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
				case SDL_WINDOWEVENT:
					m_InputManager->inputNotify_onWindowEnterLeave((SDL_WindowEventID)event.window.event);
					break;
				//case SDL_WINDOWEVENT_LEAVE:
				//	m_InputManager->inputNotify_onWindowLeave();
				//	break;
				case SDL_USEREVENT:
					// Render updates not being received and updates not tied to
					// render causes stuttering
					//ProcessUserEvent(event.user.code);
					break;

				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					break;

				case SDL_QUIT:
					bGameRunning = false;
					break;
				default:
					break;
				}
			};

			AKUUpdate();
#if MOAI_HOST_USE_FMOD_EX
			AKUFmodUpdate ();
#endif
#if MOAI_HOST_USE_FMOD_DESIGNER
			AKUFmodDesignerUpdate (( float )fSimStep );
#endif

			if ( m_bDoLuaDynamicReeval ) {		
#if defined(_WIN32) || defined (_WIN64)
				winhostext_Query ();
#elif __APPLE__
				FWReloadChangedLuaFiles ();
#endif
			}
			AKURender();
			SDL_GL_SwapWindow(m_SDLWindow);

			m_InputManager->doOnTick();

			// Update the clock.
			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;
			//printf("Tick: %i - %f\n", tick_delta, m_DeltaTime);
			tick_start = tick_end;
		}
	}
}

/**
 * System and sub-system initialization.
 *
 * @author	Jetha Chan
 * @date	8/09/2013
 */
void SledgeHost::DoSystemInit()
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

	AKUInitializeUtil();
	AKUInitializeSim();

#if MOAI_WITH_BOX2D
	AKUInitializeBox2D ();
#endif

#if MOAI_WITH_CHIPMUNK
	AKUInitializeChipmunk ();
#endif

#if MOAI_WITH_FMOD_EX
	AKUFmodLoad ();
#endif

#if MOAI_WITH_FMOD_DESIGNER
	AKUFmodDesignerInit ();
#endif

	// initialise AKU modules
	// @todo add more AKU things here
	#ifdef MOAI_WITH_LUAEXT
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();
#endif

#if MOAI_WITH_HARNESS
	AKUSetFunc_ErrorTraceback ( _debuggerTracebackFunc );
	AKUDebugHarnessInit ();
#endif

#if MOAI_WITH_HTTP_CLIENT
	AKUInitializeHttpClient ();
#endif 

#if MOAI_WITH_PARTICLE_PRESETS
	ParticlePresets ();
#endif

#if MOAI_WITH_UNTZ
	AKUInitializeUntz ();
#endif

	//#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
	//AKUAudioSamplerInit();
	//#endif

	// set AKU input configuration and reserve AKU input devices 
	m_InputManager->doAKUInit();

	// get hardware environment keys into MOAI
#if defined(_WIN32) || defined(_WIN64)
	//MOAIEnvironment environment = MOAIEnvironment::Get ();
	SledgeCore::LoadInfoXML( "info.xml", MOAIEnvironment::Get() );
#elif defined(__APPLE__)
	[SFSAkuInit MoaiTypesInit];
	[SFSAkuInit MoaiEnvironmentInit];
#endif
	
	AKUSetFunc_OpenWindow (&SledgeHost::Callback_OpenWindowFunc);

}

/**
 * Releases any system resources used.
 *
 * @author	Jetha Chan
 * @date	7/09/2013
 */
void SledgeHost::DoSystemTeardown(void)
{
#if MOAI_WITH_BOX2D
	AKUFinalizeBox2D ();
#endif

#if MOAI_WITH_CHIPMUNK
	AKUFinalizeChipmunk ();
#endif

#if MOAI_WITH_HTTP_CLIENT
	AKUFinalizeHttpClient ();
#endif

	AKUFinalizeUtil();
	AKUFinalizeSim();
	AKUFinalize();

	if(m_SDLGLContext != NULL)
		SDL_GL_DeleteContext(m_SDLGLContext);
	SDL_Quit();
	if(m_InputManager != NULL)
		delete m_InputManager;
}

/**
 * Requests the current SledgeHost instance open a window.
 *
 * @author	Jetha Chan
 * @date	8/09/2013
 *
 * @param	title	The desired window title.
 * @param	w	 	The desired window width.
 * @param	h	 	The desired window height.
 */
void SledgeHost::Callback_OpenWindowFunc (const char* title, s32 w, s32 h)
{
	SledgeHost* obj = (SledgeHost*)s_CurrentSledgeHostInstance;
	obj->DoOpenWindow(title, w, h);
}

/**
 * Opens a game window, on MOAI's request.
 *
 * @author	Jetha Chan
 * @date	8/09/2013
 *
 * @param	title	The desired window title.
 * @param	w	 	The desired window width.
 * @param	h	 	The desired window height.
 */
void SledgeHost::DoOpenWindow (const char* title, s32 w, s32 h)
{
	m_WindowPos.x = 180;
	m_WindowPos.y = 100;

	m_WindowSize.x = w;
	m_WindowSize.y = h;
	
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

	SledgeGraphicsHandler::SetWindowHandle(m_SDLWindow);


	// Get additional environment info.
#if defined(_WIN32) || defined(_WIN64)
	SledgeCore::GetAdditionalHWInfo( &(MOAIEnvironment::Get()) );
#endif
}

/**
 * Check for default script existence, and set the first one found to be loaded.
 *
 * @author	Jetha Chan
 * @date	7/09/2013
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
		if(ZLFileSys::CheckFileExists(SLEDGE_NAMESPACE::DefaultScriptLocations[idx]))
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
 * Processes the command-line arguments passed to the host.
 *
 * @author	Jetha Chan
 * @date	7/09/2013
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