//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <host-sdl/SDLHost.h>

#if _MSC_VER
	#include <Crtdbg.h>
#endif

#ifdef _WIN32
	#include <Windows.h>
#endif

//----------------------------------------------------------------//
#ifdef _WIN32
	int CALLBACK WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
		int argc = __argc;
		char** argv = __argv;

		#if _MSC_VER
			// affects assert.h assert()
			_set_error_mode ( _OUT_TO_MSGBOX );
		    
			// affects crtdbg.h _ASSERT, _ASSERTE, etc
			_CrtSetReportMode ( _CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW | _CRTDBG_MODE_FILE );
			_CrtSetReportFile ( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
		#endif

		SDLHost ( argc, argv );
	}
#endif

//----------------------------------------------------------------//
int main ( int argc, char** argv ) {

	SDLHost ( argc, argv );
    return 0;
}
