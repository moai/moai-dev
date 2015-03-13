// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sdl/MOAIDialogSDL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIDialogSDL::_show ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDialogSDL, "SST" )
	
	cc8* title			= state.GetValue < cc8* >( 1, "" );
	cc8* message		= state.GetValue < cc8* >( 2, "" );
	
	size_t nButtons = state.GetTableSize ( 3 );
	SDL_MessageBoxButtonData* buttons = ( SDL_MessageBoxButtonData* )alloca ( nButtons * sizeof ( SDL_MessageBoxButtonData ));

	for ( u32 i = 0; i < nButtons; ++i ) {
	
		SDL_MessageBoxButtonData& button = buttons [ i ];
		int buttonIdx = i + 1;
		
		button.buttonid = buttonIdx;
		button.flags = 0;
		button.text = "";
		
		state.GetField ( 3, buttonIdx );
		
		int type = lua_type ( state, -1 );
		
		if ( type == LUA_TSTRING ) {
			button.text = state.GetValue < cc8* >( -1, "" );
		}
		
		if ( type == LUA_TTABLE ) {
			button.text = state.GetField < cc8* >( -1, "text", "" );
			button.flags |= state.GetField < bool >( -1, "default", false ) ? SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT : 0;
		}
		
		state.Pop ();
	}
	
    const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,		// flags
		self->mWindow,					// window
		title,							// title
		message,						// message
		nButtons,						// numbuttons
		buttons,						// buttons
		NULL							// colorScheme
    };
	
    int buttonid;
	
    if ( SDL_ShowMessageBox ( &messageboxdata, &buttonid ) < 0 ) {
        SDL_Log ( "error displaying message box" );
        return 0;
    }
	
	if ( buttonid > 0 ) {
	
		bool handledByButton = false;
	
		if ( state.GetFieldWithType ( 3, buttonid, LUA_TTABLE )) {
			if ( state.GetFieldWithType ( -1, "func", LUA_TFUNCTION )) {
				state.DebugCall ( 0, 0 );
				handledByButton = true;
			}
			state.Pop ();
		}
		
		if ( !handledByButton ) {
			if ( state.IsType ( 4, LUA_TFUNCTION )) {
				state.CopyToTop ( 4 );
				state.Push ( buttonid );
				state.DebugCall ( 1, 0 );
			}
		}
    }

	return 0;
}

//================================================================//
// MOAIDialogSDL
//================================================================//

//----------------------------------------------------------------//
MOAIDialogSDL::MOAIDialogSDL () :
	mWindow ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDialogSDL::~MOAIDialogSDL () {
}

//----------------------------------------------------------------//
void MOAIDialogSDL::RegisterLuaClass ( MOAILuaState& state ) {

//	state.SetField ( -1, "DIALOG_RESULT_POSITIVE",	( u32 )DIALOG_RESULT_POSITIVE );
//	state.SetField ( -1, "DIALOG_RESULT_NEUTRAL", 	( u32 )DIALOG_RESULT_NEUTRAL );
//	state.SetField ( -1, "DIALOG_RESULT_NEGATIVE",	( u32 )DIALOG_RESULT_NEGATIVE );
//	state.SetField ( -1, "DIALOG_RESULT_CANCEL", 	( u32 )DIALOG_RESULT_CANCEL );
	
	luaL_Reg regTable [] = {
		{ "show",				_show },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}