// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSTATECACHE_H
#define	MOAIABSTRACTGFXSTATECACHE_H

class MOAIGfxStateCPUCache;
class MOAIGfxStateGPUCache;
class MOAIGfxStateVertexCache;

//================================================================//
// MOAIAbstractGfxStateCache
//================================================================//
class MOAIAbstractGfxStateCache {
protected:

	//----------------------------------------------------------------//
	virtual MOAIGfxStateCPUCache&			MOAIAbstractGfxStateCache_GetGfxStateCacheCPU			() = 0;
	virtual MOAIGfxStateGPUCache&			MOAIAbstractGfxStateCache_GetGfxStateCacheGPU			() = 0;
	virtual MOAIGfxStateVertexCache&		MOAIAbstractGfxStateCache_GetGfxVertexCache				() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxStateCPUCache& GetGfxStateCacheCPU () {
		return MOAIAbstractGfxStateCache_GetGfxStateCacheCPU ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxStateGPUCache& GetGfxStateCacheGPU () {
		return MOAIAbstractGfxStateCache_GetGfxStateCacheGPU ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxStateVertexCache& GetGfxVertexCache () {
		return MOAIAbstractGfxStateCache_GetGfxVertexCache ();
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractGfxStateCache () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractGfxStateCache () {
	}
};

#endif
