// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAnimCurveIndex.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValueAtTime
	@text	Return the interpolated value given a point in time along the curve. This does not change
	        the curve's built in TIME attribute (it simply performs the requisite computation on demand).
 
	@in		MOAIAnimCurveIndex self
	@in		number time
	@out	number value	The interpolated value
*/
int MOAIAnimCurveIndex::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveIndex, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	ZLIndex value = self->GetValue ( span );
	
	state.Push ( MOAILuaIndex ( value ));
	state.Push ( MOAILuaIndex ( span.mKeyID ));
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setKey
	@text	Initialize a key frame at a given time with a give value. Also set the transition type between
			the specified key frame and the next key frame.
 
	@in		MOAIAnimCurveIndex self
	@in		number index			Index of the keyframe.
	@in		number time				Location of the key frame along the curve.
	@in		number value			Value of the curve at time.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@opt	number weight			Blends between chosen ease type (of any) and a linear transition. Defaults to 1.
	@out	nil
*/
int MOAIAnimCurveIndex::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveIndex, "UNN" );

	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, 0 ) ;
	float time			= state.GetValue < float >( 3, 0.0 );
	ZLIndex value		= state.GetValue < MOAILuaIndex >( 4, 0 );
	u32 mode			= state.GetValue < u32 >( 5, ZLInterpolate::kFlat );
	float weight		= state.GetValue < float >( 6, 1.0 );
	
	if ( MOAILogMgr::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
	
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveIndex
//================================================================//

//----------------------------------------------------------------//
ZLIndex MOAIAnimCurveIndex::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
ZLIndex MOAIAnimCurveIndex::GetValue ( const MOAIAnimKeySpan& span ) const {

	return this->mSamples [ span.mKeyID ];
}

//----------------------------------------------------------------//
MOAIAnimCurveIndex::MOAIAnimCurveIndex ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAINode ( context ),
	MOAIAnimCurve ( context ),
	mValue ( 0 ) {
	
	RTTI_BEGIN ( MOAIAnimCurveIndex )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAnimCurveIndex >)
		RTTI_EXTEND ( MOAIAnimCurve )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAnimCurveIndex::~MOAIAnimCurveIndex () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::SetSample ( ZLIndex idx, ZLIndex value ) {

	if ( this->mKeys.CheckIndex ( idx )) {
		this->mSamples [ idx ] = value;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAIAnimCurve_ApplyValueAttrOp ( ZLAttribute& attr, u32 op ) {

	this->mValue = attr.Apply ( this->mValue, op, ZLAttribute::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAIAnimCurve_GetDelta ( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	ZLIndex v0 = this->GetValue ( span0 );
	ZLIndex v1 = this->GetValue ( span1 );
	
	attr.SetValue ( v1 - v0 );
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAIAnimCurve_GetValue ( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const {

	attr.SetValue ( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAIAnimCurve_GetZero ( ZLAttribute& attr ) const {

	attr.SetValue ( 0 );
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAIAnimCurve_ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveIndex::MOAINode_Update () {

	this->mValue = this->GetValue ( this->mTime );
}

