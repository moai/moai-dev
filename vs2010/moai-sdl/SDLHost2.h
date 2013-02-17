#ifndef SDLHOST2
#define SDLHOST2

#include <string>

#include <SDL.h>
#define SDL_main main
//#undef main

#ifndef UNUSED
#define UNUSED(p) (( void )p)
#endif

#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"
#include "SDLInputManager.h"


// @todo	Consider whether this should really be here.


class SdlHost2
{
public:
	static const unsigned int WINDOWTITLE_LENGTH = 255;

	SdlHost2(int argc, char** arg);
	~SdlHost2();

	AKUContextID m_AkuContext;

	void AKUCallback_OpenWindowFunc(const char* title, int width, int height);

	static void AKUCallbackWrapper_OpenWindowFunc(const char* title, int width, int height);

	void makeActive();

protected:
	char m_WindowTitle[WINDOWTITLE_LENGTH];
	vec2u m_WindowPos;
	vec2u m_WindowSize;

	//void refreshContext();

private:
	bool doInit();

	SdlInputManager* m_InputManager;
};

#endif