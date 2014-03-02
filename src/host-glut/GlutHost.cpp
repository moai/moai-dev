// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <host-glut/GlutHost.h>
#include <moai-core/host.h>

#include <string.h>
#include <host-modules/aku_modules.h>

#ifdef FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#if LUA_VERSION_NUM >= 502
	#ifdef MOAI_WITH_LUAEXT
		#undef MOAI_WITH_LUAEXT
		#define MOAI_WITH_LUAEXT 0
	#endif
#endif

#define UNUSED(p) (( void )p)

#ifdef _WIN32
	#if MOAI_WITH_FOLDER_WATCHER
		#include <FolderWatcher-win.h>
	#endif
#endif

#ifdef __APPLE__

	#include <OpenGL/OpenGL.h>

	#if MOAI_WITH_FOLDER_WATCHER
		#include <FolderWatcher-mac.h>
	#endif
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
		TOUCH,
		TOTAL,
	};
}

static bool sHasWindow = false;
static bool sExitFullscreen = false;
// static bool sDynamicallyReevaluateLuaFiles = false;

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
static void _onMultiButton( int touch_id, int x, int y, int button, int state ) {
	AKUEnqueueTouchEvent (
		GlutInputDeviceID::DEVICE,
		GlutInputDeviceSensorID::TOUCH,
		touch_id,
		state == GLUT_DOWN,
		( float )x,
		( float )y
	);
}

//----------------------------------------------------------------//
static void _onMultiMotion( int touch_id, int x, int y ) {
	AKUEnqueueTouchEvent (
		GlutInputDeviceID::DEVICE,
		GlutInputDeviceSensorID::TOUCH,
		touch_id,
		true,
		( float )x,
		( float )y
	);
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

	AKUSetScreenSize ( w, h );
	AKUSetViewSize ( w, h );
}

//----------------------------------------------------------------//
static void _onTimer ( int millisec ) {
	UNUSED ( millisec );

	double fSimStep = AKUGetSimStep ();
	int timerInterval = ( int )( fSimStep * 1000.0 );
	glutTimerFunc ( timerInterval, _onTimer, timerInterval );


	AKUModulesUpdate ();

	glutPostRedisplay ();
}

//================================================================//
// aku callbacks
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
void _AKUShowCursor () {
	glutSetCursor( GLUT_CURSOR_INHERIT ) ;
}

//----------------------------------------------------------------//
void _AKUHideCursor () {
	glutSetCursor( GLUT_CURSOR_NONE ) ;
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
		if (!glutGet(GLUT_DISPLAY_MODE_POSSIBLE))
		{
		    exit(1);
		}
		glutCreateWindow ( title );
		sHasWindow = true;
	}

	#if !AKU_WITH_TEST
		glutIgnoreKeyRepeat ( 1 );

		glutKeyboardFunc ( _onKeyDown );
		glutKeyboardUpFunc ( _onKeyUp );
		glutSpecialFunc ( _onSpecialFunc );

		glutMouseFunc ( _onMouseButton );
		glutMotionFunc ( _onMouseDrag );

		#ifdef FREEGLUT_STATIC 
		glutMultiButtonFunc ( _onMultiButton );
		glutMultiMotionFunc ( _onMultiMotion );
		#endif
		
		glutPassiveMotionFunc ( _onMouseMove );
    #endif

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
// GlutHost
//================================================================//

//----------------------------------------------------------------//
static void _cleanup () {



	AKUModulesAppFinalize ();
	AKUAppFinalize ();

  // if ( sDynamicallyReevaluateLuaFiles ) {
  //  #ifdef _WIN32
  //    winhostext_CleanUp ();
  //  #elif __APPLE__
  //    FWStopAll ();
  //  #endif
  // }
}

//----------------------------------------------------------------//
static void _printMoaiVersion () {

	static const int length = 255;
	char version [ length ];
	AKUGetMoaiVersion ( version, length );
	printf ( "%s\n", version );
}

//----------------------------------------------------------------//
int GlutHost ( int argc, char** argv ) {

	_printMoaiVersion ();

	#ifdef _DEBUG
		printf ( "DEBUG BUILD\n" );
	#endif

	atexit ( _cleanup );

	glutInit ( &argc, argv );

	GlutRefreshContext ( argc, argv);

 AKUModulesParseArgs ( argc, argv );

	if ( sHasWindow ) {
		glutTimerFunc ( 0, _onTimer, 0 );
		glutMainLoop ();
	}
	return 0;
}

void GlutRefreshContext (int argc, char** argv) {

    AKUAppInitialize ();
	AKUModulesAppInitialize ();

	AKUCreateContext ();

    AKUModulesContextInitialize ();
	AKUModulesRunLuaAPIWrapper ();

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
	AKUSetFunc_ShowCursor ( _AKUShowCursor );
	AKUSetFunc_HideCursor ( _AKUHideCursor );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );

   

}
