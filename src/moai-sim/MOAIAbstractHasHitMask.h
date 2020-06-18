// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASHITMASK_H
#define	MOAIABSTRACTHASHITMASK_H

class MOAIAbstractHitMask;

//================================================================//
// MOAIAbstractHasHitMask
//================================================================//
class MOAIAbstractHasHitMask :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int		_setHitMaskImage		( lua_State* L );
	static int		_setHitMaskScalar		( lua_State* L );
	static int		_setHitMaskThreshold	( lua_State* L );
	static int		_testHit				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIAbstractHitMask&		MOAIAbstractHasHitMask_AffirmHitMask		() = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractHitMask&		AffirmHitMask				();
								MOAIAbstractHasHitMask		();
								~MOAIAbstractHasHitMask		();
	void						SetImage					( MOAIImage* image );
	void						SetScalar					( u32 scalar );
	void						SetThreshold				( u32 threshold );
	bool						TestHit						( float x, float y );
	bool						TestHit						( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y );
};

#endif
