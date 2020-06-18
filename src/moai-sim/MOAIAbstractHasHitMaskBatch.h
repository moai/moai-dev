// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASHITMASKBATCH_H
#define	MOAIABSTRACTHASHITMASKBATCH_H

#include <moai-sim/MOAIAbstractHasHitMask.h>

class MOAIAbstractHitMaskBatch;

//================================================================//
// MOAIAbstractHasHitMaskBatch
//================================================================//
class MOAIAbstractHasHitMaskBatch :
	public virtual MOAIAbstractHasHitMask {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int			_getHitMask				( lua_State* L );
	static int			_hitMask				( lua_State* L );
	static int			_reserveHitMasks		( lua_State* L );
	static int			_setHitMask				( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractHitMask&		MOAIAbstractHasHitMask_AffirmHitMask		();

	//----------------------------------------------------------------//
	virtual MOAIAbstractHitMaskBatch&		MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch	() = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractHitMask&			AffirmHitMask						( ZLIndex index );
	MOAIAbstractHitMaskBatch&		AffirmHitMaskBatch					();
	MOAIAbstractHitMask*			GetHitMask							( ZLIndex index );
									MOAIAbstractHasHitMaskBatch			();
									~MOAIAbstractHasHitMaskBatch		();
	void							ReserveHitMasks						( ZLSize size );
	void							SetHitMask							( ZLIndex index, MOAIAbstractHitMask* hitMask );
};

#endif
