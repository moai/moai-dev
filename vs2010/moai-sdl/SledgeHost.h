#ifndef SLEDGE_HOST
#define SLEDGE_HOST

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
#include "SledgeInputManager.h"
#include "SledgeInputHandler.h"



namespace SDLUserEventType {
	enum UserEventType {
		UET_UPDATE,
		UET_RENDER
	};
}

/**
 */
class SledgeHost
{
public:
	static const unsigned int WINDOWTITLE_LENGTH = 255;

	SledgeHost(int argc, char** arg);
	~SledgeHost();

	AKUContextID m_AkuContext;

	
	/** Make this class instance the active one for the purposes of AKU
		callbacks.
	 */
	void makeActive();



#pragma region Callbacks
	unsigned int SDLCallback_OnTickFunc(unsigned int millisec, void* param);
	void AKUCallback_OpenWindowFunc(const char* title, int width, int height);
#pragma endregion
#pragma region CallbackWrappers
	static unsigned int SDLCallbackWrapper_OnTickFunc(unsigned int millisec, void* param);
	static void AKUCallbackWrapper_OpenWindowFunc(const char* title, int width, int height);
#pragma endregion
	

protected:
	char m_WindowTitleBase[WINDOWTITLE_LENGTH];
	char m_WindowTitle[WINDOWTITLE_LENGTH];
	vec2u m_WindowPos;
	vec2u m_WindowSize;
	//bool m_bHasWindow;

	SDL_TimerID m_ActiveTimer;
	SDL_TimerID m_ActiveTimer2;
	unsigned int m_TimerInterval;
	unsigned int m_TimerInterval2;
	unsigned int m_TimerInterval3;

	unsigned int m_Counter;

private:
	void runGame();
	bool doInit();

	void ProcessUserEvent(int type);
	void _doAkuUpdate();
	void _doAkuRender();

	SDL_Window* m_SDLWindow;
	SDL_GLContext m_SDLGLContext;

	SledgeInputManager* m_InputManager;

	double m_DeltaTime;
};

#endif