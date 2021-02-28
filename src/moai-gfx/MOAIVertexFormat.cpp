// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIVertexFormat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareAttribute
	@text	Declare a custom attribute (for use with programmable pipeline).
 
	@in		MOAIVertexFormat self
	@in		number index		Default value is 1.
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@in		number size			Number of elements. See OpenGL ES documentation.
	@opt	boolean normalized	See OpenGL ES documentation.
	@opt	number use			One of MOAIVertexFormat.ATTRIBUTE_COLOR, MOAIVertexFormat.ATTRIBUTE_NORMAL, MOAIVertexFormat.ATTRIBUTE_TEX_COORD, MOAIVertexFormat.ATTRIBUTE_COORD, MOAIVertexFormat.VERTEX_USER,
	@out	nil
*/
int	MOAIVertexFormat::_declareAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "UNNN" )

	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, 0 );
	u32 size					= state.GetValue < u32 >( 4, 0 );
	bool normalized				= state.GetValue < bool >( 5, false );
	u32 use						= state.GetValue < u32 >( 6, ATTRIBUTE_USER );

	self->DeclareAttribute ( index, type, size, use, normalized );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareBoneCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )
	
	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::UNSIGNED_BYTE );
	
	self->DeclareAttribute ( index, type, 1, ATTRIBUTE_BONE_COUNT, false );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareBoneIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )
	
	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::UNSIGNED_BYTE );
	u32 size					= state.GetValue < u32 >( 4, 4 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_BONE_INDICES, false );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareBoneWeights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )
	
	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::FLOAT );
	u32 size					= state.GetValue < u32 >( 4, 4 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_BONE_WEIGHTS, false );
	
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
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::UNSIGNED_BYTE );
	
	self->DeclareAttribute ( index, type, COLOR_SIZE, ATTRIBUTE_COLOR, true );

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
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::FLOAT );
	u32 size					= state.GetValue < u32 >( 4, 3 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_COORD, false );

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
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )
	
	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::FLOAT );
	
	self->DeclareAttribute ( index, type, NORMAL_SIZE, ATTRIBUTE_NORMAL, false );

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
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	ZLIndex index				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxTypeEnum::_ type		= ( MOAIGfxTypeEnum::_ )state.GetValue < u32 >( 3, MOAIGfxTypeEnum::FLOAT );
	u32 size					= state.GetValue < u32 >( 4, 2 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_TEX_COORD, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getVertexSize
	@text	Return the size (in bytes) of the vertex described by the vertex format.
 
	@in		MOAIVertexFormat self
	@out	number vertexSizeInBytes
*/
int MOAIVertexFormat::_getVertexSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormat, "U" )

	state.Push ( MOAILuaSize ( self->mVertexSize ));
	return 1;
}

//================================================================//
// MOAIVertexFormat
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormat::Clear () {

	for ( u32 i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		this->mAttributeIDsByUse [ i ].Clear ();
		this->mTotalAttributesByUse [ i ] = 0;
	}
	this->mAttributes.Clear ();
	this->mTotalAttributes = 0;
	this->mVertexSize = 0;
}

