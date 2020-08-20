// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxCmdMedium.h>

////================================================================//
//// lua
////================================================================//
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIGfxCmdMedium::_getInterfaceWithDefault ( lua_State* L ) {
//
//	MOAILuaState state ( L );
//	if ( state.IsType ( 1, LUA_TUSERDATA )) {
//		MOAICmdMedium* self = ( MOAICmdMedium* )state.GetPtrUserData ( 1 );
//		if ( self->PushWithHandler ( state, 2 )) return 1;
//		
//		self->PushWithHandler ( state, MOAIDraw::Get ());
//		return 1;
//	}
//	return 0;
//}
//
////================================================================//
//// MOAIGfxScript
////================================================================//
//
////----------------------------------------------------------------//
//MOAIGfxCmdMedium::MOAIGfxCmdMedium ( MOAIAbstractCmdStream& stream ) :
//	MOAICmdMedium ( stream ) {
//}
//
////----------------------------------------------------------------//
//MOAIGfxCmdMedium::~MOAIGfxCmdMedium () {
//}
//
////================================================================//
//// virtual
////================================================================//
//
////----------------------------------------------------------------//
//void MOAIGfxCmdMedium::MOAICmdMedium_ExtendLuaAPI ( MOAILuaState& state ) {
//	
//	luaL_Reg regTable [] = {
//		{ "_",							_getInterfaceWithDefault },
//		{ NULL, NULL }
//	};
//	luaL_register ( state, 0, regTable );
//}
