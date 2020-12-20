// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-core/host.h>
#include <moai-sdl/host.h>
#include <host-modules/aku_modules.h>

#include "SDLHostGL.h"

//================================================================//
// SDLHostGL
//================================================================//

//----------------------------------------------------------------//
SDLHostGL::SDLHostGL () {
}

//----------------------------------------------------------------//
SDLHostGL::~SDLHostGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void SDLHostGL::SDLAbstractHost_MainLoopDidFinish () {
	
//	AKUDeleteContext ( AKUGetContext ());

	//SDL_GL_DeleteContext ( context );  removing context here leaves the window and it explodes with gl error during glflush

	// TODO: presumably the context needs to be deleted?
	// TODO: what happens in multi-threaded mode?
}

//----------------------------------------------------------------//
void SDLHostGL::SDLAbstractHost_MainLoopDidStart () {
}

//----------------------------------------------------------------//
void SDLHostGL::SDLAbstractHost_OpenWindow ( const char* title, int width, int height ) {

	if ( !this->mWindow ) {
		
		this->GetDeviceToPixelScale ( this->mDeviceScaleX, this->mDeviceScaleY );
	
		this->mWindow = SDL_CreateWindow (
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			( int )( width / this->mDeviceScaleX ),
			( int )( height / this->mDeviceScaleY ),
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
		);
		this->mGLContext = SDL_GL_CreateContext ( this->mWindow );

		SDL_GL_SetAttribute ( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
		SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLESAMPLES, 16 );

		SDL_GL_SetSwapInterval ( 1 );

		AKUSetViewSize ( width, height );
		AKUSdlSetWindow ( this->mWindow );

		AKUDetectGfxContext ();

		// Enable keyboard text input.
		// According to the SDL documentation, this will open an on-screen keyboard on some platforms.
		// Currently we're using the SDL host for desktop platforms only, so this should not be a problem.
		SDL_StartTextInput ();
	}
	else {
		SDL_SetWindowSize ( this->mWindow, width, height );
	}
}

//----------------------------------------------------------------//
void SDLHostGL::SDLAbstractHost_Render () {

	AKURender ();
	SDL_GL_SwapWindow ( this->mWindow );
}
