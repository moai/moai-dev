// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASMATERIALBATCH_H
#define	MOAIHASMATERIALBATCH_H

#include <moai-gfx/MOAIMaterialBatch.h>

class MOAIImage;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIHasMaterialBatch
//================================================================//
// TODO: doxygen
class MOAIHasMaterialBatch :
	public virtual MOAIAbstractMaterialBatchInterface {
protected:

	MOAILuaSharedPtr < MOAIMaterialBatch > mMaterialBatch;

	//----------------------------------------------------------------//
	static int				_affirmMaterialBatch	( lua_State* L );
	static int				_getMaterialBatch		( lua_State* L );
	static int				_setMaterialBatch		( lua_State* L );

	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIMaterial&			MOAIAbstractMaterialBatchInterface_AffirmMaterial			( ZLIndex index );
	MOAIMaterialBatch&		MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch		();
	MOAIMaterial*			MOAIAbstractMaterialBatchInterface_GetMaterial				( ZLIndex index );
	MOAIMaterialBatch* 		MOAIAbstractMaterialBatchInterface_GetMaterialBatch 		();

public:

	DECL_LUA_FACTORY ( MOAIHasMaterialBatch )

	//----------------------------------------------------------------//
							MOAIHasMaterialBatch		();
							~MOAIHasMaterialBatch		();
};

#endif
