// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <aku/AKU.h>
#include <moaicore/moaicore.h>
#include <work/work-moai-vfs.h>

#ifdef _WIN32
	#include <glut.h>
#else
	#include <GLUT/glut.h>
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

static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;

static const int GLUT_TIMER_INTERVAL = 16;

//================================================================//
// glut callbacks
//================================================================//

//----------------------------------------------------------------//
static void _onKeyDown ( unsigned char key, int x, int y ) {
	( void )x;
	( void )y;
	
	AKUEnqueueKeyboardEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, key, true );
}

//----------------------------------------------------------------//
static void _onKeyUp ( unsigned char key, int x, int y ) {
	( void )x;
	( void )y;
	
	AKUEnqueueKeyboardEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, key, false );
}

//----------------------------------------------------------------//
static void _onMouseButton ( int button, int state, int x, int y ) {
	( void )x;
	( void )y;
	
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
}

//----------------------------------------------------------------//
void _onTimer ( int millisec ) {

	glutTimerFunc ( GLUT_TIMER_INTERVAL, _onTimer, GLUT_TIMER_INTERVAL );
	
	AKUUpdate ();
	
	#ifdef AKUGLUT_USE_FMOD
		AKUFmodUpdate ();
	#endif
	
	glutPostRedisplay ();
}

//================================================================//
// AKU callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );
void	_AKUStartGameLoopFunc			();

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
	
	sHasWindow = true;
	
	sWinX = 180;
	sWinY = 100;
	sWinWidth = width;
	sWinHeight = height;
	
	sWinWidth = width;
	sWinHeight = height;
	
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize ( sWinWidth, sWinHeight );
	glutInitWindowPosition ( sWinX, sWinY );
	glutCreateWindow ( title );

	glutIgnoreKeyRepeat ( 1 );

	glutKeyboardFunc ( _onKeyDown );
	glutKeyboardUpFunc ( _onKeyUp );
	
	glutMouseFunc ( _onMouseButton );
	glutMotionFunc ( _onMouseDrag );
	glutPassiveMotionFunc ( _onMouseMove );
	
	glutDisplayFunc ( _onPaint );
	glutReshapeFunc ( _onReshape );
	
	glutTimerFunc ( GLUT_TIMER_INTERVAL, _onTimer, GLUT_TIMER_INTERVAL );
	
	AKUDetectGfxContext ();
	AKUSetScreenSize ( width, height );
}

//----------------------------------------------------------------//
void _AKUStartGameLoopFunc () {

	if ( sHasWindow ) {
		glutMainLoop ();
	}
}

//================================================================//
// GlutHost
//================================================================//

//----------------------------------------------------------------//
int work_moai_vfs ( int argc, char** argv ) {

	glutInit ( &argc, argv );

	AKUCreateContext ();

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

	AKUMountVirtualDirectory ( "test", "anim-basic.zip" );
	AKUSetWorkingDirectory ( "test/anim-basic" );
	
	AKURunScript ( "main.lua" );

	return 0;
}

