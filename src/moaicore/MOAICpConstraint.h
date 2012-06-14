// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICPCONSTRAINT_H
#define	MOAICPCONSTRAINT_H
#if USE_CHIPMUNK

#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICpSpace.h>
#include <moaicore/MOAILua.h>

struct cpConstraint;

//================================================================//
// MOAICpConstraint
//================================================================//
/**	@name	MOAICpConstraint
	@text	Chipmunk Constraint.
*/
class MOAICpConstraint :
	public virtual MOAILuaObject,
	public MOAICpPrim {
private:

	cpConstraint*	mConstraint;

	//----------------------------------------------------------------//
	static int	_getBiasCoef				( lua_State* L );
	static int	_getMaxBias					( lua_State* L );
	static int	_getMaxForce				( lua_State* L );
	static int	_newDampedRotarySpring		( lua_State* L );
	static int	_newDampedSpring			( lua_State* L );
	static int	_newGearJoint				( lua_State* L );
	static int	_newGrooveJoint				( lua_State* L );
	static int	_newPinJoint				( lua_State* L );
	static int	_newPivotJoint				( lua_State* L );
	static int	_newRatchetJoint			( lua_State* L );
	static int	_newRotaryLimitJoint		( lua_State* L );
	static int	_newSimpleMotor				( lua_State* L );
	static int	_newSlideJoint				( lua_State* L );
	static int	_setBiasCoef				( lua_State* L );
	static int	_setMaxBias					( lua_State* L );
	static int	_setMaxForce				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			CpAddToSpace			( cpSpace* space );
	void			CpRemoveFromSpace		( cpSpace* space );

public:
	
	friend class MOAICpSpace;
	
	DECL_LUA_FACTORY ( MOAICpConstraint )
	
	//----------------------------------------------------------------//
					MOAICpConstraint		();
					~MOAICpConstraint		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif