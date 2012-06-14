// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleSystem.h>

#define IMPL_LUA_PARTICLE_OP(opcode,format)									\
	MOAI_LUA_SETUP ( MOAIParticleScript, "U" )								\
	Instruction& instruction = self->PushInstruction ( opcode, format );	\
	instruction.Parse ( state, 2 );											\
	return 0;

#define READ_ADDR(reg,bytecode)									\
	type = *( bytecode++ );										\
	regIdx = *( bytecode++ );									\
	reg = 0;													\
																\
	if ( type & PARAM_TYPE_REG_MASK ) {							\
		if ( type == PARAM_TYPE_SPRITE_REG ) {					\
			reg = &spriteRegisters [ regIdx ];					\
		}														\
		else {													\
			reg = &particleRegisters [ regIdx ];				\
		}														\
	}

#define READ_VALUE(var,bytecode)								\
	type = *( bytecode++ );										\
	if ( type == PARAM_TYPE_CONST ) {							\
		dst = ( u8* )&var;										\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );							\
	}															\
	else {														\
		regIdx = *( bytecode++ );								\
		var = 0.0f;												\
																\
		if ( type & PARAM_TYPE_REG_MASK ) {						\
			if ( type == PARAM_TYPE_SPRITE_REG ) {				\
				var = spriteRegisters [ regIdx ];				\
			}													\
			else {												\
				var = particleRegisters [ regIdx ];				\
			}													\
		}														\
	}

