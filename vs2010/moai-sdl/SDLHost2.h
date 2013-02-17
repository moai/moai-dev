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

struct vec2i
{
	unsigned int x;
	unsigned int y;
};

// @todo	Consider whether this should really be here.


class SdlHost2
{
public:
	static const unsigned int WINDOWTITLE_LENGTH = 255;

	SdlHost2(int argc, char** arg);
	~SdlHost2();

	AKUContextID m_AkuContext;

protected:
	char m_WindowTitle[WINDOWTITLE_LENGTH];
	vec2i m_WindowPos;
	vec2i m_WindowSize;

	//void refreshContext();



private:
	bool doInit();

};

#endif