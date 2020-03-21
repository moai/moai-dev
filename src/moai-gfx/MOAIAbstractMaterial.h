// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTMATERIAL_H
#define	MOAIABSTRACTMATERIAL_H

#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIAbstractMaterialInterface.h>

//================================================================//
// MOAIMaterialGlobals
//================================================================//
class MOAIMaterialGlobals {
public:

	static const u32 MOAI_UNKNOWN_MATERIAL_GLOBAL	= 0x80000000;
	static const u32 MAX_GLOBAL_LIGHTS				= 256;
	static const u32 MAX_GLOBAL_TEXTURES			= 32;

	//----------------------------------------------------------------//
	static u32			GetNamedGlobalID						( MOAILuaState& state, int idx );
};

//================================================================//
// MOAIAbstractMaterial
//================================================================//
// TODO: doxygen
class MOAIAbstractMaterial :
	public virtual MOAIAbstractMaterialInterface {
protected:

	friend class MOAIAbstractMaterial;
	friend class MOAIGfxComposerBatch;
	friend class MOAIAbstractMaterialInterface;
	friend class MOAIMaterialMgr;
		
	enum {
		BLEND_MODE_FLAG		= 0x01 << 0,
		CULL_MODE_FLAG		= 0x01 << 1,
		DEPTH_MASK_FLAG		= 0x01 << 2,
		DEPTH_TEST_FLAG		= 0x01 << 3,
		SHADER_FLAG			= 0x01 << 4,
		TEXTURE_FLAG		= 0x01 << 5,
		
		MAX_FLAG			= 0x01 << 6,
	};
	
	static const u32	DRAW_FLAGS	= BLEND_MODE_FLAG | CULL_MODE_FLAG | DEPTH_MASK_FLAG | DEPTH_TEST_FLAG;
	static const u32	ALL_FLAGS	= MAX_FLAG - 1;
	
	ZLStrongPtr < MOAIShader >		mShader;
	ZLStrongPtr < MOAITexture >		mTexture;
	
	MOAIBlendMode					mBlendMode;
	MOAICullFuncEnum::_				mCullMode;
	MOAIDepthFuncEnum::_			mDepthTest;
	bool							mDepthMask;

	u32								mFlags;
	bool							mOverwrite;
	
	//----------------------------------------------------------------//
	virtual void				MOAIAbstractMaterial_ApplyGlobals					( MOAIAbstractMaterialInterface& dest ) = 0;
	virtual MOAILight*			MOAIAbstractMaterial_ClearGlobals					() = 0;
	virtual MOAILight*			MOAIAbstractMaterial_GetLight						( u32 name ) = 0;
	virtual MOAITexture*		MOAIAbstractMaterial_GetTexture						( u32 name ) = 0;
	virtual void				MOAIAbstractMaterial_SetLight						( u32 name, MOAILight* light ) = 0;
	virtual void				MOAIAbstractMaterial_SetTexture						( u32 name, MOAITexture* light ) = 0;
	MOAIAbstractMaterial&		MOAIAbstractMaterialInterface_AffirmMaterial		();
	MOAIAbstractMaterial*		MOAIAbstractMaterialInterface_GetMaterial			();

public:

	//----------------------------------------------------------------//
						MOAIAbstractMaterial			();
	virtual				~MOAIAbstractMaterial			();
};

#endif
