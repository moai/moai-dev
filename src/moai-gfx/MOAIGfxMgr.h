// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_H
#define	MOAIGFXMGR_H

#include <moai-gfx/MOAIGfxMgr_CPUCache.h>
#include <moai-gfx/MOAIGfxMgr_GPUCache.h>
#include <moai-gfx/MOAIGfxMgr_RenderTree.h>
#include <moai-gfx/MOAIGfxMgr_VertexCache.h>
#include <moai-gfx/MOAIGfxMgrComponents.h>

//================================================================//
// MOAIGfxMgr
//================================================================//
/**	@lua	MOAIGfxMgr
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxMgr :
	public ZLContextClassAlias < MOAIGfxMgr >,
	public virtual MOAIGlobalEventSource,
	public virtual MOAIGfxMgrComponents,
	public virtual MOAIGfxMgr_CPUCache,
	public virtual MOAIGfxMgr_GPUCache,
	public virtual MOAIGfxMgr_RenderTree,
	public virtual MOAIGfxMgr_VertexCache {
protected:

	//----------------------------------------------------------------//
	MOAIGfxMgr_CPUCache&		MOAIAbstractGfxMgrComponents_GetCPUCache		();
	MOAIGfxMgr&					MOAIAbstractGfxMgrComponents_GetGfxMgr			();
	MOAIGfxMgr_GPUCache&		MOAIAbstractGfxMgrComponents_GetGPUCache		();
	MOAIGfxMgr_RenderTree&		MOAIAbstractGfxMgrComponents_GetRenderTree		();
	MOAIGfxMgr_VertexCache&		MOAIAbstractGfxMgrComponents_GetVertexCache		();

	//----------------------------------------------------------------//
	virtual MOAIShader*		MOAIAbstractGfxMgr_AffirmShader		( MOAILuaState& state, int idx ) const = 0;
	virtual MOAITexture*	MOAIAbstractGfxMgr_AffirmTexture	( MOAILuaState& state, int idx ) const = 0;

public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};

	//----------------------------------------------------------------//
	MOAIShader*			AffirmShader			( MOAILuaState& state, int idx ) const;
	MOAITexture*		AffirmTexture			( MOAILuaState& state, int idx ) const;
						MOAIGfxMgr				();
						~MOAIGfxMgr				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
