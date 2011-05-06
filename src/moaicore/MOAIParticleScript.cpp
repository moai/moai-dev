// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleSystem.h>

#define READ_BYTE(var,bytecode) \
	var = *( bytecode++ );

#define READ_FLOAT(var,bytecode) \
	*(( u32* )&var ) = *(( u32* )bytecode ); \
	bytecode += sizeof ( u32 );

#define READ_INT(var,bytecode) \
	*(( u32* )&var ) = *(( u32* )bytecode ); \
	bytecode += sizeof ( u32 );

#define IMPL_LUA_PARTICLE_OP(name,opcode,format)								\
	int MOAIParticleScript::name ( lua_State* L ) {								\
		MOAI_LUA_SETUP ( MOAIParticleScript, "U" )								\
		Instruction& instruction = self->PushInstruction ( opcode, format );	\
		instruction.Parse ( state, 2 );											\
		return 0;																\
	}

#define REG_VALUE(reg) \
	(( reg & SPRITE_REG_BIT ) ? spriteRegisters [ reg & REG_MASK ] : (( reg < nRegs ) ? particleRegisters [ reg ] : 0.0f ))

#define REG_ADDR(reg) \
	(( reg & SPRITE_REG_BIT ) ? &spriteRegisters [ reg & REG_MASK ] : (( reg < nRegs ) ? &particleRegisters [ reg ] : 0 ))

//================================================================//
// MOAIParticleScript
//================================================================//

//----------------------------------------------------------------//
u32 MOAIParticleScript::Instruction::GetSize () {

	return this->mSize;
}

//----------------------------------------------------------------//
void MOAIParticleScript::Instruction::Init ( u32 opcode, cc8* format ) {
	this->mOpcode = opcode;
	this->mFormat = format;
}

//----------------------------------------------------------------//
MOAIParticleScript::Instruction::Instruction () :
	mOpcode ( MOAIParticleScript::END ),
	mFormat ( 0 ),
	mSize ( 1 ) {
}