#define READ_INT(var,bytecode)									\
		dst = ( u8* )&var;										\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );							\
		*( dst++ ) = *( bytecode++ );

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
void MOAIParticleScript::Instruction::Parse ( MOAILuaState& state, u32 idx ) {

	u64 bits;
	u8 type;

	if ( this->mFormat ) {
		for ( u32 i = 0; this->mFormat [ i ]; ++i ) {
		
			cc8 c = this->mFormat [ i ];
			
			switch ( c ) {
				
				case 'I':
					
					this->mSize += sizeof ( u32 );
					
					this->mParams [ i ] = state.GetValue < u32 >( idx++, 0 );
					this->mTypes [ i ] = PARAM_TYPE_FLAG;
					break;
				
				case 'R':
					
					this->mSize += sizeof ( u8 );
					this->mSize += sizeof ( u8 );
					
					bits = state.GetValue < u64 >( idx++, 0 );
					type = ( bits >> 32 ) & PARAM_TYPE_MASK;

					if ( !( type & PARAM_TYPE_REG_MASK )) {
					
						this->mTypes [ i ] = PARAM_TYPE_CONST; // force load of 0 on decode
						this->mParams [ i ] = 0;
					}
					else {
					
						this->mTypes [ i ] = type;
						this->mParams [ i ] = ( u32 )( bits & 0xff );
					}
					break;
				
				case 'V':
					
					this->mSize += sizeof ( u8 );
					
					bits = state.GetValue < u64 >( idx++, 0 );
					
					type = ( bits >> 32 ) & PARAM_TYPE_MASK;
					this->mTypes [ i ] = type;
					
					if ( type & PARAM_TYPE_REG_MASK ) {
						
						this->mParams [ i ] = ( u32 )( bits & 0xff );
						this->mSize += sizeof ( u8 );
					}
					else if ( type == PARAM_TYPE_CONST ) {
					
						this->mParams [ i ] = ( u32 )bits;
						this->mSize += sizeof ( u32 );
					}
					else {
						
						// unrecognized; treat as const
						// TODO: issue warning
						this->mTypes [ i ] = PARAM_TYPE_CONST;
						this->mParams [ i ] = 0;
						this->mSize += sizeof ( u32 );
					}
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
u8* MOAIParticleScript::Instruction::Write ( u8* cursor ) {

	*( cursor++ ) = ( u8 )this->mOpcode;
	u8* src;

	if ( this->mFormat ) {
	
		for ( u32 i = 0; this->mFormat [ i ]; ++i ) {
				
			cc8 c = this->mFormat [ i ];
			
			switch ( c ) {
				
				case 'I':
					
					src = ( u8* )&this->mParams [ i ];
					*( cursor++ ) = *( src++ );
					*( cursor++ ) = *( src++ );
					*( cursor++ ) = *( src++ );
					*( cursor++ ) = *( src++ );
					break;
				
				case 'R':
					
					*( cursor++ ) = this->mTypes [ i ];
					*( cursor++ ) = ( u8 )this->mParams [ i ];
					break;
				
				case 'V':
					
					*( cursor++ ) = this->mTypes [ i ];
					
					if ( this->mTypes [ i ] == PARAM_TYPE_CONST ) {
						
						src = ( u8* )&this->mParams [ i ];
						*( cursor++ ) = *( src++ );
						*( cursor++ ) = *( src++ );
						*( cursor++ ) = *( src++ );
						*( cursor++ ) = *( src++ );
					}
					else {
						
						*( cursor++ ) = ( u8 )this->mParams [ i ];
					}
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
/**	@name	add
	@text	r0 = v0 + v1
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_add ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( ADD, "RVV" )
}

//----------------------------------------------------------------//
/**	@name	angleVec
	@text	Load two registers with the X and Y components of a unit
			vector with a given angle.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result X.
	@in		number r1			Register to store result Y.
	@in		number v0			Angle of vector (in degrees).
	@out	nil
*/
int MOAIParticleScript::_angleVec ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( ANGLE_VEC, "RRV" )
}

//----------------------------------------------------------------//
/**	@name	cos
 @text	r0 = cos(v0)
 
 @in		MOAIParticleScript self
 @in		number r0
 @in		number v0
 @out	nil
 */
int MOAIParticleScript::_cos ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( COS, "RV" )
}

//----------------------------------------------------------------//
/**	@name	cycle
	@text	Cycle v0 between v1 and v2.
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@in		number v2
	@out	nil
*/
int MOAIParticleScript::_cycle ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( CYCLE, "RVVV" )
}

//----------------------------------------------------------------//
/**	@name	div
	@text	r0 = v0 / v1
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_div ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( DIV, "RVV" )
}

//----------------------------------------------------------------//
/**	@name	ease
	@text	Load a register with a value interpolated between two numbers
			using an ease curve.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number v0			Starting value of the ease.
	@in		number v1			Ending value of the ease.
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
int MOAIParticleScript::_ease ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( EASE, "RVVI" )
}

//----------------------------------------------------------------//
/**	@name	easeDelta
	@text	Load a register with a value interpolated between two numbers
			using an ease curve. Apply as a delta.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number v0			Starting value of the ease.
	@in		number v1			Ending value of the ease.
	@in		number easeType		See MOAIEaseType for a list of ease types.
	@out	nil
*/
int MOAIParticleScript::_easeDelta ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( EASE_DELTA, "RVVI" )
}

//----------------------------------------------------------------//
/**	@name	mul
	@text	r0 = v0 * v1
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_mul ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( MUL, "RVV" )
}

//----------------------------------------------------------------//
/**	@name	norm
	@text	r0 = v0 / |v|
	@text	r1 = v1 / |v|
	@text	Where |v| == sqrt( v0^2 + v1^2)
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number r1
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_norm ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( NORM, "RRVV" )
}

//----------------------------------------------------------------//
/**	@name	packConst
	@text	Pack a const value into a particle script param.
	
	@in		MOAIParticleScript self
	@in		number const		Const value to pack.
	@out	nil
*/
int MOAIParticleScript::_packConst ( lua_State* L ) {
	MOAILuaState state ( L );

	float val = state.GetValue < float >( 1, 0.0f );
	u32 bits;
	memcpy ( &bits, &val, sizeof ( u32 ));
	state.Push ( Pack64 ( bits, PARAM_TYPE_CONST ));

	return 1;
}

//----------------------------------------------------------------//
/**	@name	packReg
	@text	Pack a register index into a particle script param.
	
	@in		MOAIParticleScript self
	@in		number regIdx		Register index to pack.
	@out	nil
*/
int MOAIParticleScript::_packReg ( lua_State* L ) {
	MOAILuaState state ( L );

	u8 val = state.GetValue < u8 >( 1, 0 ) + MOAIParticle::TOTAL_PARTICLE_REG - 1;
	state.Push ( Pack64 ( val, PARAM_TYPE_PARTICLE_REG ));

	return 1;
}

//----------------------------------------------------------------//
/**	@name	rand
	@text	Load a register with a random number from a range.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number v0			Range minimum.
	@in		number v1			Range maximum.
	@out	nil
*/
int MOAIParticleScript::_rand ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( RAND, "RVV" )
}

//----------------------------------------------------------------//
/**	@name	randVec
	@text	Load two registers with the X and Y components of a vector
			with randomly chosen direction and length.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result X.
	@in		number r1			Register to store result Y.
	@in		number v0			Minimum length of vector.
	@in		number v1			Maximum length of vector.
	@out	nil
*/
int MOAIParticleScript::_randVec ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( RAND_VEC, "RRVV" )
}

//----------------------------------------------------------------//
/**	@name	set
	@text	Load a value into a register.
	
	@in		MOAIParticleScript self
	@in		number r0			Register to store result.
	@in		number v0			Value to load.
	@out	nil
*/
int MOAIParticleScript::_set ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( SET, "RV" )
}

//----------------------------------------------------------------//
/**	@name	sin
 @text	r0 = sin(v0)
 
 @in		MOAIParticleScript self
 @in		number r0
 @in		number v0
 @out	nil
 */
int MOAIParticleScript::_sin ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( SIN, "RV" )
}

