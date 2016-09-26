// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

//================================================================//
// MOAIVertexAttribute
//================================================================//
class MOAIVertexAttribute {
private:

	friend class MOAIVertexFormat;
	
	u32			mIndex;
	u32			mSize;
	u32			mType;			// type of the element
	u32			mUse;			// index into use table
	bool		mNormalized;
	u32			mOffset;		// base addr in vertex
	u32			mSizeInBytes;
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

private:

	friend class MOAIGfxMgr;
	friend class MOAIGfxPipelineClerk;
	friend class MOAIGfxStateCache;

	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 NULL_INDEX				= 0xffffffff;
	
	static const u32 MAX_ATTR_BYTES			= 16;
	
	ZLLeanArray < MOAIVertexAttribute >		mAttributes; // this is the linear array of attributes
	ZLLeanArray < u32 >						mAttributeIDsByUse [ TOTAL_ATTRIBUTE_TYPES ]; // this is for looking up attributes by semantic meaning
	
	u32										mTotalAttributes;
	u32										mTotalAttributesByUse [ TOTAL_ATTRIBUTE_TYPES ];
	
	u32										mVertexSize;

	//----------------------------------------------------------------//
	static int					_clear							( lua_State* L );
	static int					_declareAttribute				( lua_State* L );
	static int					_declareBoneCount				( lua_State* L );
	static int					_declareBoneIndices				( lua_State* L );
	static int					_declareBoneWeights				( lua_State* L );
	static int					_declareColor					( lua_State* L );
	static int					_declareCoord					( lua_State* L );
	static int					_declareNormal					( lua_State* L );
	static int					_declareUV						( lua_State* L );
	static int					_getVertexSize					( lua_State* L );
	
	//----------------------------------------------------------------//
	void						Bind							( ZLSharedConstBuffer* buffer, bool copyBuffer ) const;
	static u32					GetComponentSize				( u32 size, u32 type );
	static u32					GetLuaIndexForUseID				( u32 useID );
	static u32					GetUseIDForLuaIndex				( u32 idx );
	size_t						ReadComponents					( ZLStream& stream, u32 useID, float* components, size_t size ) const;
	void						Unbind							() const;
	size_t						WriteComponents					( ZLStream& stream, u32 useID, const float* components, size_t size ) const;
	
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
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	static MOAIVertexFormat*		AffirmVertexFormat				( MOAILuaState& state, int idx );
	void							Clear							();
	int								Compare							( const void* v0, const void* v1, float componentEpsilon, float normEpsilon ) const;
	
	bool							ComputeBounds					( ZLBox& bounds, const void* buffer, size_t size ) const;
	bool							ComputeBounds					( ZLBox& bounds, ZLStream& stream, size_t size ) const;
	
	u32								CountAttributesByUse			( u32 useID ) const;
	u32								CountAttributeComponents		( u32 attrIdx ) const;
	u32								CountAttributeComponents		( u32 useID, u32 idx ) const;
	u32								CountBones						() const;
	u32								CountComponentsByUse			( u32 useID ) const;

	void							DeclareAttribute				( u32 index, u32 type, u32 size, u32 use, bool normalized );
	
	const MOAIVertexAttribute&		GetAttribute					( u32 attrIdx );
	const MOAIVertexAttribute&		GetAttributeByUse				( u32 useID, u32 idx );
	
	void*							GetAttributeAddress				( const MOAIVertexAttribute& attribute, u32 vtxIdx, void* buffer );
	const void*						GetAttributeAddress				( const MOAIVertexAttribute& attribute, u32 vtxIdx, const void* buffer );
	
									MOAIVertexFormat				();
									~MOAIVertexFormat				();
	
	static size_t					PackAttribute					( void* buffer, const ZLVec4D& coord, const MOAIVertexAttribute& attribute );
	
	void							PrintVertices					( ZLStream& stream ) const;
	void							PrintVertices					( ZLStream& stream, size_t size ) const;
	void							PrintVertices					( const void* buffer, size_t size ) const;
	
	ZLVec4D							ReadAttribute					( ZLStream& stream, u32 attrIdx, float yFallback, float zFallback, float wFallback ) const;
	ZLVec4D							ReadAttribute					( ZLStream& stream, u32 useID, u32 idx, float yFallback, float zFallback, float wFallback ) const;
	u32								ReadBoneCount					( ZLStream& stream, u32 idx ) const;
	size_t							ReadBones						( ZLStream& stream, float* indices, float* weights, size_t size ) const;
	ZLColorVec						ReadColor						( ZLStream& stream, u32 idx ) const;
	ZLVec4D							ReadCoord						( ZLStream& stream, u32 idx ) const;
	ZLVec3D							ReadNormal						( ZLStream& stream, u32 idx ) const;
	ZLVec3D							ReadUV							( ZLStream& stream, u32 idx ) const;
	
	void							RegisterLuaClass				( MOAILuaState& state );
	void							RegisterLuaFuncs				( MOAILuaState& state );
	
	size_t							SeekVertex						( ZLStream& stream, size_t base, size_t vertex ) const;
	
	void							SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void							SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
	
	static ZLVec4D					UnpackAttribute					( const void* buffer, const MOAIVertexAttribute& attribute, float yFallback, float zFallback, float wFallback );
	static ZLVec4D					UnpackCoord						( const void* buffer, const MOAIVertexAttribute& attribute );
	
	void							WriteAhead						( ZLStream& stream ) const; // writes an empty vertex as a placeholder
	void							WriteAttribute					( ZLStream& stream, u32 attrIdx, float x, float y, float z, float w ) const;
	void							WriteAttribute					( ZLStream& stream, u32 useID, u32 idx, float x, float y, float z, float w ) const;
	void							WriteBoneCount					( ZLStream& stream, u32 idx, u32 count ) const;
	size_t							WriteBones						( ZLStream& stream, const float* indices, const float* weights, size_t size ) const;
	void							WriteColor						( ZLStream& stream, u32 idx, u32 color ) const;
	void							WriteColor						( ZLStream& stream, u32 idx, float r, float g, float b, float a ) const;
	void							WriteCoord						( ZLStream& stream, u32 idx, float x, float y, float z, float w ) const;
	void							WriteNormal						( ZLStream& stream, u32 idx, float x, float y, float z ) const;
	void							WriteUV							( ZLStream& stream, u32 idx, float x, float y, float z ) const;
	
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
