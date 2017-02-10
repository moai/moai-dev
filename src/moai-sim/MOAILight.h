// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHT_H
#define	MOAILIGHT_H

#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAIShaderUniformBuffer.h>

class MOAILightFormat;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAILight
//================================================================//
class MOAILight :
	public virtual MOAINode,
	public MOAIShaderUniformBuffer {
private:

	MOAILuaSharedPtr < MOAILightFormat >	mFormat;
	ZLLeanArray < u8 >						mBuffer;
	ZLLeanArray < MOAITexture* >			mTextures;

	//----------------------------------------------------------------//
	static int			_getFormat					( lua_State* L );
	static int			_setFormat					( lua_State* L );
	static int			_setTexture					( lua_State* L );
	static int			_setTransform				( lua_State* L );
	static int			_setUniform					( lua_State* L );

	//----------------------------------------------------------------//
	bool								MOAINode_ApplyAttrOp						( u32 attrID, MOAIAttribute& attr, u32 op );
	MOAIShaderUniformFormatter*			MOAIShaderUniformBuffer_GetUniform			( u32 uniformID, void*& buffer );

public:

	DECL_LUA_FACTORY ( MOAILight )

	//----------------------------------------------------------------//
	void				ApplyUniforms				( void* buffer, size_t bufferSize );
	void				BindTextures				( u32 textureOffset );
						MOAILight					();
						~MOAILight					();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SetFormat					( MOAILightFormat* format );
};

#endif
