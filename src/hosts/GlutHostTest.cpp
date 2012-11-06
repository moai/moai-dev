// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <aku/AKU.h>
#include <aku/AKU-luaext.h>
#include <aku/AKU-test.h>
#include <lua-headers/moai_lua.h>
#include <GlutHostTest.h>

#ifdef _WIN32
	#include <glut.h>
#else
	#include <GLUT/glut.h>
#endif

#define UNUSED(p) (( void )p)

static bool sHasWindow = false;

static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;

//================================================================//
// glut callbacks
//================================================================//

//----------------------------------------------------------------//
static void _onPaint () {
	
	AKURender ();
	glutSwapBuffers ();
}

//----------------------------------------------------------------//
static void _onReshape( int w, int h ) {

	glutReshapeWindow ( w, h );
	AKUSetScreenSize ( w, h );
}

//----------------------------------------------------------------//
static void _onTimer ( int millisec ) {
	UNUSED ( millisec );

	int timerInterval = ( int )( AKUGetSimStep () * 1000.0 );
	glutTimerFunc ( timerInterval, _onTimer, timerInterval );
	
	AKUUpdate ();
	
	glutPostRedisplay ();
}

//================================================================//
// AKU callbacks
//================================================================//

void	_AKUOpenWindowFunc				( const char* title, int width, int height );


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
	
	glutDisplayFunc ( _onPaint );
	glutReshapeFunc ( _onReshape );
	
	AKUDetectGfxContext ();
	AKUSetScreenSize ( width, height );
}

//================================================================//
// GlutHostTest
//================================================================//

//----------------------------------------------------------------//
static void _cleanup () {
	
	AKUFinalize ();
}

//----------------------------------------------------------------//
int GlutHostTest ( int argc, char** argv ) {

	atexit ( _cleanup );
	
	glutInit ( &argc, argv );

	AKUCreateContext ();
	
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();

	AKUTestInit ();

	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
	AKURunBytecode ( moai_lua, moai_lua_SIZE );
	
	// parse the commands
	int total = argc - 1;

	for ( int i = 1; i < total; ++i ) {
		
		char* arg = argv [ i ];
		
		if ( arg [ 0 ] != '-' ) break;
		
		// filter file
		if ( strcmp ( arg, "-f" ) == 0 ) {
			AKUTestSetFilterFile ( argv [ ++i ]);
		}
		
		// filter
		if ( strcmp ( arg, "-F" ) == 0 ) {
			AKUTestSetFilter ( argv [ ++i ]);
		}
		
		// results
		if ( strcmp ( arg, "-r" ) == 0 ) {
			AKUTestSetResultsFile ( argv [ ++i ]);
		}
		
		// staging
		if ( strcmp ( arg, "-s" ) == 0 ) {
			AKUTestSetStaging ();
		}
		
		// test
		if ( strcmp ( arg, "-t" ) == 0 ) {
			AKUTestRunTest ( argv [ ++i ]);
		}

		// xml results
		if ( strcmp ( arg, "-x" ) == 0 ) {
			AKUTestSetXmlResultsFile ( argv [ ++i ]);
		}
	}
	
	for ( ; i < argc; ++i ) {
		AKUTestRunScript ( argv [ i ]);
	}
	
	if ( sHasWindow ) {
		glutTimerFunc ( 0, _onTimer, 0 );
		glutMainLoop ();
	}
	return 0;
}

