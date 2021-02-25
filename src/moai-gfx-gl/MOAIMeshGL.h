// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHGL_H
#define	MOAIMESHGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

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
	void		SetBufferAndFormat					( MOAIVertexBufferGL* buffer, MOAIVertexFormatGL* format );
	
};

//================================================================//
// MOAIMeshGL
//================================================================//
// TODO: doxygen
class MOAIMeshGL :
	public virtual MOAIMesh,
	public virtual MOAIGfxResourceGL {
protected:

	ZLGfxHandle										mVAO;

	ZLStrongPtr < MOAIIndexBufferGL >				mIndexBuffer;
	ZLLeanArray < MOAIVertexBufferWithFormatGL >	mVertexBuffers;

	//----------------------------------------------------------------//
	static int				_reserveVertexBuffers			( lua_State* L );
	static int				_setIndexBuffer					( lua_State* L );
	static int				_setVertexBuffer				( lua_State* L );

	//----------------------------------------------------------------//
	bool					AffirmVertexBuffers				( u32 idx );
	void					BindVertexArrayItems			();
	void					UnbindVertexArrayItems			();
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_SerializeIn								( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					_SerializeOut								( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void 					MOAIMesh_DrawPrims							( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count );
	MOAIIndexBuffer*		MOAIMesh_GetIndexBuffer						();
	MOAIVertexBuffer*		MOAIMesh_GetVertexBuffer					( ZLIndex idx );
	MOAIVertexFormat*		MOAIMesh_GetVertexFormat					( ZLIndex idx );
	void					MOAIMesh_ReserveVertexBuffers				( ZLSize size );
	void					MOAIMesh_SetIndexBuffer						( MOAIIndexBuffer* idxBuffer );
	void					MOAIMesh_SetVertexBuffer					( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
	void					MOAIGfxResourceGL_OnGPUBind					(); // select GPU-side resource on device for use
	bool					MOAIGfxResourceGL_OnGPUCreate				(); // create GPU-side resource
	void					MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void					MOAIGfxResourceGL_OnGPUUnbind				(); // unbind GPU-side resource
	bool					MOAIGfxResourceGL_OnGPUUpdate				();

public:
		
	DECL_LUA_FACTORY ( MOAIMeshGL )
	
	//----------------------------------------------------------------//
							MOAIMeshGL					();
							~MOAIMeshGL					();
};

#endif
