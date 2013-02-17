#ifndef SDLHOST2
#define SDLHOST2

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

namespace SDLInputDeviceID {
	enum {
		DEVICE,
		PAD_0,
		PAD_1,
		PAD_2,
		PAD_3,
		TOTAL
	};
}

namespace SDLInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL
	};
}

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