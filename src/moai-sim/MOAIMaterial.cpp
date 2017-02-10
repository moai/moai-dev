// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAIMaterial.h>
#include <moai-sim/MOAIMaterialMgr.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIMaterial::_getLight ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIMaterial, "U" )
//	MOAILight* light = self->GetLight ( state.GetValue ( 2, 0 ));
//	state.Push ( light );
//	return 1;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIMaterial::_setLight ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIMaterial, "U" )
//	return 0;
//}

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::ClearLights ( MOAILuaObject* owner ) {

	for ( u32 i = 0; i < this->mEntries.Size (); ++i ) {
		MOAIMaterialLight& entry = this->mEntries [ i ];
		owner->LuaRelease ( entry.mLight );
	}
	this->mEntries.Clear ();
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::GetLight ( u32 globalID ) {

	if ( globalID < MOAIMaterialMgr::MAX_GLOBAL_LIGHTS ) {
		for ( u32 i = 0; i < this->mEntries.Size (); ++i ) {
			MOAIMaterialLight& entry = this->mEntries [ i ];
			if ( entry.mGlobalID == globalID ) {
				return entry.mLight;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {

	this->ClearLights ();
}

//----------------------------------------------------------------//
void MOAIMaterial::ReserveLights ( u32 n ) {

	this->Clear ();
	this->mEntries.Init ( n );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetLight ( u32 globalID, MOAILight* light, MOAILuaObject* owner ) {

	if ( globalID < MOAIMaterialMgr::MAX_GLOBAL_LIGHTS ) {

		owner->LuaRetain ( light );
		
		for ( u32 i = 0; i < this->mEntries.Size (); ++i ) {
			MOAIMaterialLight& entry = this->mEntries [ i ];
			if ( entry.mGlobalID == globalID ) {
				owner->LuaRelease ( entry.mLight );
				entry.mLight = light;
				return;
			}
		}
		
		size_t idx = this->mEntries.Size ();
		this->mEntries.Grow ( globalID, 1 );
		this->mEntries[ idx ].mGlobalID = globalID;
		this->mEntries[ idx ].mLight = light;
	}
}

