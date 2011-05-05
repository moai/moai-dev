// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DJOINT_H
#define	MOAIBOX2DJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DWorld.h>

class b2Joint;

//================================================================//
// MOAIBox2DJoint
//================================================================//
/**	@name MOAIBox2DJoint
	@text Box2D joint.
*/
class MOAIBox2DJoint :
	public MOAIBox2DPrim {
private:

	b2Joint*			mJoint;
	
	USRef < MOAIBox2DJoint >	mJointA;
	USRef < MOAIBox2DJoint >	mJointB;

	//----------------------------------------------------------------//
	static int		_destroy			( lua_State* L );
	static int		_getAnchorA			( lua_State* L );
	static int		_getAnchorB			( lua_State* L );
	static int		_getBodyA			( lua_State* L );
	static int		_getBodyB			( lua_State* L );
	static int		_getReactionForce	( lua_State* L );
	static int		_getReactionTorque	( lua_State* L );
	static int		_setLimit			( lua_State* L );
	static int		_setLimitEnabled	( lua_State* L );
	static int		_setMotor			( lua_State* L );
	static int		_setMotorEnabled	( lua_State* L );
	
	//----------------------------------------------------------------//
	void		SetJoint				( b2Joint* joint );
	void		SetLimit				( float lower, float upper );
	void		SetLimitEnabled			( bool enabled );
	void		SetMotor				( float speed, float max );
	void		SetMotorEnabled			( bool enabled );

public:
	
	friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DJoint )
	
	//----------------------------------------------------------------//
	void			Destroy					();
					MOAIBox2DJoint			();
					~MOAIBox2DJoint			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
#endif