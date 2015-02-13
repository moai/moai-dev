// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESTATE_H
#define	MOAIPARTICLESTATE_H

#include <moai-sim/MOAIParticle.h>

class MOAIParticleForce;
class MOAIParticlePlugin;
class MOAIParticleScript;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleState
//================================================================//
/**	@lua	MOAIParticleState
	@text	Particle state.
*/
class MOAIParticleState :
	public virtual MOAILuaObject {
private:
	friend class MOAIParticleScript;
	friend class MOAIParticleSystem;

	typedef ZLLeanLink < MOAIParticleForce* > ForceNode;
	ZLLeanList < MOAIParticleForce* > mForces;

	float mMassRange [ 2 ];
	float mTermRange [ 2 ];

	float mDamping;

	MOAILuaSharedPtr < MOAIParticleScript >		mInit;
	MOAILuaSharedPtr < MOAIParticleScript >		mRender;
	MOAILuaSharedPtr < MOAIParticlePlugin >		mPlugin;
	MOAILuaSharedPtr < MOAIParticleState >		mNext;

	//----------------------------------------------------------------//
	static int		_clearForces			( lua_State* L );
	static int		_pushForce				( lua_State* L );
	static int		_setDamping				( lua_State* L );
	static int		_setInitScript			( lua_State* L );
	static int		_setMass				( lua_State* L );
	static int		_setNext				( lua_State* L );
	static int		_setPlugin				( lua_State* L );
	static int		_setRenderScript		( lua_State* L );
	static int		_setTerm				( lua_State* L );

	//----------------------------------------------------------------//
	void			GatherForces			( ZLVec3D& loc, ZLVec3D& velocity, float mass, float step );
	void			InitParticle			( MOAIParticleSystem& system, MOAIParticle& particle );
	void			ProcessParticle			( MOAIParticleSystem& system, MOAIParticle& particle, float step );

public:

	DECL_LUA_FACTORY ( MOAIParticleState )

	//----------------------------------------------------------------//
	void			ClearForces				();
					MOAIParticleState		();
					~MOAIParticleState		();
	void			PushForce				( MOAIParticleForce& force );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
