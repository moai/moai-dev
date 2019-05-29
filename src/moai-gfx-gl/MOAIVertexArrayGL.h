// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAYGL_H
#define	MOAIVERTEXARRAYGL_H

#include <moai-gfx-gl/MOAIAbstractGfxResourceGL.h>

class MOAIVertexBufferGL;
class MOAIVertexFormatGL;

//================================================================//
// MOAIVertexBufferWithFormatGL
//================================================================//
class MOAIVertexBufferWithFormatGL {
public:

	ZLStrongPtr < MOAIVertexBufferGL >				mBuffer;
	ZLStrongPtr < MOAIVertexFormatGL >				mFormat;

	//----------------------------------------------------------------//
				MOAIVertexBufferWithFormatGL		();
				~MOAIVertexBufferWithFormatGL		();
	void		SetBufferAndFormat				( MOAIVertexBufferGL* buffer, MOAIVertexFormatGL* format );
	
};

//================================================================//
// MOAIVertexArrayGL
//================================================================//
// TODO: doxygen
class MOAIVertexArrayGL :
	public virtual MOAIAbstractVertexArray,
	public virtual MOAIAbstractGfxResourceGL {
protected:

	ZLLeanArray < ZLGfxHandle >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	ZLIndex											mCurrentVAO;

	ZLLeanArray < MOAIVertexBufferWithFormatGL >		mVertexBuffers;

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
	
	IS ( UsingVAOs, mUseVAOs, true )
	
	DECL_LUA_FACTORY ( MOAIVertexArrayGL )
	
	//----------------------------------------------------------------//
	MOAIVertexBufferGL*		GetVertexBuffer				( ZLIndex idx );
	MOAIVertexFormatGL*		GetVertexFormat				( ZLIndex idx );
							MOAIVertexArrayGL			();
							~MOAIVertexArrayGL			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					ReserveVAOs					( u32 total );
	void					ReserveVertexBuffers		( u32 total );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void					SetVertexBuffer				( ZLIndex idx, MOAIVertexBufferGL* vtxBuffer, MOAIVertexFormatGL* vtxFormat );
};

#endif
