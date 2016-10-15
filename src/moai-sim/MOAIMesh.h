// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moai-sim/MOAIVertexArray.h>

class MOAIIndexBuffer;
class MOAIMesh;
class MOAIMeshPartition;
class MOAISelectionSpan;
class MOAISingleTexture;
class MOAIVertexAttribute;

//================================================================//
// MOAIMeshSpan
//================================================================//
class MOAIMeshSpan {
public:
	
	size_t				mBase;
	size_t				mTop;
	
	MOAIMeshSpan*		mPrev;
	MOAIMeshSpan*		mNext;
};

//================================================================//
// MOAIMeshPrimCoords
//================================================================//
class MOAIMeshPrimCoords {
public:
	
	static const u32 PRIM_POINT			= 1;
	static const u32 PRIM_LINE			= 2;
	static const u32 PRIM_TRIANGLE		= 3;
	static const u32 MAX_COORDS			= 3;
	
	u32			mIndex;
	u32			mPrimSize;
	ZLVec3D		mCoords [ MAX_COORDS ];
	
	//----------------------------------------------------------------//
	ZLBox		GetBounds		();
};

//================================================================//
// MOAIMeshPrimReader
//================================================================//
class MOAIMeshPrimReader {
private:

	friend class MOAIMesh;

	MOAIMesh*				mMesh;
	MOAIVertexFormat*		mVertexFormat;
	u32						mTotalPrims;
	
	const MOAIVertexAttribute*	mAttribute;
	const void*					mVertexBuffer;
	MOAIIndexBuffer*			mIndexBuffer;

	//----------------------------------------------------------------//
	bool		Init			( MOAIMesh& mesh, u32 vertexBufferIndex );
	ZLVec3D		ReadCoord		( u32 idx ) const;

public:

	GET_CONST ( u32, TotalPrims, mTotalPrims )

	//----------------------------------------------------------------//
	bool		GetPrimCoords	( u32 idx, MOAIMeshPrimCoords& prim ) const;
};

//================================================================//
// MOAIMesh
//================================================================//
/**	@lua	MOAIMesh
	@text	Loads a texture and renders the contents of a vertex buffer.
			Grid drawing not supported.
	
	@const	GL_POINTS
	@const	GL_LINES
	@const	GL_TRIANGLES
	@const	GL_LINE_LOOP
	@const	GL_LINE_STRIP
	@const	GL_TRIANGLE_FAN
	@const	GL_TRIANGLE_STRIP
*/
class MOAIMesh :
	public MOAIStandardDeck,
	public MOAIVertexArray {
protected:

	friend class MOAIMeshPrimReader;

	MOAILuaSharedPtr < MOAIIndexBuffer > mIndexBuffer;

	u32			mTotalElements;
	bool		mHasBounds;
	ZLBox		mBounds;

	u32			mPrimType;
	
	float		mPenWidth;
	
	MOAIMeshPartition*	mPartition;

	//----------------------------------------------------------------//
	static int			_buildQuadTree				( lua_State* L );
	static int			_buildTernaryTree			( lua_State* L );
	static int			_getPrimsForPoint			( lua_State* L );
	static int			_getRegionForPrim			( lua_State* L );
	static int			_intersectRay				( lua_State* L );
	static int			_printPartition				( lua_State* L );
	static int			_readPrimCoords				( lua_State* L );
	static int			_setBounds					( lua_State* L );
	static int			_setIndexBuffer				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	static int			_setPrimType				( lua_State* L );
	static int			_setTotalElements			( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox				ComputeMaxBounds			();
	ZLBox				GetItemBounds				( u32 idx );

public:

	DECL_LUA_FACTORY ( MOAIMesh )
	
	GET_SET ( u32, PrimType, mPrimType )
	GET_SET ( u32, TotalElements, mTotalElements )
	
	//----------------------------------------------------------------//
	void				ClearBounds					();
	u32					CountPrims					() const;
	void				DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	void				DrawIndex					( u32 idx, MOAIMeshSpan* span, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
						MOAIMesh					();
						~MOAIMesh					();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetBounds					( const ZLBox& bounds );
	void				SetIndexBuffer				( MOAIIndexBuffer* indexBuffer );
};

#endif
