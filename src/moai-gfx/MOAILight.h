// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHT_H
#define	MOAILIGHT_H

#include <moai-gfx/MOAIShaderUniformSchemaBase.h>

class MOAILightFormat;
class MOAITexture;

//================================================================//
// MOAILight
//================================================================//
class MOAILight :
	public virtual MOAINode,
	public virtual MOAIShaderUniformSchemaBase {
private:

	MOAILuaSharedPtr < MOAILightFormat >	mFormat;
	ZLLeanArray < u8 >						mBuffer;
	ZLLeanArray < MOAITexture* >	mTextures;

	//----------------------------------------------------------------//
	static int			_getFormat					( lua_State* L );
	static int			_setFormat					( lua_State* L );
	static int			_setTexture					( lua_State* L );
	static int			_setTransform				( lua_State* L );
	static int			_setUniform					( lua_State* L );

	//----------------------------------------------------------------//
	bool								MOAINode_ApplyAttrOp								( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	MOAIShaderUniformHandle				ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const;

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
