// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2DVK_H
#define	MOAITEXTURE2DVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

class MOAILogicalDeviceVK;
class MOAITextureSnapshot2DVK;
class ZLTextureFormat;

//================================================================//
// MOAITexture2DVK
//================================================================//
/**	@lua	MOAITexture2DVK
	@text	Texture class.
*/
class MOAITexture2DVK :
	public virtual MOAITexture2D,
	public virtual MOAITextureVK,
	public MOAIAbstractSnapshotFactoryVK < MOAITextureSnapshot2DVK > {
protected:

	ZLStrongPtr < MOAITextureSnapshot2DVK > mSnapshot;

	//----------------------------------------------------------------//
	MOAITextureSnapshot2DVK*		MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer );
	bool							MOAIGfxResource_FinishLoading					();
	bool							MOAIGfxResource_IsReadyForUse					() const;
	void							MOAILuaObject_RegisterLuaClass					( MOAIComposer& composer, MOAILuaState& state );
	void							MOAILuaObject_RegisterLuaFuncs					( MOAIComposer& composer, MOAILuaState& state );

public:
			
	DECL_LUA_FACTORY ( MOAITexture2DVK )
	
	//----------------------------------------------------------------//
									MOAITexture2DVK			();
									~MOAITexture2DVK		();
};

#endif
