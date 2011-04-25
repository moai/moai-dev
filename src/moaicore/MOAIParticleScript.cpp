// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleSystem.h>

#define READ_BYTE(var,bytecode) \
	var = *( bytecode++ );

#define READ_FLOAT(var,bytecode) \
	*(( u32* )&var ) = *(( u32* )bytecode ); \
	bytecode += sizeof ( u32 );

#define IMPL_LUA_PARTICLE_OP(name,opcode,format)								\
	int MOAIParticleScript::name ( lua_State* L ) {								\
		MOAI_LUA_SETUP ( MOAIParticleScript, "U" )								\
		Instruction& instruction = self->PushInstruction ( opcode, format );	\
		instruction.Parse ( state, 2 );											\
		return 0;																\
	}

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

				case 'P':
					this->mParams [ i ] = state.GetValue < u32 >( idx++, 1 ) - 1;
					break;
				
				case 'C':
				case 'R':
					this->mParams [ i ] = state.GetValue < u32 >( idx++, 0 );
					break;
				
				case 'V':
					v = state.GetValue < float >( idx++, 0.0f );
					this->mParams [ i ] = *(( u32* )&v );
					break;
			}
			
			switch ( c ) {
				
				case 'P':
				case 'R':
					this->mSize++;
					break;
				
				case 'C':
				case 'V':
					this->mSize += sizeof ( u32 );
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
u8* MOAIParticleScript::Instruction::Write ( u8* cursor ) {

	*( cursor++ ) = ( u8 )this->mOpcode;

	if ( this->mFormat ) {
	
		for ( u32 i = 0; this->mFormat [ i ]; ++i ) {
				
			cc8 c = this->mFormat [ i ];
			
			switch ( c ) {
			
				case 'P':
				case 'R':
					*( cursor++ ) = ( u8 )this->mParams [ i ];
					break;
				
				case 'C':
				case 'V':
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
IMPL_LUA_PARTICLE_OP ( _easeConst, EASE_CONST, "RVVR" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _easeVar, EASE_VAR, "RPPRVV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _initConst, INIT_CONST, "PV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _initRand, INIT_RAND, "PVV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _initRandVec, INIT_RAND_VEC, "PPVV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _randConst, RAND_CONST, "RVV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _randVar, RAND_VAR, "RPPVV" )

//----------------------------------------------------------------//
IMPL_LUA_PARTICLE_OP ( _setConst, SET_CONST, "RV" )

//----------------------------------------------------------------//
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
void MOAIParticleScript::PushSprite ( MOAIParticleSystem& system, MOAIParticle& particle, float* registers ) {

	MOAIParticleSprite sprite;

	particle.mOffset.mX	= registers [ X_OFF ];
	particle.mOffset.mY	= registers [ Y_OFF ];

	sprite.mLoc.mX		= particle.mLoc.mX + registers [ X_OFF ];
	sprite.mLoc.mY		= particle.mLoc.mY + registers [ Y_OFF ];
	
	sprite.mRot			= registers [ ROT ];
	
	sprite.mScl.mX		= registers [ X_SCL ];
	sprite.mScl.mY		= registers [ Y_SCL ];
	
	float fade			= registers [ OPACITY ];
	float glow			= 1.0f - registers [ GLOW ];
	
	sprite.mColor.Set (
		registers [ R ] * fade,
		registers [ G ] * fade,
		registers [ B ] * fade,
		registers [ A ] * fade * glow
	);
	
	sprite.mGfxID = USFloat::ToInt ( registers [ IDX ]);
	
	system.PushSprite ( sprite );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "SPRITE_X_OFF",	( u32 )X_OFF );
	state.SetField ( -1, "SPRITE_Y_OFF",	( u32 )Y_OFF );
	state.SetField ( -1, "SPRITE_ROT",		( u32 )ROT );
	state.SetField ( -1, "SPRITE_X_SCL",	( u32 )X_SCL );
	state.SetField ( -1, "SPRITE_Y_SCL",	( u32 )Y_SCL );
	state.SetField ( -1, "SPRITE_RED",		( u32 )R );
	state.SetField ( -1, "SPRITE_GREEN",	( u32 )G );
	state.SetField ( -1, "SPRITE_BLUE",		( u32 )B );
	state.SetField ( -1, "SPRITE_ALPHA",	( u32 )A );
	state.SetField ( -1, "SPRITE_OPACITY",	( u32 )OPACITY );
	state.SetField ( -1, "SPRITE_GLOW",		( u32 )GLOW );
	state.SetField ( -1, "SPRITE_IDX",		( u32 )IDX );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "easeConst",			_easeConst },
		{ "easeVar",			_easeVar },
		{ "initConst",			_initConst },
		{ "initRand",			_initRand },
		{ "initRandVec",		_initRandVec },
		{ "randConst",			_randConst },
		{ "randVar",			_randVar },
		{ "setConst",			_setConst },
		{ "sprite",				_sprite },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleScript::ResetRegisters ( float* registers ) {

	registers [ X_OFF ]		= 0.0f;
	registers [ Y_OFF ]		= 0.0f;
	registers [ ROT ]		= 0.0f;
	registers [ X_SCL ]		= 1.0f;
	registers [ Y_SCL ]		= 1.0f;
	registers [ R ]			= 1.0f;
	registers [ G ]			= 1.0f;
	registers [ B ]			= 1.0f;
	registers [ A ]			= 1.0f;
	registers [ OPACITY ]	= 1.0f;
	registers [ GLOW ]		= 0.0f;
	registers [ IDX ]		= 1.0f;
}

//----------------------------------------------------------------//
void MOAIParticleScript::Run ( MOAIParticleSystem& system, MOAIParticle& particle ) {

	u8* bytecode = this->mBytecode;
	if ( !bytecode ) return;

	float t = particle.mAge / particle.mTerm;
	u8 r0, r1, r2, r3;
	float v0, v1, v2, v3;
	float p0, p1;
	
	u32 nParams = system.mParticleSize;
	float* params = particle.mData;
	
	MOAIParticleSprite sprite;
	float r [ TOTAL ];

	bool push = false;
	
	for ( u8 opcode = *( bytecode++ ); opcode != END; opcode = *( bytecode++ )) {
		
		switch ( opcode ) {
			
			case EASE_CONST: // RVVR
			
				READ_BYTE ( r0, bytecode ); // register
				READ_FLOAT ( v0, bytecode ); // v0
				READ_FLOAT ( v1, bytecode ); // v1
				READ_BYTE ( r1, bytecode ); // ease type
				
				if ( r0 < TOTAL ) {
					r [ r0 ] = USInterpolate::Interpolate ( r1, v0, v1, t );
				}
				break;
			
			case EASE_VAR: // RPPRVV
			
				READ_BYTE ( r0, bytecode ); // register
				READ_BYTE ( r1, bytecode ); // r0
				READ_BYTE ( r2, bytecode ); // r1
				READ_BYTE ( r3, bytecode ); // ease type
				
				READ_FLOAT ( v0, bytecode ); // add
				READ_FLOAT ( v1, bytecode ); // scale
				
				if ( r0 < TOTAL ) {
				
					p0 = r1 < nParams ? params [ r1 ] : 0.0f;
					p1 = r2 < nParams ? params [ r2 ] : 0.0f;
				
					r [ r0 ] = v0 + ( USInterpolate::Interpolate ( r3, p0, p1, t ) * v1 );
				}
				break;
			
			case INIT_CONST: // PVV
			
				READ_BYTE ( r0, bytecode ); // param
				
				READ_FLOAT ( v0, bytecode );
				
				if ( r0 < nParams ) {
					params [ r0 ] = v0;
				}
				break;
			
			case INIT_RAND: // PVV
			
				READ_BYTE ( r0, bytecode ); // param
				
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				if ( r0 < nParams ) {
					params [ r0 ] = USFloat::Rand ( v0, v1 );
				}
				break;
			
			case INIT_RAND_VEC: // PPVV
			
				READ_BYTE ( r0, bytecode ); // param1
				READ_BYTE ( r1, bytecode ); // param2
				
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				v2 = USFloat::Rand ( 360.0f ) * ( float )D2R;
				v3 = USFloat::Rand ( v0, v1 );
				
				if ( r0 < nParams ) {
					params [ r0 ] = Cos ( v2 ) * v3;
				}
				
				if ( r1 < nParams ) {
					params [ r1 ] = -Sin ( v2 ) * v3;
				}
				break;
			
			case RAND_CONST: // RVV
			
				READ_BYTE ( r0, bytecode ); // register
				
				READ_FLOAT ( v0, bytecode ); // v0
				READ_FLOAT ( v1, bytecode ); // v1
				
				if ( r0 < TOTAL ) {
					r [ r0 ] = USFloat::Rand ( v0, v1 );
				}
				break;
			
			case RAND_VAR: // RPPVV
			
				READ_BYTE ( r0, bytecode ); // register
				READ_BYTE ( r1, bytecode );
				READ_BYTE ( r2, bytecode );
				
				READ_FLOAT ( v0, bytecode ); // add
				READ_FLOAT ( v1, bytecode ); // scale
				
				if ( r0 < TOTAL ) {
					
					p0 = r1 < nParams ? params [ r1 ] : 0.0f;
					p1 = r2 < nParams ? params [ r2 ] : 0.0f;
				
					r [ r0 ] = v0 + ( USFloat::Rand ( p0, p1 ) * v1 );
				}
				break;
			
			case SET_CONST: // RV
			
				READ_BYTE ( r0, bytecode ); // register
				READ_FLOAT ( v0, bytecode ); // v0
				
				if ( r0 < TOTAL ) {
					r [ r0 ] = v0;
				}
				break;
			
			case SPRITE: //
			
				if ( push ) {
					this->PushSprite ( system, particle, r );
				}
				this->ResetRegisters ( r );
				push = true;
				break;
		}
	}
	
	if ( push ) {
		this->PushSprite ( system, particle, r );
	}
}

//----------------------------------------------------------------//
STLString MOAIParticleScript::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mCompiled )

	// TODO print instructions

	return repr;
}
