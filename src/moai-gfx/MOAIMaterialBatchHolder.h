// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCHHOLDER_H
#define	MOAIMATERIALBATCHHOLDER_H

#include <moai-gfx/MOAIMaterialBatch.h>

class MOAIImage;
class MOAIMaterial;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialBatchHolder :
	public virtual MOAILuaObject {
protected:

	MOAILuaSharedPtr < MOAIMaterialBatch > mMaterialBatch;

	//----------------------------------------------------------------//
	static int				_getBlendMode			( lua_State* L );
	static int				_getCullMode			( lua_State* L );
	static int				_getDepthMask			( lua_State* L );
	static int				_getDepthTest			( lua_State* L );
	static int				_getLight				( lua_State* L );
	static int				_getMaterialBatch		( lua_State* L );
	static int				_getShader				( lua_State* L );
	static int				_getTexture				( lua_State* L );
	static int				_reserveMaterials		( lua_State* L );
	static int				_setBlendMode			( lua_State* L );
	static int				_setCullMode			( lua_State* L );
	static int				_setDepthMask			( lua_State* L );
	static int				_setDepthTest			( lua_State* L );
	static int				_setIndexBatchSize		( lua_State* L );
	static int				_setLight				( lua_State* L );
	static int				_setMaterialBatch		( lua_State* L );
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIMaterialBatchHolder )

	//----------------------------------------------------------------//
	MOAIMaterialBatch*		AffirmMaterialBatch			();
	MOAIMaterial*			GetMaterial					( ZLIndex idx = ZLIndexOp::ZERO );
							MOAIMaterialBatchHolder		();
							~MOAIMaterialBatchHolder	();
	void					MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
