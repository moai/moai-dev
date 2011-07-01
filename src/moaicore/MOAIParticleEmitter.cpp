// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticleEmitter.h>
#include <moaicore/MOAIParticleSystem.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setAngle
	@text	Set the size and angle of the emitter.
	
	@in		MOAIParticleEmitter self
	@in		number min Minimum angle in degrees.
	@in		number max Maximum angle in degrees.
	@out	nil
*/
int MOAIParticleEmitter::_setAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UNN" )

	float min = state.GetValue < float >( 2, 0.0f );
	float max = state.GetValue < float >( 3, 360.0f );

	self->SetAngleRange ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setEmission
	@text	Set the size of each emission.
	
	@in		MOAIParticleEmitter self
	@in		number min Minimum emission size.
	@opt	number max Maximum emission size. Defaults to min.
	@out	nil
*/
int MOAIParticleEmitter::_setEmission ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )

	u32 min = state.GetValue < u32 >( 2, 1 );
	u32 max = state.GetValue < u32 >( 3, min );

	self->SetEmissionRange ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMagnitude
	@text	Set the starting magnitude of particles deltas.
	
	@in		MOAIParticleEmitter self
	@in		number min Minimum magnitude.
	@opt	number max Maximum magnitude. Defaults to min.
	@out	nil
*/
int MOAIParticleEmitter::_setMagnitude ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )

	float min = state.GetValue < float >( 2, 1.0f );
	float max = state.GetValue < float >( 3, min );

	self->SetMagnitudeRange ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRadius
	@text	Set the shape and radius of the emitter.
	
	@overload
	
		@in		MOAIParticleEmitter self
		@in		number radius
		@out	nil
	
	@overload
	
		@in		MOAIParticleEmitter self
		@in		number innerRadius
		@in		number outerRadius
		@out	nil
*/
int MOAIParticleEmitter::_setRadius ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )
	
	self->mShapeID = CIRCLE;

	if ( state.GetTop () >= 3 ) {
		self->mInnerRadius = state.GetValue < float >( 2, 0.0f );
		self->mOuterRadius = state.GetValue < float >( 3, 0.0f );
		
	}
	else {
		self->mInnerRadius = 0.0f;
		self->mOuterRadius = state.GetValue < float >( 2, 0.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set the shape and dimensions of the emitter.
	
	@in		MOAIParticleEmitter self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIParticleEmitter::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UNNNN" )

	USRect rect;
	rect.mXMin = state.GetValue < float >( 2, 0.0f );
	rect.mYMin = state.GetValue < float >( 3, 0.0f );
	rect.mXMax = state.GetValue < float >( 4, 0.0f );
	rect.mYMax = state.GetValue < float >( 5, 0.0f );
	
	rect.Bless ();
	
	self->SetRect ( rect );
	self->SetShapeID ( RECT );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSystem
	@text	Attaches the emitter to a particle system.
	
	@in		MOAIParticleEmitter self
	@in		MOAIParticleSystem system
	@out	nil
*/
int MOAIParticleEmitter::_setSystem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UU" )

	MOAIParticleSystem* system = state.GetLuaObject < MOAIParticleSystem >( 2 );
	self->SetSystem ( system );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	surge
	@text	Forces the emission of one or more particles.
	
	@in		MOAIParticleEmitter self
	@opt	number total Size of sure. Default value is 1.
	@out	nil
*/
int MOAIParticleEmitter::_surge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "U" )

	u32 total = state.GetValue < u32 >( 2, 1 );

	self->Surge ( total );
	return 0;
}

//================================================================//
// MOAIParticleEmitter
//================================================================//

//----------------------------------------------------------------//
u32 MOAIParticleEmitter::GetRandomEmission () {

	return ( u32 )USFloat::Rand (( float )this->mMinEmission, ( float )this->mMaxEmission );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::GetRandomParticle ( USVec2D& loc, USVec2D& vec ) {
	
	switch ( this->mShapeID ) {
	
		case CIRCLE:
			
			loc = this->GetRandomVec ( 0.0f, 360.0f, this->mInnerRadius, this->mOuterRadius );
			break;

		case POINT:
			
			loc.mX = 0.0f;
			loc.mY = 0.0f;
			break;

		case RECT:
		
			loc.mX = USFloat::Rand ( this->mRect.mXMin, this->mRect.mXMax );
			loc.mY = USFloat::Rand ( this->mRect.mYMin, this->mRect.mYMax );
			break;
	}
	
	vec = this->GetRandomVec ( this->mMinAngle, this->mMaxAngle, this->mMinMagnitude, this->mMaxMagnitude );
}

//----------------------------------------------------------------//
USVec2D MOAIParticleEmitter::GetRandomVec ( float minAngle, float maxAngle, float min, float max ) {

	float r = USFloat::Rand ( minAngle, maxAngle ) * ( float )D2R;
	float m = USFloat::Rand ( min, max );
	
	USVec2D vec;
	vec.mX = Cos ( r ) * m;
	vec.mY = Sin ( r ) * m;
	
	return vec;
}

//----------------------------------------------------------------//
bool MOAIParticleEmitter::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAIParticleEmitter::MOAIParticleEmitter () :
	mShapeID ( POINT ),
	mInnerRadius ( 0.0f ),
	mOuterRadius ( 0.0f ),
	mMinEmission ( 1 ),
	mMaxEmission ( 1 ),
	mMinAngle ( 0.0f ),
	mMaxAngle ( 360.0f ),
	mMinMagnitude ( 0.0f ),
	mMaxMagnitude ( 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleEmitter::~MOAIParticleEmitter () {
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::RegisterLuaClass ( USLuaState& state ) {

	this->MOAITransform::RegisterLuaClass ( state );
	this->MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::RegisterLuaFuncs ( USLuaState& state ) {
	
	this->MOAITransform::RegisterLuaFuncs ( state );
	this->MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setAngle",			_setAngle },
		{ "setEmission",		_setEmission },
		{ "setMagnitude",		_setMagnitude },
		{ "setRadius",			_setRadius },
		{ "setRect",			_setRect },
		{ "setSystem",			_setSystem },
		{ "surge",				_surge },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetAngleRange ( float min, float max ) {

	this->mMinAngle = min;
	this->mMaxAngle = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetEmissionRange ( u32 min, u32 max ) {

	this->mMinEmission = min;
	this->mMaxEmission = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetMagnitudeRange ( float min, float max ) {

	this->mMinMagnitude = min;
	this->mMaxMagnitude = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::Surge ( u32 total ) {

	if ( this->mSystem ) {

		USVec2D loc;
		USVec2D vec;
		for ( u32 i = 0; i < total; ++i ) {
			this->GetRandomParticle ( loc, vec );
			this->mLocalToWorldMtx.Transform ( loc );
			this->mLocalToWorldMtx.TransformVec ( vec );
			this->mSystem->PushParticle ( loc.mX, loc.mY, vec.mX, vec.mY );
		}
	}
}

//----------------------------------------------------------------//
STLString MOAIParticleEmitter::ToString () {

	STLString repr;

	const char *shape_id;

	switch ( mShapeID ) {
		case CIRCLE:
			shape_id = "CIRCLE";
			break;
		case RECT:
			shape_id = "RECT";
			break;
		default:
			shape_id = "INVALID";
	}

	PrettyPrint ( repr, "mShapeID", shape_id );

	return repr;
}
