#include "SDLHost2.h"


#ifndef HAS_MOAILUA
#define HAS_MOAILUA
#include <lua-headers/moai_lua.h>
#endif

#ifdef SDLHOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif

SdlHost2::SdlHost2(int argc, char** arg)
{
	memset(&m_WindowPos, 0, sizeof(vec2i));
	memset(&m_WindowSize, 0, sizeof(vec2i));
	memset(&m_WindowTitle, 0, WINDOWTITLE_LENGTH);

	doInit();

}

SdlHost2::~SdlHost2()
{
	// teardown
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


	return true;
}