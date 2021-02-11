// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERNODE_H
#define	MOAIRENDERNODE_H

#include <moai-gfx/MOAIAbstractRenderable.h>

//================================================================//
// MOAIRenderNode
//================================================================//
class MOAIRenderNode :
	public virtual MOAIAbstractRenderable {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaStrongRef	mRenderRoot;

	//----------------------------------------------------------------//
	static int			_getRender					( lua_State* L );
	static int			_pushChild					( lua_State* L );
	static int			_setRender					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual void		MOAIAbstractRenderNode_Render 		( MOAIRenderPhaseEnum::_ renderPhase );

public:

	DECL_LUA_FACTORY ( MOAIRenderNode )

	//----------------------------------------------------------------//
						MOAIRenderNode				();
	virtual 			~MOAIRenderNode				();
	void				PushChild					( MOAIAbstractRenderable& node );
	void				PushChild					( MOAILuaState& state, int idx );
	static void			Render						( MOAIRenderPhaseEnum::_ renderPhase, MOAILuaMemberRef& ref );
	static void			Render						( MOAIRenderPhaseEnum::_ renderPhase, MOAILuaState& state, int idx );
};

#endif
