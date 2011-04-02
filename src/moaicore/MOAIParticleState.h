// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESTATE_H
#define	MOAIPARTICLESTATE_H

class MOAIParticleForce;
class MOAIParticleScript;

//================================================================//
// MOAIParticleState
//================================================================//
/**	@brief	Particle state.
*/
class MOAIParticleState :
	public virtual USLuaObject {
private:
	friend class MOAIParticleSystem;

	typedef USLeanLink < MOAIParticleForce* > ForceNode;
	USLeanList < MOAIParticleForce* > mForces;

	USRef < MOAIParticleScript >	mInit;
	USRef < MOAIParticleScript >	mUpdate;
	USRef < MOAIParticleScript >	mRender;

	USWeak < MOAIParticleState > mNext;

	//----------------------------------------------------------------//
	static int		_clearForces			( lua_State* L );
	static int		_init					( lua_State* L );
	static int		_pushForce				( lua_State* L );
	static int		_setNext				( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIParticleState )

	//----------------------------------------------------------------//
	void			ClearForces				();
	USVec2D			GetAcceleration			( const USVec2D& loc );
					MOAIParticleState		();
					~MOAIParticleState		();
	void			PushForce				( MOAIParticleForce& force );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
};

#endif
