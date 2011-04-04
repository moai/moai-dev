// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleEngine.h>

#define IMPL_LUA_PARTICLE_OP(name,opcode,rParams,vParams)										\
	int MOAIParticleScript::name ( lua_State* L ) {												\
		LUA_SETUP ( MOAIParticleScript, "U" )													\
		self->ParseInstruction ( state, 2, MOAIParticleEngine::opcode, rParams, vParams );		\
		return 0;																				\
	}

//================================================================//
// MOAIParticleScript
//================================================================//

//----------------------------------------------------------------//
u32 MOAIParticleScript::Instruction::GetSize () {

	return 2 + this->mTotalRegisters + ( this->mTotalValues * sizeof ( float ));
}

//----------------------------------------------------------------//
void MOAIParticleScript::Instruction::Init ( u32 opcode, u32 nRegisters, u32 nValues ) {
	this->mOpCode = opcode;
	this->mTotalRegisters = nRegisters;
	this->mTotalValues = nValues;
}

//----------------------------------------------------------------//
MOAIParticleScript::Instruction::Instruction () :
	mOpCount ( 0 ) {

	this->SetRegisters ( 0, 0, 0, 0 );
	this->SetValues ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
void MOAIParticleScript::Instruction::SetRegisters ( u8 r0, u8 r1, u8 r2, u8 r3 ) {
	this->mRegisters [ 0 ] = r0;
	this->mRegisters [ 1 ] = r1;
	this->mRegisters [ 2 ] = r2;
	this->mRegisters [ 3 ] = r3;
}

//----------------------------------------------------------------//
void MOAIParticleScript::Instruction::SetValues ( float v0, float v1, float v2, float v3 ) {
	this->mValues [ 0 ] = v0;
	this->mValues [ 1 ] = v1;
	this->mValues [ 2 ] = v2;
	this->mValues [ 3 ] = v3;
}

//----------------------------------------------------------------//
u8* MOAIParticleScript::Instruction::Write ( u8* cursor ) {

	*( cursor++ ) = ( u8 )this->mOpCode;
	*( cursor++ ) = ( u8 )this->mOpCount;
	
	for ( u32 i = 0; i < this->mTotalRegisters; ++i ) {
		*( cursor++ ) = this->mRegisters [ i ];
	}
	
	for ( u32 i = 0; i < this->mTotalValues; ++i ) {
		*(( u32* )cursor ) = *(( u32* )&this->mValues [ i ]);
		cursor += sizeof ( u32 );
	}

	return cursor;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _abs,				ABS,				1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _accAttractor,		ACC_ATTRACTOR,		3, 2 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _accForces,			ACC_FORCES,			2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _accLinear,			ACC_LINEAR,			1, 2 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _add,				ADD,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _damp,				DAMP,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _div,				DIV,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _ease,				EASE,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _initLoc,			INIT_LOC,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _initTime,			INIT_TIME,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _initVec,			INIT_VEC,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _loadAge,			LOAD_AGE,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _loadDuration,		LOAD_DURATION,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _loadStep,			LOAD_STEP,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _loadTime,			LOAD_TIME,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _mac,				MAC,				4, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _mov,				MOV,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _mul,				MUL,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _rand,				RAND,				1, 2 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _scale,				SCALE,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _scaleAdd,			SCALE_ADD,			2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _setDuration,		SET_DURATION,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _sprite,				SPRITE,				0, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteAlpha,		SPRITE_ALPHA,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteColor,		SPRITE_COLOR,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteGlow,			SPRITE_GLOW,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteIdx,			SPRITE_IDX,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteLoc,			SPRITE_LOC,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteRot,			SPRITE_ROT,			1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteScale,		SPRITE_SCALE,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _spriteSize,			SPRITE_SIZE,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _stepEuler,			STEP_EULER,			3, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _sub,				SUB,				2, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _subFromOne,			SUB_FROM_ONE,		1, 0 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
IMPL_LUA_PARTICLE_OP ( _vec2Rand,			VEC2_RAND,			1, 2 )

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleScript::_load ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleScript, "UNN" )
	
	int top = state.GetTop ();
	u32 n = 1;
	
	if ( top >= 4 ) {
		n++;
	}
	
	if ( top >= 5 ) {
		n++;
	}
	
	if ( top >= 6 ) {
		n++;
	}
	
	Instruction& instruction = self->WriteOp ( MOAIParticleEngine::LOAD, 1, n );
	instruction.mOpCount = n;
	
	instruction.SetRegisters (
		( u8 )state.GetValue < u32 >( 2, 0 ),
		0,
		0,
		0
	);
	
	instruction.SetValues (
		( float )state.GetValue < float >( 3, 0.0f ),
		( float )state.GetValue < float >( 4, 0.0f ),
		( float )state.GetValue < float >( 5, 0.0f ),
		( float )state.GetValue < float >( 6, 0.0f )
	);
	
	return 0;
}
//================================================================//
// MOAIParticleScript
//================================================================//

