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
		ARRAY_COLOR,
		ARRAY_NORMAL,
		ARRAY_TEX_COORD,
		ARRAY_VERTEX,
		TOTAL_ARRAY_TYPES,
	};

private:

	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 NULL_INDEX				= 0xffffffff;
	
	ZLLeanArray < MOAIVertexAttribute >		mAttributes;
	u32										mTotalAttributes;
	u32										mVertexSize;

	MOAIVertexAttributeUse					mAttributeUseTable [ TOTAL_ARRAY_TYPES ]; // use for fixed function pipeline and for computing bounds
	
	//----------------------------------------------------------------//
	static int			_declareAttribute				( lua_State* L );
	static int			_declareColor					( lua_State* L );
	static int			_declareCoord					( lua_State* L );
	static int			_declareNormal					( lua_State* L );
	static int			_declareUV						( lua_State* L );
	static int			_getVertexSize					( lua_State* L );
	
	//----------------------------------------------------------------//
	void				BindFixed						( void* buffer ) const;
	void				BindProgrammable				( void* buffer ) const;
	static u32			GetComponentSize				( u32 size, u32 type );
	static u32			GetIndexForUse					( u32 use );
	static u32			GetUseForIndex					( u32 idx );
	static ZLVec3D		ReadCoord						( MOAIStream& stream, size_t stride, size_t components );
	void				UnbindFixed						() const;
	void				UnbindProgrammable				() const;
	
public:
	
	friend class MOAIGfxDevice;
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	void				Bind							( void* buffer ) const;
	bool				ComputeBounds					( ZLBox& bounds, void* buffer, size_t size ) const;
	bool				ComputeBounds					( ZLBox& bounds, MOAIStream& stream, size_t size ) const;
	static bool			ComputeBounds					( ZLBox& bounds, MOAIStream& stream, size_t size, size_t offset, size_t stride, size_t components );
	size_t				CountElements					( size_t size );
	static size_t		CountElements					( size_t size, size_t offset, size_t stride );
	void				DeclareAttribute				( u32 index, u32 type, u32 size, u32 use, bool normalized );
						MOAIVertexFormat				();
						~MOAIVertexFormat				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
	void				SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
	void				Unbind							() const;
};

#endif
