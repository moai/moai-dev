// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGfxMgrVKCOMPONENTS_H
#define	MOAIGfxMgrVKCOMPONENTS_H

class MOAIGfxMgrVK;
//class MOAIGfxMgrVK_PipelineClerkVK;
class MOAIGfxMgrVK_GPUCacheVK;
class MOAIGfxMgrVK_ResourceCacheVK;
//class MOAIGfxMgrVK_ResourceClerkVK;
class MOAIGfxMgrVK_VertexCacheVK;

//================================================================//
// MOAIGfxMgrVKComponents
//================================================================//
class MOAIGfxMgrVKComponents :
	public virtual MOAIGfxMgrComponents {
protected:

	//----------------------------------------------------------------//
	virtual MOAIGfxMgrVK&						MOAIGfxMgrVKComponents_GetGfxMgrVK				() = 0;
	virtual MOAIGfxMgrVK_GPUCacheVK&			MOAIGfxMgrVKComponents_GetGPUCacheVK			() = 0;
//	virtual MOAIGfxMgrVK_PipelineClerkVK&		MOAIGfxMgrVKComponents_GetPipelineClerkVK		() = 0;
	virtual MOAIGfxMgrVK_ResourceCacheVK&		MOAIGfxMgrVKComponents_GetResourceCacheVK		() = 0;
//	virtual MOAIGfxMgrVK_ResourceClerkVK&		MOAIGfxMgrVKComponents_GetResourceClerkVK		() = 0;
	virtual MOAIGfxMgrVK_VertexCacheVK&			MOAIGfxMgrVKComponents_GetVertexCacheVK			() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxMgrVK& GetGfxMgrVK () {
		return MOAIGfxMgrVKComponents_GetGfxMgrVK ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgrVK_GPUCacheVK& GetGPUCacheVK () {
		return MOAIGfxMgrVKComponents_GetGPUCacheVK ();
	}

//	//----------------------------------------------------------------//
//	inline MOAIGfxMgrVK_PipelineClerkVK& GetPipelineClerkVK () {
//		return MOAIGfxMgrVKComponents_GetPipelineClerkVK ();
//	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgrVK_ResourceCacheVK& GetResourceCacheVK () {
		return MOAIGfxMgrVKComponents_GetResourceCacheVK ();
	}
	
//	//----------------------------------------------------------------//
//	inline MOAIGfxMgrVK_ResourceClerkVK& GetResourceClerkVK () {
//		return MOAIGfxMgrVKComponents_GetResourceClerkVK ();
//	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgrVK_VertexCacheVK& GetVertexCacheVK () {
		return MOAIGfxMgrVKComponents_GetVertexCacheVK ();
	}
	
	//----------------------------------------------------------------//
	MOAIGfxMgrVKComponents () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIGfxMgrVKComponents () {
	}
};

#endif
