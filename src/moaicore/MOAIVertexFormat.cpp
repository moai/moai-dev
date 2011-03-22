// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatElement
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatElement::Init ( u32 usage, u32 type, u32 dim ) {

	this->mUsage = usage;
	this->mType = type;
	this->mDim = dim;

	u32 size = 0;

	switch ( type ) {
		case GL_BYTE:
			size = 1;
			break;
		case GL_SHORT:
			size = 2;
			break;
		case GL_FLOAT:
			size = 4;
			break;
	}
	this->mSize = dim * size;
}

//----------------------------------------------------------------//
MOAIVertexFormatElement::MOAIVertexFormatElement () :
	mUsage ( MOAIVertexFormat::NONE ),
	mType ( GL_FLOAT ),
	mDim ( 0 ),
	mSize ( 0 ),
	mOffset ( 0 ) {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIVertexFormat::_reserve ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UN" )
	
	u32 formatSize = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( formatSize );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIVertexFormat::_setElement ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UNNNN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 usage	= state.GetValue < u32 >( 3, 0 );
	u32 type	= state.GetValue < u32 >( 4, 0 );
	u32 size	= state.GetValue < u32 >( 5, 0 );
	
	self->SetElement ( idx, usage, type, size );
	
	return 0;
}

//================================================================//
// MOAIVertexFormat
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormat::Bind () {

	this->Bless ();
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Bless () {

	if ( this->mDirty ) {

		u32 offset = 0;

		for ( u32 i = 0; i < this->mFormat.Size (); ++i ) {
			MOAIVertexFormatElement& formatElement = this->mFormat [ i ];
			formatElement.mOffset = offset;
			offset += formatElement.mSize;
		}
		
		this->mVertexSize = offset;
		this->mDirty = false;
	}
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetVertexSize () {

	this->Bless ();
	return this->mVertexSize;
}

//----------------------------------------------------------------//
MOAIVertexFormat::MOAIVertexFormat () :
	mVertexSize ( 0 ),
	mDirty ( false ) {
	
	RTTI_SINGLE ( MOAIVertexFormat )
}

//----------------------------------------------------------------//
MOAIVertexFormat::~MOAIVertexFormat () {
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "reserve",		_reserve },
		{ "setElement",		_setElement },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Reserve ( u32 formatSize ) {

	this->mFormat.Init ( formatSize );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::SetElement ( u32 idx, u32 usage, u32 type, u32 dim ) {

	if ( idx < this->mFormat.Size ()) {
		
		MOAIVertexFormatElement& element = this->mFormat [ idx ];
		element.mUsage	= usage;
		element.mType	= type;
		element.mSize	= dim;
		
		this->mDirty = true;
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::SetVertexElement ( void* buffer, size_t bufferSize, u32 vtx, u32 elemIdx, float* params ) {
	UNUSED ( bufferSize );
	// TODO: bufferSize
	
	if ( elemIdx >= this->mFormat.Size ()) return;
	this->Bless ();
	
	MOAIVertexFormatElement& formatElement = this->mFormat [ elemIdx ];
	
	void* element = ( void* )((( size_t )buffer ) + ( vtx * this->mVertexSize ) + formatElement.mOffset );
	
	for ( u32 i = 0; i < formatElement.mSize; ++i ) {
		
		switch ( formatElement.mType ) {
			case GL_BYTE:
				*( u8* )element = ( u8 )params [ i ];
				element = ( void* )(( size_t )element + sizeof ( u8 ));
				break;
			case GL_SHORT:
				*( u16* )element = ( u16 )params [ i ];
				element = ( void* )(( size_t )element + sizeof ( u16 ));
				break;
			case GL_FLOAT:
				*( float* )element = params [ i ];
				element = ( void* )(( size_t )element + sizeof ( float ));
				break;
		}
	}
}

//----------------------------------------------------------------//
STLString MOAIVertexFormat::ToString () {

	STLString repr;
	return repr;
}
