// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTRENDERABLE_H
#define	MOAIABSTRACTRENDERABLE_H

#include <moai-gfx/MOAIHasGfxScriptsForPhases.h>

//================================================================//
// MOAIAbstractRenderable
//================================================================//
class MOAIAbstractRenderable :
	public virtual MOAIHasGfxScriptsForPhases {
protected:

	friend class MOAIAbstractRenderNodeCallback;

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	STLMap < u32, ZLStrongPtr < MOAIGfxScript > > mGfxScripts;

	//----------------------------------------------------------------//
	static int			_getRenderTypeID			( lua_State* L );
	static int			_hash						( lua_State* L ); // TODO: move to util
	static int			_render						( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual bool		MOAIAbstractRenderNode_LoadGfxState		( MOAIRenderPhaseEnum::_ renderPhase );
	virtual void		MOAIAbstractRenderNode_Render 			( MOAIRenderPhaseEnum::_ renderPhase ) = 0;

public:

	//----------------------------------------------------------------//
	void				AffirmLocalScope			();
						MOAIAbstractRenderable		();
	virtual 			~MOAIAbstractRenderable		();
	void				Render						( MOAIRenderPhaseEnum::_ renderPhase );
};

#endif
