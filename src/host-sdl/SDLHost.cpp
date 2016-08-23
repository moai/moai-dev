// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

#include <SDL.h>

#include "SDLHost.h"
#include "SDLJoystick.h"
#include "SDLKeyCodeMapping.h"

#ifdef __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
	#include <limits.h>
#endif

//#include <OpenGL/gl.h>
//#include <OpenGL/glext.h>
#include <zl-gfx/ZLGfx-gles.h>

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
//		TOUCH,
		TOTAL,
	};
}

// run with multithreaded host or not
//#define GFX_ASYNC 1

// counters for testing async timing issues (dropped frames)
static const int SIM_UPDATE_INTERVAL		= 1;
static const int LOAD_UPDATE_INTERVAL		= 1;
static const int RENDER_UPDATE_INTERVAL		= 1;

static float sDeviceScaleX = 1.0f;
static float sDeviceScaleY = 1.0f;

static SDL_Window* sWindow = 0;

typedef int ( *DisplayModeFunc ) (int, SDL_DisplayMode *);

static void SetScreenSize ( DisplayModeFunc func);

//================================================================//
// WorkerThreadInfo
//================================================================//
class WorkerThreadInfo {
public:

	enum {
		LOADING_FLAG		= 0x01,
		RENDER_FLAG			= 0x02,
	};
	
private:

	static SDL_mutex*		sDisplayListMutex;
	//static SDL_GLContext	sSharedContext;

	SDL_Thread*		mThread;
	SDL_cond*		mCondition;
	SDL_mutex*		mConditionMutex;
	bool			mIsDone;
	
	SDL_GLContext	mContext;
	
	int				mMask;
	
	//----------------------------------------------------------------//
	static int ThreadMain ( void* data ) {

		SDL_LockMutex ( sDisplayListMutex );

		WorkerThreadInfo* info = ( WorkerThreadInfo* )data;

		SDL_GL_MakeCurrent ( sWindow, info->mContext );
		
		if ( info->mMask & LOADING_FLAG ) {
			AKUDisplayListEnable ( AKU_DISPLAY_LIST_LOADING );
		}
		
		if ( info->mMask & RENDER_FLAG ) {
			AKUDetectGfxContext ();
			AKUDisplayListEnable ( AKU_DISPLAY_LIST_DRAWING );
		}

		SDL_UnlockMutex ( sDisplayListMutex );

		int loadUpdateCounter = 0;
		int renderUpdateCounter = 0;

		while ( info->mIsDone == false ) {
		
			SDL_LockMutex ( info->mConditionMutex );
			SDL_CondWait ( info->mCondition, info->mConditionMutex );
			SDL_UnlockMutex ( info->mConditionMutex );
			
			
			if ( loadUpdateCounter >= ( LOAD_UPDATE_INTERVAL - 1 )) {
				loadUpdateCounter = 0;
			
				if ( info->mMask & LOADING_FLAG ) {
				
					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
					AKUDisplayListProcess ( AKU_DISPLAY_LIST_LOADING );
					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
				}
			}
			
			if ( renderUpdateCounter >= ( RENDER_UPDATE_INTERVAL - 1 )) {
				renderUpdateCounter = 0;
			
				if ( info->mMask & RENDER_FLAG ) {
					
					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
					AKUDisplayListProcess ( AKU_DISPLAY_LIST_DRAWING );
					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
					
					SDL_GL_SwapWindow ( sWindow );
				}
			}
			
			loadUpdateCounter++;
			renderUpdateCounter++;
		}
		
		SDL_GL_DeleteContext ( info->mContext );
		
		return 0;
	}

public:

	//----------------------------------------------------------------//
	static void DisplayListBeginPhase ( int list ) {
	
		SDL_LockMutex ( sDisplayListMutex );
		AKUDisplayListBeginPhase ( list );
		SDL_UnlockMutex ( sDisplayListMutex );
	}
	
	//----------------------------------------------------------------//
	static void DisplayListEndPhase ( int list ) {
	
		SDL_LockMutex ( sDisplayListMutex );
		AKUDisplayListEndPhase ( list );
		SDL_UnlockMutex ( sDisplayListMutex );
	}

	//----------------------------------------------------------------//
	void Signal () {
	
		bool hasContent = false;
		
		if (( this->mMask & LOADING_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_LOADING )) {
			hasContent = true;
		}
		
		if (( this->mMask & RENDER_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_DRAWING )) {
			hasContent = true;
		}
	
		if ( hasContent ) {
			SDL_LockMutex ( this->mConditionMutex );
			SDL_CondSignal ( this->mCondition );
			SDL_UnlockMutex ( this->mConditionMutex );
		}
	}
	
	//----------------------------------------------------------------//
	void Start ( int mask, const char* name ) {

		this->mContext = SDL_GL_CreateContext ( sWindow );
		SDL_GL_SetSwapInterval ( 1 );

		this->mMask					= mask;
		this->mCondition			= SDL_CreateCond ();
		this->mConditionMutex		= SDL_CreateMutex ();
		this->mThread				= SDL_CreateThread ( WorkerThreadInfo::ThreadMain, name, this );
	}
	
	//----------------------------------------------------------------//
	void Stop () {
	
		SDL_LockMutex ( this->mConditionMutex );
		this->mIsDone = true;
		SDL_CondSignal ( this->mCondition );
		SDL_UnlockMutex ( this->mConditionMutex );
		SDL_WaitThread ( this->mThread, 0 );
	}
	
	//----------------------------------------------------------------//
	WorkerThreadInfo () :
		mThread ( 0 ),
		mCondition ( 0 ),
		mConditionMutex ( 0 ),
		mIsDone ( false ) {
		
		if ( !sDisplayListMutex ) {
			sDisplayListMutex = SDL_CreateMutex ();
		}
	}
};

