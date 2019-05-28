// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGFXMGRGL_H
#define	ZLABSTRACTGFXMGRGL_H

class ZLGfxMgrGL;
class ZLGfxPipelineClerkGL;
class ZLGfxResourceClerkGL;
class ZLGfxStateCPUCache;
class ZLGfxStateGPUCacheGL;
class ZLGfxStateVertexCache;

//================================================================//
// ZLAbstractGfxMgrGL
//================================================================//
class ZLAbstractGfxMgrGL {
protected:

	//----------------------------------------------------------------//
	inline ZLGfxMgrGL& GetGfxMgr () {
		return ZLAbstractGfxStateCache_GetGfxMgr ();
	}

	//----------------------------------------------------------------//
	virtual ZLGfxMgrGL&					ZLAbstractGfxStateCache_GetGfxMgr					() = 0;
	virtual ZLGfxPipelineClerkGL&			ZLAbstractGfxStateCache_GetGfxPipelineClerk			() = 0;
	virtual ZLGfxResourceClerkGL&			ZLAbstractGfxStateCache_GetGfxResourceClerk			() = 0;
	virtual ZLGfxStateCPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheCPU			() = 0;
	virtual ZLGfxStateGPUCacheGL&			ZLAbstractGfxStateCache_GetGfxStateCacheGPU			() = 0;
	virtual ZLGfxStateVertexCache&		ZLAbstractGfxStateCache_GetGfxVertexCache			() = 0;

public:

	//----------------------------------------------------------------//
	inline ZLGfxPipelineClerkGL& GetGfxPipelineClerk () {
		return ZLAbstractGfxStateCache_GetGfxPipelineClerk ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxResourceClerkGL& GetGfxResourceClerk () {
		return ZLAbstractGfxStateCache_GetGfxResourceClerk ();
	}

	//----------------------------------------------------------------//
	inline ZLGfxStateCPUCache& GetGfxStateCacheCPU () {
		return ZLAbstractGfxStateCache_GetGfxStateCacheCPU ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxStateGPUCacheGL& GetGfxStateCacheGPU () {
		return ZLAbstractGfxStateCache_GetGfxStateCacheGPU ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxStateVertexCache& GetGfxVertexCache () {
		return ZLAbstractGfxStateCache_GetGfxVertexCache ();
	}
	
	//----------------------------------------------------------------//
	ZLAbstractGfxMgrGL () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractGfxMgrGL () {
	}
};

#endif
