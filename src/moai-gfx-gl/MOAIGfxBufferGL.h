// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERGL_H
#define	MOAIGFXBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class MOAIVertexFormatGL;

//================================================================//
// MOAIGfxBufferGL
//================================================================//
/**	@lua	MOAIGfxBufferGL
	@text	Base class for MOAIVertexBufferGL and MOAIIndexBufferGL.
*/
class MOAIGfxBufferGL :
	public virtual MOAIGfxBuffer,
	public virtual MOAIGfxResourceGL {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	friend class MOAIGfxMgrGL_DisplayListClerkGL;
	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class MOAIVertexArrayItem;
	friend class MOAIVertexArrayGL;
	friend class MOAIMeshGL;
	
	enum {
		UPDATE_MODE_MAPBUFFER,
		UPDATE_MODE_ORPHAN,
		UPDATE_MODE_SUBDATA,
	};
	
	ZLLeanArray < ZLGfxHandle >		mVBOs;
	ZLIndex							mCurrentVBO;
	ZLGfxEnum::_					mTarget;

	bool							mCopyOnUpdate;
	
	//----------------------------------------------------------------//
	static int				_reserveVBOs			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLSharedConstBuffer*	GetBufferForBind		( ZLGfx& gfx );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_SerializeIn								( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					_SerializeOut								( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void					MOAIGfxResourceGL_OnGPUBind					();
	bool					MOAIGfxResourceGL_OnGPUCreate				();
	void					MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					MOAIGfxResourceGL_OnGPUUnbind				();
	bool					MOAIGfxResourceGL_OnGPUUpdate				();

public:
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	GET ( u32, Target, mTarget )
	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
	
//	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
	size_t					CountVBOs					();
							MOAIGfxBufferGL				( ZLContext& context );
							~MOAIGfxBufferGL			();
	void					ReserveVBOs					( ZLSize gpuBuffers );
	void					Update						();
};

#endif
