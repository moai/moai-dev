// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>
#include <GlutHost.h>
#include <string.h>

#define UNUSED(p) (( void )p)

#ifdef GLUTHOST_USE_DEBUGGER
	#include <aku/AKU-debugger.h>
#endif

#ifdef GLUTHOST_USE_FMOD
	#include <aku/AKU-fmod.h>
#endif

#ifdef GLUTHOST_USE_LUAEXT
	#include <aku/AKU-luaext.h>
#endif

#ifdef GLUTHOST_USE_UNTZ
	#include <aku/AKU-untz.h>
#endif

#ifdef GLUTHOST_USE_AUDIOSAMPLER
	#include <aku/AKU-audiosampler.h>
#endif

#ifdef GLUTHOST_USE_PARTICLE_PRESETS
	#include <ParticlePresets.h>
#endif

#ifdef _WIN32
	#include <glut.h>
	#include <FolderWatcher-win.h>
#else
	#include <GLUT/glut.h>
#endif

#ifdef __APPLE__
	#include <FolderWatcher-mac.h>
	#include <OpenGL/OpenGL.h>
#endif

namespace GlutInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace GlutInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}

static bool sHasWindow = false;
static bool sExitFullscreen = false;
static bool sDynamicallyReevaluateLuaFiles = false;

static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;
static int sModifiers;

//================================================================//
// helper functions
//================================================================//

//----------------------------------------------------------------//
static void _updateModifiers () {
	int newModifiers = glutGetModifiers ();
	int changedModifiers = newModifiers ^ sModifiers;
	if ( changedModifiers & GLUT_ACTIVE_SHIFT ) {
		AKUEnqueueKeyboardShiftEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_SHIFT) != 0 );
	}
	if ( changedModifiers & GLUT_ACTIVE_CTRL ) {
		AKUEnqueueKeyboardControlEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_CTRL) != 0 );
	}
	if ( changedModifiers & GLUT_ACTIVE_ALT ) {
		AKUEnqueueKeyboardAltEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_ALT) != 0 );
	}
	sModifiers = newModifiers;
}

//================================================================//
// glut callbacks
//================================================================//

//----------------------------------------------------------------//
static void _onKeyDown ( unsigned char key, int x, int y ) {
	( void )x;
	( void )y;

	_updateModifiers ();
	
	AKUEnqueueKeyboardEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, key, true );
}

//----------------------------------------------------------------//
static void _onKeyUp ( unsigned char key, int x, int y ) {
	( void )x;
	( void )y;

	_updateModifiers ();
	
	AKUEnqueueKeyboardEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, key, false );
}

//----------------------------------------------------------------//
static void _onSpecialFunc ( int key, int x, int y ) {
	( void )x;
	( void )y;
	
	_updateModifiers ();

	if ( key == GLUT_KEY_F1 ) {
	
		static bool toggle = true;
		
		if ( toggle ) {
			AKUReleaseGfxContext ();
		}
		else {
			AKUDetectGfxContext ();
		}
		toggle = !toggle;
	}
	
	if ( key == GLUT_KEY_F2 ) {
	
		AKUSoftReleaseGfxResources ( 0 );
	}
}

//----------------------------------------------------------------//
static void _onMouseButton ( int button, int state, int x, int y ) {
	( void )x;
	( void )y;

	_updateModifiers ();
	
	switch ( button ) {
		case GLUT_LEFT_BUTTON:
			AKUEnqueueButtonEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_LEFT, ( state == GLUT_DOWN ));
		break;
		case GLUT_MIDDLE_BUTTON:
			AKUEnqueueButtonEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_MIDDLE, ( state == GLUT_DOWN ));
		break;
		case GLUT_RIGHT_BUTTON:
			AKUEnqueueButtonEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_RIGHT, ( state == GLUT_DOWN ));
		break;
	}
}

//----------------------------------------------------------------//
static void _onMouseDrag ( int x, int y ) {

	AKUEnqueuePointerEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::POINTER, x, y );
}

//----------------------------------------------------------------//
static void _onMouseMove ( int x, int y ) {

	AKUEnqueuePointerEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::POINTER, x, y );
}

//----------------------------------------------------------------//
static void _onPaint () {
	
	AKURender ();
	glutSwapBuffers ();
}

//----------------------------------------------------------------//
static void _onReshape( int w, int h ) {

	if ( sExitFullscreen ) {
	
		w = sWinWidth;
		h = sWinHeight;
		
		sExitFullscreen = false;
	}

	glutReshapeWindow ( w, h );
	AKUSetScreenSize ( w, h );
	AKUSetViewSize ( w, h );
}

//----------------------------------------------------------------//
static void _onTimer ( int millisec ) {
	UNUSED ( millisec );

	int timerInterval = ( int )( AKUGetSimStep () * 1000.0 );
	glutTimerFunc ( timerInterval, _onTimer, timerInterval );
	
	#ifdef GLUTHOST_USE_DEBUGGER
        AKUDebugHarnessUpdate ();
    #endif
	
	AKUUpdate ();
	
	#ifdef AKUGLUT_USE_FMOD
		AKUFmodUpdate ();
	#endif
	
	if ( sDynamicallyReevaluateLuaFiles ) {		
		#ifdef _WIN32
			winhostext_Query ();
		#elif __APPLE__
			FWReloadChangedLuaFiles ();
		#endif
	}
	
	glutPostRedisplay ();
}