//----------------------------------------------------------------//
int MOAIVertexFormat::Compare ( const void* v0, const void* v1, float componentEpsilon, float normEpsilon ) const {

	for ( ZLIndex i = 0; i < this->mTotalAttributes; ++i ) {

		const MOAIVertexAttribute& attribute = this->mAttributes [ i ];
		
		float tolerance = attribute.mType == MOAIGfxTypeEnum::FLOAT ? componentEpsilon : 0.0f;
		
		const void* a0 = ( const void* )(( size_t )v0 + attribute.mOffset );
		const void* a1 = ( const void* )(( size_t )v1 + attribute.mOffset );
		
		ZLVec4D coord0 = MOAIVertexFormat::UnpackCoord ( a0, attribute );
		ZLVec4D coord1 = MOAIVertexFormat::UnpackCoord ( a1, attribute );
		
		if ( attribute.mUse == ATTRIBUTE_NORMAL ) {
		
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
bool MOAIVertexFormat::ComputeAABB ( ZLBox& aabb, const void* buffer, size_t size ) const {

	ZLByteStream stream;
	stream.SetBuffer ( buffer, size, size );
	return this->ComputeAABB ( aabb, stream, size );
}

//----------------------------------------------------------------//
bool MOAIVertexFormat::ComputeAABB ( ZLBox& aabb, ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( u32 )( size / this->mVertexSize ) : 0; // TODO: cast
	if ( !total ) return false;
	
	u32 nCoordsPerVert = this->mTotalAttributesByUse [ ATTRIBUTE_COORD ];
	if ( !nCoordsPerVert ) return false;
	
	size_t base = stream.GetCursor ();
	
	ZLVec4D coord = this->ReadCoord ( stream, 0);
	
	aabb.Init ( coord );
	aabb.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	if ( nCoordsPerVert > 0 ) {
	
		for ( ZLIndex i = 0; i < total; ++i ) {
		
			this->SeekVertex ( stream, base, i );
		
			for ( ZLIndex j = 0; j < nCoordsPerVert; ++j ) {
				coord = this->ReadCoord ( stream, j );
				aabb.Grow ( coord );
			}
		}
	}
	else {
	
		for ( ZLIndex i = 0; i < total; ++i ) {
			this->SeekVertex ( stream, base, i );
			coord = this->ReadCoord ( stream, 0);
			aabb.Grow ( coord );
		}
	}
	this->SeekVertex ( stream, base, total );
	return true;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::CountAttributesByUse ( u32 useID ) const {

	return useID < TOTAL_ATTRIBUTE_TYPES ? this->mTotalAttributesByUse [ useID ] : 0;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::CountAttributeComponents ( ZLIndex attrIdx ) const {

	return attrIdx < this->mAttributes.Size () ? this->mAttributes [ attrIdx ].mSize : 0;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::CountAttributeComponents ( u32 useID, ZLIndex idx ) const {

	if (( useID < TOTAL_ATTRIBUTE_TYPES ) && ( idx < this->mAttributeIDsByUse [ useID ].Size ())) {
		return this->CountAttributeComponents( this->mAttributeIDsByUse [ useID ][ idx ]);
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::CountBones () const {

	u32 nIndices = this->CountComponentsByUse ( ATTRIBUTE_BONE_INDICES );
	u32 nWeights = this->CountComponentsByUse ( ATTRIBUTE_BONE_WEIGHTS );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::CountComponentsByUse ( u32 useID ) const {

	u32 nComponents = 0;
	u32 nAttributes = this->mTotalAttributesByUse [ useID ];
	for ( ZLIndex i = 0; i < nAttributes; ++i ) {
	
		ZLIndex attrIdx = this->mAttributeIDsByUse [ useID ][ i ];
		const MOAIVertexAttribute& attr = this->mAttributes [ attrIdx ];
		nComponents += attr.mSize;
	}
	return nComponents;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::DeclareAttribute ( ZLIndex index, MOAIGfxTypeEnum::_ type, u32 size, u32 use, bool normalized ) {

	ZLIndex attrIdx = this->mTotalAttributes++;
	this->mAttributes.Grow ( this->mTotalAttributes );
	MOAIVertexAttribute& attr = this->mAttributes [ attrIdx ];
	
	u32 offset = this->mVertexSize;
	
	attr.mIndex			= index == ZLIndexOp::INVALID ? attrIdx : index;
	attr.mSize			= size;
	attr.mType			= type;
	attr.mUse			= use;
	attr.mNormalized	= normalized;
	attr.mOffset		= offset;
	attr.mSizeInBytes	= MOAIVertexFormat::GetComponentSize ( size, type );
	
	this->mVertexSize	+= attr.mSizeInBytes;
	
	if ( use < TOTAL_ATTRIBUTE_TYPES ) {
		
		ZLIndex idx = this->mTotalAttributesByUse [ use ]++;
		
		this->mAttributeIDsByUse [ use ].Grow (( ZLSize )idx + 1 );
		this->mAttributeIDsByUse [ use ][ idx ] = attrIdx;
	}
}

//----------------------------------------------------------------//
const MOAIVertexAttribute& MOAIVertexFormat::GetAttribute ( ZLIndex attrIdx ) {

	assert ( attrIdx < this->mAttributes.Size ());
	return this->mAttributes [ attrIdx ];
}

//----------------------------------------------------------------//
const MOAIVertexAttribute* MOAIVertexFormat::GetAttributeByUse ( u32 useID, ZLIndex attrIndex ) const {

	if ( attrIndex < this->mTotalAttributesByUse [ useID ]) {
		return &this->mAttributes [ this->mAttributeIDsByUse [ useID ][ attrIndex ]];
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetComponentSize ( u32 size, u32 type ) {

	u32 bytes;
	switch ( type ) {
	
		case MOAIGfxTypeEnum::BYTE:
		case MOAIGfxTypeEnum::UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case MOAIGfxTypeEnum::SHORT:
		case MOAIGfxTypeEnum::UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case ZLGfxEnum::FIXED:
		case MOAIGfxTypeEnum::FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return size * bytes;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetLuaIndexForUseID ( u32 useID ) {

	switch ( useID ) {
		case ATTRIBUTE_BONE_INDICES:	return 'i';
		case ATTRIBUTE_BONE_WEIGHTS:	return 'w';
		case ATTRIBUTE_BONE_COUNT:		return 'k';
		case ATTRIBUTE_COLOR:			return 'c';
		case ATTRIBUTE_COORD:			return 'x';
		case ATTRIBUTE_NORMAL:			return 'n';
		case ATTRIBUTE_TEX_COORD:		return 't';
		case ATTRIBUTE_USER:			return 'u';
	}
	
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::GetUseIDForLuaIndex ( u32 idx ) {

	switch ( idx ) {
		case 'i':						return ATTRIBUTE_BONE_INDICES;
		case 'w':						return ATTRIBUTE_BONE_WEIGHTS;
		case 'k':						return ATTRIBUTE_BONE_COUNT;
		case 'c':						return ATTRIBUTE_COLOR;
		case 'x':						return ATTRIBUTE_COORD;
		case 'n':						return ATTRIBUTE_NORMAL;
		case 't':						return ATTRIBUTE_TEX_COORD;
		case 'u':						return ATTRIBUTE_USER;
	}
	
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat::MOAIVertexFormat ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	mTotalAttributes ( 0 ),
	mVertexSize ( 0 ) {
	
	RTTI_BEGIN ( MOAIVertexFormat )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIVertexFormat >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIVertexFormat >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	for ( u32 i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		this->mTotalAttributesByUse [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormat::~MOAIVertexFormat () {
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::PackAttribute ( void* buffer, const ZLVec4D& coord, const MOAIVertexAttribute& attribute ) {

	u32 components = attribute.mSize;
	bool normalized = attribute.mNormalized;

	switch ( attribute.mType ) {
	
		case MOAIGfxTypeEnum::BYTE:
			return MOAIVertexFormat::PackAttribute < s8 >( buffer, coord, components, normalized ? 127.0f : 1.0f );
		
		case MOAIGfxTypeEnum::FLOAT:
			return MOAIVertexFormat::PackAttribute < float >( buffer, coord, components, 1.0f );
		
		case MOAIGfxTypeEnum::SHORT:
			return MOAIVertexFormat::PackAttribute < s16 >( buffer, coord, components, normalized ? 32767.0f : 1.0f );
		
		case MOAIGfxTypeEnum::UNSIGNED_BYTE:
			return MOAIVertexFormat::PackAttribute < u8 >( buffer, coord, components, normalized ? 255.0f : 1.0f );
		
		case MOAIGfxTypeEnum::UNSIGNED_SHORT:
			return MOAIVertexFormat::PackAttribute < u16 >( buffer, coord, components, normalized ? 65535.0f : 1.0f );
	}
	
	return 0;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::PrintVertices ( ZLStream& stream ) const {

	this->PrintVertices ( stream, stream.GetLength () - stream.GetCursor ());
}

//----------------------------------------------------------------//
void MOAIVertexFormat::PrintVertices ( ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( u32 )( size / this->mVertexSize ) : 0; // TODO: cast
	if ( !total ) return;

	for ( ZLIndex i = 0; i < total; ++i ) {
	
		printf ( "vertex %d:\n", ( int )ZLSize ( i ));
		
		for ( ZLIndex j = 0; j < this->mTotalAttributes; ++j ) {
			const MOAIVertexAttribute& attribute = this->mAttributes [ j ];
			
			printf ( "  %d: ", ( int )ZLSize ( j ));
			
			for ( u32 k = 0; k < attribute.mSize; ++k ) {
			
				if ( k ) {
					printf ( ", " );
				}
			
				switch ( attribute.mType  ) {
				
					case MOAIGfxTypeEnum::BYTE:
						printf ( "%d", ( int )stream.Read < s8 >( 0 ));
						break;
					
					case MOAIGfxTypeEnum::UNSIGNED_BYTE:
						printf ( "%d", ( int )stream.Read < u8 >( 0 ));
						break;
					
					case MOAIGfxTypeEnum::SHORT:
						printf ( "%d", ( int )stream.Read < s16 >( 0 ));
						break;
					
					case MOAIGfxTypeEnum::UNSIGNED_SHORT:
						printf ( "%d", ( int )stream.Read < u16 >( 0 ));
						break;
					
					case MOAIGfxTypeEnum::FLOAT:
						printf ( "%f", ( float )stream.Read < float >( 0 ));
						break;
				}
			}
			printf ( "\n" );
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::PrintVertices ( const void* buffer, size_t size ) const {

	ZLByteStream stream;
	stream.SetBuffer ( buffer, size, size );
	this->PrintVertices ( stream, size );
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::ReadAttribute ( ZLStream& stream, ZLIndex attrIdx, float yFallback, float zFallback, float wFallback ) const {

	if ( attrIdx < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const MOAIVertexAttribute& attribute = this->mAttributes [ attrIdx ];
		
		size_t base = stream.GetCursor ();
		stream.SetCursor ( base + attribute.mOffset );
		stream.ReadBytes ( buffer, attribute.mSizeInBytes );
		stream.SetCursor ( base );
		
		return this->UnpackAttribute ( buffer, attribute, yFallback, zFallback, wFallback );
	}
	return ZLVec4D ( 0.0f, yFallback, zFallback, wFallback );
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::ReadAttribute ( ZLStream& stream, u32 useID, ZLIndex idx, float yFallback, float zFallback, float wFallback ) const {

	if ( idx < this->mTotalAttributesByUse [ useID ]) {
		return this->ReadAttribute ( stream, this->mAttributeIDsByUse [ useID ][ idx ], yFallback, zFallback, wFallback);
	}
	return ZLVec4D ( 0.0f, yFallback, zFallback, wFallback );
}

//----------------------------------------------------------------//
u32 MOAIVertexFormat::ReadBoneCount ( ZLStream& stream, ZLIndex idx ) const {

	ZLVec4D boneCount = this->ReadAttribute ( stream, ATTRIBUTE_BONE_COUNT, idx, 0.0f, 0.0f, 0.0f );
	return ( u32 )boneCount.mX;
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::ReadBones ( ZLStream& stream, float* indices, float* weights, size_t size ) const {
	
	size_t nIndices = this->ReadComponents ( stream, ATTRIBUTE_BONE_INDICES, indices, size );
	size_t nWeights = this->ReadComponents ( stream, ATTRIBUTE_BONE_WEIGHTS, weights, size );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
ZLColorVec MOAIVertexFormat::ReadColor ( ZLStream& stream, ZLIndex idx ) const {

	ZLVec4D color = this->ReadAttribute ( stream, ATTRIBUTE_COLOR, idx, 0.0f, 0.0f, 1.0f );
	return ZLColorVec ( color.mX, color.mY, color.mZ, color.mW );
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::ReadComponents ( ZLStream& stream, u32 useID, float* components, size_t size ) const {

	size_t remaining = size;
	size_t n = this->mTotalAttributesByUse [ useID ];
	
	for ( ZLIndex i = 0; ( i < n ) && remaining; ++i ) {
	
		const MOAIVertexAttribute& attribute = this->mAttributes [ this->mAttributeIDsByUse [ useID ][ i ]];
		
		size_t attrSize = attribute.mSize;
		
		ZLVec4D vector = this->ReadAttribute ( stream, useID, i, 0.0f, 0.0f, 0.0f );
		
		memcpy ( components, &vector, attrSize * sizeof ( float ));
		
		remaining = remaining - attrSize;
		components = &components [ attrSize ];
	}
	
	return size - remaining;
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::ReadCoord ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_COORD, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D MOAIVertexFormat::ReadNormal ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_NORMAL, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D MOAIVertexFormat::ReadUV ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_TEX_COORD, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::SeekVertex ( ZLStream& stream, size_t base, size_t vertex ) const {

	stream.SetCursor ( base + ( vertex * this->mVertexSize ));
	return stream.GetCursor ();
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::UnpackAttribute ( const void* buffer, const MOAIVertexAttribute& attribute, float yFallback, float zFallback, float wFallback ) {

	u32 components = attribute.mSize;
	bool normalized = attribute.mNormalized;

	switch ( attribute.mType ) {
	
		case MOAIGfxTypeEnum::BYTE:
			return MOAIVertexFormat::UnpackAttribute < s8 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 127.0f : 1.0f );
		
		case MOAIGfxTypeEnum::FLOAT:
			return MOAIVertexFormat::UnpackAttribute < float >( buffer, components, yFallback, zFallback, wFallback, 1.0f );
		
		case MOAIGfxTypeEnum::SHORT:
			return MOAIVertexFormat::UnpackAttribute < s16 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 32767.0f : 1.0f );
		
		case MOAIGfxTypeEnum::UNSIGNED_BYTE:
			return MOAIVertexFormat::UnpackAttribute < u8 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 255.0f : 1.0f );
		
		case MOAIGfxTypeEnum::UNSIGNED_SHORT:
			return MOAIVertexFormat::UnpackAttribute < u16 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 65535.0f : 1.0f );
	}

	return ZLVec4D ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
ZLVec4D MOAIVertexFormat::UnpackCoord ( const void* buffer, const MOAIVertexAttribute& attribute ) {
	
	return MOAIVertexFormat::UnpackAttribute ( buffer, attribute, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteAhead ( ZLStream& stream ) const {

	size_t base = stream.GetCursor ();

	void* buffer = alloca ( this->mVertexSize );
	memset ( buffer, 0, this->mVertexSize );
	
	stream.WriteBytes ( buffer, this->mVertexSize );
	stream.SetCursor ( base );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteAttribute ( ZLStream& stream, ZLIndex attrIdx, float x, float y, float z, float w ) const {

	if ( attrIdx < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const MOAIVertexAttribute& attribute = this->mAttributes [ attrIdx ];
		
		size_t size = this->PackAttribute ( buffer, ZLVec4D ( x, y, z, w ), attribute );
		
		if ( size ) {
			size_t base = stream.GetCursor ();
			stream.SetCursor ( base + attribute.mOffset );
			stream.WriteBytes ( buffer, size );
			stream.SetCursor ( base );
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteAttribute ( ZLStream& stream, u32 useID, ZLIndex idx, float x, float y, float z, float w ) const {

	if ( idx < this->mTotalAttributesByUse [ useID ]) {
		this->WriteAttribute ( stream, this->mAttributeIDsByUse [ useID ][ idx ], x, y, z, w );
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteBoneCount ( ZLStream& stream, ZLIndex idx, u32 count ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_BONE_COUNT, idx, ( float )count, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::WriteBones ( ZLStream& stream, const float* indices, const float* weights, size_t size ) const {

	size_t nIndices = this->WriteComponents ( stream, ATTRIBUTE_BONE_INDICES, indices, size );
	size_t nWeights = this->WriteComponents ( stream, ATTRIBUTE_BONE_WEIGHTS, weights, size );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteColor ( ZLStream& stream, ZLIndex idx, u32 color ) const {

	ZLColorVec colorVec ( color );
	this->WriteAttribute ( stream, ATTRIBUTE_COLOR, idx, colorVec.mR, colorVec.mG, colorVec.mB, colorVec.mA );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteColor ( ZLStream& stream, ZLIndex idx, float r, float g, float b, float a ) const {
	
	this->WriteAttribute ( stream, ATTRIBUTE_COLOR, idx, r, g, b, a );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteCoord ( ZLStream& stream, ZLIndex idx, float x, float y, float z, float w ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_COORD, idx, x, y, z, w );
}

//----------------------------------------------------------------//
size_t MOAIVertexFormat::WriteComponents ( ZLStream& stream, u32 useID, const float* components, size_t size ) const {

	size_t remaining = size;
	size_t n = this->mTotalAttributesByUse [ useID ];
	
	for ( ZLIndex i = 0; ( i < n ) && remaining; ++i ) {
	
		const MOAIVertexAttribute& attribute = this->mAttributes [ this->mAttributeIDsByUse [ useID ][ i ]];
		
		size_t attrSize = attribute.mSize;
		
		ZLVec4D vector ( 0.0f, 0.0f, 0.0f, 0.0f );
		memcpy ( &vector, components, attrSize * sizeof ( float ));
		
		this->WriteAttribute ( stream, useID, i, vector.mX, vector.mY, vector.mZ, vector.mW );
		
		remaining = remaining - attrSize;
		components = &components [ attrSize ];
	}
	
	return size - remaining;
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteNormal ( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_NORMAL, idx, x, y, z, 0.0f );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::WriteUV ( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_TEX_COORD, idx, x, y, z, 0.0f );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormat::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField ( -1, "GL_BYTE",					( u32 )MOAIGfxTypeEnum::BYTE );
	state.SetField ( -1, "GL_FLOAT",				( u32 )MOAIGfxTypeEnum::FLOAT );
	state.SetField ( -1, "GL_SHORT",				( u32 )MOAIGfxTypeEnum::SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE",		( u32 )MOAIGfxTypeEnum::UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT",		( u32 )MOAIGfxTypeEnum::UNSIGNED_SHORT );
	
	state.SetField ( -1, "ATTRIBUTE_BONE_INDICES",	( u32 )ATTRIBUTE_BONE_INDICES );
	state.SetField ( -1, "ATTRIBUTE_BONE_WEIGHTS",	( u32 )ATTRIBUTE_BONE_WEIGHTS );
	state.SetField ( -1, "ATTRIBUTE_COLOR",			( u32 )ATTRIBUTE_COLOR );
	state.SetField ( -1, "ATTRIBUTE_COORD",			( u32 )ATTRIBUTE_COORD );
	state.SetField ( -1, "ATTRIBUTE_NORMAL",		( u32 )ATTRIBUTE_NORMAL );
	state.SetField ( -1, "ATTRIBUTE_TEX_COORD",		( u32 )ATTRIBUTE_TEX_COORD );
	state.SetField ( -1, "ATTRIBUTE_USER",			( u32 )ATTRIBUTE_USER );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "declareAttribute",		_declareAttribute },
		{ "declareBoneCount",		_declareBoneCount },
		{ "declareBoneIndices",		_declareBoneIndices },
		{ "declareBoneWeights",		_declareBoneWeights },
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
void MOAIVertexFormat::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
	if ( history.Visit ( *this )) return;

	this->mTotalAttributes		= state.GetFieldValue < cc8*, u32 >( -1, "mTotalAttributes", 0 );
	this->mVertexSize			= state.GetFieldValue < cc8*, u32 >( -1, "mVertexSize", 0 );

	this->mAttributes.Init ( this->mTotalAttributes );

	state.PushField ( -1, "mAttributes" );
	for ( ZLIndex i = 0; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attribute = this->mAttributes [ i ];
		
		state.PushField ( -1,  ( int )( i + 1 ));
		
		attribute.mIndex			= state.GetFieldValue < cc8*, u32 >( -1, "mIndex", 0 );
		attribute.mSize				= state.GetFieldValue < cc8*, u32 >( -1, "mSize", 0 );
		attribute.mType				= ( MOAIGfxTypeEnum::_ )state.GetFieldValue < cc8*, u32 >( -1, "mType", 0 );
		attribute.mNormalized		= state.GetFieldValue < cc8*, bool >( -1, "mNormalized", false );
		attribute.mOffset			= state.GetFieldValue < cc8*, u32 >( -1, "mOffset", 0 );
		
		state.Pop ();
	}
	state.Pop ();
	
	state.PushField ( -1, "mAttributeUseTable" );
	for ( ZLIndex i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		
		u32 luaUseIdx = MOAIVertexFormat::GetLuaIndexForUseID ( i );
		state.PushField ( -1, luaUseIdx );
		
		u32 nAttributesByUse = ( u32 )state.GetTableSize ( -1 );
		this->mTotalAttributesByUse [ i ] = nAttributesByUse;
		this->mAttributeIDsByUse [ i ].Init ( nAttributesByUse );
		
		for ( ZLIndex j = 0; j < nAttributesByUse; ++j ) {
			
			this->mAttributeIDsByUse [ i ][ j ] = state.GetFieldValue < MOAILuaIndex, MOAILuaIndex >( -1, j, 0 );
		}
		state.Pop ();
	}
	state.Pop ();
}

//----------------------------------------------------------------//
void MOAIVertexFormat::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "mTotalAttributes", MOAILuaSize ( this->mTotalAttributes ));
	state.SetField ( -1, "mVertexSize", MOAILuaSize ( this->mVertexSize ));

	lua_newtable ( state );
	for ( ZLIndex i = 0; i < this->mTotalAttributes; ++i ) {
	
		MOAIVertexAttribute& attribute = this->mAttributes [ i ];
	
		state.Push ( MOAILuaIndex ( i ));
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
	for ( ZLIndex i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		
		state.Push ( MOAIVertexFormat::GetLuaIndexForUseID ( i ));
		lua_newtable ( state );
		
		ZLSize nAttributesByUse = this->mTotalAttributesByUse [ i ];
		for ( ZLIndex j = 0; j < nAttributesByUse; ++j ) {
			state.SetField ( -1, MOAILuaIndex ( j + i ), MOAILuaIndex ( this->mAttributeIDsByUse [ i ][ j ]));
		}
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mAttributeUseTable" );
}
