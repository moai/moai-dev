// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXMGR_H
#define	MOAIABSTRACTGFXMGR_H

#include <moai-gfx/MOAIAbstractGfxMgrComponents.h>
#include <moai-gfx/MOAIAbstractGfxStateGPUCache.h>
#include <moai-gfx/MOAIAbstractVertexCache.h>
#include <moai-gfx/MOAIGfxStateCPUCache.h>

//================================================================//
// MOAIAbstractGfxMgr
//================================================================//
/**	@lua	MOAIAbstractGfxMgr
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIAbstractGfxMgr :
	public ZLContextClassAlias < MOAIAbstractGfxMgr >,
	public virtual MOAIGlobalEventSource,
	public virtual MOAIAbstractGfxMgrComponents,
	public virtual MOAIAbstractGfxStateGPUCache,
	public virtual MOAIAbstractVertexCache,
	public virtual MOAIGfxStateCPUCache {
protected:

	//----------------------------------------------------------------//
	MOAIAbstractGfxMgr&				MOAIAbstractGfxMgrComponents_GetAbstractGfxMgr				();
	MOAIAbstractGfxStateGPUCache&	MOAIAbstractGfxMgrComponents_GetAbstractGfxStateGPUCache	();
	MOAIAbstractVertexCache&		MOAIAbstractGfxMgrComponents_GetAbstractVertexCache			();
	MOAIGfxStateCPUCache& 			MOAIAbstractGfxMgrComponents_GetGfxStateCPUCache			();

	//----------------------------------------------------------------//
	virtual MOAIAbstractShader*		MOAIAbstractGfxMgr_AffirmShader		( MOAILuaState& state, int idx ) const = 0;
	virtual MOAIAbstractTexture*	MOAIAbstractGfxMgr_AffirmTexture	( MOAILuaState& state, int idx ) const = 0;

public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};

	//----------------------------------------------------------------//
	MOAIAbstractShader*		AffirmShader				( MOAILuaState& state, int idx ) const;
	MOAIAbstractTexture*	AffirmTexture				( MOAILuaState& state, int idx ) const;
							MOAIAbstractGfxMgr			();
							~MOAIAbstractGfxMgr			();
};

#endif
