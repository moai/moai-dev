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
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
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
	void							_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractMaterial&			MOAIAbstractMaterialInterface_AffirmMaterial	();
	MOAIAbstractMaterial*			MOAIAbstractMaterialInterface_GetMaterial		();
	
public:

	//----------------------------------------------------------------//
	MOAIAbstractMaterial&			AffirmMaterial							();
	MOAIMaterial&					AffirmMaterial							( ZLIndex index );
	MOAIMaterialBatch&				AffirmMaterialBatch						();
	MOAIAbstractMaterial*			GetMaterial								();
	MOAIMaterial*					GetMaterial								( ZLIndex index );
	MOAIMaterialBatch*				GetMaterialBatch						();
									MOAIAbstractMaterialBatchInterface		();
	virtual							~MOAIAbstractMaterialBatchInterface		();
};

#endif
