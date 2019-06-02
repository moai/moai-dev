// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIParticleEmitter.h>
#include <moai-sim/MOAIParticleSystem.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setAngle
	@text	Set the size and angle of the emitter.
	
	@in		MOAIParticleEmitter self
	@in		number min					Minimum angle in degrees.
	@in		number max					Maximum angle in degrees.
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
/**	@lua	setEmission
	@text	Set the size of each emission.
	
	@in		MOAIParticleEmitter self
	@in		number min					Minimum emission size.
	@opt	number max					Maximum emission size. Defaults to min.
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
/**	@lua	setMagnitude
	@text	Set the starting magnitude of particles deltas.
	
	@in		MOAIParticleEmitter self
	@in		number min					Minimum magnitude.
	@opt	number max					Maximum magnitude. Defaults to min.
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
// TODO: doxygen
int MOAIParticleEmitter::_setMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "U" )
	
	self->mMaskProp.Set ( *self, state.GetLuaObject < MOAIPartitionHull >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRadius
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
/**	@lua	setRect
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

	ZLRect rect;
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
/**	@lua	setState
	@text	Set initial state index for new particles.
	
	@in		MOAIParticleEmitter self
	@in		number	state	index of MOAIParticleState in attached MOAIParticleSystem
	@out	nil
*/
int MOAIParticleEmitter::_setState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "U" )
	
	self->mParticleState = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSystem
	@text	Attaches the emitter to a particle system.
	
	@in		MOAIParticleEmitter self
	@in		MOAIParticleSystem system
	@out	nil
*/
int MOAIParticleEmitter::_setSystem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "UU" )

	self->mSystem.Set ( *self, state.GetLuaObject < MOAIParticleSystem >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	surge
	@text	Forces the emission of one or more particles.
	
	@in		MOAIParticleEmitter self
	@opt	number total				Size of surge. Default value is a random emission value for emitter.
	@out	nil
*/
int MOAIParticleEmitter::_surge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleEmitter, "U" )

	u32 total = state.GetValue < u32 >( 2, self->GetRandomEmission ());

	self->Surge ( total );
	return 0;
}

//================================================================//
// MOAIParticleEmitter
//================================================================//

//----------------------------------------------------------------//
u32 MOAIParticleEmitter::GetRandomEmission () {

	return ( u32 )ZLFloat::Rand (( float )this->mMinEmission, ( float )this->mMaxEmission );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::GetRandomParticle ( ZLVec3D& loc, ZLVec3D& vec ) {
	
	switch ( this->mShapeID ) {
	
		case CIRCLE:
			
			loc = this->GetRandomVec ( 0.0f, 360.0f, this->mInnerRadius, this->mOuterRadius );
			break;

		default:
		case POINT:
			
			loc.mX = 0.0f;
			loc.mY = 0.0f;
			loc.mZ = 0.0f;
			break;

		case RECT:
		
			loc.mX = ZLFloat::Rand ( this->mRect.mXMin, this->mRect.mXMax );
			loc.mY = ZLFloat::Rand ( this->mRect.mYMin, this->mRect.mYMax );
			loc.mZ = 0.0f;
			break;
	}
	
	vec = this->GetRandomVec ( this->mMinAngle, this->mMaxAngle, this->mMinMagnitude, this->mMaxMagnitude );
}

//----------------------------------------------------------------//
ZLVec3D MOAIParticleEmitter::GetRandomVec ( float minAngle, float maxAngle, float min, float max ) {

	float r = ZLFloat::Rand ( minAngle, maxAngle ) * ( float )D2R;
	float m = ZLFloat::Rand ( min, max );
	
	ZLVec3D vec;
	vec.mX = Cos ( r ) * m;
	vec.mY = Sin ( r ) * m;
	vec.mZ = 0.0f;
	
	return vec;
}

//----------------------------------------------------------------//
bool MOAIParticleEmitter::MaskParticle ( const ZLVec3D& loc ) {

	return this->mMaskProp ? this->mMaskProp->Inside ( loc, 0.0f ) : true;
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
	mMaxMagnitude ( 1.0f ),
	mEmission ( 0 ),
	mParticleState ( ZLIndexOp::ZERO ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleEmitter::~MOAIParticleEmitter () {

	this->mSystem.Set ( *this, 0 );
	this->mMaskProp.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "setAngle",			_setAngle },
		{ "setEmission",		_setEmission },
		{ "setMagnitude",		_setMagnitude },
		{ "setMask",			_setMask },
		{ "setRadius",			_setRadius },
		{ "setRect",			_setRect },
		{ "setState",			_setState },
		{ "setSystem",			_setSystem },
		{ "surge",				_surge },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_SerializeOut ( composer, state, serializer ));
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
	
	this->mEmission += total;
	this->ScheduleUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIParticleEmitter::MOAIAction_IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAIParticleEmitter::MOAINode_Update () {

	MOAITransform::MOAINode_Update ();
	
	if ( this->mSystem ) {

		ZLVec3D loc;
		ZLVec3D vec;
		for ( u32 i = 0; i < this->mEmission; ++i ) {
			this->GetRandomParticle ( loc, vec );
			this->mLocalToWorldMtx.Transform ( loc );
			
			if ( this->MaskParticle ( loc )) {
			
				this->mLocalToWorldMtx.TransformVec ( vec );
				this->mSystem->PushParticle ( loc.mX, loc.mY, vec.mX, vec.mY, this->mParticleState );
			}
		}
	}
	
	this->mEmission = 0;
}
