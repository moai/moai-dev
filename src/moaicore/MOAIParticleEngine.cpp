// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIParticleEngine.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>

#define READ_BYTE(var,bytecode) \
	var = *( bytecode++ );

#define READ_FLOAT(var,bytecode) \
	*(( u32* )&var ) = *(( u32* )bytecode ); \
	bytecode += sizeof ( u32 );

//================================================================//
// MOAIParticleEngine
//================================================================//

//----------------------------------------------------------------//
void MOAIParticleEngine::RunBytecode ( MOAIParticleSystem& system, u8* bytecode, float registers []) {

	MOAIParticleEngine::RunBytecode ( system, bytecode, 0, 0.0f, registers );
}

//----------------------------------------------------------------//
void MOAIParticleEngine::RunBytecode ( MOAIParticleSystem& system, u8* bytecode, MOAIParticleState* state, float registers []) {

	MOAIParticleEngine::RunBytecode ( system, bytecode, state, 0.0f, registers );
}

//----------------------------------------------------------------//
void MOAIParticleEngine::RunBytecode ( MOAIParticleSystem& system, u8* bytecode, MOAIParticleState* state, float step, float registers []) {

	if ( !bytecode ) return;

	u8 n;
	u8 r0, r1, r2;
	float v0, v1, v2, v3;
	USColorVec color;

	bool pushSprite = false;
	MOAIParticleSprite sprite;
	MOAIParticleSprite resetSprite;

	resetSprite.mColor = 0xffffffff;
	resetSprite.mGfxID = 0;
	resetSprite.mLoc.mX = 0.0f;
	resetSprite.mLoc.mY = 0.0f;
	resetSprite.mRot = 0.0f;
	resetSprite.mScale.mX = 1.0f;
	resetSprite.mScale.mY = 1.0f;
	resetSprite.mGlow = 0.0f;

	USGLQuad quad;
	
	MOAIParticle& particle = *system.mParticle;

	u8 opcode = *( bytecode++ );
	for ( ; opcode != END; opcode = *( bytecode++ )) {
		
		n = *( bytecode++ );
		
		switch ( opcode ) {
			
			case ABS:
				
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:
						v0 = registers [ r0 ];
						registers [ r0++ ]	= v0 < 0.0f ? -v0 : v0;
					case 3:
						v0 = registers [ r0 ];
						registers [ r0++ ]	= v0 < 0.0f ? -v0 : v0;
					case 2:
						v0 = registers [ r0 ];
						registers [ r0++ ]	= v0 < 0.0f ? -v0 : v0;
					case 1:
						v0 = registers [ r0 ];
						registers [ r0 ]	= v0 < 0.0f ? -v0 : v0;
				}
				break;
			
			case ACC_ATTRACTOR: {
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				READ_BYTE ( r2, bytecode );
				
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				USVec2D vec;
				vec.mX = registers [ r1 ];
				vec.mY = registers [ r1 + 1 ];
				
				USVec2D loc;
				loc.mX = registers [ r2 ];
				loc.mY = registers [ r2 + 1 ];
				
				vec.Sub ( loc );
				
				float dist = vec.NormSafe () / v0;
				if ( dist <= 1.0f ) {
				
					dist = 1.0f - dist;
					dist = dist * dist * v1;
					
					registers [ r0++ ]	+= vec.mX * dist;
					registers [ r0 ]	+= vec.mY * dist;
				}
				break;
			}
			
			case ACC_FORCES:
				
				if ( state ) {
					
					READ_BYTE ( r0, bytecode );
					READ_BYTE ( r1, bytecode );
					
					USVec2D loc;
					loc.mX = registers [ r1 ];
					loc.mY = registers [ r1 + 1 ];
					
					USVec2D acceleration = state->GetAcceleration ( loc );
					
					registers [ r0 ]		+= acceleration.mX;
					registers [ r0 + 1 ]	+= acceleration.mY;
				}
				break;
			
			case ACC_LINEAR:
			
				READ_BYTE ( r0, bytecode );
				
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				registers [ r0++ ]	+= v0;
				registers [ r0 ]	+= v1;
				
				break;
			
			case ADD:
				
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	+= registers [ r1++ ];
					case 3:	registers [ r0++ ]	+= registers [ r1++ ];
					case 2:	registers [ r0++ ]	+= registers [ r1++ ];
					case 1:	registers [ r0 ]	+= registers [ r1 ];
				}
				break;
			
			case DAMP:
				
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				v0 = registers [ r1 ];
				
				switch ( n ) {
					case 4:	registers [ r0 ]	= USFloat::Damp ( registers [ r0 ], v0 ); r0++;
					case 3:	registers [ r0 ]	= USFloat::Damp ( registers [ r0 ], v0 ); r0++;
					case 2:	registers [ r0 ]	= USFloat::Damp ( registers [ r0 ], v0 ); r0++;
					case 1:	registers [ r0 ]	= USFloat::Damp ( registers [ r0 ], v0 );
				}
				break;
			
			case DIV:
				
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:
						v0 = registers [ r1++ ];
						registers [ r0 ]	= v0 == 0.0f ? 0.0f : registers [ r0 ] / v0;
						r0++;
					case 3:
						v0 = registers [ r1++ ];
						registers [ r0 ]	= v0 == 0.0f ? 0.0f : registers [ r0 ] / v0;
						r0++;
					case 2:
						v0 = registers [ r1++ ];
						registers [ r0 ]	= v0 == 0.0f ? 0.0f : registers [ r0 ] / v0;
						r0++;
					case 1:
						v0 = registers [ r1 ];
						registers [ r0 ]	= v0 == 0.0f ? 0.0f : registers [ r0 ] / v0;
				}
				break;
				
			case EASE:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0 ]	= USInterpolate::Curve ( r1, registers [ r0 ]); r0++;
					case 3:	registers [ r0 ]	= USInterpolate::Curve ( r1, registers [ r0 ]); r0++;
					case 2:	registers [ r0 ]	= USInterpolate::Curve ( r1, registers [ r0 ]); r0++;
					case 1:	registers [ r0 ]	= USInterpolate::Curve ( r1, registers [ r0 ]);
				}
				break;
			
			case INIT_LOC:
				break;
			
			case INIT_TIME:
				break;
			
			case INIT_VEC:
				break;
			
			case LOAD:
			
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:
						READ_FLOAT ( v0, bytecode );
						registers [ r0++ ] = v0;
					case 3:
						READ_FLOAT ( v0, bytecode );
						registers [ r0++ ] = v0;
					case 2:
						READ_FLOAT ( v0, bytecode );
						registers [ r0++ ] = v0;
					case 1:
						READ_FLOAT ( v0, bytecode );
						registers [ r0 ] = v0;
				}
				break;
			
			case LOAD_AGE:
			
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	= particle.mAge;
					case 3:	registers [ r0++ ]	= particle.mAge;
					case 2:	registers [ r0++ ]	= particle.mAge;
					case 1:	registers [ r0 ]	= particle.mAge;
				}
				break;
			
			case LOAD_DURATION:
				
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	= particle.mDuration;
					case 3:	registers [ r0++ ]	= particle.mDuration;
					case 2:	registers [ r0++ ]	= particle.mDuration;
					case 1:	registers [ r0 ]	= particle.mDuration;
				}
				break;
			
			case LOAD_STEP:
				
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	= step;
					case 3:	registers [ r0++ ]	= step;
					case 2:	registers [ r0++ ]	= step;
					case 1:	registers [ r0 ]	= step;
				}
				break;
			
			case LOAD_TIME:
				
				READ_BYTE ( r0, bytecode );
				
				v0 = particle.mDuration == 0.0f ? 0.0f : particle.mAge / particle.mDuration;
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	= v0;
					case 3:	registers [ r0++ ]	= v0;
					case 2:	registers [ r0++ ]	= v0;
					case 1:	registers [ r0 ]	= v0;
				}
				break;
			
			case MAC:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0 ]	+= registers [ r0 ] * registers [ r1++ ]; r0++;
					case 3:	registers [ r0 ]	+= registers [ r0 ] * registers [ r1++ ]; r0++;
					case 2:	registers [ r0 ]	+= registers [ r0 ] * registers [ r1++ ]; r0++;
					case 1:	registers [ r0 ]	+= registers [ r0 ] * registers [ r1 ];
				}
				break;
			
			case MOV:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0 ]	= registers [ r1++ ]; r0++;
					case 3:	registers [ r0 ]	= registers [ r1++ ]; r0++;
					case 2:	registers [ r0 ]	= registers [ r1++ ]; r0++;
					case 1:	registers [ r0 ]	= registers [ r1 ];
				}
				break;
			
			case MUL:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0 ]	*= registers [ r1++ ]; r0++;
					case 3:	registers [ r0 ]	*= registers [ r1++ ]; r0++;
					case 2:	registers [ r0 ]	*= registers [ r1++ ]; r0++;
					case 1:	registers [ r0 ]	*= registers [ r1 ];
				}
				break;
			
			case RAND:
			
				READ_BYTE ( r0, bytecode );
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	= USFloat::Rand ( v0, v1 );
					case 3:	registers [ r0++ ]	= USFloat::Rand ( v0, v1 );
					case 2:	registers [ r0++ ]	= USFloat::Rand ( v0, v1 );
					case 1:	registers [ r0 ]	= USFloat::Rand ( v0, v1 );
				}
				break;
			
			case SCALE:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				v0 = registers [ r1 ];
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	*= v0;
					case 3:	registers [ r0++ ]	*= v0;
					case 2:	registers [ r0++ ]	*= v0;
					case 1:	registers [ r0 ]	*= v0;
				}
				break;
			
			case SCALE_ADD:
				
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				v0 = registers [ r1 ];
				
				switch ( n ) {
					case 4:	registers [ r0 ]	+= registers [ r0 ] * v0; r0++;
					case 3:	registers [ r0 ]	+= registers [ r0 ] * v0; r0++;
					case 2:	registers [ r0 ]	+= registers [ r0 ] * v0; r0++;
					case 1:	registers [ r0 ]	+= registers [ r0 ] * v0;
				}
				break;
			
			case SET_DURATION:
				
				READ_BYTE ( r0, bytecode );
				
				particle.mDuration = registers [ r0 ];
				break;
			
			case SPRITE:
			
				if ( pushSprite ) {
					system.PushSprite ( sprite );
				}
				
				sprite = resetSprite;
				pushSprite = true;
				break;
			
			case SPRITE_ALPHA:
			
				READ_BYTE ( r0, bytecode );
				
				r1 = ( u8 )( registers [ r0 ] * 255.0f );
				
				sprite.mColor =	((((( sprite.mColor >> 0x00 ) & 0xFF ) * ( u32 )r1 ) >> 0x08 ) << 0x00 ) +
								((((( sprite.mColor >> 0x08 ) & 0xFF ) * ( u32 )r1 ) >> 0x08 ) << 0x08 ) +
								((((( sprite.mColor >> 0x10 ) & 0xFF ) * ( u32 )r1 ) >> 0x08 ) << 0x10 ) +
								(( u32 )r1 << 0x18 );
				break;
			
			case SPRITE_COLOR:
			
				READ_BYTE ( r0, bytecode );
				
				v0 = registers [ r0++ ];
				v1 = registers [ r0++ ];
				v2 = registers [ r0++ ];
				v3 = registers [ r0 ];
				
				sprite.mColor = USColor::PackRGBA ( v0, v1, v2, v3 );
				
				break;

			case SPRITE_GLOW:
				
				READ_BYTE ( r0, bytecode );
				sprite.mGlow = registers [ r0 ];
				break;
			
			case SPRITE_IDX:
			
				READ_BYTE ( r0, bytecode );
				sprite.mGfxID = ( u32 )registers [ r0 ];
				break;
			
			case SPRITE_LOC:
			
				READ_BYTE ( r0, bytecode );
				sprite.mLoc.mX = registers [ r0++ ];
				sprite.mLoc.mY = registers [ r0 ];
				break;
			
			case SPRITE_ROT:
			
				READ_BYTE ( r0, bytecode );
				sprite.mRot = registers [ r0 ];
				break;
			
			case SPRITE_SCALE:
			
				READ_BYTE ( r0, bytecode );
				sprite.mScale.mX = registers [ r0++ ];
				sprite.mScale.mY = registers [ r0 ];
				break;
			
			case SPRITE_SIZE:
			
				READ_BYTE ( r0, bytecode );
				sprite.mScale.mX = registers [ r0 ];
				sprite.mScale.mY = registers [ r0 ];
				break;
			
			case STEP_EULER: {
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				READ_BYTE ( r2, bytecode );
				
				USVec2D loc;
				loc.mX = registers [ r0 ];
				loc.mY = registers [ r0 + 1 ];
				
				registers [ r0 ]		+= registers [ r1 ] * step;
				registers [ r0 + 1 ]	+= registers [ r1 + 1 ] * step;
				
				registers [ r1 ]		+= registers [ r2 ] * step;
				registers [ r1 + 1 ]	+= registers [ r2 + 1 ] * step;
				
				break;
			}
			case SUB:
			
				READ_BYTE ( r0, bytecode );
				READ_BYTE ( r1, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0++ ]	-= registers [ r1++ ];
					case 3:	registers [ r0++ ]	-= registers [ r1++ ];
					case 2:	registers [ r0++ ]	-= registers [ r1++ ];
					case 1:	registers [ r0 ]	-= registers [ r1 ];
				}
				break;
			
			case SUB_FROM_ONE:
			
				READ_BYTE ( r0, bytecode );
				
				switch ( n ) {
					case 4:	registers [ r0 ]	= 1.0f - registers [ r0 ]; r0++;
					case 3:	registers [ r0 ]	= 1.0f - registers [ r0 ]; r0++;
					case 2:	registers [ r0 ]	= 1.0f - registers [ r0 ]; r0++;
					case 1:	registers [ r0 ]	= 1.0f - registers [ r0 ];
				}
				break;
			
			case VEC2_RAND:
			
				READ_BYTE ( r0, bytecode );
				READ_FLOAT ( v0, bytecode );
				READ_FLOAT ( v1, bytecode );
				
				v2 = USFloat::Rand ( 360.0f ) * ( float )D2R;
				v3 = USFloat::Rand ( v0, v1 );
				
				registers [ r0++ ] = Cos ( v2 ) * v3;
				registers [ r0 ] = Sin ( v2 ) * v3;
				break;

			//case UNIT_BOUNCE:
			//	READ_BYTE ( r0, bytecode );
			//	READ_BYTE ( r1, bytecode );
			//	registers [ r0 ] = USFloat::Abs ( Sin ( registers [ r0 ] * ( float )PI ));
			//	break;
			//
			//case UNIT_WAVE:
			//	READ_BYTE ( r0, bytecode );
			//	READ_BYTE ( r1, bytecode );
			//	READ_FLOAT ( v0, bytecode );
			//	READ_FLOAT ( v1, bytecode );
			//	registers [ r0 ] = Sin (( registers [ r1 ] * v0 ) * ( float )TWOPI ) * v1;
			//	break;
			//
			//case UNPACK_COLOR:
			//	READ_BYTE ( r0, bytecode );
			//	READ_BYTE ( r1, bytecode );
			//	
			//	color.SetRGBA (( u32& )registers [ r1 ]);
			//	
			//	cursor = &registers [ r0 ];
			//	
			//	cursor [ 0 ] = color.mR;
			//	cursor [ 1 ] = color.mG;
			//	cursor [ 2 ] = color.mB;
			//	cursor [ 3 ] = color.mA;
			//	
			//	break;
		}
	}
	
	if ( pushSprite ) {
		system.PushSprite ( sprite );
	}
}
