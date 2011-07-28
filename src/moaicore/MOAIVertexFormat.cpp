// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatElem
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatElem::Bind ( void* buffer, u32 stride, GLenum use ) const {

	if ( this->mIsEnabled ) {
		
		buffer = ( void* )(( size_t )buffer + this->mOffset );
	
		switch ( use ) {
			case GL_COLOR_ARRAY:
				glColorPointer ( this->mSize, this->mType, stride, buffer );
				break;
			case GL_NORMAL_ARRAY:
				glNormalPointer ( this->mType, stride, buffer );
				break;
			case GL_TEXTURE_COORD_ARRAY:
				glTexCoordPointer ( this->mSize, this->mType, stride, buffer );
				break;
			case GL_VERTEX_ARRAY:
				glVertexPointer ( this->mSize, this->mType, stride, buffer );
				break;
			default:
				break;
		}
		
		glEnableClientState ( use );
	}
	else {
		
		glDisableClientState ( use );
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormatElem::Set () {

	this->mIsEnabled = false;
}

//----------------------------------------------------------------//
void MOAIVertexFormatElem::Set ( GLint size, GLenum type, u32 offset ) {

	this->mSize = size;
	this->mType = type;
	this->mOffset = offset;
	this->mIsEnabled = true;
}

//----------------------------------------------------------------//
void MOAIVertexFormatElem::Unbind ( GLenum use ) const {

	glDisableClientState ( use );
}


//----------------------------------------------------------------//
MOAIVertexFormatElem::MOAIVertexFormatElem () :
	mSize ( 0 ),
	mType ( 0 ),
	mOffset ( 0 ),
	mIsEnabled ( false ) {
}

//================================================================//
// MOAIVertexFormatAttr
//================================================================//
 	
//----------------------------------------------------------------//
void MOAIVertexFormatAttr::Bind ( void* buffer, u32 stride ) const {

	buffer = ( void* )(( size_t )buffer + this->mOffset );

	glVertexAttribPointer (	this->mIndex, this->mSize, this->mType, this->mNormalized, stride, buffer );
	glEnableVertexAttribArray ( this->mIndex );
}

//----------------------------------------------------------------//
void MOAIVertexFormatAttr::Set ( GLint index, GLint size, GLenum type, GLboolean normalized, u32 offset ) {

	this->mIndex = index;
	this->mSize = size;
	this->mType = type;
	this->mNormalized = normalized;
	this->mOffset = offset;
}

//----------------------------------------------------------------//
void MOAIVertexFormatAttr::Unbind () const {

	glDisableVertexAttribArray ( this->mIndex );
}

//----------------------------------------------------------------//
MOAIVertexFormatAttr::MOAIVertexFormatAttr () {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Removes all format information and returns the format to
			an empty state.
	
	@in		MOAIVertexFormat self
	@out	nil
*/
int	MOAIVertexFormat::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	self->Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareColor
	@text	Declare a vertex color.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareColorArray ( type );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareCoord
	@text	Declare a vertex coord.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of coord elements. See OpenGL ES documentation.
	@in		number size			Number of coord elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	u32 size = state.GetValue < u32 >( 3, 0 );
	self->DeclareVertexArray ( type, size );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareNormal
	@text	Declare a vertex normal.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of normal elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareNormalArray ( type );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareUV
	@text	Declare a vertex texture coord.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of texture coord elements. See OpenGL ES documentation.
	@in		number size			Number of texture coord elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	u32 size = state.GetValue < u32 >( 3, 0 );
	self->DeclareTexCoordArray ( type, size );

	return 0;
}

//================================================================//
// MOAIVertexFormat
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormat::Bind ( void* buffer ) const {

	if ( MOAIGfxDevice::Get ().IsProgrammable ()) {
		
		for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
			this->mAttributes [ i ].Bind ( buffer, this->mVertexSize );
		}
	}
	else {
		this->mElements [ ARRAY_COLOR ].Bind ( buffer, this->mVertexSize, GL_COLOR_ARRAY );
		this->mElements [ ARRAY_NORMAL ].Bind ( buffer, this->mVertexSize, GL_NORMAL_ARRAY );
		this->mElements [ ARRAY_TEX_COORD ].Bind ( buffer, this->mVertexSize, GL_TEXTURE_COORD_ARRAY );
		this->mElements [ ARRAY_VERTEX ].Bind ( buffer, this->mVertexSize, GL_VERTEX_ARRAY );
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Clear () {

	this->mVertexSize = 0;
	this->mTotalAttributes = 0;
	
	this->mElements [ ARRAY_COLOR ].Set ();
	this->mElements [ ARRAY_NORMAL ].Set ();
	this->mElements [ ARRAY_TEX_COORD ].Set ();
	this->mElements [ ARRAY_VERTEX ].Set ();
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( void* buffer, u32 size, USRect& bounds ) {

	// TODO: GLES2

	u32 total = ( u32 )( size / this->mVertexSize );
	if ( !total ) return false;

	MOAIVertexFormatElem& coordElem = this->mElements [ ARRAY_VERTEX ];
	if ( coordElem.mType != GL_FLOAT ) return false; // TODO: handle other types
	if ( coordElem.mSize < 2 ) return false;
	
	buffer = ( void* )(( size_t )buffer + coordElem.mOffset );
	
	USVec2D* coord = ( USVec2D* )buffer;
	bounds.Init ( *coord );
	bounds.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	for ( u32 i = 1; i < total; ++i ) {
		
		buffer = ( void* )(( size_t )buffer + this->mVertexSize );
		coord = ( USVec2D* )buffer;
		bounds.Grow ( *coord );
	}
	
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareAttribute ( GLint index, GLint size, GLenum type, GLboolean normalized ) {

	this->mAttributes.Grow ( this->mTotalAttributes + 1 );

	MOAIVertexFormatAttr& attr = this->mAttributes [ this->mTotalAttributes++ ];
	
	u32 offset = this->mVertexSize;
	attr.Set ( index, size, type, normalized, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareColorArray ( GLenum type ) {

	MOAIVertexFormatElem& elem = this->mElements [ ARRAY_COLOR ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( COLOR_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( COLOR_SIZE, type );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareNormalArray ( GLenum type ) {

	MOAIVertexFormatElem& elem = this->mElements [ ARRAY_NORMAL ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( NORMAL_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( NORMAL_SIZE, type );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareTexCoordArray ( GLenum type, GLint size ) {

	MOAIVertexFormatElem& elem = this->mElements [ ARRAY_TEX_COORD ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareVertexArray ( GLenum type, GLint size ) {

	MOAIVertexFormatElem& elem = this->mElements [ ARRAY_VERTEX ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetSize ( GLint size, GLenum type ) {

	u32 bytes;
	switch ( type ) {
	
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case GL_FIXED:
		case GL_FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return size * bytes;
}

//----------------------------------------------------------------//
GLenum MOAIVertexFormat::GetColorType () const {

	return this->mElements [ ARRAY_COLOR ].mType;
}

//----------------------------------------------------------------//
MOAIVertexFormat::MOAIVertexFormat () :
	mVertexSize ( 0 ),
	mTotalAttributes ( 0 ) {
	
	RTTI_SINGLE ( USLuaObject )
	
	this->Clear ();
}

//----------------------------------------------------------------//
MOAIVertexFormat::~MOAIVertexFormat () {
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
	
	state.SetField ( -1, "GL_BYTE", ( u32 )GL_BYTE );
	//state.SetField ( -1, "GL_FIXED", ( u32 )GL_FIXED );
	state.SetField ( -1, "GL_FLOAT", ( u32 )GL_FLOAT );
	state.SetField ( -1, "GL_SHORT", ( u32 )GL_SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE", ( u32 )GL_UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT", ( u32 )GL_UNSIGNED_SHORT );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "declareColor",			_declareColor },
		{ "declareCoord",			_declareCoord },
		{ "declareNormal",			_declareNormal },
		{ "declareUV",				_declareUV },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::ReserveAttributes ( u32 total ) {

	this->mAttributes.Init ( total );
	this->mTotalAttributes = 0;
}

//----------------------------------------------------------------//
STLString MOAIVertexFormat::ToString () {

	STLString repr;
	return repr;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Unbind () const {

	if ( this->mTotalAttributes ) {
	
		for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
			this->mAttributes [ i ].Unbind ();
		}
	}
	else {
		this->mElements [ ARRAY_COLOR ].Unbind ( GL_COLOR_ARRAY );
		this->mElements [ ARRAY_NORMAL ].Unbind ( GL_NORMAL_ARRAY );
		this->mElements [ ARRAY_TEX_COORD ].Unbind ( GL_TEXTURE_COORD_ARRAY );
		this->mElements [ ARRAY_VERTEX ].Unbind ( GL_VERTEX_ARRAY );
	}
}
