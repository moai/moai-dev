// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICPBODY_H
#define	MOAICPBODY_H
#if USE_CHIPMUNK

#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICpSpace.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransformBase.h>

struct cpBody;
class MOAICpShape;

//================================================================//
// MOAICpBody
//================================================================//
/**	@name	MOAICpBody
	@text	Chipmunk Body.
	
	@const	NONE
	@const	REMOVE_BODY
	@const	REMOVE_BODY_AND_SHAPES
*/
class MOAICpBody :
	public MOAITransformBase,
	public MOAICpPrim {
private:

	cpBody* mBody;
	u32		mRemoveFlag;

	typedef USLeanList < MOAICpShape* >::Iterator ShapeIt;
	USLeanList < MOAICpShape* > mShapes;

	//----------------------------------------------------------------//
	static int	_activate			( lua_State* L );
	static int	_addCircle			( lua_State* L );
	static int	_addPolygon			( lua_State* L );
	static int	_addRect			( lua_State* L );
	static int	_addSegment			( lua_State* L );
	static int	_applyForce			( lua_State* L );
	static int	_applyImpulse		( lua_State* L );
	static int	_getAngle			( lua_State* L );
	static int	_getAngVel			( lua_State* L );
	static int	_getForce			( lua_State* L );
	static int	_getMass			( lua_State* L );
	static int	_getMoment			( lua_State* L );
	static int	_getPos				( lua_State* L );
	static int	_getRot				( lua_State* L );
	static int	_getTorque			( lua_State* L );
	static int	_getVel				( lua_State* L );
	static int	_isSleeping			( lua_State* L );
	static int	_isStatic			( lua_State* L );
	static int	_isRogue			( lua_State* L );
	static int	_localToWorld		( lua_State* L );
	static int	_new				( lua_State* L );
	static int	_newStatic			( lua_State* L );
	static int	_resetForces		( lua_State* L );
	static int	_setAngle			( lua_State* L );
	static int	_setAngVel			( lua_State* L );
	static int	_setForce			( lua_State* L );
	static int	_setMass			( lua_State* L );
	static int	_setMoment			( lua_State* L );
	static int	_setPos				( lua_State* L );
	static int	_setRemoveFlag		( lua_State* L );
	static int	_setTorque			( lua_State* L );
	static int	_setVel				( lua_State* L );
	static int	_sleep				( lua_State* L );
	static int	_sleepWithGroup		( lua_State* L );
	static int	_worldToLocal		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AttachShape				( MOAICpShape& shape );
	void			ClearShapes				();
	void			CpAddToSpace			( cpSpace* space );
	void			CpRemoveFromSpace		( cpSpace* space );
	void			DoRemove				();
	void			OnDepNodeUpdate			();

public:
	
	friend class MOAICpConstraint;
	friend class MOAICpShape;
	friend class MOAICpSpace;
	
	DECL_LUA_FACTORY ( MOAICpBody )
	
	enum {
		NONE,
		REMOVE_BODY,
		REMOVE_BODY_AND_SHAPES,
	};
	
	//----------------------------------------------------------------//
					MOAICpBody				();
					~MOAICpBody				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif