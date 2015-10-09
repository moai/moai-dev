// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moai-sim/MOAIVertexArray.h>

class MOAIIndexBuffer;
class MOAIMesh;
class MOAISelectionSpan;
class MOAISingleTexture;

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

	MOAILuaSharedPtr < MOAIIndexBuffer > mIndexBuffer;

	u32			mTotalElements;
	bool		mHasBounds;
	ZLBox		mBounds;

	u32			mPrimType;
	
	float		mPenWidth;

	//----------------------------------------------------------------//
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