SDL_mutex* WorkerThreadInfo::sDisplayListMutex = 0;

//================================================================//
// aku callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );
void    _AKUShowCursor					();
void    _AKUHideCursor					();
void	_AKUSetTextInputRectFunc		( int xMin, int yMin, int xMax, int yMax );

//----------------------------------------------------------------//
void _AKUShowCursor () {
	SDL_ShowCursor(1);
}

//----------------------------------------------------------------//
void _AKUHideCursor () {
	SDL_ShowCursor(0);
}

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

    //videomode change
    SDL_SetWindowFullscreen(sWindow, SDL_WINDOW_FULLSCREEN);
	SetScreenSize( SDL_GetCurrentDisplayMode );
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

    //videomode change
    SDL_SetWindowFullscreen(sWindow, 0);
	SetScreenSize( SDL_GetDesktopDisplayMode );
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	
	if ( !sWindow ) {
		
		GetDeviceToPixelScale ( sDeviceScaleX, sDeviceScaleY );
	
		SDL_GL_SetAttribute ( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
		sWindow = SDL_CreateWindow (
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			( int )( width / sDeviceScaleX ),
			( int )( height / sDeviceScaleY ),
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
		);

		AKUSetViewSize ( width, height );
		AKUSdlSetWindow ( sWindow );

		// Enable keyboard text input.
		// According to the SDL documentation, this will open an on-screen keyboard on some platforms.
		// Currently we're using the SDL host for desktop platforms only, so this should not be a problem.
		SDL_StartTextInput ();
	}
	else {
		SDL_SetWindowSize ( sWindow, width, height );
	}
}

//----------------------------------------------------------------//
void _AKUSetTextInputRectFunc ( int xMin, int yMin, int xMax, int yMax ) {

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

void	_SDL_LogOutputFunction	( void *userdata, int category, SDL_LogPriority priority, const char *message );

//----------------------------------------------------------------//
void _SDL_LogOutputFunction ( void *userdata, int category, SDL_LogPriority priority, const char *message ) {
	UNUSED ( userdata );
	UNUSED ( category );
	UNUSED ( priority );

	printf ( "%s", message );
}

//================================================================//
// helpers
//================================================================//

static void			Finalize				();
static void			Init					( int argc, char** argv );
static void			MainLoop				();
static void			PrintMoaiVersion		();
static void			SetScreenDpi			();

//----------------------------------------------------------------//
void Finalize () {

	AKUModulesAppFinalize ();
	AKUAppFinalize ();
	
	SDL_Quit ();
}

#ifndef __APPLE__

//----------------------------------------------------------------//
void GetDeviceToPixelScale ( float& w, float& h ) {

	w = 1.0f;
	h = 1.0f;
}

#endif

//----------------------------------------------------------------//
void Init ( int argc, char** argv ) {

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

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );

	AKUSetFunc_ShowCursor ( _AKUShowCursor );
	AKUSetFunc_HideCursor ( _AKUHideCursor );

	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
	
	AKUSetFunc_SetTextInputRect ( _AKUSetTextInputRectFunc );
	
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
void MainLoop () {
	
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLESAMPLES, 16 );
	
	#if GFX_ASYNC
	
		WorkerThreadInfo loadingThread;
		loadingThread.Start ( WorkerThreadInfo::LOADING_FLAG, "Loading Thread" );
		
		WorkerThreadInfo renderThread;
		renderThread.Start ( WorkerThreadInfo::RENDER_FLAG, "Render Thread" );

		SDL_GL_MakeCurrent ( sWindow, NULL );
	
	#else
	
		SDL_GLContext context = SDL_GL_CreateContext ( sWindow );
		SDL_GL_SetSwapInterval ( 1 );
	
		AKUDetectGfxContext ();
	
	#endif

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
	
	int simUpdateCounter = 0;
	
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
						( int )( sdlEvent.motion.x * sDeviceScaleX ),
						( int )( sdlEvent.motion.y * sDeviceScaleY )
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
		
		if ( simUpdateCounter >= ( SIM_UPDATE_INTERVAL - 1 )) {
			simUpdateCounter = 0;
		
			#if GFX_ASYNC
			
				WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_LOGIC_PHASE );
				AKURender ();
				WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_LOGIC_PHASE );
				
				loadingThread.Signal ();
				renderThread.Signal ();
			
			#else
			
				AKURender ();
				SDL_GL_SwapWindow ( sWindow );
			
			#endif
			
			simUpdateCounter++;
		}
		
		Uint32 frameDelta = ( Uint32 )( AKUGetSimStep () * 1000.0 );
		Uint32 currentFrame = SDL_GetTicks ();
		Uint32 delta = currentFrame - lastFrame;
		
		if ( delta < frameDelta ) {
			SDL_Delay ( frameDelta - delta );
		}
		lastFrame = SDL_GetTicks ();
	}
	
	#if GFX_ASYNC
	
		loadingThread.Stop ();
		renderThread.Stop ();
	#else
	
		AKUDeleteContext ( AKUGetContext ());
		SDL_GL_DeleteContext ( context );
	#endif
}

//----------------------------------------------------------------//
void PrintMoaiVersion () {

	static const int length = 255;
	char version [ length ];
	AKUGetMoaiVersion ( version, length );
	printf ( "%s\n", version );
}

//----------------------------------------------------------------//
void SetScreenSize ( DisplayModeFunc func ) {

    SDL_DisplayMode dm;

    if ( func != NULL && func( 0, &dm ) == 0 ) {
    	AKUSetScreenSize(dm.w, dm.h);
    }
}


//----------------------------------------------------------------//
void SetScreenDpi() {

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
