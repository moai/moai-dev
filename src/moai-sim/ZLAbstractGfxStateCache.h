// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGFXSTATECACHE_H
#define	ZLABSTRACTGFXSTATECACHE_H

class ZLGfxStateCPUCache;
class ZLGfxStateGPUCache;
class ZLGfxStateVertexCache;

//================================================================//
// ZLAbstractGfxStateCache
//================================================================//
class ZLAbstractGfxStateCache {
protected:

	//----------------------------------------------------------------//
	virtual ZLGfxStateCPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheCPU			() = 0;
	virtual ZLGfxStateGPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheGPU			() = 0;
	virtual ZLGfxStateVertexCache&		ZLAbstractGfxStateCache_GetGfxVertexCache			() = 0;

public:

	//----------------------------------------------------------------//
	inline ZLGfxStateCPUCache& GetGfxStateCacheCPU () {
		return ZLAbstractGfxStateCache_GetGfxStateCacheCPU ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxStateGPUCache& GetGfxStateCacheGPU () {
		return ZLAbstractGfxStateCache_GetGfxStateCacheGPU ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxStateVertexCache& GetGfxVertexCache () {
		return ZLAbstractGfxStateCache_GetGfxVertexCache ();
	}
	
	//----------------------------------------------------------------//
	ZLAbstractGfxStateCache () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractGfxStateCache () {
	}
};

#endif
