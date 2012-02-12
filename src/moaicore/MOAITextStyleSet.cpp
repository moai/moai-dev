// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyleSet.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAITextStyleSet::_setStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyleSet, "U" )
	return 0;
}

//================================================================//
// MOAITextStyleSet
//================================================================//

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleSet::FindStyle ( cc8* styleName ) {

	if ( this->mStyleMap.contains ( styleName )) {
		return this->mStyleMap [ styleName ];
	}
	return this->mDefault;
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleSet::GetDefault () {

	return this->mDefault;
}

//----------------------------------------------------------------//
MOAITextStyleSet::MOAITextStyleSet () :
	mDefault ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextStyleSet::~MOAITextStyleSet () {
}

//----------------------------------------------------------------//
void MOAITextStyleSet::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITextStyleSet::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
	
	luaL_Reg regTable [] = {
		{ "setStyle",				_setStyle },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SetDefault ( MOAITextStyle* style ) {

	this->mDefault = style;
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

	if ( !styleName ) return;
	
	if ( style ) {
		this->mStyleMap [ styleName ] = style;
	}
	else {
		if ( this->mStyleMap.contains ( styleName )) {
			this->mStyleMap.erase ( styleName );
		}
	}
}
