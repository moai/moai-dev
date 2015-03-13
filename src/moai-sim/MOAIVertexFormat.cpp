// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	declareAttribute
	@text	Declare a custom attribute (for use with programmable pipeline).
	
	@in		MOAIVertexFormat self
	@in		number index		Default value is 1.
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@in		number size			Number of elements. See OpenGL ES documentation.
	@opt	boolean normalized	See OpenGL ES documentation.
	@out	nil
*/
int	MOAIVertexFormat::_declareAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNNN" )

	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	u32 size			= state.GetValue < u32 >( 4, 0 );
	bool normalized		= state.GetValue < bool >( 5, false );

	self->DeclareAttribute ( index, type, size, NULL_INDEX, normalized );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareColor
	@text	Declare a vertex color.
	
	@in		MOAIVertexFormat self
	@in		number index
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	
	self->DeclareAttribute ( index, type, COLOR_SIZE, ARRAY_COLOR, true );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareCoord
	@text	Declare a vertex coordinate.
	
	@in		MOAIVertexFormat self
	@in		number index
	@in		number type			Data type of coordinate elements. See OpenGL ES documentation.
	@in		number size			Number of coordinate elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNNN" )

	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	u32 size			= state.GetValue < u32 >( 4, 0 );
	
	self->DeclareAttribute ( index, type, size, ARRAY_VERTEX, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareNormal
	@text	Declare a vertex normal.
	
	@in		MOAIVertexFormat self
	@in		number index
	@in		number type			Data type of normal elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNN" )
	
	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	
	self->DeclareAttribute ( index, type, NORMAL_SIZE, ARRAY_NORMAL, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUV
	@text	Declare a vertex texture coordinate.
	
	@in		MOAIVertexFormat self
	@in		number index
	@in		number type			Data type of texture coordinate elements. See OpenGL ES documentation.
	@in		number size			Number of texture coordinate elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNNN" )

	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	u32 size			= state.GetValue < u32 >( 4, 0 );
	
	self->DeclareAttribute ( index, type, size, ARRAY_TEX_COORD, false );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_getVertexSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	state.Push ( self->mVertexSize );
	return 1;
}

//================================================================//
// MOAIVertexFormat
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormat::Bind ( void* buffer ) const {

	if ( MOAIGfxDevice::Get ().IsProgrammable ()) {
		this->BindProgrammable ( buffer );
	}
	else {
		this->BindFixed ( buffer );
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::BindFixed ( void* buffer ) const {
	UNUSED ( buffer );

	#if USE_OPENGLES1
		for ( u32 i = 0; i < TOTAL_ARRAY_TYPES; ++i ) {
		
			const MOAIVertexAttributeUse& attrUse = this->mAttributeUseTable [ i ];
			
			if ( attrUse.mAttrID == NULL_INDEX ) {
				zglDisableClientState ( attrUse.mUse );
			}
			else {
			
				MOAIVertexAttribute& attr = this->mAttributes [ attrUse.mAttrID ];
				
				void* addr = ( void* )(( size_t )buffer + attr.mOffset );
				
				switch ( attrUse.mUse ) {
					case ZGL_PIPELINE_COLOR_ARRAY:
						zglColorPointer ( attr.mSize, attr.mType, this->mVertexSize, addr );
						break;
					case ZGL_PIPELINE_NORMAL_ARRAY:
						zglNormalPointer ( attr.mType, this->mVertexSize, addr );
						break;
					case ZGL_PIPELINE_TEXTURE_COORD_ARRAY:
						zglTexCoordPointer ( attr.mSize, attr.mType, this->mVertexSize, addr );
						break;
					case ZGL_PIPELINE_VERTEX_ARRAY:
						zglVertexPointer ( attr.mSize, attr.mType, this->mVertexSize, addr );
						break;
					default:
						break;
				}
				zglEnableClientState ( attrUse.mUse );
			}
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIVertexFormat::BindProgrammable ( void* buffer ) const {

	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attr = this->mAttributes [ i ];

		void* addr = ( void* )(( size_t )buffer + attr.mOffset );
		zglVertexAttribPointer ( attr.mIndex, attr.mSize, attr.mType, attr.mNormalized, this->mVertexSize, addr );
		zglEnableVertexAttribArray ( attr.mIndex );
	}
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( ZLBox& bounds, void* buffer, size_t size ) const {

	MOAIByteStream stream;
	stream.Open ( buffer, size );
	return this->ComputeBounds ( bounds, stream, size );
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( ZLBox& bounds, MOAIStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( size / this->mVertexSize ) : 0;
	if ( !total ) return false;

	u32 coordAttributeIdx = this->mAttributeUseTable [ ARRAY_VERTEX ].mAttrID;
	if ( coordAttributeIdx >= this->mTotalAttributes ) return false;

	MOAIVertexAttribute& coordAttr = this->mAttributes [ coordAttributeIdx ];
	if ( coordAttr.mType != ZGL_TYPE_FLOAT ) return false; // TODO: handle other types
	if ( coordAttr.mSize < 2 ) return false;
	
	return MOAIVertexFormat::ComputeBounds ( bounds, stream, size, coordAttr.mOffset, this->mVertexSize, coordAttr.mSize );
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( ZLBox& bounds, MOAIStream& stream, size_t size, size_t offset, size_t stride, size_t components ) {

	if ( components < 2 ) return false;

	u32 total = MOAIVertexFormat::CountElements ( size, offset, stride );
	if ( !total ) return false;
	
	stream.Seek ( offset, SEEK_CUR );
	
	ZLVec3D coord = MOAIVertexFormat::ReadCoord ( stream, stride, components );
	
	bounds.Init ( coord );
	bounds.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	for ( u32 i = 1; i < total; ++i ) {
	
		coord = MOAIVertexFormat::ReadCoord ( stream, stride, components );
		bounds.Grow ( coord );
	}
	return true;
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::CountElements ( size_t size ) {

	return ( size_t )( size / this->mVertexSize );
}


//----------------------------------------------------------------//
size_t MOAIVertexFormat::CountElements ( size_t size, size_t offset, size_t stride ) {

	return stride ? ( size_t )(( size - offset ) / stride ) : 0;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareAttribute ( u32 index, u32 type, u32 size, u32 use, bool normalized ) {

	u32 attrID = this->mTotalAttributes++;
	this->mAttributes.Grow ( this->mTotalAttributes );
	MOAIVertexAttribute& attr = this->mAttributes [ attrID ];
	
	u32 offset = this->mVertexSize;
	
	attr.mIndex = index;
	attr.mSize = size;
	attr.mType = type;
	attr.mNormalized = normalized;
	attr.mOffset = offset;
	
	this->mVertexSize += MOAIVertexFormat::GetComponentSize ( size, type );
	
	//u32 useIdx = MOAIVertexFormat::GetIndexForUse ( use );
	u32 useIdx = use;
	if ( useIdx < TOTAL_ARRAY_TYPES ) {
		this->mAttributeUseTable [ useIdx ].mAttrID = attrID;
	}
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetComponentSize ( u32 size, u32 type ) {

	u32 bytes;
	switch ( type ) {
	
		case ZGL_TYPE_BYTE:
		case ZGL_TYPE_UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case ZGL_TYPE_SHORT:
		case ZGL_TYPE_UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case ZGL_FIXED:
		case ZGL_TYPE_FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return size * bytes;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetIndexForUse ( u32 use ) {
	UNUSED ( use );

	#if USE_OPENGLES1
		switch ( use ) {
			case ZGL_PIPELINE_COLOR_ARRAY:			return ARRAY_COLOR;
			case ZGL_PIPELINE_NORMAL_ARRAY:			return ARRAY_NORMAL;
			case ZGL_PIPELINE_TEXTURE_COORD_ARRAY:	return ARRAY_TEX_COORD;
			case ZGL_PIPELINE_VERTEX_ARRAY:			return ARRAY_VERTEX;
			default:								break;
		}
	#endif
	return NULL_INDEX;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetUseForIndex ( u32 idx ) {
	UNUSED ( idx );

	#if USE_OPENGLES1
		switch ( idx ) {
			case ARRAY_COLOR:			return ZGL_PIPELINE_COLOR_ARRAY;
			case ARRAY_NORMAL:			return ZGL_PIPELINE_NORMAL_ARRAY;
			case ARRAY_TEX_COORD:		return ZGL_PIPELINE_TEXTURE_COORD_ARRAY;
			case ARRAY_VERTEX:			return ZGL_PIPELINE_VERTEX_ARRAY;
			default:					break;
		}
	#endif
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat::MOAIVertexFormat () :
	mTotalAttributes ( 0 ),
	mVertexSize ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->mAttributes.Init ( TOTAL_ARRAY_TYPES );
	
	for ( u32 i = 0; i < TOTAL_ARRAY_TYPES; ++i ) {
		this->mAttributeUseTable [ i ].mUse = MOAIVertexFormat::GetUseForIndex ( i );
		this->mAttributeUseTable [ i ].mAttrID = NULL_INDEX;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormat::~MOAIVertexFormat () {
}

//----------------------------------------------------------------//
ZLVec3D MOAIVertexFormat::ReadCoord ( MOAIStream& stream, size_t stride, size_t components ) {

	size_t next = stream.GetCursor () + stride;
	
	ZLVec3D coord (
		stream.Read < float >( 0.0f ),
		stream.Read < float >( 0.0f ),
		components > 2 ? stream.Read < float >( 0.0f ) : 0.0f
	);
	
	stream.Seek ( next, SEEK_SET );
	return coord;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GL_BYTE", ( u32 )ZGL_TYPE_BYTE );
	//state.SetField ( -1, "GL_FIXED", ( u32 )GL_FIXED );
	state.SetField ( -1, "GL_FLOAT", ( u32 )ZGL_TYPE_FLOAT );
	state.SetField ( -1, "GL_SHORT", ( u32 )ZGL_TYPE_SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE", ( u32 )ZGL_TYPE_UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT", ( u32 )ZGL_TYPE_UNSIGNED_SHORT );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "declareAttribute",		_declareAttribute },
		{ "declareColor",			_declareColor },
		{ "declareCoord",			_declareCoord },
		{ "declareNormal",			_declareNormal },
		{ "declareUV",				_declareUV },
		{ "getVertexSize",			_getVertexSize },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	this->mTotalAttributes		= state.GetField < u32 >( -1, "mTotalAttributes", 0 );
	this->mVertexSize			= state.GetField < u32 >( -1, "mVertexSize", 0 );

	this->mAttributes.Init ( this->mTotalAttributes );

	state.GetField ( -1, "mAttributes" );
	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attribute = this->mAttributes [ i ];
		
		state.GetField ( -1, i + 1 );
		
		attribute.mIndex			= state.GetField < u32 >( -1, "mIndex", 0 );
		attribute.mSize				= state.GetField < u32 >( -1, "mSize", 0 );
		attribute.mType				= state.GetField < u32 >( -1, "mType", 0 );
		attribute.mNormalized		= state.GetField < bool >( -1, "mNormalized", 0 );
		attribute.mOffset			= state.GetField < u32 >( -1, "mOffset", 0 );
		
		state.Pop ();
	}
	lua_pop ( state, 1 );
	
	state.GetField ( -1, "mAttributeUseTable" );
	for ( u32 i = 0; i < TOTAL_ARRAY_TYPES; ++i ) {
		
		MOAIVertexAttributeUse& attributeUse = this->mAttributeUseTable [ i ];
		
		state.GetField ( -1, i + 1 );
		
		attributeUse.mUse			= state.GetField < u32 >( -1, "mUse", 0 );
		attributeUse.mAttrID		= state.GetField < u32 >( -1, "mAttrID", 0 );
		
		state.Pop ();
	}
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	state.SetField ( -1, "mTotalAttributes", this->mTotalAttributes );
	state.SetField ( -1, "mVertexSize", this->mVertexSize );

	lua_newtable ( state );
	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
	
		MOAIVertexAttribute& attribute = this->mAttributes [ i ];
	
		state.Push ( i + 1 );
		lua_newtable ( state );
		
		state.SetField ( -1, "mIndex",			attribute.mIndex );
		state.SetField ( -1, "mSize",			attribute.mSize );
		state.SetField ( -1, "mType",			attribute.mType );
		state.SetField ( -1, "mNormalized",		attribute.mNormalized );
		state.SetField ( -1, "mOffset",			attribute.mOffset );
		
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mAttributes" );

	lua_newtable ( state );
	for ( u32 i = 0; i < TOTAL_ARRAY_TYPES; ++i ) {
		
		MOAIVertexAttributeUse& attributeUse = this->mAttributeUseTable [ i ];
	
		state.Push ( i + 1 );
		lua_newtable ( state );
		
		state.SetField ( -1, "mUse",			attributeUse.mUse );
		state.SetField ( -1, "mAttrID",			attributeUse.mAttrID );
		
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mAttributeUseTable" );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Unbind () const {

	if ( MOAIGfxDevice::Get ().IsProgrammable ()) {
		this->UnbindProgrammable ();
	}
	else {
		this->UnbindFixed ();
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::UnbindFixed () const {

	#if USE_OPENGLES1
		for ( u32 i = 0; i < TOTAL_ARRAY_TYPES; ++i ) {
			zglDisableClientState ( this->mAttributeUseTable [ i ].mUse );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIVertexFormat::UnbindProgrammable () const {

	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attr = this->mAttributes [ i ];
		zglDisableVertexAttribArray ( attr.mIndex );
	}
}
