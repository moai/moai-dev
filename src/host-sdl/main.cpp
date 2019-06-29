//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>

#if _MSC_VER
	#include <Crtdbg.h>
#endif

#ifdef _WIN32
	#include <Windows.h>
#endif

#include <host-modules/aku_modules.h>

#if AKU_WITH_GFX_GL
	#include "SDLHostGL.h"
#elif AKU_WITH_GFX_VK
	#include "SDLHostVK.h"
#endif

//----------------------------------------------------------------//
void _run ( int argc, char** argv );
void _run ( int argc, char** argv ) {

	#if AKU_WITH_GFX_GL
		SDLHostGL ().Run ( argc, argv );
	#elif AKU_WITH_GFX_VK
		SDLHostVK ().Run ( argc, argv );
	#endif
}

//----------------------------------------------------------------//
#ifdef _WIN32
	int CALLBACK WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
		(( void )hInstance );
		(( void )hPrevInstance );
		(( void )lpCmdLine );
		(( void )nCmdShow );

		int argc = __argc;
		char** argv = __argv;

		#if _MSC_VER
			// affects assert.h assert()
			_set_error_mode ( _OUT_TO_MSGBOX );
		    
			// affects crtdbg.h _ASSERT, _ASSERTE, etc
			_CrtSetReportMode ( _CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW | _CRTDBG_MODE_FILE );
			_CrtSetReportFile ( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
		#endif

		_run ( argc, argv );
	}
#endif

//----------------------------------------------------------------//
int main ( int argc, char** argv ) {

	_run ( argc, argv );
    return 0;
}
