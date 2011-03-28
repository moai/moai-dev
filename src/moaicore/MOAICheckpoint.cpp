// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICheckpoint.h>

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICheckpoint::_newKey ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAICheckpoint* self = state.GetLuaObject < MOAICheckpoint >( 1 );
	if ( !self ) return 0;

	u32 key = self->AddKey ();
	state.Push ( key );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICheckpoint::_releaseKey ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAICheckpoint* self = state.GetLuaObject < MOAICheckpoint >( 1 );
	if ( !self ) return 0;

	u32 key = state.GetValue < u32 >( 2, NULL_ID );
	
	self->ReleaseKey ( key );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICheckpoint::_skip ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAICheckpoint* self = state.GetLuaObject < MOAICheckpoint >( 1 );
	if ( !self ) return 0;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICheckpoint::_sync ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAICheckpoint* self = state.GetLuaObject < MOAICheckpoint >( 1 );
	if ( !self ) return 0;

	u32 key = state.GetValue < u32 >( 2, NULL_ID );

	bool result = self->Sync ( key );
	state.Push ( result );

	return 1;
}

//================================================================//
// MOAICheckpoint
//================================================================//

//----------------------------------------------------------------//
u32 MOAICheckpoint::AddKey () {

	if ( this->mFreeKeys == NULL_ID ) {
	
		u32 base = this->mKeys.Size ();
		u32 top = base + BLOCK_SIZE;
		this->mKeys.Resize ( top );
		
		for ( u32 keyID = base; keyID < top; ++keyID ) {
			MOAISyncKey& key = this->mKeys [ keyID ];
		
			key.mNext = this->mFreeKeys;
			key.mCount = NULL_ID;
			this->mFreeKeys = keyID;
		}
	}
	
	u32 keyID = this->mFreeKeys;
	MOAISyncKey& key = this->mKeys [ keyID ];
	
	this->mFreeKeys = key.mNext;
	key.mNext = this->mActiveKeys;
	key.mCount = 0;
	key.mState = MOAISyncKey::IDLE;
	
	this->mActiveKeys = keyID;
	
	return keyID;
}

//----------------------------------------------------------------//
void MOAICheckpoint::AdvanceKeys () {

	u32 keyID = this->mActiveKeys;
	while ( keyID != NULL_ID ) {
		MOAISyncKey& key = this->mKeys [ keyID ];
		keyID = key.mNext;
		
		key.mCount--;
		key.mState = MOAISyncKey::RESET;
	}
}

//----------------------------------------------------------------//
bool MOAICheckpoint::CheckKeysPending () {
	
	// check to see if any key is lagging
	u32 keyID = this->mActiveKeys;
	while ( keyID != NULL_ID ) {
		MOAISyncKey& key = this->mKeys [ keyID ];
		keyID = key.mNext;
		
		if ( key.mCount < 1 ) {
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
MOAICheckpoint::MOAICheckpoint () :
	mActiveKeys ( NULL_ID ),
	mFreeKeys ( NULL_ID ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAICheckpoint::~MOAICheckpoint () {
}

//----------------------------------------------------------------//
void MOAICheckpoint::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAICheckpoint::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "newKey",				_newKey },
		{ "releaseKey",			_releaseKey },
		{ "skip",				_skip },
		{ "sync",				_sync },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICheckpoint::ReleaseKey ( u32 keyID ) {

	MOAISyncKey& key = this->mKeys [ keyID ];
	
	// bail if the key is already free
	if ( key.mCount == NULL_ID ) return;
	
	// remove from the active list
	if ( this->mActiveKeys == keyID ) {
		this->mActiveKeys = key.mNext;
	}
	else {
		u32 activeKeyID = this->mActiveKeys;
		while ( activeKeyID != NULL_ID ) {
			MOAISyncKey& activeKey = this->mKeys [ activeKeyID ];
			activeKeyID = activeKey.mNext;
			
			if ( activeKey.mNext == keyID ) {
				activeKey.mNext = key.mNext;
				break;
			}
		}
	}
	
	// add to the free list
	key.mNext = this->mFreeKeys;
	this->mFreeKeys = keyID;
	
	// set the count to null
	key.mCount = NULL_ID;
}

//----------------------------------------------------------------//
void MOAICheckpoint::Skip ( u32 keyID ) {

	MOAISyncKey& key = this->mKeys [ keyID ];

	key.mCount++;
	key.mState = MOAISyncKey::IDLE;
}

//----------------------------------------------------------------//
bool MOAICheckpoint::Sync ( u32 keyID ) {

	MOAISyncKey& key = this->mKeys [ keyID ];
	
	if ( key.mState == MOAISyncKey::RESET ) {
		key.mState = MOAISyncKey::IDLE;
		return false;
	}
	
	// update the current key
	if ( key.mCount < 1 ) {
		key.mCount++;
		key.mState = MOAISyncKey::BUSY;
	}
	
	// if any keys are pending, return
	if ( !this->CheckKeysPending ()) {
		
		// no keys are pending, so advance keys to the next state
		this->AdvanceKeys ();
	}
	
	// no keys are busy, so continue
	return !( key.mState == MOAISyncKey::IDLE );
}

//----------------------------------------------------------------//
STLString MOAICheckpoint::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mBusy )
	PRETTY_PRINT ( repr, mKeys )

	return repr;
}
