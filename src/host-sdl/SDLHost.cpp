// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <moai-core/host.h>
#include <host-modules/aku_modules.h>
#include <host-sdl/SDLHost.h>

#include <SDL.h>

#ifdef MOAI_OS_WINDOWS
    //#include <windows.h>
#elif defined(MOAI_OS_LINUX)
    #include <X11/Xlib.h>      //XOpenDisplay,etc
    #include <xcb/xcb.h>
    #include <xcb/xcb_aux.h> 
    #include <xcb/randr.h>
#endif

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
		MOUSE_WHEEL,
		JOYSTICK,
		TOUCH,
		TOTAL,
		JOYSTICK_BUTTON,
	};
}

static SDL_Window* sWindow = 0;
static int sWinWidth;
static int sWinHeight;
typedef int ( *DisplayModeFunc ) (int, SDL_DisplayMode *);
static void SetScreenSize ( DisplayModeFunc func);

//================================================================//
// joystick
//================================================================//
struct AXIS_MOTION {
    int x;
    int y;
    AXIS_MOTION() : x(0), y(0) { }
};
AXIS_MOTION axis_motion;
static const int JOYSTICK_DEAD_ZONE = 8000;
void JoystickHandleAxisMotion(const SDL_Event & event) {
    if( event.jaxis.axis == 0 ) { //X axis
        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            axis_motion.x = -1;
        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            axis_motion.x =  1;
        } else { 
            axis_motion.x = 0;
        }
    } else if ( event.jaxis.axis == 1 ) { // Y axis
        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            axis_motion.y = -1;
        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            axis_motion.y =  1;
        } else {
            axis_motion.y = 0;
        }
    }
}

//================================================================//
// aku callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );
void    _AKUShowCursor					();
void    _AKUHideCursor					();

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

	/* // uncomment this to debug fullscreen problems
	int c = SDL_GetNumVideoDisplays();
	for(int i=0;i<c;i++) {
		SDL_Rect rect;
		SDL_GetDisplayBounds(0, &rect);
		printf("%d: %d x %d\n", i, rect.w, rect.h);
	}*/

	// warning: may not work properly on linux unless packages are installed at compile time
	//  sudo apt-get install libxrandr-dev libx11-dev libxinerama-dev
	//  http://getmoai.com/forums/post17419.html#p17419
    SDL_SetWindowFullscreen(sWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SetScreenSize( SDL_GetCurrentDisplayMode );
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

    SDL_SetWindowFullscreen(sWindow, 0);
	SetScreenSize( SDL_GetDesktopDisplayMode );
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	
	if ( !sWindow ) {
		sWindow = SDL_CreateWindow ( title, 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			width, height, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		sWinWidth = width;
		sWinHeight = height;
		SDL_GL_CreateContext ( sWindow );
		SDL_GL_SetSwapInterval ( 1 );
		AKUDetectGfxContext ();
		AKUSetScreenSize ( width, height );
	}
}

//----------------------------------------------------------------//
void _AKUShowCursor () {
	SDL_ShowCursor(1);
}

//----------------------------------------------------------------//
void _AKUHideCursor () {
	SDL_ShowCursor(0);
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
	AKUSetInputDeviceWheel			( InputDeviceID::DEVICE, InputSensorID::MOUSE_WHEEL,	"mouseWheel" );
	AKUSetInputDeviceJoystick       ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK,	    "joystick" );
	AKUSetInputDeviceJoystick       ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK_BUTTON,"joystickButton" );
	
	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
	
	AKUModulesParseArgs ( argc, argv );
	
	if ( SDL_NumJoysticks() >= 1 ) {
		SDL_JoystickOpen(0);
	}

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
					/*
					 * TODO:
					 * SDL_JOYBALLMOTION joystick trackball motion
					 * SDL_JOYHATMOTION	 joystick hat position change
					 * SDL_JOYBUTTONDOWN joystick button pressed
					 * SDL_JOYBUTTONUP	 joystick button released
					 * SDL_JOYDEVICEADDED	joystick connected
					 * SDL_JOYDEVICEREMOVED	joystick disconnected
					 * */

				case SDL_MOUSEWHEEL: 

						if ( sdlEvent.wheel.which != SDL_TOUCH_MOUSEID ) {
                            //const int32_t x = sdlEvent.wheel.x;
							const int32_t y = sdlEvent.wheel.y; 

							//XXX: x or y ?
							AKUEnqueueWheelEvent ( InputDeviceID::DEVICE, InputSensorID::MOUSE_WHEEL, y );
						}
					break;

				case SDL_JOYAXISMOTION:
						
						//TODO: array's of Joysticks

						//printf("SDL_JOYAXISMOTION\n");
                        //const Joystick::AXIS_MOTION & axis = joystick0->HandleAxisMotion(sdlEvent);
                        JoystickHandleAxisMotion(sdlEvent);

				        AKUEnqueueJoystickEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK, axis_motion.x, axis_motion.y );

					break;

				case SDL_JOYBUTTONDOWN:

					//printf("SDL_JOYBUTTONDOWN\n");
					AKUEnqueueJoystickEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK, sdlEvent.jbutton.button+2, (sdlEvent.jbutton.state == SDL_PRESSED) );
//					AKUEnqueueJoystickButtonEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK_BUTTON, sdlEvent.jbutton.button, (sdlEvent.jbutton.state == SDL_PRESSED) );
					break;

				case SDL_JOYBUTTONUP:
						
					//printf("SDL_JOYBUTTONUP\n");
					AKUEnqueueJoystickEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK, sdlEvent.jbutton.button+2, (sdlEvent.jbutton.state == SDL_PRESSED) );
				    //AKUEnqueueJoystickButtonEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK_BUTTON, sdlEvent.jbutton.button, (sdlEvent.jbutton.state == SDL_PRESSED) );
					break;

				case SDL_WINDOWEVENT:
					// Note: this only support fullscreen videomode change.
					// Not for the event "resize", by default SDL main window is not resizable(at least Linux)
					if ( sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
							sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ) {
						
						AKUSetViewSize(sdlEvent.window.data1, sdlEvent.window.data2);
					/*} else if ( sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST ) {
						// If the focus is lost, it must be stopped.
						SDL_StopTextInput();
						
					} else if ( sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED ) {
						// Start when the focus is given.
						// TODO:Restored the edit text.
						SDL_StartTextInput();*/
					}
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
void SetScreenSize(DisplayModeFunc func ) {

    SDL_DisplayMode dm;

    if ( func != NULL && func( 0, &dm ) == 0 ) {
    	AKUSetScreenSize(dm.w, dm.h);
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
