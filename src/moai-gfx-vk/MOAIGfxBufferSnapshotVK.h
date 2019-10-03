// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERSNAPSHOTVK_H
#define	MOAIGFXBUFFERSNAPSHOTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIUtilityBufferVK.h>

class MOAIGfxBufferVK;

//================================================================//
// MOAIGfxBufferSnapshotVK
//================================================================//
// TODO: doxygen
class MOAIGfxBufferSnapshotVK :
	public MOAIAbstractSnapshotVK,
	public MOAIUtilityBufferVK {
protected:
	
	//----------------------------------------------------------------//
	void				MOAIAbstractSnapshotVK_OnUnpin				();

public:
	
	//----------------------------------------------------------------//
	void				Initialize							( MOAIGfxBufferVK& buffer );
						MOAIGfxBufferSnapshotVK				();
						~MOAIGfxBufferSnapshotVK			();
};

#endif
