#include "pch.h"

#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIStream.h>
#include <moaicore/MOAITextureRaw.h>

int MOAITextureRaw::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureRaw, "U" )

	self->Init ( state, 2 );
	return 0;
}

//================================================================//
// MOAITextureRaw
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState* MOAITextureRaw::AffirmTexture ( MOAILuaState& state, int idx ) {

	MOAIGfxState* gfxState = 0;

	gfxState = state.GetLuaObject < MOAITextureBase >( idx, false );
	if ( gfxState ) return gfxState;

	MOAITextureRaw* texture = new MOAITextureRaw ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}

//----------------------------------------------------------------//
bool MOAITextureRaw::Init ( MOAILuaState& state, int idx ) {
	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( idx, false );
	this->mFrameWidth = state.GetValue<u32>( idx + 1, 0);
	this->mFrameHeight = state.GetValue<u32>( idx + 2, 0);

	void* bytes;
	size_t size;
	data->Lock ( &bytes, &size );

	this->mData = malloc(size);
	memcpy(this->mData, bytes, size);

	data->Unlock ();

	this->Load();

	return true;
}

//----------------------------------------------------------------//
bool MOAITextureRaw::IsRenewable () {

	if ( this->mData ) return true;

	return false;
}

//----------------------------------------------------------------//
MOAITextureRaw::MOAITextureRaw () :
mData ( 0 ),
mFrameWidth ( 0 ),
mFrameHeight ( 0 ){

	RTTI_BEGIN
	RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureRaw::~MOAITextureRaw () {
	this->Clear();
}

//----------------------------------------------------------------//
void MOAITextureRaw::OnClear () {
	MOAITextureBase::OnClear ();

	this->mDebugName.clear ();

	if ( this->mData ) {
		free ( this->mData );
		this->mData = NULL;
	}
	this->mFrameWidth = 0;
	this->mFrameHeight = 0;
}

//----------------------------------------------------------------//
void MOAITextureRaw::OnCreate () {
	if ( this->mData ) {
		this->CreateTextureFromRaw ( (uint8_t *)this->mData, this->mFrameWidth, this->mFrameHeight );
	}
}

//----------------------------------------------------------------//
void MOAITextureRaw::OnLoad () {
	if ( this->mData ) {
		this->mWidth = this->mFrameWidth;
		this->mHeight = this->mFrameHeight;
	}
}

//----------------------------------------------------------------//
void MOAITextureRaw::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITextureRaw::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITextureBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
