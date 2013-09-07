#include "stdafx.h"
#ifndef SLEDGE_HOST
#define SLEDGE_HOST

#include "UtilityTypes.h"
#include "SledgeCore.h"
#include "SledgeInputManager.h"
#include "SledgeInputHandler.h"
#include "SledgeGraphicsHandler.h"

#include <moaicore/MOAIEnvironment.h>


/**
 * SDL 2.0-based MOAI host, written for Stirfire Studios.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 */
class SledgeHost
{
private:
	char			m_WindowTitle[SLEDGE_NAMESPACE::WINDOWTITLE_LENGTH];
	char			m_WindowTitleBase[SLEDGE_NAMESPACE::WINDOWTITLE_LENGTH];
	vec2<u32>		m_WindowPos;
	vec2<u32>		m_WindowSize;

	AKUContextID	m_AkuContext;

	SDL_Window*		m_SDLWindow;
	SDL_GLContext	m_SDLGLContext;

	SledgeInputManager* m_InputManager;

	char*			m_LastScript;
	bool			m_bDoLuaDynamicReeval;

	s32				m_StatusCode;
	static void*	s_CurrentSledgeHostInstance;
public:
	explicit	SledgeHost				( s32 argc, char** argv );
				~SledgeHost				(  );
	//----------------------------------------------------------------//
	s32			CheckStatus				(  );
	void		MakeActive				(  );
	void		RunGame					(  );

private:
	//----------------------------------------------------------------//
	void		DoSystemInit			(  );
	void		DoSystemTeardown		(  );
	bool		DoDefaultScriptCheck	(  );
	bool		DoProcessArgs			( s32 argc, char* argv[] );
	// CALLBACKS -----------------------------------------------------//
	static void Callback_OpenWindowFunc	( const char* title, s32 w, s32 h );
	//----------------------------------------------------------------//
	void		DoOpenWindow			( const char* title, s32 w, s32 h );
};

#endif