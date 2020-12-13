// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTRENDERNODE_H
#define	MOAIABSTRACTRENDERNODE_H

#include <moai-gfx/MOAIHasGfxScript.h>

//================================================================//
// MOAIAbstractRenderNode
//================================================================//
class MOAIAbstractRenderNode :
	public virtual MOAILuaObject {
protected:

	friend class MOAIAbstractRenderNodeCallback;

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIScope >		mScope;
	
	STLMap < u32, ZLStrongPtr < MOAIGfxScript > > mGfxScripts;

	//----------------------------------------------------------------//
	static int			_getGfxScript				( lua_State* L );
	static int			_gfxScript					( lua_State* L );
	static int			_render						( lua_State* L );
	static int			_setGfxScript				( lua_State* L );
	static int			_setScope					( lua_State* L );

	//----------------------------------------------------------------//
	void				InvokeGfxScript				( u32 renderPhase );
	bool				LoadGfxState				( u32 renderPhase );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual bool		MOAIAbstractRenderNode_LoadGfxState		( u32 renderPhase );
	virtual void		MOAIAbstractRenderNode_Render 			( u32 renderPhase ) = 0;

public:

	enum {
		RENDER_PHASE_DRAW = 0,
		RENDER_PHASE_DRAW_DEBUG,
	};

	//----------------------------------------------------------------//
	MOAIGfxScript*		AffirmGfxScript				( u32 renderPhase );
	MOAIGfxScript*		GetGfxScript				( u32 renderPhase );
						MOAIAbstractRenderNode		();
	virtual 			~MOAIAbstractRenderNode		();
	void				Render						( u32 renderPhase );
	void				SetGfxScript				( u32 renderPhase, MOAIGfxScript* gfxScript );
};

#endif
