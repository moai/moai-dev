// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAY_H
#define	MOAIVERTEXARRAY_H

#include <moai-sim/MOAIGfxStateCache.h>

class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIVertexArray
//================================================================//
// TODO: doxygen
class MOAIVertexArray :
	public MOAIGfxResource {
protected:

	ZLLeanArray < ZLGfxHandle* >					mVAOs; // vertex array objects to bind all the vertex and buffer state
	u32												mCurrentVAO;

	ZLLeanArray < MOAIVertexBufferWithFormat >		mVertexBuffers;

	bool				mUseVAOs;

	//----------------------------------------------------------------//
	static int			_reserveVAOs				( lua_State* L );
	static int			_reserveVertexBuffers		( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
	bool				AffirmVertexBuffers			( u32 idx );
	void				BindVertexArrayItems		();
	bool				OnCPUCreate					(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void				OnCPUDestroy				(); // clear any CPU-side memory used by class
	void				OnGPUBind					(); // select GPU-side resource on device for use
	bool				OnGPUCreate					(); // create GPU-side resource
	void				OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void				OnGPUUnbind					(); // unbind GPU-side resource
	bool				OnGPUUpdate					();
	void				UnbindVertexArrayItems		();

public:
	
	DECL_LUA_FACTORY ( MOAIVertexArray )
	
	IS ( UsingVAOs, mUseVAOs, true )
	
	//----------------------------------------------------------------//
	MOAIVertexBuffer*	GetVertexBuffer				( u32 idx );
	MOAIVertexFormat*	GetVertexFormat				( u32 idx );
						MOAIVertexArray				();
						~MOAIVertexArray			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetVertexBuffer				( u32 idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
};

#endif
