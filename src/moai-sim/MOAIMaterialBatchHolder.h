// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCHHOLDER_H
#define	MOAIMATERIALBATCHHOLDER_H

#include <moai-sim/MOAIMaterialBatch.h>

class MOAIImage;
class MOAIShader;
class MOAITextureBase;

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialBatchHolder :
	public virtual MOAILuaObject {
private:
	
	MOAILuaSharedPtr < MOAIMaterialBatch > mMaterialBatch;
	
	//----------------------------------------------------------------//
	static int			_clearBlendMode				( lua_State* L );
	static int			_clearCullMode				( lua_State* L );
	static int			_clearDepthMask				( lua_State* L );
	static int			_clearDepthTest				( lua_State* L );
	static int			_clearShader				( lua_State* L );
	static int			_clearTexture				( lua_State* L );
	static int			_getBlendMode				( lua_State* L );
	static int			_getCullMode				( lua_State* L );
	static int			_getDepthMask				( lua_State* L );
	static int			_getDepthTest				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getMaterialBatch			( lua_State* L );
	static int			_getShader					( lua_State* L );
	static int			_getTexture					( lua_State* L );
	static int			_reserveMaterials			( lua_State* L );
	static int			_setBlendMode				( lua_State* L );
	static int			_setCullMode				( lua_State* L );
	static int			_setDepthMask				( lua_State* L );
	static int			_setDepthTest				( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	static int			_setMaterialBatch			( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );

	//----------------------------------------------------------------//
	MOAIMaterialBatch*	AffirmMaterialBatch			();

public:

	DECL_LUA_FACTORY ( MOAIMaterialBatchHolder )

	//----------------------------------------------------------------//
	MOAIMaterial*		GetMaterial					( u32 idx = 0 );
						MOAIMaterialBatchHolder		();
	virtual				~MOAIMaterialBatchHolder	();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
