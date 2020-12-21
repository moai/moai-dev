// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIImageTexture.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	updateRegion
	@text	Update either a sub-region of the texture or the whole
			texture to match changes in the image. Updated regions will
			be reloaded from the image the next time the texture is bound.

	@in		MOAIImageTexture self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImageTexture::_updateRegion ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImageTexture, "U" )
	
	if ( state.GetTop () > 1 ) {
		ZLIntRect rect = state.GetRect < int >( 2 );
		self->UpdateRegion ( rect );
	}
	else {
		self->UpdateRegion ();
	}
	return 0;
}

//================================================================//
// MOAIImageTexture
//================================================================//

//----------------------------------------------------------------//
MOAIImageTexture::MOAIImageTexture () {
	
	RTTI_BEGIN ( MOAIImageTexture )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIImageTexture >)
		RTTI_EXTEND ( RTTIBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImageTexture::~MOAIImageTexture () {
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion () {
	
	this->mRegion = this->GetRect ();
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion ( ZLIntRect rect ) {
	
	rect.Bless ();
	this->GetRect ().Clip ( rect );
	
	this->mRegion.Grow ( rect, this->mRegion.Area () > 0 );

	this->ScheduleForGPUUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageTexture::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIImageTexture::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "invalidate",					_updateRegion }, // TODO: deprecate
		{ "updateRegion",				_updateRegion },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImageTexture::MOAIImage_OnImageStatusChanged ( bool isOK ) {

	if ( isOK ) {
		this->ScheduleForGPUUpdate ();
	}
}
