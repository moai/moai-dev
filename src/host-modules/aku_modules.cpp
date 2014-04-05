// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <lua-headers/moai_lua.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAppFinalize () {
	
	#if AKU_WITH_BOX2D
		AKUBox2DAppFinalize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppFinalize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppFinalize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppFinalize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppFinalize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestAppFinalize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzAppFinalize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppFinalize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAppInitialize () {

	#if AKU_WITH_BOX2D
		AKUBox2DAppInitialize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppInitialize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestAppInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzAppInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesContextInitialize () {

	#if AKU_WITH_BOX2D
		AKUBox2DContextInitialize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExContextInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerContextInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtContextInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimContextInitialize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestContextInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzContextInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilContextInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsContextInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesParseArgs ( int argc, char** argv ) {

	#if AKU_WITH_TEST

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
		
	#else
	
		if ( argc < 2 ) {
			AKURunScript ( "main.lua" );
		}
		else {

			AKUSetArgv ( argv );

			for ( int i = 1; i < argc; ++i ) {
				char* arg = argv [ i ];
				if (( strcmp ( arg, "-s" ) == 0 ) && ( ++i < argc )) {
					char* script = argv [ i ];
					AKURunString ( script );
				}
				else {
					AKURunScript ( arg );
				}
			}
		}
	
	#endif
}

//----------------------------------------------------------------//
void AKUModulesRunLuaAPIWrapper () {

	AKURunData ( moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
}

//----------------------------------------------------------------//
void AKUModulesUpdate () {

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerUpdate ();
	#endif
	
	#if AKU_WITH_FMOD_EX
		AKUFmodExUpdate ();
	#endif
	
	#if AKU_WITH_HARNESS
		AKUHarnessUpdate ()
	#endif

	#if AKU_WITH_SIM
		AKUUpdate ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsUpdate ();
	#endif
}

void AKUModulesPause () {
	#if AKU_WITH_UNTZ
		AKUUntzPause ();
	#endif
	
	#if AKU_WITH_SIM
		AKUPause ( true );
	#endif

}

void AKUModulesResume () {
	#if AKU_WITH_UNTZ
		AKUUntzResume ();
	#endif

	#if AKU_WITH_SIM
		AKUPause ( false );
	#endif
}