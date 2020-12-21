// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTRENDERNODE_H
#define	MOAIABSTRACTRENDERNODE_H

#include <moai-gfx/MOAIHasGfxScriptsForPhases.h>

//================================================================//
// MOAIAbstractRenderNode
//================================================================//
class MOAIAbstractRenderNode :
	public virtual MOAIHasGfxScriptsForPhases {
protected:

	friend class MOAIAbstractRenderNodeCallback;

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIScope >	mLocalScope;
	MOAILuaSharedPtr < MOAIScope >	mSharedScope;
	
	STLMap < u32, ZLStrongPtr < MOAIGfxScript > > mGfxScripts;

	//----------------------------------------------------------------//
	static int			_getRenderTypeID			( lua_State* L );
	static int			_hash						( lua_State* L );
	static int			_localScope					( lua_State* L );
	static int			_render						( lua_State* L );
	static int			_setSharedScope				( lua_State* L );
	static int			_sharedScope				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual bool		MOAIAbstractRenderNode_LoadGfxState		( MOAIRenderPhaseEnum::_ renderPhase );
	virtual void		MOAIAbstractRenderNode_Render 			( MOAIRenderPhaseEnum::_ renderPhase ) = 0;

public:

	//----------------------------------------------------------------//
	void				AffirmLocalScope			();
						MOAIAbstractRenderNode		();
	virtual 			~MOAIAbstractRenderNode		();
	void				Render						( MOAIRenderPhaseEnum::_ renderPhase );
};

#endif
