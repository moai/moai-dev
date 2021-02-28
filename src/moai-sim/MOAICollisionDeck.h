// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONDECK_H
#define	MOAICOLLISIONDECK_H

#include <moai-sim/MOAIDeck.h>

class MOAICollisionShape;

//================================================================//
// MOAICollisionDeck
//================================================================//
// TODO: doxygen
class MOAICollisionDeck :
	public MOAIDeck {
private:

	ZLLeanArray < ZLStrongPtr < MOAICollisionShape > >		mShapes;
	
	//----------------------------------------------------------------//
	static int		_reserveShapes			( lua_State* L );
	static int		_setBox					( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setQuad				( lua_State* L );

	//----------------------------------------------------------------//
	MOAICollisionShape&		AffirmShape						( ZLIndex idx );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIDeck_GetBounds				();
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	void					MOAIDeck_Render					( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );
	
public:
	
	DECL_LUA_FACTORY ( MOAICollisionDeck )
	
	//----------------------------------------------------------------//
					MOAICollisionDeck		( ZLContext& context );
					~MOAICollisionDeck		();
	void			ReserveShapes			( u32 totalShapes );
	void			SetBox					( ZLIndex idx, const ZLBox& box );
	void			SetRect					( ZLIndex idx, const ZLRect& rect );
	void			SetQuad					( ZLIndex idx, const ZLQuad& quad );
};

#endif
