// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

#include <moai-gfx/MOAIGfxConsts.h>

//================================================================//
// MOAIVertexAttribute
//================================================================//
class MOAIVertexAttribute {
public:
	
	u32						mIndex;
	u32						mSize;
	MOAIGfxTypeEnum::_		mType;			// type of the element
	u32						mUse;			// index into use table
	bool					mNormalized;
	u32						mOffset;		// base addr in vertex
	u32						mSizeInBytes;
};

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@lua	MOAIVertexFormat
	@text	Vertex format class.
*/
class MOAIVertexFormat :
	public virtual MOAILuaObject {
public:

	enum {
	
		// these are the known attribute types
		ATTRIBUTE_BONE_INDICES,		// i
		ATTRIBUTE_BONE_WEIGHTS,		// w
		ATTRIBUTE_BONE_COUNT,		// k
		ATTRIBUTE_COLOR,			// c
		ATTRIBUTE_COORD,			// x
		ATTRIBUTE_NORMAL,			// n
		ATTRIBUTE_TEX_COORD,		// t
		ATTRIBUTE_USER,				// u
		
		TOTAL_ATTRIBUTE_TYPES,
	};

protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIGfxPipelineClerkGL;
	friend class ZLGfxStateGPUCacheGL;
	friend class MOAIVertexArray;
	
	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 NULL_INDEX				= 0xffffffff;
	
	static const u32 MAX_ATTR_BYTES			= 16;
	
	ZLLeanArray < MOAIVertexAttribute >		mAttributes; // this is the linear array of attributes
	ZLLeanArray < ZLIndex >					mAttributeIDsByUse [ TOTAL_ATTRIBUTE_TYPES ]; // this is for looking up attributes by semantic meaning
	
	ZLSize									mTotalAttributes;
	ZLSize									mTotalAttributesByUse [ TOTAL_ATTRIBUTE_TYPES ];
	
	ZLSize									mVertexSize;
	
	//----------------------------------------------------------------//
	static int			_clear							( lua_State* L );
	static int			_declareAttribute				( lua_State* L );
	static int			_declareBoneCount				( lua_State* L );
	static int			_declareBoneIndices				( lua_State* L );
	static int			_declareBoneWeights				( lua_State* L );
	static int			_declareColor					( lua_State* L );
	static int			_declareCoord					( lua_State* L );
	static int			_declareNormal					( lua_State* L );
	static int			_declareUV						( lua_State* L );
	static int			_getVertexSize					( lua_State* L );
	
	//----------------------------------------------------------------//
	static u32			GetComponentSize				( u32 size, u32 type );
	static u32			GetLuaIndexForUseID				( u32 useID );
	static u32			GetUseIDForLuaIndex				( u32 idx );
	size_t				ReadComponents					( ZLStream& stream, u32 useID, float* components, size_t size ) const;
	size_t				WriteComponents					( ZLStream& stream, u32 useID, const float* components, size_t size ) const;
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn					( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut					( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static size_t PackAttribute ( void* buffer, const ZLVec4D& coord, u32 components, float normalize ) {
	
		switch ( components ) {
			case 4:
				(( TYPE* )buffer )[ 3 ] = ( TYPE )( coord.mW * normalize );
			case 3:
				(( TYPE* )buffer )[ 2 ] = ( TYPE )( coord.mZ * normalize );
			case 2:
				(( TYPE* )buffer )[ 1 ] = ( TYPE )( coord.mY * normalize );
			default:
				(( TYPE* )buffer )[ 0 ] = ( TYPE )( coord.mX * normalize );
		}
		return components * sizeof ( TYPE );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static ZLVec4D UnpackAttribute ( const void* buffer, u32 components, float yFallback, float zFallback, float wFallback, float normalize ) {
	
		
		return ZLVec4D (
				(( float )((( TYPE* )buffer )[ 0 ]) / normalize ),
				components > 1 ? (( float )((( TYPE* )buffer )[ 1 ]) / normalize ) : yFallback,
				components > 2 ? (( float )((( TYPE* )buffer )[ 2 ]) / normalize ) : zFallback,
				components > 3 ? (( float )((( TYPE* )buffer )[ 3 ]) / normalize ) : wFallback
		);
	}
	
public:
		
	GET_CONST ( ZLSize, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	void							Clear							();
	int								Compare							( const void* v0, const void* v1, float componentEpsilon, float normEpsilon ) const;
	
	bool							ComputeAABB						( ZLBox& aabb, const void* buffer, size_t size ) const;
	bool							ComputeAABB						( ZLBox& aabb, ZLStream& stream, size_t size ) const;
	
	u32								CountAttributesByUse			( u32 useID ) const;
	u32								CountAttributeComponents		( ZLIndex attrIdx ) const;
	u32								CountAttributeComponents		( u32 useID, ZLIndex idx ) const;
	u32								CountBones						() const;
	u32								CountComponentsByUse			( u32 useID ) const;

	void							DeclareAttribute				( ZLIndex index, MOAIGfxTypeEnum::_ type, u32 size, u32 use, bool normalized );
	
	const MOAIVertexAttribute&		GetAttribute					( ZLIndex attrIdx );
	const MOAIVertexAttribute*		GetAttributeByUse				( u32 useID, ZLIndex attrIndex ) const;
	
	static size_t					PackAttribute					( void* buffer, const ZLVec4D& coord, const MOAIVertexAttribute& attribute );
	
	void							PrintVertices					( ZLStream& stream ) const;
	void							PrintVertices					( ZLStream& stream, size_t size ) const;
	void							PrintVertices					( const void* buffer, size_t size ) const;
	
	ZLVec4D							ReadAttribute					( ZLStream& stream, ZLIndex attrIdx, float yFallback, float zFallback, float wFallback ) const;
	ZLVec4D							ReadAttribute					( ZLStream& stream, u32 useID, ZLIndex idx, float yFallback, float zFallback, float wFallback ) const;
	u32								ReadBoneCount					( ZLStream& stream, ZLIndex idx ) const;
	size_t							ReadBones						( ZLStream& stream, float* indices, float* weights, size_t size ) const;
	ZLColorVec						ReadColor						( ZLStream& stream, ZLIndex idx ) const;
	ZLVec4D							ReadCoord						( ZLStream& stream, ZLIndex idx ) const;
	ZLVec3D							ReadNormal						( ZLStream& stream, ZLIndex idx ) const;
	ZLVec3D							ReadUV							( ZLStream& stream, ZLIndex idx ) const;
	
	size_t							SeekVertex						( ZLStream& stream, size_t base, size_t vertex ) const;
	
	static ZLVec4D					UnpackAttribute					( const void* buffer, const MOAIVertexAttribute& attribute, float yFallback, float zFallback, float wFallback );
	static ZLVec4D					UnpackCoord						( const void* buffer, const MOAIVertexAttribute& attribute );
	
	void							WriteAhead						( ZLStream& stream ) const; // writes an empty vertex as a placeholder
	void							WriteAttribute					( ZLStream& stream, ZLIndex attrIdx, float x, float y, float z, float w ) const;
	void							WriteAttribute					( ZLStream& stream, u32 useID, ZLIndex idx, float x, float y, float z, float w ) const;
	void							WriteBoneCount					( ZLStream& stream, ZLIndex idx, u32 count ) const;
	size_t							WriteBones						( ZLStream& stream, const float* indices, const float* weights, size_t size ) const;
	void							WriteColor						( ZLStream& stream, ZLIndex idx, u32 color ) const;
	void							WriteColor						( ZLStream& stream, ZLIndex idx, float r, float g, float b, float a ) const;
	void							WriteCoord						( ZLStream& stream, ZLIndex idx, float x, float y, float z, float w ) const;
	void							WriteNormal						( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const;
	void							WriteUV							( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const;
	
									MOAIVertexFormat				( ZLContext& context );
									~MOAIVertexFormat				();
	
	//----------------------------------------------------------------//
	inline void* GetAttributeAddress ( const MOAIVertexAttribute& attribute, void* vertexBuffer, u32 vtxIdx ) const {

		return ( void* )this->GetAttributeAddress ( attribute, ( const void* )vertexBuffer, vtxIdx );
	}

	//----------------------------------------------------------------//
	inline const void* GetAttributeAddress ( const MOAIVertexAttribute& attribute, const void* vertexBuffer , u32 vtxIdx ) const {

		return ( const void* )(( uintptr )vertexBuffer + ( vtxIdx * this->mVertexSize ) + attribute.mOffset );
	}
};

#endif
