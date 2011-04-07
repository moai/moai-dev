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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UNN" )

	float min = state.GetValue < float >( 2, 0.0f );
	float max = state.GetValue < float >( 3, 360.0f );

	self->SetAngle ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setEmission ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )

	u32 min = state.GetValue < u32 >( 2, 1 );
	u32 max = state.GetValue < u32 >( 3, min );

	self->SetEmission ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )

	float min = state.GetValue < float >( 2, 1.0f );
	float max = state.GetValue < float >( 3, min );

	self->SetFrequency ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setMagnitude ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )

	float min = state.GetValue < float >( 2, 1.0f );
	float max = state.GetValue < float >( 3, min );

	self->SetMagnitude ( min, max );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setRadius ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UN" )
	
	self->mShapeID	= CIRCLE;

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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleEmitter::_setSystem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UU" )

	MOAIParticleSystem* system = state.GetLuaObject < MOAIParticleSystem >( 2 );
	self->SetSystem ( system );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
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
u32 MOAIParticleEmitter::GetEmission () {

	return ( u32 )USFloat::Rand (( float )this->mMinEmission, ( float )this->mMaxEmission );
}

//----------------------------------------------------------------//
float MOAIParticleEmitter::GetFrequency () {

	return 1.0f / USFloat::Rand ( this->mMinFrequency, this->mMaxFrequency );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::GetParticle ( USVec2D& loc, USVec2D& vec ) {

	USAffine2D localToWorld = this->GetLocalToWorldMtx ();
	
	switch ( this->mShapeID ) {
	
		case CIRCLE:
			
			loc = this->GetRandomVec ( 0.0f, 360.0f, this->mInnerRadius, this->mOuterRadius );
			localToWorld.Transform ( loc );
			break;

		case POINT:
			
			loc = localToWorld.GetTranslation ();
			break;

		case RECT:
		
			loc.mX = USFloat::Rand ( this->mRect.mXMin, this->mRect.mXMax );
			loc.mY = USFloat::Rand ( this->mRect.mYMin, this->mRect.mYMax );
			localToWorld.Transform ( loc );
		
			break;
	}
	
	vec = this->GetRandomVec ( this->mMinAngle, this->mMaxAngle, this->mMinMagnitude, this->mMaxMagnitude );
	localToWorld.TransformVec ( vec );
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
	mTime ( 0.0f ),
	mEmitTime ( 0.0f ),
	mMinFrequency ( 1.0f ),
	mMaxFrequency ( 1.0f ),
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
void MOAIParticleEmitter::OnUpdate ( float step ) {

	this->mTime += step;
	if ( !this->mSystem ) return;
	if ( this->mTime < this->mEmitTime ) return;
	
	u32 emission = this->GetEmission ();
	this->Surge ( emission );
	
	this->mEmitTime = this->mTime + this->GetFrequency ();
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
		{ "setFrequency",		_setFrequency },
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
void MOAIParticleEmitter::SetAngle ( float min, float max ) {

	this->mMinAngle = min;
	this->mMaxAngle = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetEmission ( u32 min, u32 max ) {

	this->mMinEmission = min;
	this->mMaxEmission = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetFrequency ( float min, float max ) {

	this->mMinFrequency = min;
	this->mMaxFrequency = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::SetMagnitude ( float min, float max ) {

	this->mMinMagnitude = min;
	this->mMaxMagnitude = max;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::Surge ( u32 total ) {

	USVec2D loc;
	USVec2D vec;
	for ( u32 i = 0; i < total; ++i ) {
		this->GetParticle ( loc, vec );
		this->mSystem->PushParticle ( loc.mX, loc.mY, vec.mX, vec.mY );
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
	PRETTY_PRINT ( repr, mTime )
	PRETTY_PRINT ( repr, mEmitTime )
	PRETTY_PRINT ( repr, mMinFrequency )
	PRETTY_PRINT ( repr, mMaxFrequency )
	PRETTY_PRINT ( repr, mMinEmission )
	PRETTY_PRINT ( repr, mMaxEmission )
	PRETTY_PRINT ( repr, mMinAngle )
	PRETTY_PRINT ( repr, mMaxAngle )
	PRETTY_PRINT ( repr, mMinMagnitude )
	PRETTY_PRINT ( repr, mMaxMagnitude )
	PRETTY_PRINT ( repr, mSystem )

	return repr;
}
