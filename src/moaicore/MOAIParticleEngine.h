// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEENGINE_H
#define	MOAIPARTICLEENGINE_H

class MOAIParticleState;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleEngine
//================================================================//
class MOAIParticleEngine {
private:
	
	//----------------------------------------------------------------//

public:
	
	enum {
		END = 0,
		ABS,
		ACC_ATTRACTOR,
		ACC_FORCES,
		ACC_LINEAR,
		ADD,
		DAMP,
		DIV,
		EASE,
		INIT_LOC,
		INIT_TIME,
		INIT_VEC,
		LOAD,
		LOAD_AGE,
		LOAD_DURATION,
		LOAD_STEP,
		LOAD_TIME,
		MAC,
		MOV,
		MUL,
		RAND,
		SCALE,
		SCALE_ADD,
		SET_DURATION,
		SPRITE,
		SPRITE_ALPHA,
		SPRITE_COLOR,
		SPRITE_GLOW,
		SPRITE_IDX,
		SPRITE_LOC,
		SPRITE_ROT,
		SPRITE_SCALE,
		SPRITE_SIZE,
		STEP_EULER,
		SUB,
		SUB_FROM_ONE,
		VEC2_RAND,
	};
	
	//----------------------------------------------------------------//
	static void		RunBytecode		( MOAIParticleSystem& system, u8* bytecode, float registers []);
	static void		RunBytecode		( MOAIParticleSystem& system, u8* bytecode, MOAIParticleState* state, float registers []);
	static void		RunBytecode		( MOAIParticleSystem& system, u8* bytecode, MOAIParticleState* state, float step, float registers []);
};

#endif
