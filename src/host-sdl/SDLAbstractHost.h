// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	SDLABSTRACTHOST_H
#define	SDLABSTRACTHOST_H

#include <SDL.h>

#include "SDLJoystick.h"
#include "SDLKeyCodeMapping.h"

#define UNUSED(p) (( void )p)

//================================================================//
// SDLAbstractHost
//================================================================//
class SDLAbstractHost {
protected:

	typedef int ( *DisplayModeFunc )( int, SDL_DisplayMode* );

	float 			mDeviceScaleX;
	float 			mDeviceScaleY;

	SDL_Window*		mWindow;

	//----------------------------------------------------------------//
	static void		_AKUEnterFullscreenModeFunc		( void* userdata );
	static void		_AKUExitFullscreenModeFunc		( void* userdata );
	static void		_AKUOpenWindowFunc				( const char* title, int width, int height, void* userdata );
	static void		_AKUShowCursor					( void* userdata );
	static void		_AKUHideCursor					( void* userdata );
	static void		_AKUSetTextInputRectFunc		( int xMin, int yMin, int xMax, int yMax, void* userdata );

	//----------------------------------------------------------------//
	static void		_SDL_LogOutputFunction			( void *userdata, int category, SDL_LogPriority priority, const char *message );

	//----------------------------------------------------------------//
	static void		Finalize						();
	void			Init							( int argc, char** argv );
	void			PrintMoaiVersion				();
	void			SetScreenSize					( DisplayModeFunc func );
	void			SetScreenDpi					();
	void			MainLoop						();
	
	//----------------------------------------------------------------//
	virtual void	SDLAbstractHost_MainLoopDidFinish	() = 0;
	virtual void	SDLAbstractHost_MainLoopDidStart	() = 0;
	virtual void	SDLAbstractHost_OpenWindow			( const char* title, int width, int height ) = 0;
	virtual void	SDLAbstractHost_Render				() = 0;

public:

	//----------------------------------------------------------------//
	void			GetDeviceToPixelScale			( float& w, float& h );
	void			Run								( int argc, char** arg );
					SDLAbstractHost					();
	virtual			~SDLAbstractHost				();
};

#endif
