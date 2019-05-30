// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIImageTextureGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	updateRegion
	@text	Update either a sub-region of the texture or the whole
			texture to match changes in the image. Updated regions will
			be reloaded from the image the next time the texture is bound.

	@in		MOAIImageTextureGL self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImageTextureGL::_updateRegion ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImageTextureGL, "U" )
	
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
// MOAIImageTextureGL
//================================================================//

//----------------------------------------------------------------//
MOAIImageTextureGL::MOAIImageTextureGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIImage )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
	
	this->mDebugName = "(stacktrace from MOAIImageTextureGL)";
}

//----------------------------------------------------------------//
MOAIImageTextureGL::~MOAIImageTextureGL () {
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIImage::RegisterLuaClass ( state );
	MOAITextureGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIImage::RegisterLuaFuncs ( state );
	MOAITextureGL::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "invalidate",					_updateRegion }, // TODO: deprecate
		{ "updateRegion",				_updateRegion },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIImage::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIImage::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::UpdateRegion () {
	
	this->mRegion = this->GetRect ();
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::UpdateRegion ( ZLIntRect rect ) {
	
	rect.Bless ();
	this->GetRect ().Clip ( rect );
	
	if ( this->GetState () == STATE_NEEDS_GPU_UPDATE ) {
		this->mRegion.Grow ( rect );
	}
	else {
		this->mRegion = rect;
	}
	
	this->ScheduleForGPUUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageTextureGL::MOAIImage_OnImageStatusChanged	( bool isOK ) {

	if ( isOK ) {
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::ZLAbstractGfxResource_OnClearDirty () {

	this->mRegion.Clear ();
}

//----------------------------------------------------------------//
bool MOAIImageTextureGL::ZLAbstractGfxResource_OnGPUCreate () {

	if ( !this->IsOK ()) return false;
	
	this->mRegion.Clear ();
	if ( this->CreateTextureFromImage ( *this )) {
		return this->ZLAbstractGfxResource_OnGPUUpdate ();
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageTextureGL::ZLAbstractGfxResource_OnGPUUpdate () {

	bool result = true;

	if ( this->mRegion.Area () > 0 ) {
		result = this->UpdateTextureFromImage ( *this, this->mRegion );
		this->mRegion.Clear ();
	}
	return result && MOAITextureGL::ZLAbstractGfxResource_OnGPUUpdate ();
}
