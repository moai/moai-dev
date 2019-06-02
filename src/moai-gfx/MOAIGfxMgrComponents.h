// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRCOMPONENTS_H
#define	MOAIGFXMGRCOMPONENTS_H

class MOAIGfxMgr;
class MOAIGfxMgr_CPUCache;
class MOAIGfxMgr_GPUCache;
class MOAIGfxMgr_RenderTree;
class MOAIGfxMgr_VertexCache;

//================================================================//
// MOAIGfxMgrComponents
//================================================================//
class MOAIGfxMgrComponents {
protected:
	
	//----------------------------------------------------------------//
	virtual MOAIGfxMgr_CPUCache&		MOAIGfxMgrComponents_GetCPUCache		() = 0;
	virtual MOAIGfxMgr&					MOAIGfxMgrComponents_GetGfxMgr			() = 0;
	virtual MOAIGfxMgr_GPUCache&		MOAIGfxMgrComponents_GetGPUCache		() = 0;
	virtual MOAIGfxMgr_RenderTree&		MOAIGfxMgrComponents_GetRenderTree		() = 0;
	virtual MOAIGfxMgr_VertexCache&		MOAIGfxMgrComponents_GetVertexCache		() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxMgr_CPUCache& GetCPUCache () {
		return MOAIGfxMgrComponents_GetCPUCache ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgr& GetGfxMgr () {
		return MOAIGfxMgrComponents_GetGfxMgr ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgr_GPUCache& GetGPUCache () {
		return MOAIGfxMgrComponents_GetGPUCache ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgr_RenderTree& GetRenderTree () {
		return MOAIGfxMgrComponents_GetRenderTree ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgr_VertexCache& GetVertexCache () {
		return MOAIGfxMgrComponents_GetVertexCache ();
	}
	
	//----------------------------------------------------------------//
	MOAIGfxMgrComponents () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIGfxMgrComponents () {
	}
};

#endif
