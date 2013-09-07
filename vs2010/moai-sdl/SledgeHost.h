#include "stdafx.h"
#ifndef SLEDGE_HOST
#define SLEDGE_HOST

#include "UtilityTypes.h"
#include "SledgeCore.h"
#include "SledgeInputManager.h"
#include "SledgeInputHandler.h"
#include "SledgeGraphicsHandler.h"

#include <moaicore/MOAIEnvironment.h>

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
	u32 SDLCallback_OnTickFunc(unsigned int millisec, void* param);
	void AKUCallback_OpenWindowFunc(const char* title, int width, int height);
#pragma endregion
#pragma region CallbackWrappers
	static u32 SDLCallbackWrapper_OnTickFunc(unsigned int millisec, void* param);
	static void AKUCallbackWrapper_OpenWindowFunc(const char* title, int width, int height);
#pragma endregion
	

protected:
	char		m_WindowTitleBase[WINDOWTITLE_LENGTH];
	char		m_WindowTitle[WINDOWTITLE_LENGTH];
	vec2<u32>	m_WindowPos;
	vec2<u32>	m_WindowSize;

	SDL_TimerID m_ActiveTimer;
	SDL_TimerID m_ActiveTimer2;
	u32			m_TimerInterval;
	u32			m_TimerInterval2;
	u32			m_TimerInterval3;

	u32			m_Counter;

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