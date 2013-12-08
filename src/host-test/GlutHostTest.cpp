// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <host-test/GlutHostTest.h>
#include <lua-headers/moai_lua.h>
#include <moai-http-client/host.h>
#include <moai-sim/host.h>
#include <moai-test/host.h>
#include <moai-util/host.h>

#if LUA_VERSION_NUM < 502
	#ifdef MOAI_WITH_LUAEXT
		#undef MOAI_WITH_LUAEXT
		#define MOAI_WITH_LUAEXT 0
	#endif
#endif

#ifdef _WIN32

	#include <glut.h>
	
	#if MOAI_WITH_FOLDER_WATCHER
		#include <FolderWatcher-win.h>
	#endif
#else
	#ifdef MOAI_OS_LINUX
	  	#include <GL/glut.h>
	#else
		#include <GLUT/glut.h>
	#endif
#endif

#if MOAI_WITH_LUAEXT
	#include <moai-luaext/host.h>
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
	
	AKUFinalizeUtil ();
	AKUFinalizeSim ();
	
	#if MOAI_WITH_HTTP_CLIENT
		AKUFinalizeHttpClient ();
	#endif
	
	AKUFinalize ();
}

//----------------------------------------------------------------//
int GlutHostTest ( int argc, char** argv ) {

	atexit ( _cleanup );
	
	glutInit ( &argc, argv );

	AKUCreateContext ();
	
	AKUInitializeUtil ();
	AKUInitializeSim ();
	
	#if MOAI_WITH_HTTP_CLIENT
		AKUInitializeHttpClient ();
	#endif
	
	#if MOAI_WITH_LUAEXT
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuafilesystem ();
		AKUExtLoadLuasocket ();
		AKUExtLoadLuasql ();
	#endif

	AKUTestInit ();

	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
	AKURunData ( moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
	
	// parse the commands
	int total = argc - 1;
	int i = 1;

	for ( ; i < total; ++i ) {
		
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

