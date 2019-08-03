// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCHHOLDER_H
#define	MOAIMATERIALBATCHHOLDER_H

#include <moai-gfx/MOAIMaterialBatch.h>

class MOAIImage;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialBatchHolder :
	public virtual MOAIAbstractMaterialBatchInterface {
protected:

	MOAILuaSharedPtr < MOAIMaterialBatch > mMaterialBatch;

	//----------------------------------------------------------------//
	static int				_affirmMaterialBatch	( lua_State* L );
	static int				_getMaterialBatch		( lua_State* L );
	static int				_setMaterialBatch		( lua_State* L );

	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass								( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs								( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn									( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut									( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	MOAIMaterial&			MOAIAbstractMaterialBatchInterface_AffirmMaterial			( ZLIndex index );
	MOAIMaterialBatch&		MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch		();
	MOAIMaterial*			MOAIAbstractMaterialBatchInterface_GetMaterial				( ZLIndex index );
	MOAIMaterialBatch* 		MOAIAbstractMaterialBatchInterface_GetMaterialBatch 		();

public:

	DECL_LUA_FACTORY ( MOAIMaterialBatchHolder )

	//----------------------------------------------------------------//
							MOAIMaterialBatchHolder		();
							~MOAIMaterialBatchHolder	();
};

#endif
