// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHT_H
#define	MOAILIGHT_H

#include <moai-gfx/MOAIShaderUniformSchema.h>

class MOAILightFormat;
class MOAITexture;

//================================================================//
// MOAILight
//================================================================//
class MOAILight :
	public virtual MOAINode,
	public virtual MOAIShaderUniformSchema {
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
	void						MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	bool						MOAINode_ApplyAttrOp						( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	MOAIShaderUniformHandle		MOAIAbstractShaderUniformSchema_GetUniformHandle	( void* buffer, ZLIndex uniformID ) const;

public:

	DECL_LUA_FACTORY ( MOAILight )

	//----------------------------------------------------------------//
	void				ApplyUniforms				( void* buffer, size_t bufferSize );
	void				BindTextures				( u32 textureOffset );
						MOAILight					();
						~MOAILight					();
	void				SetFormat					( MOAILightFormat* format );
};

#endif
