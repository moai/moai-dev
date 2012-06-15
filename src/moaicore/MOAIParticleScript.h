// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESCRIPT_H
#define	MOAIPARTICLESCRIPT_H

#include <moaicore/MOAILua.h>

class MOAIParticle;
class MOAIParticleState;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleScript
//================================================================//
/**	@name	MOAIParticleScript
	@text	Particle script.

	@const	PARTICLE_X
	@const	PARTICLE_Y
	@const	PARTICLE_DX
	@const	PARTICLE_DY

	@const	SPRITE_X_LOC
	@const	SPRITE_Y_LOC
	@const	SPRITE_ROT
	@const	SPRITE_X_SCL
	@const	SPRITE_Y_SCL
	@const	SPRITE_RED
	@const	SPRITE_GREEN
	@const	SPRITE_BLUE
	@const	SPRITE_OPACITY
	@const	SPRITE_GLOW
	@const	SPRITE_IDX
*/
class MOAIParticleScript :
	public virtual MOAILuaObject {
private:
	
	friend class MOAIParticleState;
	
	static const u32 MAX_PARTICLE_REGISTERS = 256;
	static const u32 PARTICLE_REGISTER_MASK = 0x000000ff;
	
	enum {
		SPRITE_X_LOC,
		SPRITE_Y_LOC,
		SPRITE_ROT,
		SPRITE_X_SCL,
		SPRITE_Y_SCL,
		SPRITE_RED,
		SPRITE_GREEN,
		SPRITE_BLUE,
		SPRITE_OPACITY,
		SPRITE_GLOW,
		SPRITE_IDX,
		TOTAL_SPRITE_REG,
	};
	
	enum {
		END = 0,
		ADD,
		ANGLE_VEC,
		COS,
		CYCLE,
		DIV,
		EASE,
		EASE_DELTA,
		MUL,
		NORM,
		RAND,
		RAND_VEC,
		SET,
		SIN,
		SPRITE,
		SUB,
		TAN,
		TIME,
		VEC_ANGLE,
		WRAP,
	};
	
	//----------------------------------------------------------------//
	class Instruction {
	public:
		
		static const u32 MAX_PARAMS = 8;
		
		u32		mOpcode;
		u32		mParams [ MAX_PARAMS ];
		u8		mTypes [ MAX_PARAMS ];
		cc8*	mFormat;
		u32		mSize;
		
		//----------------------------------------------------------------//
		u32		GetSize			();
		void	Init			( u32 opcode, cc8* format );
				Instruction		();
		void	Parse			( MOAILuaState& state, u32 idx );
		u8*		Write			( u8* cursor );
	};

	typedef STLList < Instruction >::iterator InstructionIt;
	STLList < Instruction > mInstructions;
	
	USLeanArray < u8 > mBytecode;
	bool mCompiled;

	//----------------------------------------------------------------//
	static int		_add				( lua_State* L );
	static int		_angleVec			( lua_State* L );
	static int		_cos				( lua_State* L );
	static int		_cycle				( lua_State* L );
	static int		_div				( lua_State* L );
	static int		_ease				( lua_State* L );
	static int		_easeDelta			( lua_State* L );
	static int		_mul				( lua_State* L );
	static int		_norm				( lua_State* L );
	static int		_packConst			( lua_State* L );
	static int		_packReg			( lua_State* L );
	static int		_rand				( lua_State* L );
	static int		_randVec			( lua_State* L );
	static int		_set				( lua_State* L );
	static int		_sin				( lua_State* L );
	static int		_sprite				( lua_State* L );
	static int		_sub				( lua_State* L );
	static int		_tan				( lua_State* L );
	static int		_time				( lua_State* L );
	static int		_vecAngle			( lua_State* L );
	static int		_wrap				( lua_State* L );
	
	//----------------------------------------------------------------//
	static u64		Pack64					( u32 low, u32 hi );
	Instruction&	PushInstruction			( u32 op, cc8* format );
	void			PushSprite				( MOAIParticleSystem& system, float* registers );
	void			ResetRegisters			( float* spriteRegisters, float* particleRegisters );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleScript )
	
	enum {
		PARAM_TYPE_FLAG				= 0x00,
		PARAM_TYPE_CONST			= 0x01,
		PARAM_TYPE_PARTICLE_REG		= 0x02,
		PARAM_TYPE_SPRITE_REG		= 0x04,
		
		PARAM_TYPE_REG_MASK			= 0x06,
		PARAM_TYPE_MASK				= 0x07,
	};
	
	//----------------------------------------------------------------//
	u8*				Compile					();
					MOAIParticleScript		();
					~MOAIParticleScript		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Run						( MOAIParticleSystem& system, MOAIParticle& particle, float t0, float t1 );
};

#endif