//----------------------------------------------------------------//
void MOAIParticleScript::Instruction::Parse ( USLuaState& state, u32 idx ) {

	float v;

	if ( this->mFormat ) {
		for ( u32 i = 0; this->mFormat [ i ]; ++i ) {
		
			cc8 c = this->mFormat [ i ];
			
			switch ( c ) {

				case 'I':
				case 'R':
					this->mParams [ i ] = state.GetValue < u32 >( idx++, 0 );
					break;
				
				case 'F':
					v = state.GetValue < float >( idx++, 0.0f );
					this->mParams [ i ] = *(( u32* )&v );
					break;
			}
			
			switch ( c ) {
				
				case 'R':
					this->mSize++;
					break;
				
				case 'I':
				case 'F':
					this->mSize += sizeof ( u32 );
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
u8* MOAIParticleScript::Instruction::Write ( u8* cursor ) {

	*( cursor++ ) = ( u8 )this->mOpcode;
	u8 r = 0;

	if ( this->mFormat ) {
	
		for ( u32 i = 0; this->mFormat [ i ]; ++i ) {
				
			cc8 c = this->mFormat [ i ];
			
			switch ( c ) {
				
				case 'R':
					r = ( u8 )this->mParams [ i ];
					
					if (( r & SPRITE_REG_BIT ) == 0 ) {
						// register belongs to particle
						// if not reserved, offset or set equal to reg mask if 0
						if (( r & PARTICLE_REG_BIT ) == 0 ) {
							r = r ? r + MOAIParticle::TOTAL_PARTICLE_REG - 1 : REG_MASK ;
						}
						r = r & REG_MASK;
					}
					*( cursor++ ) = r;
					break;
				
				case 'I':
				case 'F':
					*(( u32* )cursor ) = this->mParams [ i ];
					cursor += sizeof ( u32 );
					break;
			}
		}
	}
	return cursor;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	easeConst
	@text	Load a register with a value interpolated between two numbers
			using an ease curve.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number start		Starting value of the ease (from const).
	@in		number end			ESnding value of the ease (from const).
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _easeConst, EASE_CONST, "RFFI" )

//----------------------------------------------------------------//
/**	@name	easeConstDelta
	@text	Load a register with a value interpolated between two numbers
			using an ease curve. Apply as a delta.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number start		Starting value of the ease (from const).
	@in		number end			ESnding value of the ease (from const).
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _easeConstDelta, EASE_CONST_DELTA, "RFFI" )

//----------------------------------------------------------------//
/**	@name	easeVar
	@text	Load a register with a value interpolated between two numbers
			using an ease curve.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number r1			Starting value of the ease (from register).
	@in		number r2			Ending value of the ease (from register).
	@in		number add			Value to add to result (from const).
	@in		number scale		Value to scale result (from const).
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _easeVar, EASE_VAR, "RRRIFF" )

//----------------------------------------------------------------//
/**	@name	easeVarDelta
	@text	Load a register with a value interpolated between two numbers
			using an ease curve. Apply as a delta.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number r1			Starting value of the ease (from register).
	@in		number r2			Ending value of the ease (from register).
	@in		number add			Value to add to result (from const).
	@in		number scale		Value to scale result (from const).
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _easeVarDelta, EASE_VAR_DELTA, "RRRIFF" )

//----------------------------------------------------------------//
/**	@name	randConst
	@text	Load a register with a random number from a range.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number min			Index of variable containing range minimum (from const).
	@in		number max			Index of variable containing range maximum (from const).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _randConst, RAND_CONST, "RFF" )

//----------------------------------------------------------------//
/**	@name	randVar
	@text	Load a register with a random number from a range.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number r1			Index of variable containing range minimum (from register).
	@in		number r2			Index of variable containing range maximum (from register).
	@in		number add			Value to add to result (from const).
	@in		number scale		Value to scale result (from const).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _randVar, RAND_VAR, "RRRFF" )

//----------------------------------------------------------------//
/**	@name	randVec
	@text	Load two registers with the X and Y components of a vector
			with randomly chosen direction and length.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result X.
	@in		number r1			Register to store result Y.
	@in		number r2			Minimum length of vector (from register).
	@in		number r3			Maximum length of vector (from register).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _randVec, RAND_VEC, "RRRR" )

//----------------------------------------------------------------//
/**	@name	initRandVec
	@text	Load two registers with the X and Y components of a vector
			with randomly chosen direction and length.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result X.
	@in		number r1			Register to store result Y.
	@in		number min			Minimum length of vector (from const).
	@in		number max			Maximum length of vector (from const).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _randVecConst, RAND_VEC_CONST, "RRFF" )

//----------------------------------------------------------------//
/**	@name	setConst
	@text	Load a value into a register.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number value		Value to load (from const).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _setConst, SET_CONST, "RF" )

//----------------------------------------------------------------//
/**	@name	setVar
	@text	Load a value into a register.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number r1			Value to load (from register).
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _setVar, SET_VAR, "RR" )

//----------------------------------------------------------------//
/**	@name	sprite
	@text	Push a new sprite for rendering. To render a particle, first
			call 'sprite' to create a new sprite at the particle's location.
			Then modify the sprite's registers to create animated effects
			based on the age of the particle (normalized to its term).
	
	@in		MOAIParticleScript self
	@out	nil
*/
IMPL_LUA_PARTICLE_OP ( _sprite, SPRITE, "" )

//================================================================//
// MOAIParticleScript
//================================================================//

//----------------------------------------------------------------//
u8* MOAIParticleScript::Compile () {

	if ( this->mCompiled ) return this->mBytecode;
	

	Instruction end;
	end.Init ( END, "" );

	u32 size = 0;
	FOREACH ( InstructionIt, instructionIt, this->mInstructions ) {
		Instruction& instruction = *instructionIt;
		size += instruction.GetSize ();
	}
	size += end.GetSize ();
	
	this->mBytecode.Init ( size );
	
	u8* cursor = this->mBytecode;
	FOREACH ( InstructionIt, instructionIt, this->mInstructions ) {
		Instruction& instruction = *instructionIt;
		cursor = instruction.Write ( cursor );
	}
	end.Write ( cursor );
	
	this->mInstructions.clear ();
	this->mCompiled = true;
	return this->mBytecode;
}

//----------------------------------------------------------------//
MOAIParticleScript::MOAIParticleScript () :
	mCompiled ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleScript::~MOAIParticleScript () {
}

//----------------------------------------------------------------//
MOAIParticleScript::Instruction& MOAIParticleScript::PushInstruction ( u32 op, cc8* format ) {

	Instruction instruction;
	instruction.Init ( op, format );
	
	this->mInstructions.push_back ( instruction );
	return this->mInstructions.back ();
}

//----------------------------------------------------------------//
void MOAIParticleScript::PushSprite ( MOAIParticleSystem& system, float* registers ) {

	MOAIParticleSprite sprite;

	sprite.mLoc.mX		= registers [ SPRITE_X_LOC ];
	sprite.mLoc.mY		= registers [ SPRITE_Y_LOC ];
	
	sprite.mRot			= registers [ SPRITE_ROT ];
	
	sprite.mScl.mX		= registers [ SPRITE_X_SCL ];
	sprite.mScl.mY		= registers [ SPRITE_Y_SCL ];
	
	float opacity		= registers [ SPRITE_OPACITY ];
	float glow			= 1.0f - registers [ SPRITE_GLOW ];
	
	sprite.mColor.Set (
		registers [ SPRITE_RED ] * opacity,
		registers [ SPRITE_GREEN ] * opacity,
		registers [ SPRITE_BLUE ] * opacity,
		opacity * glow
	);
	
	sprite.mGfxID = USFloat::ToInt ( registers [ SPRITE_IDX ]);
	
	system.PushSprite ( sprite );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "PARTICLE_X",		( s32 )( MOAIParticle::PARTICLE_X | PARTICLE_REG_BIT ));
	state.SetField ( -1, "PARTICLE_Y",		( s32 )( MOAIParticle::PARTICLE_Y | PARTICLE_REG_BIT ));
	state.SetField ( -1, "PARTICLE_DX",		( s32 )( MOAIParticle::PARTICLE_DX | PARTICLE_REG_BIT ));
	state.SetField ( -1, "PARTICLE_DY",		( s32 )( MOAIParticle::PARTICLE_DY | PARTICLE_REG_BIT ));

	state.SetField ( -1, "SPRITE_X_LOC",	( u32 )( SPRITE_X_LOC | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_Y_LOC",	( u32 )( SPRITE_Y_LOC | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_ROT",		( u32 )( SPRITE_ROT | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_X_SCL",	( u32 )( SPRITE_X_SCL | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_Y_SCL",	( u32 )( SPRITE_Y_SCL | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_RED",		( u32 )( SPRITE_RED | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_GREEN",	( u32 )( SPRITE_GREEN | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_BLUE",		( u32 )( SPRITE_BLUE | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_OPACITY",	( u32 )( SPRITE_OPACITY | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_GLOW",		( u32 )( SPRITE_GLOW | SPRITE_REG_BIT ));
	state.SetField ( -1, "SPRITE_IDX",		( u32 )( SPRITE_IDX | SPRITE_REG_BIT ));
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "easeConst",			_easeConst },
		{ "easeConstDelta",		_easeConstDelta },
		{ "easeVar",			_easeVar },
		{ "easeVarDelta",		_easeVarDelta },
		{ "randConst",			_randConst },
		{ "randVar",			_randVar },
		{ "randVec",			_randVec },
		{ "randVecConst",		_randVecConst },
		{ "setConst",			_setConst },
		{ "setVar",				_setVar },
		{ "sprite",				_sprite },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleScript::ResetRegisters ( float* spriteRegisters, float* particleRegisters ) {

	spriteRegisters [ SPRITE_X_LOC ]		= particleRegisters [ MOAIParticle::PARTICLE_X ];
	spriteRegisters [ SPRITE_Y_LOC ]		= particleRegisters [ MOAIParticle::PARTICLE_Y ];
	spriteRegisters [ SPRITE_ROT ]			= 0.0f;
	spriteRegisters [ SPRITE_X_SCL ]		= 1.0f;
	spriteRegisters [ SPRITE_Y_SCL ]		= 1.0f;
	spriteRegisters [ SPRITE_RED ]			= 1.0f;
	spriteRegisters [ SPRITE_GREEN ]		= 1.0f;
	spriteRegisters [ SPRITE_BLUE ]			= 1.0f;
	spriteRegisters [ SPRITE_OPACITY ]		= 1.0f;
	spriteRegisters [ SPRITE_GLOW ]			= 0.0f;
	spriteRegisters [ SPRITE_IDX ]			= 1.0f;
}

//----------------------------------------------------------------//
void MOAIParticleScript::Run ( MOAIParticleSystem& system, MOAIParticle& particle, float step ) {

	u8* bytecode = this->mBytecode;
	if ( !bytecode ) return;

	float t0 = particle.mAge / particle.mTerm;
	
	particle.mAge += step;
	float t1 = particle.mAge / particle.mTerm;
	
	u8 r0, r1, r2, r3;
	float f0, f1, f2, f3;
	float v0, v1;
	u32 i0;
	
	USVec2D loc;
	USVec2D vel;
	
	u32 nRegs = system.mParticleSize;
	float* particleRegisters = particle.mData;
	
	MOAIParticleSprite sprite;
	float spriteRegisters [ TOTAL_SPRITE_REG ];
	
	float* r = 0;

	bool push = false;
	
	for ( u8 opcode = *( bytecode++ ); opcode != END; opcode = *( bytecode++ )) {
		
		switch ( opcode ) {
			
			case EASE_CONST: // RFFI
				
				READ_BYTE	( r0, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				READ_INT	( i0, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = USInterpolate::Interpolate ( i0, f0, f1, t1 );
				}
				break;
			
			case EASE_CONST_DELTA: // RFFI
				
				READ_BYTE	( r0, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				READ_INT	( i0, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					
					v0 = USInterpolate::Interpolate ( i0, f0, f1, t0 );
					v1 = USInterpolate::Interpolate ( i0, f0, f1, t1 );
					
					*r += ( v1 - v0 );
				}
				break;
			
			case EASE_VAR: // RRRIFF
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				READ_BYTE	( r2, bytecode );
				READ_INT	( i0, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					
					f2 = REG_VALUE ( r1 );
					f3 = REG_VALUE ( r2 );
					
					*r = f0 + ( USInterpolate::Interpolate ( i0, f2, f3, t1 ) * f1 );
				}
				break;
			
			case EASE_VAR_DELTA: // RRRIFF
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				READ_BYTE	( r2, bytecode );
				READ_INT	( i0, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
				
					f2 = REG_VALUE ( r1 );
					f3 = REG_VALUE ( r2 );
					
					v0 = f0 + ( USInterpolate::Interpolate ( i0, f2, f3, t0 ) * f1 );
					v1 = f0 + ( USInterpolate::Interpolate ( i0, f2, f3, t1 ) * f1 );
					
					*r += ( v1 - v0 );
				}
				break;
				
			case RAND_CONST: // RFF
				
				READ_BYTE	( r0, bytecode ); // register
				READ_FLOAT	( f0, bytecode ); // f0
				READ_FLOAT	( f1, bytecode ); // f1
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = USFloat::Rand ( f0, f1 );
				}
				break;
				
			case RAND_VAR: // RRRFF
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				READ_BYTE	( r2, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = f0 + ( USFloat::Rand ( REG_VALUE ( r1 ), REG_VALUE ( r2 )) * f1 );
				}
				break;
				
			case RAND_VEC: // RRRR
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				READ_BYTE	( r2, bytecode );
				READ_BYTE	( r3, bytecode );
				
				f0 = USFloat::Rand ( 360.0f ) * ( float )D2R;
				f1 = USFloat::Rand ( REG_VALUE ( r2 ),  REG_VALUE ( r3 ));
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = Cos ( f0 ) * f1;
				}
				
				r = REG_ADDR ( r1 );
				if ( r ) {
					*r = -Sin ( f0 ) * f1;
				}
				break;
				
			case RAND_VEC_CONST: // RRFF
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				READ_FLOAT	( f0, bytecode );
				READ_FLOAT	( f1, bytecode );
				
				f2 = USFloat::Rand ( 360.0f ) * ( float )D2R;
				f3 = USFloat::Rand ( f0, f1 );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = Cos ( f2 ) * f3;
				}
				
				r = REG_ADDR ( r1 );
				if ( r ) {
					*r = -Sin ( f2 ) * f3;
				}
				break;
				
			case SET_CONST: // RF
				
				READ_BYTE	( r0, bytecode );
				READ_FLOAT	( f0, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = f0;
				}
				break;
				
			case SET_VAR: // RR
				
				READ_BYTE	( r0, bytecode );
				READ_BYTE	( r1, bytecode );
				
				r = REG_ADDR ( r0 );
				if ( r ) {
					*r = REG_VALUE ( r1 );
				}
				break;
				
			case SPRITE: //
				
				if ( push ) {
					this->PushSprite ( system, spriteRegisters );
				}
				this->ResetRegisters ( spriteRegisters, particleRegisters );
				push = true;
				break;
		}
	}
	
	if ( push ) {
		this->PushSprite ( system, spriteRegisters );
	}
}

//----------------------------------------------------------------//
STLString MOAIParticleScript::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mCompiled )

	// TODO print instructions

	return repr;
}
