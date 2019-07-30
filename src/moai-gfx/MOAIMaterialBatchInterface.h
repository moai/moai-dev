// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCHINTERFACE_H
#define	MOAIMATERIALBATCHINTERFACE_H

#include <moai-gfx/MOAIMaterialInterface.h>

class MOAIImage;
class MOAIMaterial;
class MOAIMaterialBatch;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIMaterialBatchInterface
//================================================================//
// TODO: doxygen
class MOAIMaterialBatchInterface :
	public virtual MOAIMaterialInterface {
protected:
	
	//----------------------------------------------------------------//
	static int			_affirmMaterial				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getMaterial				( lua_State* L );
	static int			_reserveMaterials			( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );

	//----------------------------------------------------------------//
	virtual MOAIMaterial&			MOAIMaterialBatchInterface_AffirmMaterial			( ZLIndex index ) = 0;
	virtual MOAIMaterialBatch&		MOAIMaterialBatchInterface_AffirmMaterialBatch		() = 0;
	virtual MOAIMaterial*			MOAIMaterialBatchInterface_GetMaterial				( ZLIndex index ) = 0;
	virtual MOAIMaterialBatch*		MOAIMaterialBatchInterface_GetMaterialBatch			() = 0;
	
	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	MOAIMaterial&			MOAIMaterialInterface_AffirmMaterial		();
	MOAIMaterial*			MOAIMaterialInterface_GetMaterial			();
	
public:

	//----------------------------------------------------------------//
	MOAIMaterial&			AffirmMaterial						();
	MOAIMaterial&			AffirmMaterial						( ZLIndex index );
	MOAIMaterialBatch&		AffirmMaterialBatch					();
	MOAIMaterial*			GetMaterial							();
	MOAIMaterial*			GetMaterial							( ZLIndex index );
	MOAIMaterialBatch*		GetMaterialBatch					();
							MOAIMaterialBatchInterface			();
	virtual					~MOAIMaterialBatchInterface			();
};

#endif
