// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXMGRCOMPONENTS_H
#define	MOAIABSTRACTGFXMGRCOMPONENTS_H

class MOAIAbstractGfxMgr;
class MOAIAbstractGfxStateGPUCache;
class MOAIAbstractVertexCache;
class MOAIGfxStateCPUCache;

//================================================================//
// MOAIAbstractGfxMgrComponents
//================================================================//
class MOAIAbstractGfxMgrComponents {
protected:
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxMgr&				MOAIAbstractGfxMgrComponents_GetAbstractGfxMgr				() = 0;
	virtual MOAIAbstractGfxStateGPUCache&	MOAIAbstractGfxMgrComponents_GetAbstractGfxStateGPUCache	() = 0;
	virtual MOAIAbstractVertexCache&		MOAIAbstractGfxMgrComponents_GetAbstractVertexCache			() = 0;
	virtual MOAIGfxStateCPUCache&			MOAIAbstractGfxMgrComponents_GetGfxStateCPUCache			() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIAbstractGfxMgr& GetAbstractGfxMgr () {
		return MOAIAbstractGfxMgrComponents_GetAbstractGfxMgr ();
	}

	//----------------------------------------------------------------//
	inline MOAIAbstractGfxStateGPUCache& GetAbstractGfxStateGPUCache () {
		return MOAIAbstractGfxMgrComponents_GetAbstractGfxStateGPUCache ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIAbstractVertexCache& GetAbstractVertexCache () {
		return MOAIAbstractGfxMgrComponents_GetAbstractVertexCache ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxStateCPUCache& GetGfxStateCPUCache () {
		return MOAIAbstractGfxMgrComponents_GetGfxStateCPUCache ();
	}
};

#endif
