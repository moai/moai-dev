// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERNODE_H
#define	MOAIRENDERNODE_H

#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIHasGfxScript.h>

//================================================================//
// MOAIRenderNode
//================================================================//
class MOAIRenderNode :
	public virtual MOAIAbstractDrawable,
	public virtual MOAIHasGfxScript {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaStrongRef						mRenderRoot;
	MOAILuaSharedPtr < MOAIScope >		mScope;

	//----------------------------------------------------------------//
	static int			_render						( lua_State* L );
	static int			_setRender					( lua_State* L );
	static int			_setScope					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIDrawable_Draw			( int subPrimID );
	void				MOAIDrawable_DrawDebug		( int subPrimID );
	virtual void		MOAIRenderNode_Render 		();

public:

	DECL_LUA_FACTORY ( MOAIRenderNode )

	//----------------------------------------------------------------//
						MOAIRenderNode				();
	virtual 			~MOAIRenderNode				();
	void				Render						();
};

#endif
