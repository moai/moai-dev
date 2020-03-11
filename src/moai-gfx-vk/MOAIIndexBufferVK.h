// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFERVK_H
#define	MOAIINDEXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxBufferVK.h>

//================================================================//
// MOAIIndexBufferVK
//================================================================//
/**	@lua	MOAIIndexBufferVK
	@text	Buffer for vertex indices.
*/
class MOAIIndexBufferVK :
	public virtual MOAIIndexBuffer,
	public virtual MOAIGfxBufferVK,
	public virtual MOAIGfxBufferWithUsageVK < VK_BUFFER_USAGE_INDEX_BUFFER_BIT > {
protected:

	//----------------------------------------------------------------//
	
public:
	
//	friend class MOAIGfxMgrVK_PipelineClerkVK;
	
	DECL_LUA_FACTORY ( MOAIIndexBufferVK )
	
	//----------------------------------------------------------------//
					MOAIIndexBufferVK		();
					~MOAIIndexBufferVK		();
};

#endif
