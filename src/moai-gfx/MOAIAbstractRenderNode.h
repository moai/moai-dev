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

	MOAILuaSharedPtr < MOAIScope >	mLocalScope;
	MOAILuaSharedPtr < MOAIScope >	mSharedScope;
	
	STLMap < u32, ZLStrongPtr < MOAIGfxScript > > mGfxScripts;

	//----------------------------------------------------------------//
	static int			_getGfxScript				( lua_State* L );
	static int			_getRenderTypeID			( lua_State* L );
	static int			_gfxScript					( lua_State* L );
	static int			_hash						( lua_State* L );
	static int			_localScope					( lua_State* L );
	static int			_render						( lua_State* L );
	static int			_setGfxScript				( lua_State* L );
	static int			_setSharedScope				( lua_State* L );
	static int			_sharedScope				( lua_State* L );

	//----------------------------------------------------------------//
	bool				LoadGfxState				( u32 renderPhase );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual bool		MOAIAbstractRenderNode_LoadGfxState		( u32 renderPhase );
	virtual void		MOAIAbstractRenderNode_Render 			( u32 renderPhase ) = 0;

public:

	enum {
		RENDER_PHASE_DRAW 			= 0x51a7d193, // FNV1a ( "DRAW" )
		RENDER_PHASE_DRAW_DEBUG		= 0x21c4b503, // FNV1a ( "DRAW_DEBUG" )
	};

	//----------------------------------------------------------------//
	MOAIGfxScript*		AffirmGfxScript				( u32 renderPhase );
	void				AffirmLocalScope			();
	MOAIGfxScript*		GetGfxScript				( u32 renderPhase );
						MOAIAbstractRenderNode		();
	virtual 			~MOAIAbstractRenderNode		();
	void				Render						( u32 renderPhase );
	void				SetGfxScript				( u32 renderPhase, MOAIGfxScript* gfxScript );
};

#endif
