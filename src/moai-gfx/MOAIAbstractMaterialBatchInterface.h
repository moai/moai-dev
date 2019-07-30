// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTMATERIALBATCHINTERFACE_H
#define	MOAIABSTRACTMATERIALBATCHINTERFACE_H

#include <moai-gfx/MOAIAbstractMaterialInterface.h>

class MOAIImage;
class MOAIMaterial;
class MOAIMaterialBatch;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIAbstractMaterialBatchInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractMaterialBatchInterface :
	public virtual MOAIAbstractMaterialInterface {
protected:
	
	//----------------------------------------------------------------//
	static int			_affirmMaterial				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getMaterial				( lua_State* L );
	static int			_reserveMaterials			( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );

	//----------------------------------------------------------------//
	virtual MOAIMaterial&			MOAIAbstractMaterialBatchInterface_AffirmMaterial			( ZLIndex index ) = 0;
	virtual MOAIMaterialBatch&		MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch		() = 0;
	virtual MOAIMaterial*			MOAIAbstractMaterialBatchInterface_GetMaterial				( ZLIndex index ) = 0;
	virtual MOAIMaterialBatch*		MOAIAbstractMaterialBatchInterface_GetMaterialBatch			() = 0;
	
	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass					( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs					( MOAIComposer& composer, MOAILuaState& state );
	MOAIMaterial&			MOAIAbstractMaterialInterface_AffirmMaterial	();
	MOAIMaterial*			MOAIAbstractMaterialInterface_GetMaterial		();
	
public:

	//----------------------------------------------------------------//
	MOAIMaterial&			AffirmMaterial							();
	MOAIMaterial&			AffirmMaterial							( ZLIndex index );
	MOAIMaterialBatch&		AffirmMaterialBatch						();
	MOAIMaterial*			GetMaterial								();
	MOAIMaterial*			GetMaterial								( ZLIndex index );
	MOAIMaterialBatch*		GetMaterialBatch						();
							MOAIAbstractMaterialBatchInterface		();
	virtual					~MOAIAbstractMaterialBatchInterface		();
};

#endif
