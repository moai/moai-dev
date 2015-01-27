// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <host-modules/aku_modules_util.h>
#include <lua-headers/moai_lua.h>
#include <lua-headers/moai_test_mgr_lua.h>

//================================================================//
// local
//================================================================//

int _parseArgStringAndCall ( char* exeName, char* scriptName, int argc, char** argv, int i );

//----------------------------------------------------------------//
int _parseArgStringAndCall ( char* exeName, char* scriptName, int argc, char** argv, int i ) {

	int mode = AKU_AS_ARGS;
	char* args = 0;

	if ( i < ( argc - 1 )) {
	
		char* arg = argv [ i + 1 ];
	
		if (( strcmp ( arg, "-a" ) == 0 )) {
			i += 2;
			args = argv [ i ];
			mode = AKU_AS_ARGS;
		}
		else if (( strcmp ( arg, "-p" ) == 0 )) {
			i += 2;
			args = argv [ i ];
			mode = AKU_AS_PARAMS;
		}
	}
	
	if ( args ) {
		AKUCallFuncWithArgString ( exeName, scriptName, args, mode );
	}
	else {
		AKUCallFunc ();
	}
	
	return i;
}

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesParseArgs ( int argc, char** argv ) {

	if ( argc < 2 ) {
		AKULoadFuncFromFile ( "main.lua" );
		AKUCallFunc ();
	}
	else {
	
		for ( int i = 1; i < argc; ++i ) {
		
			char* arg = argv [ i ];
			
			if (( strcmp ( arg, "-s" ) == 0 )) {
			
				char* script = argv [ ++i ];
				AKULoadFuncFromString ( script );
				i = _parseArgStringAndCall ( argv [ 0 ], 0, argc, argv, i );
			}
			else if (( strcmp ( arg, "-f" ) == 0 )) {
				
				char* filename = argv [ ++i ];
				AKULoadFuncFromFile ( filename );
				i = _parseArgStringAndCall ( argv [ 0 ], filename, argc, argv, i );
			}
			else {
				
				int argc2		= argc - 2;
				char** argv2	= argc2 > 0 ? &argv [ i + 1 ] : 0;
				
				AKULoadFuncFromFile ( arg );
				AKUCallFuncWithArgArray ( argv [ 0 ], argv [ i ], argc2, argv2, AKU_AS_ARGS );
				break;
			}
		}
	}
}

//----------------------------------------------------------------//
void AKUModulesRunLuaAPIWrapper () {

	AKULoadFuncFromBuffer ( moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
	AKUCallFunc ();
	
	AKULoadFuncFromBuffer ( moai_test_mgr_lua, moai_test_mgr_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
	AKUCallFunc ();
}
