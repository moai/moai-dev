// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <moai-core/host.h>
#include <moai-sdl/host.h>
#include <host-modules/aku_modules.h>

#ifdef MOAI_OS_WINDOWS
    #include <windows.h>
#elif defined ( MOAI_OS_LINUX )
    #include <X11/Xlib.h>      //XOpenDisplay,etc
    #include <xcb/xcb.h>
    #include <xcb/xcb_aux.h> 
    #include <xcb/randr.h>
#endif


#ifdef __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
	#include <limits.h>

	extern void AppleGetDeviceToPixelScale ( float& w, float& h );
#endif

#include "SDLAbstractHost.h"

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
		TOTAL,
	};
}

//================================================================//
// aku callbacks
//================================================================//

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUShowCursor ( void* userdata ) {
	UNUSED ( userdata );
	SDL_ShowCursor ( 1 );
}

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUHideCursor ( void* userdata ) {
	UNUSED ( userdata );
	SDL_ShowCursor ( 0 );
}

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUEnterFullscreenModeFunc ( void* userdata ) {
	SDLAbstractHost* self = ( SDLAbstractHost* )userdata;
	assert ( self );

    // videomode change
    SDL_SetWindowFullscreen ( self->mWindow, SDL_WINDOW_FULLSCREEN );
	self->SetScreenSize( SDL_GetCurrentDisplayMode );
}

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUExitFullscreenModeFunc ( void* userdata ) {
	SDLAbstractHost* self = ( SDLAbstractHost* )userdata;
	assert ( self );

    // videomode change
    SDL_SetWindowFullscreen ( self->mWindow, 0 );
	self->SetScreenSize ( SDL_GetDesktopDisplayMode );
}

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUOpenWindowFunc ( const char* title, int width, int height, void* userdata ) {
	SDLAbstractHost* self = ( SDLAbstractHost* )userdata;
	assert ( self );
	
	self->SDLAbstractHost_OpenWindow ( title, width, height );
}

//----------------------------------------------------------------//
void SDLAbstractHost::_AKUSetTextInputRectFunc ( int xMin, int yMin, int xMax, int yMax, void* userdata ) {
	UNUSED ( userdata );

	SDL_Rect sdlRect;
	sdlRect.x = xMin;
	sdlRect.y = yMin;
	sdlRect.w = xMax - xMin;
	sdlRect.h = yMax - yMin;
	
	SDL_SetTextInputRect ( &sdlRect );
}

//================================================================//
// SDL callbacks
//================================================================//

//----------------------------------------------------------------//
void SDLAbstractHost::_SDL_LogOutputFunction ( void *userdata, int category, SDL_LogPriority priority, const char *message ) {
	UNUSED ( userdata );
	UNUSED ( category );
	UNUSED ( priority );

	printf ( "%s", message );
}

//================================================================//
// SDLAbstractHost
//================================================================//

//----------------------------------------------------------------//
void SDLAbstractHost::Finalize () {

	AKUDeleteContext ( AKUGetContext ());
	AKUModulesAppFinalize ();
	AKUAppFinalize ();
	
	SDL_Quit ();
}

//----------------------------------------------------------------//
void SDLAbstractHost::GetDeviceToPixelScale ( float& w, float& h ) {

	#ifdef __APPLE__
		AppleGetDeviceToPixelScale ( w, h );
	#else
		w = 1.0f;
		h = 1.0f;
	#endif
}

