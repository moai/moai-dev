// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <moai-core/host.h>
#include <host-modules/aku_modules.h>
#include <host-sdl/SDLHost.h>

#include <SDL.h>

#define UNUSED(p) (( void )p)

namespace InputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace InputSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOUCH,
		TOTAL,
	};
}

static SDL_Window* sWindow = 0;

//================================================================//
// aku callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

	printf ( "UNSUPPORTED\n" );
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

	printf ( "UNSUPPORTED\n" );
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	
	if ( !sWindow ) {
		sWindow = SDL_CreateWindow ( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		SDL_GL_CreateContext ( sWindow );
		SDL_GL_SetSwapInterval ( 1 );
		AKUDetectGfxContext ();
		AKUSetScreenSize ( width, height );
	}
}

//================================================================//
// helpers
//================================================================//

static void	Finalize			();
static void	Init				( int argc, char** argv );
static void	MainLoop			();
static void	PrintMoaiVersion	();

//----------------------------------------------------------------//
void Finalize () {

	AKUModulesAppFinalize ();
	AKUAppFinalize ();
	
	SDL_Quit ();
}

//----------------------------------------------------------------//
void Init ( int argc, char** argv ) {

	SDL_Init ( SDL_INIT_EVERYTHING );
	PrintMoaiVersion ();

	#ifdef _DEBUG
		printf ( "DEBUG BUILD\n" );
	#endif

	AKUAppInitialize ();
	AKUModulesAppInitialize ();

	AKUCreateContext ();
	AKUModulesContextInitialize ();
	AKUModulesRunLuaAPIWrapper ();

	AKUSetInputConfigurationName ( "SDL" );

	AKUReserveInputDevices			( InputDeviceID::TOTAL );
	AKUSetInputDevice				( InputDeviceID::DEVICE, "device" );
	
	AKUReserveInputDeviceSensors	( InputDeviceID::DEVICE, InputSensorID::TOTAL );
	AKUSetInputDeviceKeyboard		( InputDeviceID::DEVICE, InputSensorID::KEYBOARD,		"keyboard" );
	AKUSetInputDevicePointer		( InputDeviceID::DEVICE, InputSensorID::POINTER,		"pointer" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_LEFT,		"mouseLeft" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_MIDDLE,	"mouseMiddle" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_RIGHT,	"mouseRight" );

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
	
	AKUModulesParseArgs ( argc, argv );
	
	atexit ( Finalize ); // do this *after* SDL_Init
}

//----------------------------------------------------------------//
void MainLoop () {

	Uint32 lastFrame = SDL_GetTicks();
	
	bool running = true;
	while ( running ) {
	
		SDL_Event sdlEvent;
		
		while ( SDL_PollEvent ( &sdlEvent )) {  
			   
			switch ( sdlEvent.type )  {
			
				case SDL_QUIT:
				
					running = false;
					break;
				
				case SDL_KEYDOWN:
				case SDL_KEYUP:	{
					int key = sdlEvent.key.keysym.sym;
					if (key & 0x40000000) key = (key & 0x3FFFFFFF) + 256;
			
					AKUEnqueueKeyboardEvent ( InputDeviceID::DEVICE, InputSensorID::KEYBOARD, key, sdlEvent.key.type == SDL_KEYDOWN );

				} 	break;
					
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
	
					switch ( sdlEvent.button.button ) {
					
						case SDL_BUTTON_LEFT:
							AKUEnqueueButtonEvent ( InputDeviceID::DEVICE, InputSensorID::MOUSE_LEFT, ( sdlEvent.type == SDL_MOUSEBUTTONDOWN ));
							break;
						
						case SDL_BUTTON_MIDDLE:
							AKUEnqueueButtonEvent ( InputDeviceID::DEVICE, InputSensorID::MOUSE_MIDDLE, ( sdlEvent.type == SDL_MOUSEBUTTONDOWN ));
							break;
						
						case SDL_BUTTON_RIGHT:
							AKUEnqueueButtonEvent ( InputDeviceID::DEVICE, InputSensorID::MOUSE_RIGHT, ( sdlEvent.type == SDL_MOUSEBUTTONDOWN ));
							break;
					}

					break;

				case SDL_MOUSEMOTION:
				
					AKUEnqueuePointerEvent ( InputDeviceID::DEVICE, InputSensorID::POINTER, sdlEvent.motion.x, sdlEvent.motion.y );
					break;
			}
		}
		
		AKUModulesUpdate ();
		
		AKURender ();
		SDL_GL_SwapWindow ( sWindow );
		
		Uint32 frameDelta = ( Uint32 )( AKUGetSimStep () * 1000.0 );
		Uint32 currentFrame = SDL_GetTicks ();
		Uint32 delta = currentFrame - lastFrame;
		
		if ( delta < frameDelta ) {
			SDL_Delay ( frameDelta - delta );
		}
		lastFrame = SDL_GetTicks();
	}
}

//----------------------------------------------------------------//
void PrintMoaiVersion () {

	static const int length = 255;
	char version [ length ];
	AKUGetMoaiVersion ( version, length );
	printf ( "%s\n", version );
}

//================================================================//
// SDLHost
//================================================================//

//----------------------------------------------------------------//
int SDLHost ( int argc, char** argv ) {

	Init ( argc, argv );

	if ( sWindow ) {
		MainLoop ();
	}

	return 0;
}