//----------------------------------------------------------------//
/**	@name	sprite
	@text	Push a new sprite for rendering. To render a particle, first
			call 'sprite' to create a new sprite at the particle's location.
			Then modify the sprite's registers to create animated effects
			based on the age of the particle (normalized to its term).
	
	@in		MOAIParticleScript self
	@out	nil
*/
int MOAIParticleScript::_sprite ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( SPRITE, "" )
}

//----------------------------------------------------------------//
/**	@name	sub
	@text	r0 = v0 - v1
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_sub ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( SUB, "RVV" )
}

//----------------------------------------------------------------//
/**	@name	tan
 @text	r0 = tan(v0)
 
 @in		MOAIParticleScript self
 @in		number r0
 @in		number v0
 @out	nil
 */
int MOAIParticleScript::_tan ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( TAN, "RV" )
}

//----------------------------------------------------------------//
/**	@name	time
	@text	Load the normalized age of the particle into a register.
	
	@in		MOAIParticleScript self
	@in		number r0
	@out	nil
*/
int MOAIParticleScript::_time ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( TIME, "R" )
}

//----------------------------------------------------------------//
/**	@name	wrap
	@text	Wrap v0 between v1 and v2.
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@in		number v2
	@out	nil
*/
int MOAIParticleScript::_wrap ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( WRAP, "RVVV" )
}

//----------------------------------------------------------------//
/**	@name	vecAngle
	@text	Compute angle (in degrees) between v0 and v1.
	
	@in		MOAIParticleScript self
	@in		number r0
	@in		number v0
	@in		number v1
	@out	nil
*/
int MOAIParticleScript::_vecAngle ( lua_State* L ) {
	IMPL_LUA_PARTICLE_OP ( VEC_ANGLE, "RVV" )
}

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
	
	u8* top = ( u8* )(( uintptr )cursor + size );
	UNUSED ( top );
	
	FOREACH ( InstructionIt, instructionIt, this->mInstructions ) {
		Instruction& instruction = *instructionIt;
		cursor = instruction.Write ( cursor );
	}
	cursor = end.Write ( cursor );
	
	assert ( cursor == top );
	
	this->mInstructions.clear ();
	this->mCompiled = true;
	return this->mBytecode;
}

//----------------------------------------------------------------//
MOAIParticleScript::MOAIParticleScript () :
	mCompiled ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
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
u64 MOAIParticleScript::Pack64 ( u32 low, u32 hi ) {

	u64 val64 = 0; 
	u32 lohi [ 2 ]; 
      
	lohi [ 0 ] = low; 
	lohi [ 1 ] = hi; 
 
	memcpy ( &val64, lohi, sizeof ( val64 )); 
 
	return val64;
}

