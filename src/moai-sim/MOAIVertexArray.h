// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAY_H
#define	MOAIVERTEXARRAY_H

class MOAIVertexArray;
class MOAIVertexBuffer;
class ZLVertexBufferWithFormat;
class MOAIVertexFormat;

//================================================================//
// MOAIVertexArray
//================================================================//
// TODO: doxygen
class MOAIVertexArray :
	public MOAIGfxResource < ZLAbstractGfxResource > {
protected:

	ZLLeanArray < ZLGfxHandle >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	ZLIndex											mCurrentVAO;

	ZLLeanArray < ZLVertexBufferWithFormat >		mVertexBuffers;

	bool				mUseVAOs;

	//----------------------------------------------------------------//
	static int			_reserveVAOs				( lua_State* L );
	static int			_reserveVertexBuffers		( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
	bool				AffirmVertexBuffers			( u32 idx );
	void				BindVertexArrayItems		();
	void				UnbindVertexArrayItems		();
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void				ZLAbstractGfxResource_OnCPUDestroy				(); // clear any CPU-side memory used by class
	void				ZLAbstractGfxResource_OnGPUBind					(); // select GPU-side resource on device for use
	bool				ZLAbstractGfxResource_OnGPUCreate				(); // create GPU-side resource
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void				ZLAbstractGfxResource_OnGPUUnbind				(); // unbind GPU-side resource
	bool				ZLAbstractGfxResource_OnGPUUpdate				();

public:
	
	DECL_LUA_FACTORY ( MOAIVertexArray )
	
	IS ( UsingVAOs, mUseVAOs, true )
	
	//----------------------------------------------------------------//
	MOAIVertexBuffer*	GetVertexBuffer				( ZLIndex idx );
	MOAIVertexFormat*	GetVertexFormat				( ZLIndex idx );
						MOAIVertexArray				();
						~MOAIVertexArray			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
};

#endif
