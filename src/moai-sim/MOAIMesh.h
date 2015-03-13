// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moai-sim/MOAIDeck.h>

class MOAIGfxBuffer;
class MOAIMesh;
class MOAITextureBase;

//================================================================//
// MOAIVertexBuffer
//================================================================//
class MOAIVertexBuffer {
public:
	
	MOAILuaSharedPtr < MOAIGfxBuffer >		mBuffer;
	MOAILuaSharedPtr < MOAIVertexFormat >	mFormat;
	
	//----------------------------------------------------------------//
	void		Bind					();
				MOAIVertexBuffer		();
				~MOAIVertexBuffer		();
	void		SetBufferAndFormat		( MOAIMesh& owner, MOAIGfxBuffer* buffer, MOAIVertexFormat* format );
	void		Unbind					();
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
	public MOAIDeck,
	public MOAIGfxResource {
private:

	ZLLeanArray < u32 >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	u32										mCurrentVAO;

	ZLLeanArray < MOAIVertexBuffer >		mVertexBuffers;
	MOAILuaSharedPtr < MOAIGfxBuffer >		mIndexBuffer;

	u32			mIndexSizeInBytes;

	u32			mTotalElements;
	bool		mHasBounds;
	ZLBox		mBounds;

	u32			mPrimType;
	
	float		mPenWidth;
	float		mPointSize;
	
	bool		mUseVAOs;
	bool		mNeedsRefresh;

	//----------------------------------------------------------------//
	static int			_reserveVAOs				( lua_State* L );
	static int			_reserveVertexBuffers		( lua_State* L );
	static int			_setBounds					( lua_State* L );
	static int			_setIndexBuffer				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	static int			_setPointSize				( lua_State* L );
	static int			_setPrimType				( lua_State* L );
	static int			_setTotalElements			( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
	bool				AffirmVertexBuffers			( u32 idx );
	void				BindVertex					();
	ZLBox				ComputeMaxBounds			();
	ZLBox				GetItemBounds				( u32 idx );
	bool				OnCPUCreate					(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void				OnCPUDestroy				(); // clear any CPU-side memory used by class
	void				OnGPUBind					(); // select GPU-side resource on device for use
	bool				OnGPUCreate					(); // create GPU-side resource
	void				OnGPUDestroy				(); // schedule GPU-side resource for destruction
	void				OnGPULost					(); // clear any handles or references to GPU-side (called by 'Abandon')
	void				OnGPUUnbind					(); // unbind GPU-side resource
	void				UnbindVertex				();

public:
	
	DECL_LUA_FACTORY ( MOAIMesh )
	
	//----------------------------------------------------------------//
	void				ClearBounds					();
	void				DrawIndex					( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	MOAIGfxState*		GetShaderDefault			();
						MOAIMesh					();
						~MOAIMesh					();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetIndexBuffer				( MOAIGfxBuffer* indexBuffer );
	void				SetVertexBuffer				( u32 idx, MOAIGfxBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
	void				SetVertexBufferBounds		( u32 idx, bool hasBounds, const ZLBox& bounds );
};

#endif
