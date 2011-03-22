// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAITextSpoolAction.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( more ) more ( self )</tt>\n
\n
	Returns true if there is more text to spool.
	@param self (in)
	@param more (out)
*/
int MOAITextSpoolAction::_more ( lua_State* L ) {
	LUA_SETUP ( MOAITextSpoolAction, "U" )

	bool more = self->More ();
	lua_pushboolean ( L, more );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>() nextPage ( self )</tt>\n
\n
	Advances to the next page and restarts the spooling action.
	@param self (in)
*/
int MOAITextSpoolAction::_nextPage ( lua_State* L ) {
	LUA_SETUP ( MOAITextSpoolAction, "U" )

	if ( self->mTextBox ) {
		self->mTextBox->NextPage ( false );
		self->Start ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setSpeed ( speed )</tt>\n
\n
	Sets the base spool speed.
	@param self (in)
	@param speed (in)
*/
int MOAITextSpoolAction::_setSpeed ( lua_State* L ) {
	LUA_SETUP ( MOAITextSpoolAction, "UN" )
	
	self->mSpoolSpeed = state.GetValue < float >( 2, self->mSpoolSpeed );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() start ( self )</tt>\n
\n
	Starts the spool action. Overrides the MOAIAction 'start' method.
	@param self (in)
*/
int MOAITextSpoolAction::_start ( lua_State* L ) {
	LUA_SETUP ( MOAITextSpoolAction, "U" )

	if ( self->mTextBox ) {
		self->mTextBox->mReveal = 0;
		self->mTextBox->mSpool = 0.0f;
		self->Start ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() throttle ( self, throttle )</tt>\n
\n
	Scales the spool speed.
	@param self (in)
	@param throttle (in)
*/
int MOAITextSpoolAction::_throttle ( lua_State* L ) {
	LUA_SETUP ( MOAITextSpoolAction, "U" )

	self->mThrottle = state.GetValue < float >( 2, self->mThrottle );
	
	return 0;
}

//================================================================//
// MOAITextSpoolAction
//================================================================//

const float MOAITextSpoolAction::DEFAULT_SPOOL_SPEED = 24.0f;

//----------------------------------------------------------------//
bool MOAITextSpoolAction::IsBusy () {

	if ( this->IsActive () && this->mTextBox ) {
		this->mTextBox->Layout ();
		return ( this->mTextBox->mReveal < this->mTextBox->mLayout.GetTop ());
	}
	return false;
}

//----------------------------------------------------------------//
MOAITextSpoolAction::MOAITextSpoolAction () :
	mSpoolSpeed ( DEFAULT_SPOOL_SPEED ),
	mThrottle ( 1.0f ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAITextSpoolAction::~MOAITextSpoolAction () {
}

//----------------------------------------------------------------//
bool MOAITextSpoolAction::More () {

	if ( this->mTextBox ) {
		if ( this->mTextBox->More ()) {
			return true;
		}
	}
	return this->IsBusy ();
}

//----------------------------------------------------------------//
void MOAITextSpoolAction::OnUpdate ( float step ) {
	
	if ( this->mTextBox ) {
		this->mTextBox->Spool ( this->mSpoolSpeed * this->mThrottle * step );
	}
}

//----------------------------------------------------------------//
void MOAITextSpoolAction::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITextSpoolAction::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "more",				_more },
		{ "nextPage",			_nextPage },
		{ "setSpeed",			_setSpeed },
		{ "start",				_start },
		{ "throttle",			_throttle },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAITextSpoolAction::ToString () {

	STLString repr( MOAIAction::ToString () );

	return repr;
}
