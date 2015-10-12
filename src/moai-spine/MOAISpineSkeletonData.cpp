// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-spine/MOAISpineSkeletonData.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	load

 	@in		MOAISpineSkeletonData self
	@in		string	skeleton json file path
	@in		string	atlas file path
	@in		number	scale
	@out	nil
*/
int MOAISpineSkeletonData::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeletonData, "USS" )
	
	cc8* skeletonPath = state.GetValue < cc8* >( 2, "" );
	cc8* atlasPath = state.GetValue < cc8* >( 3, "" );
	float scale = state.GetValue < float >( 4, 1.0f );
	
	self->mAtlas = spAtlas_createFromFile ( atlasPath, 0 );
	if ( !self->mAtlas ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_FileNotFound_S, atlasPath );
		return 0;
	}
	
	spSkeletonJson* json = spSkeletonJson_create ( self->mAtlas );
	json->scale = scale;
	self->mSkeletonData = spSkeletonJson_readSkeletonDataFile ( json, skeletonPath );
	spSkeletonJson_dispose(json);
	
	if ( !self->mSkeletonData ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_FileOpenError_S, json->error );
		return 0;
	}
	
	return 0;
}

//================================================================//
// MOAIFoo
//================================================================//

//----------------------------------------------------------------//
MOAISpineSkeletonData::MOAISpineSkeletonData ():
	mSkeletonData ( 0 ),
	mAtlas ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpineSkeletonData::~MOAISpineSkeletonData () {
	if ( mSkeletonData ) {
		spSkeletonData_dispose ( mSkeletonData );
	}
	
	if ( mAtlas ) {
		spAtlas_dispose ( mAtlas );
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeletonData::RegisterLuaClass ( MOAILuaState &state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAISpineSkeletonData::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "load",	_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

