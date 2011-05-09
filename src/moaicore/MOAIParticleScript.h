// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESCRIPT_H
#define	MOAIPARTICLESCRIPT_H

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
	public virtual USLuaObject {
private:
	
	friend class MOAIParticleState;
	
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
		REG_MASK			= 0x3f, // mask for register lookups - up to 64 registers
		PARTICLE_REG_BIT	= 0x40, // flag default particle registers - resolved when parsing commands
		SPRITE_REG_BIT		= 0x80, // select sprite registers vs. particle registers
	};
	
	enum {
		END = 0,
		
		EASE_CONST,
		EASE_CONST_DELTA,
		EASE_VAR,
		EASE_VAR_DELTA,
		
		RAND_CONST,
		RAND_VAR,
		
		RAND_VEC,
		RAND_VEC_CONST,
		
		SET_CONST,
		SET_VAR,
		
		SPRITE,
	};
	
	//----------------------------------------------------------------//
	class Instruction {
	public:
		
		static const u32 MAX_PARAMS = 8;
		
		u32		mOpcode;
		u32		mParams [ MAX_PARAMS ];
		cc8*	mFormat;
		u32		mSize;
		
		//----------------------------------------------------------------//
		u32		GetSize			();
		void	Init			( u32 opcode, cc8* format );
				Instruction		();
		void	Parse			( USLuaState& state, u32 idx );
		u8*		Write			( u8* cursor );
	};

	typedef STLList < Instruction >::iterator InstructionIt;
	STLList < Instruction > mInstructions;
	
	USLeanArray < u8 > mBytecode;
	bool mCompiled;

	//----------------------------------------------------------------//
	static int		_easeConst			( lua_State* L );
	static int		_easeConstDelta		( lua_State* L );
	static int		_easeVar			( lua_State* L );
	static int		_easeVarDelta		( lua_State* L );
	static int		_randConst			( lua_State* L );
	static int		_randVar			( lua_State* L );
	static int		_randVec			( lua_State* L );
	static int		_randVecConst		( lua_State* L );
	static int		_setConst			( lua_State* L );
	static int		_setVar				( lua_State* L );
	static int		_sprite				( lua_State* L );
	
	//----------------------------------------------------------------//
	Instruction&	PushInstruction			( u32 op, cc8* format );
	void			PushSprite				( MOAIParticleSystem& system, float* registers );
	void			ResetRegisters			( float* spriteRegisters, float* particleRegisters );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleScript )
	
	//----------------------------------------------------------------//
	u8*				Compile					();
					MOAIParticleScript		();
					~MOAIParticleScript		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			Run						( MOAIParticleSystem& system, MOAIParticle& particle, float step );
	STLString		ToString				();
};

#endif
