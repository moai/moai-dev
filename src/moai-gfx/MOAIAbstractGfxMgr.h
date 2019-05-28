// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXMGR_H
#define	MOAIABSTRACTGFXMGR_H

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
	public virtual ZLAbstractGfxMgr {
protected:

	//----------------------------------------------------------------//
	virtual ZLAbstractShader*		MOAIAbstractGfxMgr_AffirmShader		( MOAILuaState& state, int idx ) const = 0;
	virtual ZLAbstractTexture*		MOAIAbstractGfxMgr_AffirmTexture	( MOAILuaState& state, int idx ) const = 0;

public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};

	//----------------------------------------------------------------//
	ZLAbstractShader*		AffirmShader				( MOAILuaState& state, int idx ) const;
	ZLAbstractTexture*		AffirmTexture				( MOAILuaState& state, int idx ) const;
							MOAIAbstractGfxMgr			();
							~MOAIAbstractGfxMgr			();
};

#endif
