// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

//================================================================//
// MOAIVertexAttributeUse
//================================================================//
class MOAIVertexAttributeUse {
private:

	friend class MOAIVertexFormat;
	
	u32			mUse;
	u32			mAttrID;
};

//================================================================//
// MOAIVertexAttribute
//================================================================//
class MOAIVertexAttribute {
private:

	friend class MOAIVertexFormat;
	
	u32			mIndex;
	u32			mSize;
	u32			mType;			// type of the element
	u32			mUse;
	bool		mNormalized;
	u32			mOffset;
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
	
		// these are the known uses, supported by fixed function
		ARRAY_COLOR,
		ARRAY_NORMAL,
		ARRAY_TEX_COORD,
		ARRAY_VERTEX,
		
		TOTAL_ARRAY_TYPES,
		
		// custom uses
		VERTEX_USE_TUPLE,
	};

private:

	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 NULL_INDEX				= 0xffffffff;
	
	static const u32 MAX_ATTR_BYTES			= 16;
	
	ZLLeanArray < MOAIVertexAttribute >		mAttributes;
	u32										mTotalAttributes;
	u32										mVertexSize;

	MOAIVertexAttributeUse					mAttributeUseTable [ TOTAL_ARRAY_TYPES ]; // use for fixed function pipeline and for computing bounds
	
	//----------------------------------------------------------------//
	static int					_declareAttribute				( lua_State* L );
	static int					_declareColor					( lua_State* L );
	static int					_declareCoord					( lua_State* L );
	static int					_declareNormal					( lua_State* L );
	static int					_declareUV						( lua_State* L );
	static int					_getVertexSize					( lua_State* L );
	
	//----------------------------------------------------------------//
	void						BindFixed						( const void* buffer ) const;
	void						BindProgrammable				( const void* buffer ) const;
	static u32					GetComponentSize				( u32 size, u32 type );
	static u32					GetIndexForUse					( u32 use );
	static u32					GetUseForIndex					( u32 idx );
	static size_t				PackAttribute					( void* buffer, const ZLVec4D& coord, const MOAIVertexAttribute& attribute );
	static ZLVec4D				ReadCoord						( MOAIStream& stream, size_t stride, size_t components );
	void						UnbindFixed						() const;
	void						UnbindProgrammable				() const;
	static ZLVec4D				UnpackAttribute					( const void* buffer, const MOAIVertexAttribute& attribute, float zFallback, float wFallback );
	static ZLVec4D				UnpackCoord						( const void* buffer, const MOAIVertexAttribute& attribute );
	
public:
	
	friend class MOAIGfxDevice;
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	static MOAIVertexFormat*	AffirmVertexFormat				( MOAILuaState& state, int idx );
	void						Bind							( const void* buffer ) const;
	int							Compare							( const void* v0, const void* v1, float componentEpsilon, float normEpsilon );
	bool						ComputeBounds					( ZLBox& bounds, void* buffer, size_t size ) const;
	bool						ComputeBounds					( ZLBox& bounds, MOAIStream& stream, size_t size ) const;
	static bool					ComputeBounds					( ZLBox& bounds, MOAIStream& stream, size_t size, size_t offset, size_t stride, size_t components );
	size_t						CountElements					( size_t size );
	static size_t				CountElements					( size_t size, size_t offset, size_t stride );
	void						DeclareAttribute				( u32 index, u32 type, u32 size, u32 use, bool normalized );
								MOAIVertexFormat				();
								~MOAIVertexFormat				();
	void						PrintVertices					( ZLStream& stream, size_t size ) const;
	void						RegisterLuaClass				( MOAILuaState& state );
	void						RegisterLuaFuncs				( MOAILuaState& state );
	
	size_t						SeekVertex						( ZLStream& stream, size_t base, size_t vertex );
	
	void						SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
	void						SnapCoords						( void* dest, float xSnap, float ySnap, float zSnap );
	void						Unbind							() const;
	
	void						WriteAttribute					( ZLStream& stream, size_t base, size_t vertex, u32 attrID, float x, float y, float z, float w );
	void						WriteColor						( ZLStream& stream, size_t base, size_t vertex, u32 color );
	void						WriteColor						( ZLStream& stream, size_t base, size_t vertex, float r, float g, float b, float a );
	void						WriteNormal						( ZLStream& stream, size_t base, size_t vertex, float x, float y, float z );
	void						WriteUV							( ZLStream& stream, size_t base, size_t vertex, float x, float y, float z );
	void						WriteVertex						( ZLStream& stream, size_t base, size_t vertex, float x, float y, float z, float w );
};

#endif