//----------------------------------------------------------------//
u8* MOAIParticleScript::Compile () {

	if ( this->mCompiled ) return this->mBytecode;
	this->mCompiled = true;

	Instruction end;
	end.Init ( MOAIParticleEngine::END, 0, 0 );

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
	return this->mBytecode;
}

//----------------------------------------------------------------//
u8* MOAIParticleScript::GetBytecode () {
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
void MOAIParticleScript::ParseInstruction ( USLuaState& state, u32 idx, u32 op, u32 rParams, u32 vParams ) {

	Instruction& instruction = this->WriteOp ( op, rParams, vParams );

	for ( u32 i = 0; i < rParams; ++i ) {
		instruction.mRegisters [ i ] = ( u8 )state.GetValue < u32 >( idx++, 0 );
	}
	
	for ( u32 i = 0; i < vParams; ++i ) {
		instruction.mValues [ i ] = state.GetValue < float >( idx++, 0 );
	}
	
	instruction.mOpCount = state.GetValue < u32 >( idx++, 1 );
}

//----------------------------------------------------------------//
MOAIParticleScript::~MOAIParticleScript () {
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "abs",				_abs },
		{ "accAttractor",		_accAttractor },
		{ "accForces",			_accForces },
		{ "accLinear",			_accLinear },
		{ "add",				_add },
		{ "damp",				_damp },
		{ "div",				_div },
		{ "ease",				_ease },
		{ "initLoc",			_initLoc },
		{ "initTime",			_initTime },
		{ "initVec",			_initVec },
		{ "load",				_load },
		{ "loadAge",			_loadAge },
		{ "loadDuration",		_loadDuration },
		{ "loadStep",			_loadStep },
		{ "loadTime",			_loadTime },
		{ "mac",				_mac },
		{ "mov",				_mov },
		{ "mul",				_mul },
		{ "rand",				_rand },
		{ "scale",				_scale },
		{ "scaleAdd",			_scaleAdd },
		{ "setDuration",		_setDuration },
		{ "sprite",				_sprite },
		{ "spriteAlpha",		_spriteAlpha },
		{ "spriteColor",		_spriteColor },
		{ "spriteGlow",			_spriteGlow },
		{ "spriteIdx",			_spriteIdx },
		{ "spriteLoc",			_spriteLoc },
		{ "spriteRot",			_spriteRot },
		{ "spriteScale",		_spriteScale },
		{ "spriteSize",			_spriteSize },
		{ "stepEuler",			_stepEuler },
		{ "sub",				_sub },
		{ "subFromOne",			_subFromOne },
		{ "vec2Rand",			_vec2Rand },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleScript::WriteBinaryRegOp ( u32 op, u32 r0, u32 r1 ) {

	Instruction instruction;
	instruction.Init ( op, 2, 0 );
	
	instruction.SetRegisters (( u8 )r0, ( u8 )r1, 0, 0 );
	instruction.SetValues ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mInstructions.push_back ( instruction );
}

//----------------------------------------------------------------//
void MOAIParticleScript::WriteBinaryValOp ( u32 op, u32 r0, float v0 ) {

	Instruction instruction;
	instruction.Init ( op, 1, 1 );
	
	instruction.SetRegisters (( u8 )r0, 0, 0, 0 );
	instruction.SetValues ( v0, 0.0f, 0.0f, 0.0f );
	
	this->mInstructions.push_back ( instruction );
}

//----------------------------------------------------------------//
MOAIParticleScript::Instruction& MOAIParticleScript::WriteOp ( u32 op, u32 nRegisters, u32 nValues ) {

	Instruction instruction;
	instruction.Init ( op, nRegisters, nValues );
	
	this->mInstructions.push_back ( instruction );
	return this->mInstructions.back ();
}

//----------------------------------------------------------------//
STLString MOAIParticleScript::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mCompiled )

	// TODO print instructions

	return repr;
}
