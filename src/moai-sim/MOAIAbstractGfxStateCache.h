// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSTATECACHE_H
#define	MOAIABSTRACTGFXSTATECACHE_H

class MOAIGfxStateCPU;
class MOAIGfxStateGPU;
class MOAIGfxVertexCache;

//================================================================//
// MOAIAbstractGfxStateCache
//================================================================//
class MOAIAbstractGfxStateCache {
protected:

	//----------------------------------------------------------------//
	virtual MOAIGfxStateCPU&			MOAIAbstractGfxStateCache_GetGfxStateCacheCPU			() = 0;
	virtual MOAIGfxStateGPU&			MOAIAbstractGfxStateCache_GetGfxStateCacheGPU			() = 0;
	virtual MOAIGfxVertexCache&			MOAIAbstractGfxStateCache_GetGfxVertexCache				() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxStateCPU& GetGfxStateCacheCPU () {
		return MOAIAbstractGfxStateCache_GetGfxStateCacheCPU ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxStateGPU& GetGfxStateCacheGPU () {
		return MOAIAbstractGfxStateCache_GetGfxStateCacheGPU ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxVertexCache& GetGfxVertexCache () {
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
