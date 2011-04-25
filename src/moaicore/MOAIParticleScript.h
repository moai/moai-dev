// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESCRIPT_H
#define	MOAIPARTICLESCRIPT_H

class MOAIParticle;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleScript
//================================================================//
/**	@name	MOAIParticleScript
	@text	Particle script.
*/
class MOAIParticleScript :
	public virtual USLuaObject {
private:
	
	friend class MOAIParticleState;
	
	enum {
		X_OFF,
		Y_OFF,
		ROT,
		X_SCL,
		Y_SCL,
		R,
		G,
		B,
		A,
		OPACITY,
		GLOW,
		IDX,
		TOTAL,
	};
	
	enum {
		END = 0,
		EASE_CONST,
		EASE_VAR,
		INIT_CONST,
		INIT_RAND,
		INIT_RAND_VEC,
		RAND_CONST,
		RAND_VAR,
		SET_CONST,
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
	static int		_easeVar			( lua_State* L );
	static int		_initConst			( lua_State* L );
	static int		_initRand			( lua_State* L );
	static int		_initRandVec		( lua_State* L );
	static int		_randConst			( lua_State* L );
	static int		_randVar			( lua_State* L );
	static int		_setConst			( lua_State* L );
	static int		_sprite				( lua_State* L );
	
	//----------------------------------------------------------------//
	Instruction&	PushInstruction			( u32 op, cc8* format );
	void			PushSprite				( MOAIParticleSystem& system, MOAIParticle& particle, float* registers );
	void			ResetRegisters			( float* registers );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleScript )
	
	//----------------------------------------------------------------//
	u8*				Compile					();
					MOAIParticleScript		();
					~MOAIParticleScript		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			Run						( MOAIParticleSystem& system, MOAIParticle& particle );
	STLString		ToString				();
};

#endif