//----------------------------------------------------------------//
void SDLAbstractHost::Init ( int argc, char** argv ) {

	SDL_LogSetOutputFunction ( _SDL_LogOutputFunction, 0 );
	SDL_LogSetPriority ( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE );

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

	SetScreenSize ( SDL_GetDesktopDisplayMode );

    SetScreenDpi ();

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

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc, this );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc, this );

	AKUSetFunc_ShowCursor ( _AKUShowCursor, this );
	AKUSetFunc_HideCursor ( _AKUHideCursor, this );

	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc, this );
	
	AKUSetFunc_SetTextInputRect ( _AKUSetTextInputRectFunc, this );
	
	#ifdef __APPLE__
	
		//are we a bundle?
		CFBundleRef bref = CFBundleGetMainBundle ();
		if ( bref == NULL || CFBundleGetIdentifier ( bref ) == NULL ) {
			AKUModulesParseArgs ( argc, argv );
		}
		else {
		
			CFURLRef bundleurl = CFBundleCopyResourcesDirectoryURL ( bref );
			assert ( bundleurl != NULL );
			
			UInt8 buf [ PATH_MAX ];
			CFURLGetFileSystemRepresentation ( bundleurl, true, buf, PATH_MAX );

			AKUSetWorkingDirectory(( const char * )buf );
			AKULoadFuncFromFile ( "bootstrap.lua" );
			AKUCallFunc ();
		}
	#else
	
		AKUModulesParseArgs ( argc, argv );
	
	#endif

	atexit ( Finalize ); // do this *after* SDL_Init
}

//----------------------------------------------------------------//
void SDLAbstractHost::MainLoop () {
	
	assert ( this->mWindow );
	
	this->SDLAbstractHost_MainLoopDidStart ();

	// TODO: array's of Joysticks
	Joystick * joystick0 = NULL;

	if ( SDL_NumJoysticks () < 1 ) {
		std::cerr << "No Joysticks connected." << std::endl;
	}
	else {
		joystick0 = new Joystick ( 0 ); // 0 == first joystick of system.
		if ( joystick0->isOpen () || !joystick0->Open ()) {
			delete joystick0;
			joystick0 = NULL;
		}
	}
		
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
					if ( sdlEvent.key.repeat ) break;
					int moaiKeyCode = GetMoaiKeyCode ( sdlEvent );
					AKUEnqueueKeyboardKeyEvent ( InputDeviceID::DEVICE, InputSensorID::KEYBOARD, moaiKeyCode, sdlEvent.key.type == SDL_KEYDOWN );
					break;
				}
				
				case SDL_TEXTINPUT: {
					AKUEnqueueKeyboardTextEvent ( InputDeviceID::DEVICE, InputSensorID::KEYBOARD, sdlEvent.text.text );
					break;
				}
				case SDL_TEXTEDITING: {
					char *text = sdlEvent.edit.text;
					int start = sdlEvent.edit.start;
					int length = sdlEvent.edit.length;
					
					AKUEnqueueKeyboardEditEvent ( InputDeviceID::DEVICE, InputSensorID::KEYBOARD, text, start, length, SDL_TEXTEDITINGEVENT_TEXT_SIZE );
					break;
				}
				
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

				case SDL_MOUSEWHEEL:

						if ( sdlEvent.wheel.which != SDL_TOUCH_MOUSEID ) {
                            //const int32_t x = sdlEvent.wheel.x;
							const int32_t y = sdlEvent.wheel.y;

							//XXX: x or y ?
							AKUEnqueueWheelEvent ( InputDeviceID::DEVICE, InputSensorID::MOUSE_WHEEL, ( float )y );
						}
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
				case SDL_JOYAXISMOTION:
					
						//TODO: array's of Joysticks

						if ( sdlEvent.jaxis.which == 0 /* what joystick? */  && joystick0 != NULL ) {

                            const Joystick::AXIS_MOTION & axis = joystick0->HandleAxisMotion(sdlEvent);
					        AKUEnqueueJoystickEvent ( InputDeviceID::DEVICE, InputSensorID::JOYSTICK, ( float )axis.x, ( float )axis.y );
						}
					break;

				case SDL_MOUSEMOTION:
				
					AKUEnqueuePointerEvent (
						InputDeviceID::DEVICE,
						InputSensorID::POINTER,
						( int )( sdlEvent.motion.x * this->mDeviceScaleX ),
						( int )( sdlEvent.motion.y * this->mDeviceScaleY )
					);
					break;

				case SDL_WINDOWEVENT:
					// Note: this only support fullscreen videomode change.
					// Not for the event "resize", by default SDL main window is not resizable(at least Linux)
					if ( sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
							sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ) {
						
						AKUSetViewSize(sdlEvent.window.data1, sdlEvent.window.data2);
					}
					else if ( sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST ) {
						// If the focus is lost, it must be stopped.
						SDL_StopTextInput();
						
						// Clear Editing text.
						AKUEnqueueKeyboardEditEvent ( InputDeviceID::DEVICE, InputSensorID::KEYBOARD, "", 0, 0, SDL_TEXTEDITINGEVENT_TEXT_SIZE );
					}
					else if ( sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED ) {
						// Start when the focus is given.
						// TODO:Restored the edit text.
						SDL_StartTextInput();
					}
					break;
			}
		}
		
		AKUModulesUpdate ();
		this->SDLAbstractHost_Render ();
		
		Uint32 frameDelta = ( Uint32 )( AKUGetSimStep () * 1000.0 );
		Uint32 currentFrame = SDL_GetTicks ();
		Uint32 delta = currentFrame - lastFrame;
		
		if ( delta < frameDelta ) {
			SDL_Delay ( frameDelta - delta );
		}
		lastFrame = SDL_GetTicks ();
	}
	
	this->SDLAbstractHost_MainLoopDidFinish ();
}

