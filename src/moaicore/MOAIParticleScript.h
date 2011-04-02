// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESCRIPT_H
#define	MOAIPARTICLESCRIPT_H

//================================================================//
// MOAIParticleScript
//================================================================//
/**	@brief	Particle script.
*/
class MOAIParticleScript :
	public virtual USLuaObject {
private:
	
	//----------------------------------------------------------------//
	class Instruction {
	public:
	
		u32		mOpCode;
		u32		mOpCount;
		u8		mRegisters [ 4 ];
		float	mValues [ 4 ];
		u32		mTotalRegisters;
		u32		mTotalValues;
		
		//----------------------------------------------------------------//
		u32		GetSize			();
		void	Init			( u32 opcode, u32 nRegisters, u32 nValues );
				Instruction		();
		void	SetRegisters	( u8 r0, u8 r1, u8 r2, u8 r3 );
		void	SetValues		( float v0, float v1, float v2, float v3 );
		u8*		Write			( u8* cursor );
	};

	typedef STLList < Instruction >::iterator InstructionIt;
	STLList < Instruction > mInstructions;
	
	USLeanArray < u8 > mBytecode;
	bool mCompiled;

	//----------------------------------------------------------------//
	static int		_abs				( lua_State* L );
	static int		_accAttractor		( lua_State* L );
	static int		_accForces			( lua_State* L );
	static int		_accLinear			( lua_State* L );
	static int		_add				( lua_State* L );
	static int		_damp				( lua_State* L );
	static int		_div				( lua_State* L );
	static int		_ease				( lua_State* L );
	static int		_initLoc			( lua_State* L );
	static int		_initTime			( lua_State* L );
	static int		_initVec			( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_loadAge			( lua_State* L );
	static int		_loadDuration		( lua_State* L );
	static int		_loadStep			( lua_State* L );
	static int		_loadTime			( lua_State* L );
	static int		_mac				( lua_State* L );
	static int		_mov				( lua_State* L );
	static int		_mul				( lua_State* L );
	static int		_rand				( lua_State* L );
	static int		_scale				( lua_State* L );
	static int		_scaleAdd			( lua_State* L );
	static int		_setDuration		( lua_State* L );
	static int		_sprite				( lua_State* L );
	static int		_spriteAlpha		( lua_State* L );
	static int		_spriteColor		( lua_State* L );
	static int		_spriteGlow			( lua_State* L );
	static int		_spriteIdx			( lua_State* L );
	static int		_spriteLoc			( lua_State* L );
	static int		_spriteRot			( lua_State* L );
	static int		_spriteScale		( lua_State* L );
	static int		_spriteSize			( lua_State* L );
	static int		_stepEuler			( lua_State* L );
	static int		_sub				( lua_State* L );
	static int		_subFromOne			( lua_State* L );
	static int		_vec2Rand			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ParseInstruction		( USLuaState& state, u32 idx, u32 op, u32 rParams, u32 vParams );
	void			WriteBinaryRegOp		( u32 op, u32 r0, u32 r1 );
	void			WriteBinaryValOp		( u32 op, u32 r0, float v0 );
	Instruction&	WriteOp					( u32 op, u32 nRegisters, u32 nValues );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleScript )
	
	//----------------------------------------------------------------//
	u8*				Compile					();
	u8*				GetBytecode				();
					MOAIParticleScript		();
					~MOAIParticleScript		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
