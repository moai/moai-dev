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

	m_InputManager = new SdlInputManager();

	doInit();

	AKURunBytecode ( moai_lua, moai_lua_SIZE );


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


}

void SdlHost2::makeActive()
{
	CurrentSdlHost2Instance = (void*)this;
}

SdlHost2::~SdlHost2()
{
	// teardown
	delete m_InputManager;
}

bool SdlHost2::doInit()
{
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

	// refresh the AKU context
	m_AkuContext = AKUGetContext();
	if(m_AkuContext) {
		AKUDeleteContext(m_AkuContext);
	}
	m_AkuContext = AKUCreateContext();

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

void SdlHost2::AKUCallback_OpenWindowFunc
	(const char* title, int width, int height)
{
	printf("OpenWindowFunc!");
}

void SdlHost2::AKUCallbackWrapper_OpenWindowFunc
	(const char* title, int width, int height)
{
	SdlHost2* obj = (SdlHost2*)CurrentSdlHost2Instance;

	obj->AKUCallback_OpenWindowFunc(title, width, height);
}