// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimMixer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_apply ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_buildChannels ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	self->BuildChannels ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_insertClip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_insertCycle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_popClip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_popCycle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_pushClip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	MOAIAnimLoop* loop		= state.GetLuaObject < MOAIAnimLoop >( 2, true );
	
	if ( loop ) {
	
		float start				= state.GetValue < float >( 3, 0.0f );
		float finish			= state.GetValue < float >( 4, loop->GetLength ());
		
		u32 clipID = self->PushClip ( *loop, start, finish );
		state.Push ( clipID );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_pushCycle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_remove ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	u32 identifier = state.GetValue < u32 >( 2, 0xffffffff );
	self->Remove ( identifier );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_reserveChannels ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	self->ReserveChannels( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_setChannelLink( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	MOAINode* target = state.GetLuaObject < MOAINode >( 3, true );
	if ( !target ) return 0;
	
	u32 channelID				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 attrID					= state.GetValue < u32 >( 4, 0 );
	//bool relative				= state.GetValue < bool >( 5, false );
	
	self->SetChannelLink ( channelID, target, attrID );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_setChannelType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "U" );

	u32 channelID	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 type		= state.GetValue < u32 >( 3, MOAIAnimChannel::CHANNEL_TYPE_UNKNOWN );

	self->SetChannelType ( channelID, type );

	return 0;
}

//----------------------------------------------------------------//
int MOAIAnimMixer::_setSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "UNN" );
	
	u32 identifier			= state.GetValue < u32 >( 2, 0xffffffff );
	float value				= state.GetValue < float >( 3, 0.0f );
	
	self->SetAttribute ( identifier, MOAIAnimTrack::ANIM_TRACK_SPEED, value );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAnimMixer::_setWeight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "UNN" );
	
	u32 identifier			= state.GetValue < u32 >( 2, 0xffffffff );
	float value				= state.GetValue < float >( 3, 0.0f );
	
	self->SetAttribute ( identifier, MOAIAnimTrack::ANIM_TRACK_WEIGHT, value );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimMixer::_transition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimMixer, "UNNNN" );

	u32 identifier			= state.GetValue < u32 >( 2, 0xffffffff );
	u32 attrID				= state.GetValue < u32 >( 3, 0xffffffff );
	float value				= state.GetValue < float >( 4, 0.0f );
	float duration			= state.GetValue < float >( 5, 0.0f );
	u32 curve				= state.GetValue < u32 >( 6, ZLInterpolate::kLinear );
	bool popOnEnd			= state.GetValue < bool >( 7, false );

	self->Transition ( identifier, attrID, value, duration, curve, popOnEnd );

	return 0;
}

//================================================================//
// MOAIAnimMixer
//================================================================//

//----------------------------------------------------------------//
MOAIAnimClip& MOAIAnimMixer::AffirmClip ( MOAIAnimLoop& loop, float clipStart, float clipEnd ) {

	MOAIAnimClip* clip = 0;
	if ( this->mClipPool.Count ()) {
		clip = *this->mClipPool.PopFront ();
	}
	else {
		clip = new MOAIAnimClip ();
		clip->mLink.Data ( clip );
		clip->mIdentifier = this->mClipsByIndex.Size ();
		this->mClipsByIndex.Grow ( clip->mIdentifier + 1 );
		this->mClipsByIndex [ clip->mIdentifier ] = clip;
	}
	
	clip->mActive		= true;
	clip->mLoop			= &loop;
	clip->mClipStart	= clipStart;
	clip->mClipEnd		= clipEnd;
	clip->mClipLength	= clipEnd - clipStart;
	clip->mWeight		= 1.0f;
	clip->mSpeed		= 1.0f;
	
	return *clip;
}

//----------------------------------------------------------------//
MOAIAnimCycle& MOAIAnimMixer::AffirmCycle () {

	MOAIAnimCycle* cycle = 0;
	if ( this->mCyclePool.Count ()) {
		cycle = *this->mCyclePool.PopFront ();
	}
	else {
		cycle = new MOAIAnimCycle ();
		cycle->mLink.Data ( cycle );
		
		size_t index = this->mCyclesByIndex.Size ();
		
		cycle->mIdentifier = index | CYCLE_FLAG;
		this->mCyclesByIndex.Grow ( index + 1 );
		this->mCyclesByIndex [ index ] = cycle;
	}
	
	cycle->mActive = true;
	cycle->mNormalizedTime	= 0.0f;
	cycle->mWeight			= 1.0f;
	cycle->mSpeed			= 1.0f;
	
	return *cycle;
}

