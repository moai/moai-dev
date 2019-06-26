// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAYVK_H
#define	MOAIVERTEXARRAYVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIVertexBufferVK;
class MOAIVertexFormatVK;

////================================================================//
//// MOAIVertexBufferWithFormatVK
////================================================================//
//class MOAIVertexBufferWithFormatVK {
//public:
//
//	ZLStrongPtr < MOAIVertexBufferVK >				mBuffer;
//	ZLStrongPtr < MOAIVertexFormatVK >				mFormat;
//
//	//----------------------------------------------------------------//
//				MOAIVertexBufferWithFormatVK		();
//				~MOAIVertexBufferWithFormatVK		();
//	void		SetBufferAndFormat				( MOAIVertexBufferVK* buffer, MOAIVertexFormatVK* format );
//
//};

//================================================================//
// MOAIVertexArrayVK
//================================================================//
// TODO: doxygen
class MOAIVertexArrayVK :
	public virtual MOAIVertexArray,
	public virtual MOAIGfxResourceVK {
protected:

//	ZLLeanArray < ZLGfxHandle >						mVAOs; // vertex array objects to bind all the vertex and buffer state
//	ZLIndex											mCurrentVAO;
//
//	ZLLeanArray < MOAIVertexBufferWithFormatVK >	mVertexBuffers;
//
//	bool					mUseVAOs;

	//----------------------------------------------------------------//
//	static int				_reserveVAOs				( lua_State* L );
//	static int				_reserveVertexBuffers		( lua_State* L );
//	static int				_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
//	bool					AffirmVertexBuffers			( u32 idx );
//	void					BindVertexArrayItems		();
//	void					UnbindVertexArrayItems		();
	
	//----------------------------------------------------------------//
	bool					MOAIGfxResource_OnCPUCreate					(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void					MOAIGfxResource_OnCPUPurgeRecoverable		(); // clear any recoverable CPU-side memory used by class
	void					MOAIGfxResourceVK_OnGPUBind					(); // select GPU-side resource on device for use
	bool					MOAIGfxResourceVK_OnGPUCreate				(); // create GPU-side resource
	void					MOAIGfxResourceVK_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void					MOAIGfxResourceVK_OnGPUUnbind				(); // unbind GPU-side resource
	bool					MOAIGfxResourceVK_OnGPUUpdate				();
	void					MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut					( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	MOAIVertexBuffer*		MOAIVertexArray_GetVertexBuffer				( ZLIndex idx );
	MOAIVertexFormat*		MOAIVertexArray_GetVertexFormat				( ZLIndex idx );
	void					MOAIVertexArray_SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );

public:
	
//	IS ( UsingVAOs, mUseVAOs, true )
	
	DECL_LUA_FACTORY ( MOAIVertexArrayVK )
	
	//----------------------------------------------------------------//
							MOAIVertexArrayVK			();
							~MOAIVertexArrayVK			();
//	void					ReserveVAOs					( u32 total );
//	void					ReserveVertexBuffers		( u32 total );
};

#endif