//----------------------------------------------------------------//
void MOAIParticleScript::PushSprite ( MOAIParticleSystem& system, float* registers ) {

	AKUParticleSprite sprite;

	sprite.mXLoc		= registers [ SPRITE_X_LOC ];
	sprite.mYLoc		= registers [ SPRITE_Y_LOC ];
	
	sprite.mZRot		= registers [ SPRITE_ROT ];
	
	sprite.mXScl		= registers [ SPRITE_X_SCL ];
	sprite.mYScl		= registers [ SPRITE_Y_SCL ];
	
	float opacity		= registers [ SPRITE_OPACITY ];
	float glow			= 1.0f - registers [ SPRITE_GLOW ];
	
	sprite.mRed			= registers [ SPRITE_RED ] * opacity;
	sprite.mGreen		= registers [ SPRITE_GREEN ] * opacity;
	sprite.mBlue		= registers [ SPRITE_BLUE ] * opacity;
	sprite.mAlpha		= opacity * glow;
	
	sprite.mGfxID		= USFloat::ToInt ( registers [ SPRITE_IDX ]);
	
	system.PushSprite ( sprite );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "PARTICLE_X",			Pack64 ( MOAIParticle::PARTICLE_X, PARAM_TYPE_PARTICLE_REG ));
	state.SetField ( -1, "PARTICLE_Y",			Pack64 ( MOAIParticle::PARTICLE_Y, PARAM_TYPE_PARTICLE_REG ));
	state.SetField ( -1, "PARTICLE_DX",			Pack64 ( MOAIParticle::PARTICLE_DX, PARAM_TYPE_PARTICLE_REG ));
	state.SetField ( -1, "PARTICLE_DY",			Pack64 ( MOAIParticle::PARTICLE_DY, PARAM_TYPE_PARTICLE_REG ));

	state.SetField ( -1, "SPRITE_X_LOC",		Pack64 ( SPRITE_X_LOC, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_Y_LOC",		Pack64 ( SPRITE_Y_LOC, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_ROT",			Pack64 ( SPRITE_ROT, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_X_SCL",		Pack64 ( SPRITE_X_SCL, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_Y_SCL",		Pack64 ( SPRITE_Y_SCL, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_RED",			Pack64 ( SPRITE_RED, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_GREEN",		Pack64 ( SPRITE_GREEN, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_BLUE",			Pack64 ( SPRITE_BLUE, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_OPACITY",		Pack64 ( SPRITE_OPACITY, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_GLOW",			Pack64 ( SPRITE_GLOW, PARAM_TYPE_SPRITE_REG ));
	state.SetField ( -1, "SPRITE_IDX",			Pack64 ( SPRITE_IDX, PARAM_TYPE_SPRITE_REG ));
	
	luaL_Reg regTable [] = {
		{ "packConst",			_packConst },
		{ "packReg",			_packReg },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleScript::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "add",				_add },
		{ "angleVec",			_angleVec },
		{ "cos",				_cos },
		{ "cycle",				_cycle },
		{ "div",				_div },
		{ "ease",				_ease },
		{ "easeDelta",			_easeDelta },
		{ "mul",				_mul },
		{ "norm",				_norm },
		{ "rand",				_rand },
		{ "randVec",			_randVec },
		{ "set",				_set },
		{ "sin",				_sin },
		{ "sprite",				_sprite },
		{ "sub",				_sub },
		{ "tan",				_tan },
		{ "time",				_time },
		{ "vecAngle",			_vecAngle },
		{ "wrap",				_wrap },
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
void MOAIParticleScript::Run ( MOAIParticleSystem& system, MOAIParticle& particle, float t0, float t1 ) {

	u8* dst;
	u8* bytecode = this->mBytecode;
	if ( !bytecode ) return;
	
	float particleRegisters [ MAX_PARTICLE_REGISTERS ];
	memcpy ( particleRegisters, particle.mData, sizeof ( float ) * system.mParticleSize );
	
	float spriteRegisters [ TOTAL_SPRITE_REG ];
	
	float* r0;
	float* r1;
	float v0, v1, v2, v3;
	u32 i0;
	
	u8 type;
	u8 regIdx;

	bool push = false;
	
	for ( u8 opcode = *( bytecode++ ); opcode != END; opcode = *( bytecode++ )) {
		
		switch ( opcode ) {
			
			case ADD: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = v0 + v1;
				}
				break;
			
			case ANGLE_VEC: // RRV
				
				// TODO: should pass in a length as well
				READ_ADDR ( r0, bytecode );
				READ_ADDR ( r1, bytecode );
				READ_VALUE ( v0, bytecode );

				if( r0 && r1){
					*r0 = ( float )( Cos ( v0 * ( float )D2R ));
					*r1 = ( float )( Sin ( v0 * ( float )D2R ));
				}
				break;
			
			case COS: // RVV
				READ_ADDR   ( r0, bytecode );
				READ_VALUE  ( v0, bytecode );
				
				if ( r0 ) {
					*r0 = ( float )( cos ( v0 ) );
				}
				break;

			case CYCLE: // RVVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				READ_VALUE	( v2, bytecode );
				
				if ( r0 ) {
					v3 = v2 - v1;
					v3 = ( v3 < 0.0f ) ? -v3 : v3;
					
					float cycle = ( v0 - v1 ) / v3;
					int cycleIdx = USFloat::ToInt ( USFloat::Floor ( cycle ));
					float cycleDec = cycle - ( float )cycleIdx;
					
					if ( cycleIdx & 0x01 ) {
						cycleDec = 1.0f - cycleDec;
					}
					
					v0 = v1 + ( cycleDec * v3 );
					
					*r0 = v0;
				}
				break;
			
			case DIV: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = v0 / v1;
				}
				break;
			
			case EASE: // RVVI
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				READ_INT	( i0, bytecode );
				
				if ( r0 ) {
					*r0 = USInterpolate::Interpolate ( i0, v0, v1, t1 );
				}
				break;
			
			case EASE_DELTA: // RVVI
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				READ_INT	( i0, bytecode );
				
				if ( r0 ) {
					
					v2 = USInterpolate::Interpolate ( i0, v0, v1, t0 );
					v3 = USInterpolate::Interpolate ( i0, v0, v1, t1 );
					
					*r0 += ( v3 - v2 );
				}
				break;
			
			case MUL: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = v0 * v1;
				}
				break;
			
			case NORM:
				
				READ_ADDR ( r0, bytecode );
				READ_ADDR ( r1, bytecode );
				READ_VALUE ( v0, bytecode );
				READ_VALUE ( v1, bytecode );

				if ( r0 && r1 ) {
					v3 = Sqrt (( v0 * v0 ) + ( v1 * v1 ));
					if ( v3 ) {
						*r0 = ( float )( v0 / v3 );
						*r1 = ( float )( v1 / v3 );
					}
					else {
						*r0 = 0;
						*r1 = 0;
					}
				}
				break;
			
			case RAND: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = USFloat::Rand ( v0, v1 );
				}
				break;
				
			case RAND_VEC: // RRVV
				
				READ_ADDR	( r0, bytecode );
				READ_ADDR	( r1, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				v2 = USFloat::Rand ( 360.0f ) * ( float )D2R;
				v3 = USFloat::Rand ( v0,  v1 );
				
				if ( r0 ) {
					*r0 = Cos ( v2 ) * v3;
				}
				
				if ( r1 ) {
					*r1 = -Sin ( v2 ) * v3;
				}
				break;

			case SET: // RV
				
				READ_ADDR	( r0, bytecode )
				READ_VALUE	( v0, bytecode )
				
				if ( r0 ) {
					*r0 = v0;
				}
				break;
			case SIN: // RV

				READ_ADDR   ( r0, bytecode );
				READ_VALUE  ( v0, bytecode );
				
				if ( r0 ) {
					*r0 = ( float )( sin ( v0 ) );
				}
				break;

			case SPRITE: //
				
				if ( push ) {
					this->PushSprite ( system, spriteRegisters );
				}
				this->ResetRegisters ( spriteRegisters, particleRegisters );
				push = true;
				break;
			
			case SUB: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = v0 - v1;
				}
				break;

			case TAN: // RV

				READ_ADDR   ( r0, bytecode );
				READ_VALUE  ( v0, bytecode );
				
				if ( r0 ) {
					*r0 = ( float )( tan ( v0 ) );
				}
				break;

			case TIME: // RVV
				
				READ_ADDR	( r0, bytecode );
				
				if ( r0 ) {
					*r0 = t1;
				}
				break;
			
			case VEC_ANGLE: // RVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				
				if ( r0 ) {
					*r0 = ( float )( atan2 ( v0, v1 ) * R2D );
				}
				break;
			
			case WRAP: // RVVV
				
				READ_ADDR	( r0, bytecode );
				READ_VALUE	( v0, bytecode );
				READ_VALUE	( v1, bytecode );
				READ_VALUE	( v2, bytecode );
				
				if ( r0 ) {
					v3 = v2 - v1;
					
					while ( v0 < v1 ) {
						v0 += v3;
					}
					
					while ( v0 >= v2 ) {
						v0 -= v3;
					}
					
					*r0 = v0;
				}
				break;
		}
	}
	
	memcpy ( particle.mData, particleRegisters, sizeof ( float ) * system.mParticleSize );
	
	if ( push ) {
		this->PushSprite ( system, spriteRegisters );
	}
}
