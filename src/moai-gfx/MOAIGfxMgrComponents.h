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
	virtual MOAIGfxMgr_CPUCache&		MOAIAbstractGfxMgrComponents_GetCPUCache		() = 0;
	virtual MOAIGfxMgr&					MOAIAbstractGfxMgrComponents_GetGfxMgr			() = 0;
	virtual MOAIGfxMgr_GPUCache&		MOAIAbstractGfxMgrComponents_GetGPUCache		() = 0;
	virtual MOAIGfxMgr_RenderTree&		MOAIAbstractGfxMgrComponents_GetRenderTree		() = 0;
	virtual MOAIGfxMgr_VertexCache&		MOAIAbstractGfxMgrComponents_GetVertexCache		() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxMgr_CPUCache& GetCPUCache () {
		return MOAIAbstractGfxMgrComponents_GetCPUCache ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgr& GetGfxMgr () {
		return MOAIAbstractGfxMgrComponents_GetGfxMgr ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgr_GPUCache& GetGPUCache () {
		return MOAIAbstractGfxMgrComponents_GetGPUCache ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgr_RenderTree& GetRenderTree () {
		return MOAIAbstractGfxMgrComponents_GetRenderTree ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgr_VertexCache& GetVertexCache () {
		return MOAIAbstractGfxMgrComponents_GetVertexCache ();
	}
	
	//----------------------------------------------------------------//
	MOAIGfxMgrComponents () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIGfxMgrComponents () {
	}
};

#endif
