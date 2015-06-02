// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

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
	@opt	number use			One of MOAIVertexFormat.ARRAY_COLOR, MOAIVertexFormat.ARRAY_NORMAL, MOAIVertexFormat.ARRAY_TEX_COORD, MOAIVertexFormat.ARRAY_VERTEX, MOAIVertexFormat.VERTEX_USE_TUPLE,
	@out	nil
*/
int	MOAIVertexFormat::_declareAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNNN" )

	u32 index			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type			= state.GetValue < u32 >( 3, 0 );
	u32 size			= state.GetValue < u32 >( 4, 0 );
	bool normalized		= state.GetValue < bool >( 5, false );
	
	u32 use				= state.GetValue < u32 >( 6, VERTEX_USE_TUPLE );

	self->DeclareAttribute ( index, type, size, use, normalized );
	
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
MOAIVertexFormat* MOAIVertexFormat::AffirmVertexFormat ( MOAILuaState& state, int idx ) {

	MOAIVertexFormat* format = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		format = MOAIVertexFormatMgr::Get ().GetFormat ( state.GetValue < u32 >( idx, MOAIVertexFormatMgr::UNKNOWN_FORMAT ));
	}
	else {
		format = state.GetLuaObject < MOAIVertexFormat >( 2, true );
	}
	return format;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::Bind ( const void* buffer ) const {

	if ( MOAIGfxDevice::Get ().IsProgrammable ()) {
		this->BindProgrammable ( buffer );
	}
	else {
		this->BindFixed ( buffer );
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::BindFixed ( const void* buffer ) const {
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
void MOAIVertexFormat::BindProgrammable ( const void* buffer ) const {

	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attr = this->mAttributes [ i ];

		void* addr = ( void* )(( size_t )buffer + attr.mOffset );
		zglVertexAttribPointer ( attr.mIndex, attr.mSize, attr.mType, attr.mNormalized, this->mVertexSize, addr );
		zglEnableVertexAttribArray ( attr.mIndex );
	}
}

//----------------------------------------------------------------//
int MOAIVertexFormat::Compare ( const void* v0, const void* v1, float componentEpsilon, float normEpsilon ) const {

	for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {

		MOAIVertexAttribute& attribute = this->mAttributes [ i ];
			
		float tolerance = attribute.mType == ZGL_TYPE_FLOAT ? componentEpsilon : 0.0f;
		
		ZLVec4D coord0 = MOAIVertexFormat::UnpackCoord ( v0, attribute );
		ZLVec4D coord1 = MOAIVertexFormat::UnpackCoord ( v1, attribute );
		
		if ( attribute.mUse == ARRAY_NORMAL ) {
		
			if ( coord0.ZLVec3D::Dot ( coord1 ) < ( 1.0f - normEpsilon )) {
			
				if ( coord0.mX != coord1.mX ) {
					return ( coord0.mX < coord1.mX ) ? -1 : 1;
				}
				
				if ( coord0.mY != coord1.mY ) {
					return ( coord0.mY < coord1.mY ) ? -1 : 1;
				}
				
				if ( coord0.mZ != coord1.mZ ) {
					return ( coord0.mZ < coord1.mZ ) ? -1 : 1;
				}
			}
			
			if ( ABS ( coord0.mW - coord1.mW ) > tolerance ) {
				return ( coord0.mW < coord1.mW ) ? -1 : 1;
			}
		}
		else {
		
			if ( ABS ( coord0.mX - coord1.mX ) > tolerance ) {
				return ( coord0.mX < coord1.mX ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mY - coord1.mY ) > tolerance ) {
				return ( coord0.mY < coord1.mY ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mZ - coord1.mZ ) > tolerance ) {
				return ( coord0.mZ < coord1.mZ ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mW - coord1.mW ) > tolerance ) {
				return ( coord0.mW < coord1.mW ) ? -1 : 1;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( ZLBox& bounds, const void* buffer, size_t size ) const {

	ZLByteStream stream;
	stream.SetBuffer ( buffer, size, size );
	return this->ComputeBounds ( bounds, stream, size );
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeBounds ( ZLBox& bounds, ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( size / this->mVertexSize ) : 0;
	if ( !total ) return false;
	
	size_t base = stream.GetCursor ();
	
	ZLVec4D coord = this->ReadCoord ( stream );
	
	bounds.Init ( coord );
	bounds.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	for ( u32 i = 1; i < total; ++i ) {
		this->SeekVertex ( stream, base, i );
		coord = this->ReadCoord ( stream );
		bounds.Grow ( coord );
		
	}
	this->SeekVertex ( stream, base, total );
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareAttribute ( u32 index, u32 type, u32 size, u32 use, bool normalized ) {

	u32 attrID = this->mTotalAttributes++;
	this->mAttributes.Grow ( this->mTotalAttributes );
	MOAIVertexAttribute& attr = this->mAttributes [ attrID ];
	
	u32 offset = this->mVertexSize;
	
	attr.mIndex			= index;
	attr.mSize			= size;
	attr.mType			= type;
	attr.mUse			= use;
	attr.mNormalized	= normalized;
	attr.mOffset		= offset;
	attr.mSizeInBytes	= MOAIVertexFormat::GetComponentSize ( size, type );
	
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
size_t MOAIVertexFormat::PackAttribute ( void* buffer, const ZLVec4D& coord, const MOAIVertexAttribute& attribute ) {

	u32 components = attribute.mSize;
	size_t size = 0;

	switch ( attribute.mType ) {
	
		case ZGL_TYPE_BYTE: {
			
			float normalize = attribute.mNormalized ? 127.0f : 1.0f;
			
			switch ( components ) {
				case 4:
					(( s8* )buffer )[ 3 ] = ( s8 )( coord.mW * normalize );
				case 3:
					(( s8* )buffer )[ 2 ] = ( s8 )( coord.mZ * normalize );
				default:
					(( s8* )buffer )[ 1 ] = ( s8 )( coord.mY * normalize );
					(( s8* )buffer )[ 0 ] = ( s8 )( coord.mX * normalize );
			}
			
			size = components;
			break;
		}
		
		case ZGL_TYPE_FLOAT: {
		
			switch ( components ) {
				case 4:
					(( float* )buffer )[ 3 ] = coord.mW;
				case 3:
					(( float* )buffer )[ 2 ] = coord.mZ;
				default:
					(( float* )buffer )[ 1 ] = coord.mY;
					(( float* )buffer )[ 0 ] = coord.mX;
			}
			
			size = components * sizeof ( float );
			break;
		}
		
		case ZGL_TYPE_SHORT: {
		
			float normalize = attribute.mNormalized ? 32767.0f : 1.0f;
		
			switch ( components ) {
				case 4:
					(( s16* )buffer )[ 3 ] = ( s16 )( coord.mW * normalize );
				case 3:
					(( s16* )buffer )[ 2 ] = ( s16 )( coord.mZ * normalize );
				default:
					(( s16* )buffer )[ 1 ] = ( s16 )( coord.mY * normalize );
					(( s16* )buffer )[ 0 ] = ( s16 )( coord.mX * normalize );
			}
			
			size = components * sizeof ( s16 );
			break;
		}
		
		case ZGL_TYPE_UNSIGNED_BYTE: {
		
			float normalize = attribute.mNormalized ? 255.0f : 1.0f;
		
			switch ( components ) {
				case 4:
					(( u8* )buffer )[ 3 ] = ( u8 )( coord.mW * normalize );
				case 3:
					(( u8* )buffer )[ 2 ] = ( u8 )( coord.mZ * normalize );
				default:
					(( u8* )buffer )[ 1 ] = ( u8 )( coord.mY * normalize );
					(( u8* )buffer )[ 0 ] = ( u8 )( coord.mX * normalize );
			}
			
			size = components;
			break;
		}
		
		case ZGL_TYPE_UNSIGNED_SHORT: {
			
			float normalize = attribute.mNormalized ? 65535.0f : 1.0f;
			
			switch ( components ) {
				case 4:
					(( u16* )buffer )[ 3 ] = ( u16 )( coord.mW * normalize );
				case 3:
					(( u16* )buffer )[ 2 ] = ( u16 )( coord.mZ * normalize );
				default:
					(( u16* )buffer )[ 1 ] = ( u16 )( coord.mY * normalize );
					(( u16* )buffer )[ 0 ] = ( u16 )( coord.mX * normalize );
			}
			
			size = components * sizeof ( u16 );
			break;
		}
	}
	
	return size;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::PrintVertices ( ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( size / this->mVertexSize ) : 0;
	if ( !total ) return;

	for ( u32 i = 0; i < total; ++i ) {
	
		printf ( "vertex %d:\n", i );
		
		for ( u32 j = 0; j < this->mTotalAttributes; ++j ) {
			MOAIVertexAttribute& attribute = this->mAttributes [ j ];
			
			printf ( "  %d: ", j );
			
			for ( u32 k = 0; k < attribute.mSize; ++k ) {
			
				if ( k ) {
					printf ( ", " );
				}
			
				switch ( attribute.mType  ) {
				
					case ZGL_TYPE_BYTE:
						printf ( "%d", ( int )stream.Read < s8 >( 0 ));
						break;
					
					case ZGL_TYPE_UNSIGNED_BYTE:
						printf ( "%d", ( int )stream.Read < u8 >( 0 ));
						break;
					
					case ZGL_TYPE_SHORT:
						printf ( "%d", ( int )stream.Read < s16 >( 0 ));
						break;
					
					case ZGL_TYPE_UNSIGNED_SHORT:
						printf ( "%d", ( int )stream.Read < u16 >( 0 ));
						break;
					
					case ZGL_TYPE_FLOAT:
						printf ( "%f", ( float )stream.Read < float >( 0 ));
						break;
				}
			}
			printf ( "\n" );
		}
	}
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::ReadAttribute ( ZLStream& stream, u32 attrID, float zFallback, float wFallback ) const {

	if ( attrID < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const MOAIVertexAttribute& attribute = this->mAttributes [ attrID ];
		
		size_t base = stream.GetCursor ();
		stream.Seek ( base + attribute.mOffset, SEEK_SET );
		stream.ReadBytes ( buffer, attribute.mSizeInBytes );
		stream.Seek ( base, SEEK_SET );
		
		return this->UnpackAttribute ( buffer, attribute, zFallback, wFallback );
	}
	return ZLVec4D ( 0.0f, 0.0f, zFallback, wFallback );
}

//----------------------------------------------------------------//
ZLColorVec MOAIVertexFormat::ReadColor ( ZLStream& stream ) const {

	ZLVec4D color = this->ReadAttribute ( stream, this->mAttributeUseTable [ ARRAY_COLOR ].mAttrID, 0.0f, 1.0f );
	return ZLColorVec ( color.mX, color.mY, color.mZ, color.mW );
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::ReadCoord ( ZLStream& stream ) const {

	return this->ReadAttribute ( stream, this->mAttributeUseTable [ ARRAY_VERTEX ].mAttrID, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D MOAIVertexFormat::ReadNormal ( ZLStream& stream ) const {

	return this->ReadAttribute ( stream, this->mAttributeUseTable [ ARRAY_NORMAL ].mAttrID, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D MOAIVertexFormat::ReadUV ( ZLStream& stream ) const {

	return this->ReadAttribute ( stream, this->mAttributeUseTable [ ARRAY_TEX_COORD ].mAttrID, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GL_BYTE",					( u32 )ZGL_TYPE_BYTE );
	state.SetField ( -1, "GL_FLOAT",				( u32 )ZGL_TYPE_FLOAT );
	state.SetField ( -1, "GL_SHORT",				( u32 )ZGL_TYPE_SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE",		( u32 )ZGL_TYPE_UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT",		( u32 )ZGL_TYPE_UNSIGNED_SHORT );
	
	state.SetField ( -1, "ARRAY_COLOR",				( u32 )ARRAY_COLOR );
	state.SetField ( -1, "ARRAY_NORMAL",			( u32 )ARRAY_NORMAL );
	state.SetField ( -1, "ARRAY_TEX_COORD",			( u32 )ARRAY_TEX_COORD );
	state.SetField ( -1, "ARRAY_VERTEX",			( u32 )ARRAY_VERTEX );
	state.SetField ( -1, "VERTEX_USE_TUPLE",		( u32 )VERTEX_USE_TUPLE );
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
size_t MOAIVertexFormat::SeekVertex ( ZLStream& stream, size_t base, size_t vertex ) const {

	stream.Seek ( base + ( vertex * this->mVertexSize ), SEEK_SET );
	return stream.GetCursor ();
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

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::UnpackAttribute ( const void* buffer, const MOAIVertexAttribute& attribute, float zFallback, float wFallback ) {

	ZLVec4D coord;

	u32 components = attribute.mSize;

	switch ( attribute.mType ) {
	
		case ZGL_TYPE_BYTE: {
		
			float normalize = attribute.mNormalized ? 127.0f : 1.0f;
		
			coord.Init (
				(( float )((( s8* )buffer )[ 0 ]) / normalize ),
				(( float )((( s8* )buffer )[ 1 ]) / normalize ),
				components > 2 ? (( float )((( s8* )buffer )[ 2 ]) / normalize ) : zFallback,
				components > 3 ? (( float )((( s8* )buffer )[ 3 ]) / normalize ) : wFallback
			);
			break;
		}
		
		case ZGL_TYPE_FLOAT: {
		
			coord.Init (
				(( float* )buffer )[ 0 ],
				(( float* )buffer )[ 1 ],
				components > 2 ? (( float* )buffer )[ 2 ] : zFallback,
				components > 3 ? (( float* )buffer )[ 3 ] : wFallback
			);
			break;
		}
		
		case ZGL_TYPE_SHORT: {
		
			float normalize = attribute.mNormalized ? 32767.0f : 1.0f;
		
			coord.Init (
				(( float )((( s16* )buffer )[ 0 ]) / normalize ),
				(( float )((( s16* )buffer )[ 1 ]) / normalize ),
				components > 2 ? (( float )((( s16* )buffer )[ 2 ]) / normalize ) : zFallback,
				components > 3 ? (( float )((( s16* )buffer )[ 3 ]) / normalize ) : wFallback
			);
			break;
		}
		
		case ZGL_TYPE_UNSIGNED_BYTE: {
		
			float normalize = attribute.mNormalized ? 255.0f : 1.0f;
		
			coord.Init (
				(( float )((( u8* )buffer )[ 0 ]) / normalize ),
				(( float )((( u8* )buffer )[ 1 ]) / normalize ),
				components > 2 ? (( float )((( u8* )buffer )[ 2 ]) / normalize ) : zFallback,
				components > 3 ? (( float )((( u8* )buffer )[ 3 ]) / normalize ) : wFallback
			);
			break;
		}
		
		case ZGL_TYPE_UNSIGNED_SHORT: {
			
			float normalize = attribute.mNormalized ? 65535.0f : 1.0f;
			
			coord.Init (
				(( float )((( u16* )buffer )[ 0 ]) / normalize ),
				(( float )((( u16* )buffer )[ 1 ]) / normalize ),
				components > 2 ? (( float )((( u16* )buffer )[ 2 ]) / normalize ) : zFallback,
				components > 3 ? (( float )((( u16* )buffer )[ 3 ]) / normalize ) : wFallback
			);
			break;
		}
	}

	return coord;
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::UnpackCoord ( const void* buffer, const MOAIVertexAttribute& attribute ) {
	
	return MOAIVertexFormat::UnpackAttribute ( buffer, attribute, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteAhead ( ZLStream& stream ) const {

	size_t base = stream.GetCursor ();

	void* buffer = alloca ( this->mVertexSize );
	memset ( buffer, 0, this->mVertexSize );
	
	stream.WriteBytes ( buffer, this->mVertexSize );
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteAttribute ( ZLStream& stream, u32 attrID, float x, float y, float z, float w ) const {

	if ( attrID < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const MOAIVertexAttribute& attribute = this->mAttributes [ attrID ];
		
		size_t size = this->PackAttribute ( buffer, ZLVec4D ( x, y, z, w ), attribute );
		
		if ( size ) {
			size_t base = stream.GetCursor ();
			stream.Seek ( base + attribute.mOffset, SEEK_SET );
			stream.WriteBytes ( buffer, size );
			stream.Seek ( base, SEEK_SET );
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteColor ( ZLStream& stream, u32 color ) const {

	ZLColorVec colorVec ( color );
	this->WriteAttribute ( stream, this->mAttributeUseTable [ ARRAY_COLOR ].mAttrID, colorVec.mR, colorVec.mG, colorVec.mB, colorVec.mA );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteColor ( ZLStream& stream, float r, float g, float b, float a ) const {
	
	this->WriteAttribute ( stream, this->mAttributeUseTable [ ARRAY_COLOR ].mAttrID, r, g, b, a );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteCoord ( ZLStream& stream, float x, float y, float z, float w ) const {

	this->WriteAttribute ( stream, this->mAttributeUseTable [ ARRAY_VERTEX ].mAttrID, x, y, z, w );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteNormal ( ZLStream& stream, float x, float y, float z ) const {

	this->WriteAttribute ( stream, this->mAttributeUseTable [ ARRAY_NORMAL ].mAttrID, x, y, z, 0.0f );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteUV ( ZLStream& stream, float x, float y, float z ) const {

	this->WriteAttribute ( stream, this->mAttributeUseTable [ ARRAY_TEX_COORD ].mAttrID, x, y, z, 0.0f );
}
