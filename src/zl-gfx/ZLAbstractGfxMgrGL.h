// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGFXMGRGL_H
#define	ZLABSTRACTGFXMGRGL_H

class ZLGfxMgrGL;
class ZLGfxPipelineClerk;
class ZLGfxResourceClerk;
class ZLGfxStateCPUCache;
class ZLGfxStateGPUCache;
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
	virtual ZLGfxPipelineClerk&			ZLAbstractGfxStateCache_GetGfxPipelineClerk			() = 0;
	virtual ZLGfxResourceClerk&			ZLAbstractGfxStateCache_GetGfxResourceClerk			() = 0;
	virtual ZLGfxStateCPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheCPU			() = 0;
	virtual ZLGfxStateGPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheGPU			() = 0;
	virtual ZLGfxStateVertexCache&		ZLAbstractGfxStateCache_GetGfxVertexCache			() = 0;

public:

	//----------------------------------------------------------------//
	inline ZLGfxPipelineClerk& GetGfxPipelineClerk () {
		return ZLAbstractGfxStateCache_GetGfxPipelineClerk ();
	}
	
	//----------------------------------------------------------------//
	inline ZLGfxResourceClerk& GetGfxResourceClerk () {
		return ZLAbstractGfxStateCache_GetGfxResourceClerk ();
	}

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
	ZLAbstractGfxMgrGL () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractGfxMgrGL () {
	}
};

#endif
