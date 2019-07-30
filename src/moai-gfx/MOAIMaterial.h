// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIAbstractMaterialGlobalsContext.h>
#include <moai-gfx/MOAIMaterialInterface.h>

//================================================================//
// MOAIMaterial
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public virtual MOAIMaterialInterface {
private:

	friend class MOAIMaterial;
	friend class MOAIMaterialBatch;
	friend class MOAIMaterialInterface;
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
	
	ZLStrongPtr < MOAIShader >	mShader;
	ZLStrongPtr < MOAITexture >	mTexture;
	
	MOAIBlendMode		mBlendMode;
	int					mCullMode;
	int					mDepthTest;
	bool				mDepthMask;

	u32					mFlags;
	bool				mOverwrite;
	
	ZLStrongPtr < MOAIAbstractMaterialGlobalsContext >	mGlobals;
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	MOAIMaterial&		MOAIMaterialInterface_AffirmMaterial		();
	MOAIMaterial*		MOAIMaterialInterface_GetMaterial			();

public:

	DECL_LUA_FACTORY ( MOAIMaterial )

	//----------------------------------------------------------------//
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
};

#endif