//================================================================//
// AKU callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

	if ( sHasWindow ) {

		sExitFullscreen = false;
		glutFullScreen ();
	}
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

	if ( sHasWindow ) {

		sExitFullscreen = true;
		glutPositionWindow ( sWinX, sWinY );
		glutReshapeWindow ( sWinWidth, sWinHeight );
	}
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {

	
	sWinX = 180;
	sWinY = 100;
	sWinWidth = width;
	sWinHeight = height;
	
	sWinWidth = width;
	sWinHeight = height;
	
	if ( !sHasWindow ) {
		glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
		glutInitWindowSize ( sWinWidth, sWinHeight );
		glutInitWindowPosition ( sWinX, sWinY );
		glutCreateWindow ( title );
		sHasWindow = true;
	}

	glutIgnoreKeyRepeat ( 1 );

	glutKeyboardFunc ( _onKeyDown );
	glutKeyboardUpFunc ( _onKeyUp );
	glutSpecialFunc ( _onSpecialFunc );
	
	glutMouseFunc ( _onMouseButton );
	glutMotionFunc ( _onMouseDrag );
	glutPassiveMotionFunc ( _onMouseMove );
	
	glutDisplayFunc ( _onPaint );
	glutReshapeFunc ( _onReshape );
	
	AKUDetectGfxContext ();
	AKUSetScreenSize ( width, height );

#ifdef __APPLE__
	GLint sync = 1;
	CGLContextObj ctx = CGLGetCurrentContext();
	CGLSetParameter (ctx, kCGLCPSwapInterval, &sync);
#endif
}

//================================================================//
// AKU-debugger callbacks
//================================================================//

#if GLUTHOST_USE_DEBUGGER
    void    _AKUErrorTracebackFunc      ( const char* message, lua_State* L, int level );

    void _AKUErrorTracebackFunc ( const char* message, lua_State* L, int level ) {
        AKUDebugHarnessHandleError ( message, L, level );
    }
#endif

//================================================================//
// GlutHost
//================================================================//

//----------------------------------------------------------------//
static void _cleanup () {

	// TODO:
	// don't call this on windows; atexit conflict with untz
	// possible to fix?
	//AKUClearMemPool ();
	
	AKUFinalize ();
	
	if ( sDynamicallyReevaluateLuaFiles ) {
		#ifdef _WIN32
			winhostext_CleanUp ();
		#elif __APPLE__
			FWStopAll ();
		#endif
	}
}

//----------------------------------------------------------------//
int GlutHost ( int argc, char** argv ) {

	// TODO: integrate this nicely with host
	//AKUInitMemPool ( 100 * 1024 * 1024 );
	atexit ( _cleanup );

	glutInit ( &argc, argv );

	GlutRefreshContext ();

	char* lastScript = NULL;

	if ( argc < 2 ) {
		AKURunScript ( "main.lua" );
	}
	else {

		AKUSetArgv ( argv );

		for ( int i = 1; i < argc; ++i ) {
			char* arg = argv [ i ];
			if ( strcmp( arg, "-e" ) == 0 ) {
				sDynamicallyReevaluateLuaFiles = true;
			}
			else if ( strcmp( arg, "-s" ) == 0 && ++i < argc ) {
				char* script = argv [ i ];
				AKURunString ( script );
			}
			else {
				AKURunScript ( arg );
				lastScript = arg;
			}
		}
	}
	
	//assuming that the last script is the entry point we watch for that directory and its subdirectories
	if ( lastScript && sDynamicallyReevaluateLuaFiles ) {
		#ifdef _WIN32
			winhostext_WatchFolder ( lastScript );
		#elif __APPLE__
			FWWatchFolder( lastScript );
		#endif
	}
	
	if ( sHasWindow ) {
		glutTimerFunc ( 0, _onTimer, 0 );
		glutMainLoop ();
	}
	return 0;
}

void GlutRefreshContext () {
	AKUContextID context = AKUGetContext ();
	if ( context ) {
		AKUDeleteContext ( context );
	}
	AKUCreateContext ();

	#ifdef GLUTHOST_USE_FMOD
		AKUFmodLoad ();
	#endif
	
	#ifdef GLUTHOST_USE_LUAEXT
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuafilesystem ();
		AKUExtLoadLuasocket ();
		AKUExtLoadLuasql ();
	#endif
	
	#ifdef GLUTHOST_USE_UNTZ
		AKUUntzInit ();
	#endif
	
	#ifdef GLUTHOST_USE_AUDIOSAMPLER
        AKUAudioSamplerInit();	
	#endif
	
	#ifdef GLUTHOST_USE_PARTICLE_PRESETS
		ParticlePresets ();
	#endif

	AKUSetInputConfigurationName ( "AKUGlut" );

	AKUReserveInputDevices			( GlutInputDeviceID::TOTAL );
	AKUSetInputDevice				( GlutInputDeviceID::DEVICE, "device" );
	
	AKUReserveInputDeviceSensors	( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::TOTAL );
	AKUSetInputDeviceKeyboard		( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD,		"keyboard" );
	AKUSetInputDevicePointer		( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::POINTER,		"pointer" );
	AKUSetInputDeviceButton			( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_LEFT,	"mouseLeft" );
	AKUSetInputDeviceButton			( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_MIDDLE,	"mouseMiddle" );
	AKUSetInputDeviceButton			( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_RIGHT,	"mouseRight" );

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );

	#ifdef GLUTHOST_USE_DEBUGGER
		AKUSetFunc_ErrorTraceback ( _AKUErrorTracebackFunc );
		AKUDebugHarnessInit ();
	#endif

	AKURunBytecode ( moai_lua, moai_lua_SIZE );
}