//----------------------------------------------------------------//
MOAIAnimTransition& MOAIAnimMixer::AffirmTransition () {

	MOAIAnimTransition* transition = 0;
	if ( this->mTransitionPool.Count ()) {
		transition = *this->mTransitionPool.PopFront ();
	}
	else {
		transition = new MOAIAnimTransition ();
		transition->mLink.Data ( transition );
	}
	return *transition;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::Apply ( float t ) {
	
	for ( ZLLeanLink < MOAIAnimCycle* >* cycleIt = this->mStack.Head (); cycleIt; cycleIt = cycleIt->Next ()) {
		MOAIAnimCycle& cycle = **cycleIt;
		
		for ( ZLLeanLink < MOAIAnimClip* >* clipIt = cycle.mStack.Head (); clipIt; clipIt = clipIt->Next ()) {
			MOAIAnimClip& clip = **clipIt;
			
			if ( clip.mWeight > 0.0f ) {
				assert ( clip.mLoop );
				clip.mLoop->Apply ( *this, clip.mClipStart + ( cycle.mNormalizedTime * clip.mClipLength ), clip.mWeight );
			}
		}
	}
	
	MOAIAttribute attr;
	
	size_t nChannels = this->mChannels.Size ();
	for ( size_t i = 0; i < nChannels; ++i ) {
	
		MOAIAnimChannel& channel	= this->mChannels [ i ];
		MOAIAnimChannelLink& link	= this->mLinks [ i ];
		MOAINode* target = link.mTarget;
	
		if ( target ) {
			if( channel.GetValue ( attr )) {
				target->ApplyAttrOp ( link.mAttrID, attr, MOAIAttribute::SET );
				target->ScheduleUpdate ();
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::BuildChannels () {

	size_t nChannels = this->mChannels.Size ();
	size_t bufferSize = 0;
	
	for ( size_t i = 0; i < nChannels; ++i ) {
	
		MOAIAnimChannel& channel = this->mChannels [ i ];
		
		size_t channelSize = channel.GetSize ();
		
		if ( channelSize > 0 ) {
			channel.mBuffer = ( void* )bufferSize;
			bufferSize += channel.GetSize ();
		}
		else {
			channel.mType = MOAIAnimChannel::CHANNEL_TYPE_UNKNOWN;
			channel.mBuffer = 0;
		}
	}
	
	this->mChannelBuffer.Init ( bufferSize );
	void* buffer = this->mChannelBuffer.Data ();
	
	for ( size_t i = 0; i < nChannels; ++i ) {
		MOAIAnimChannel& channel = this->mChannels [ i ];
		channel.mBuffer = channel.mType == MOAIAnimChannel::CHANNEL_TYPE_UNKNOWN ? 0 : ( void* )(( uintptr )buffer + ( uintptr )channel.mBuffer );
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::DeactivateClip ( MOAIAnimClip& clip ) {

	this->mClipPool.PushBack ( clip.mLink );
	clip.mLoop			= 0;
	clip.mActive		= false;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::DeactivateCycle ( MOAIAnimCycle& cycle ) {

	while ( cycle.mStack.Count ()) {
		MOAIAnimClip* clip = *cycle.mStack.PopFront ();
		this->DeactivateClip ( *clip );
	}
	this->mCyclePool.PushBack ( cycle.mLink );
	cycle.mActive = false;
}

//----------------------------------------------------------------//
MOAIAnimChannel* MOAIAnimMixer::GetChannel ( u32 channelID ) {

	return channelID < this->mChannels.Size () ? &this->mChannels [ channelID ] : 0;
}

//----------------------------------------------------------------//
MOAIAnimClip* MOAIAnimMixer::GetClip ( u32 identifier ) {

	if ( identifier & CYCLE_FLAG ) return 0;

	u32 index = identifier & INDEX_MASK;
	return index < this->mClipsByIndex.Size () ? this->mClipsByIndex [ index ] : 0;
}

//----------------------------------------------------------------//
MOAIAnimCycle* MOAIAnimMixer::GetCycle ( u32 identifier ) {

	if ( !( identifier & CYCLE_FLAG )) return 0;

	u32 index = identifier & INDEX_MASK;
	return index < this->mCyclesByIndex.Size () ? this->mCyclesByIndex [ index ] : 0;
}

//----------------------------------------------------------------//
MOAIAnimTrack* MOAIAnimMixer::GetTrack ( u32 identifier ) {

	u32 index = identifier & INDEX_MASK;

	if ( identifier & CYCLE_FLAG ) {
		if ( index < this->mCyclesByIndex.Size ()) {
			return this->mCyclesByIndex [ index ];
		}
	}
	else {
		if ( index < this->mClipsByIndex.Size ()) {
			return this->mClipsByIndex [ index ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
ZLResult < u32 > MOAIAnimMixer::InsertClip ( u32 identifier, MOAIAnimLoop& loop, float clipStart, float clipEnd ) {

	MOAIAnimClip* clip = this->GetClip ( identifier );

	if ( clip && clip->mActive ) {
		clip->mLink.List ()->InsertAfter ( clip->mLink, this->AffirmClip ( loop, clipStart, clipEnd ).mLink );
	}
	return ZLError ();
}

//----------------------------------------------------------------//
ZLResult < u32 > MOAIAnimMixer::InsertCycle ( u32 identifier ) {

	MOAIAnimCycle* cycle = this->GetCycle ( identifier );

	if ( cycle && cycle->mActive ) {
		cycle->mLink.List ()->InsertAfter ( cycle->mLink, this->AffirmCycle ().mLink );
	}
	return ZLError ();
}

//----------------------------------------------------------------//
MOAIAnimMixer::MOAIAnimMixer () :
	mTime ( 0.0f ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIAnimMixer::~MOAIAnimMixer () {

	for ( size_t i = 0; i < this->mCyclesByIndex.Size (); ++i ) {
		if ( this->mCyclesByIndex [ i ]) {
			delete this->mCyclesByIndex [ i ];
		}
	}
	
	for ( size_t i = 0; i < this->mClipsByIndex.Size (); ++i ) {
		if ( this->mClipsByIndex [ i ]) {
			delete this->mClipsByIndex [ i ];
		}
	}
	
	for ( size_t i = 0; i < this->mClipsByIndex.Size (); ++i ) {
		if ( this->mClipsByIndex [ i ]) {
			delete this->mClipsByIndex [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::PopClip () {

	if ( this->mStack.Count ()) {
		MOAIAnimCycle* cycle = this->mStack.Back ();
		if ( cycle->mStack.Count ()) {
			MOAIAnimClip* clip = *cycle->mStack.PopBack ();
			this->DeactivateClip ( *clip );
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::PopCycle () {

	if ( this->mStack.Count ()) {
		MOAIAnimCycle* cycle = *this->mStack.PopBack ();
		this->DeactivateCycle ( *cycle );
	}
}

//----------------------------------------------------------------//
ZLResult < u32 > MOAIAnimMixer::PushClip ( MOAIAnimLoop& loop, float clipStart, float clipEnd ) {

	if ( this->mStack.Count () == 0 ) {
		this->PushCycle ();
	}
	
	MOAIAnimCycle* cycle = this->mStack.Back ();
	MOAIAnimClip& clip = this->AffirmClip ( loop, clipStart, clipEnd );
	cycle->mStack.PushBack ( clip.mLink );
	return clip.mIdentifier;
}

//----------------------------------------------------------------//
ZLResult < u32 > MOAIAnimMixer::PushCycle () {

	MOAIAnimCycle& cycle = this->AffirmCycle ();
	this->mStack.PushBack ( cycle.mLink );
	return cycle.mIdentifier;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "CHANNEL_TYPE_COLOR",		( u32 )MOAIAnimChannel::CHANNEL_TYPE_COLOR );
	state.SetField ( -1, "CHANNEL_TYPE_FLOAT",		( u32 )MOAIAnimChannel::CHANNEL_TYPE_FLOAT );
	state.SetField ( -1, "CHANNEL_TYPE_QUAT",		( u32 )MOAIAnimChannel::CHANNEL_TYPE_QUAT );
	state.SetField ( -1, "CHANNEL_TYPE_VEC",		( u32 )MOAIAnimChannel::CHANNEL_TYPE_VEC );
	
	state.SetField ( -1, "ANIM_TRACK_SPEED",		( u32 )MOAIAnimTrack::ANIM_TRACK_SPEED );
	state.SetField ( -1, "ANIM_TRACK_WEIGHT",		( u32 )MOAIAnimTrack::ANIM_TRACK_WEIGHT );
}

//----------------------------------------------------------------//
void MOAIAnimMixer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "apply",				_apply },
		{ "buildChannels",		_buildChannels },
		{ "insertClip",			_insertClip },
		{ "insertCycle",		_insertCycle },
		{ "popClip",			_popClip },
		{ "popCycle",			_popCycle },
		{ "pushClip",			_pushClip },
		{ "pushCycle",			_pushCycle },
		{ "remove",				_remove },
		{ "reserveChannels",	_reserveChannels },
		{ "setChannelLink",		_setChannelLink },
		{ "setChannelType",		_setChannelType },
		{ "setSpeed",			_setSpeed },
		{ "setWeight",			_setWeight },
		{ "transition",			_transition },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimMixer::Remove ( u32 identifier ) {

	u32 index = identifier & INDEX_MASK;

	if ( identifier & CYCLE_FLAG ) {
		
		if ( index < this->mCyclesByIndex.Size ()) {
			MOAIAnimCycle* cycle = this->mCyclesByIndex [ index ];
			if ( cycle->mActive ) {
				cycle->mLink.Remove ();
				this->DeactivateCycle ( *cycle );
			}
		}
	}
	else {

		if ( index < this->mClipsByIndex.Size ()) {
			MOAIAnimClip* clip = this->mClipsByIndex [ index ];
			if ( clip->mActive ) {
				clip->mLink.Remove ();
				this->DeactivateClip ( *clip );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::ReserveChannels ( size_t nChannels ) {

	this->mChannels.Init ( nChannels );
	this->mLinks.Init ( nChannels );
	this->mChannelBuffer.Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimMixer::SetAttribute ( u32 identifier, u32 attrID, float value ) {

	assert ( attrID < MOAIAnimTrack::TOTAL_ATTRIBUTES );
	
	MOAIAnimTrack* track = this->GetTrack ( identifier );
	if ( track ) {
		track->mAttributes [ attrID ] = value;
	}
}

//----------------------------------------------------------------//
void MOAIAnimMixer::SetChannelType ( u32 channelID, u32 type ) {

	assert ( channelID < this->mChannels.Size ());
	this->mChannels [ channelID ].mType = type;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::SetChannelLink ( u32 channelID, MOAINode* target, u32 attrID ) {

	assert ( channelID < this->mLinks.Size ());
	MOAIAnimChannelLink& link = this->mLinks [ channelID ];
	link.mTarget = target;
	link.mAttrID = attrID;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::Transition ( u32 identifier, u32 attrID, float value, float duration, u32 curve, bool popOnEnd ) {

	assert ( attrID < MOAIAnimTrack::TOTAL_ATTRIBUTES );

	MOAIAnimTrack* target = this->GetClip ( identifier );

	if ( target && target->mActive ) {
		
		MOAIAnimTransition& transition = this->AffirmTransition ();
		
		transition.mTarget			= target;
		transition.mAttribute		= &target->mAttributes [ attrID ];
		
		transition.mCurve			= curve;
		transition.mStartTime		= this->mTime;
		transition.mDuration		= duration;
		transition.mStartValue		= *transition.mAttribute;
		transition.mEndValue		= value;
		transition.mPopOnEnd		= popOnEnd;
		
		this->mActiveTransitions.PushBack ( transition.mLink );
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIAnimMixer::MOAIAction_IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAIAnimMixer::MOAIAction_Update ( double step ) {

	//float t0 = this->GetTime ();
	//this->DoStep (( float )step );
	//float t1 = this->GetTime ();
	//this->Apply ( t0, t1 );

	this->mTime += step;

	// update all the transitions
	
	ZLLeanLink < MOAIAnimTransition* >* transitionIt = this->mActiveTransitions.Head ();
	while ( transitionIt ) {
		MOAIAnimTransition& transition = **transitionIt;
		transitionIt = transitionIt->Next ();
	
		float t = ( this->mTime - transition.mStartTime ) / transition.mDuration;
		if ( t <= 0.0f ) {
		
			*transition.mAttribute = transition.mStartValue;
		}
		else if ( t >= 1.0f ) {
		
			*transition.mAttribute = transition.mEndValue;
			if ( transition.mPopOnEnd ) {
				this->Remove ( transition.mTarget->mIdentifier );
				continue;
			}
			this->mTransitionPool.PushBack ( transition.mLink );
		}
		else {
			*transition.mAttribute = ZLInterpolate::Interpolate ( transition.mCurve, transition.mStartValue, transition.mEndValue, t );
		}
	}

	for ( ZLLeanLink < MOAIAnimCycle* >* cycleIt = this->mStack.Head (); cycleIt; cycleIt = cycleIt->Next ()) {
		MOAIAnimCycle& cycle = **cycleIt;
		
		float cycleLength = 0.0f;
		
		// TODO: blend by weight to get cycleLength
		for ( ZLLeanLink < MOAIAnimClip* >* clipIt = cycle.mStack.Head (); clipIt; clipIt = clipIt->Next ()) {
			MOAIAnimClip& clip = **clipIt;
			cycleLength =  ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, cycleLength, clip.mClipLength, clip.mWeight );
		}
		cycle.mNormalizedTime = fmodf (( cycle.mNormalizedTime * cycleLength ) + step, cycleLength ) / cycleLength;
	}

	this->Apply ( this->mTime );
}