//----------------------------------------------------------------//
void SDLAbstractHost::PrintMoaiVersion () {

	char buffer [ 255 ];
	
	printf ( "%s\n", AKUGetMoaiVersion ( buffer, sizeof ( buffer )));
	printf ( "%s\n", AKUGetMoaiCommit ( buffer, sizeof ( buffer )));
	printf ( "%s\n", AKUGetMoaiAuthor ( buffer, sizeof ( buffer )));
}

//----------------------------------------------------------------//
void SDLAbstractHost::Run ( int argc, char** argv ) {

	this->Init ( argc, argv );

	if ( this->mWindow ) {
		this->MainLoop ();
	}
}

//----------------------------------------------------------------//
SDLAbstractHost::SDLAbstractHost () :
	mDeviceScaleX ( 1 ),
	mDeviceScaleY ( 1 ),
	mWindow ( NULL ) {
}

//----------------------------------------------------------------//
SDLAbstractHost::~SDLAbstractHost () {
}

//----------------------------------------------------------------//
void SDLAbstractHost::SetScreenSize ( DisplayModeFunc func ) {

    SDL_DisplayMode dm;

    if ( func != NULL && func ( 0, &dm ) == 0 ) {
    	AKUSetScreenSize ( dm.w, dm.h );
    }
}


//----------------------------------------------------------------//
void SDLAbstractHost::SetScreenDpi () {

	#ifdef MOAI_OS_WINDOWS

		HDC hDC = GetWindowDC ( NULL );
		int widthInMm = GetDeviceCaps ( hDC, HORZSIZE );
		double widthInInches = widthInMm / 25.4;
		int widthInPixels = GetDeviceCaps ( hDC, HORZRES );
		AKUSetScreenDpi (( int )( widthInPixels / widthInInches ));

	#elif defined ( MOAI_OS_LINUX )

		char* display_name = getenv( "DISPLAY" );
		if ( !display_name ) return;

		int nscreen = 0;
		xcb_connection_t* conn = xcb_connect( display_name, &nscreen );
		if ( !conn ) return;

		xcb_screen_t* screen = xcb_aux_get_screen( conn, nscreen );

		double widthInInches = screen->width_in_millimeters / 25.4;
		int widthInPixels = screen->width_in_pixels;

		AKUSetScreenDpi(( int )widthInPixels / widthInInches );

		xcb_disconnect( conn );
	
	#endif
}
