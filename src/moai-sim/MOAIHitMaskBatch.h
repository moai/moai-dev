// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHITMASKBATCH_H
#define	MOAIHITMASKBATCH_H

class MOAIHitMask;

//================================================================//
// MOAIHitMaskBatch
//================================================================//
class MOAIHitMaskBatch :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLLeanArray < ZLStrongPtr < MOAIHitMask > >	mHitMasks;

	//----------------------------------------------------------------//
	static int		_getHitMask				( lua_State* L );
	static int		_reserveHitMasks		( lua_State* L );
	static int		_setHitMask				( lua_State* L );
	static int		_setImage				( lua_State* L );
	static int		_setScalar				( lua_State* L );
	static int		_setThreshold			( lua_State* L );
	static int		_testHit				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIHitMaskBatch		();
					~MOAIHitMaskBatch		();
	void			SetImage				( ZLImage* image );
	void			SetScalar				( u32 idx, u32 scalar );
	void			SetThreshold			( u32 idx, u32 threshold );
	bool			TestHit					( u32 idx, float x, float y );
	bool			TestHit					( u32 idx, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y );
};

#endif
