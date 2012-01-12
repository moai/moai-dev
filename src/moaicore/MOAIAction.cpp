// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	addChild
	@text	Attaches a child action for updating.

	@in		MOAIAction self
	@in		MOAIAction child
	@out	MOAIAction self
*/
int MOAIAction::_addChild ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "UU" )
	
	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2 );
	
	state.CopyToTop ( 1 );
	
	if ( !action ) return 1;
	self->AddChild ( *action );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	clear
	@text	Removes all child actions.

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	self->ClearChildren ();
	state.CopyToTop ( 1 );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isActive
	@text	Checks to see if an action is currently in the action tree.

	@in		MOAIAction self
	@out	bool isActive
*/
int MOAIAction::_isActive ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsActive ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isBusy
	@text	Checks to see if an action is currently busy. An action is
			'busy' only if it is 'active' and not 'done.'

	@in		MOAIAction self
	@out	bool isBusy
*/
int MOAIAction::_isBusy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsBusy ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isDone
	@text	Checks to see if an action is 'done.' Definition of 'done'
			is up to individual action implementations.

	@in		MOAIAction self
	@out	bool isDone
*/
int MOAIAction::_isDone ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsDone ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	start
	@text	Adds the action to a parent action or the root of the action tree.

	@in		MOAIAction self
	@opt	MOAIAction parent		Default value is MOAIActionMgr.getRoot ()
	@out	MOAIAction self
*/
int MOAIAction::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2 );
	
	if ( action ) {
		self->Start ( *action );
	}
	else {
		self->Start ();
	}

	state.CopyToTop ( 1 );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Removed the action from its parent action; action will
			stop being updated.

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	self->Stop ();
	state.CopyToTop ( 1 );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	throttle
	@text	Sets the actions throttle. Throttle is a scalar on time.
			Is is passed to the action's children.
	
	@in		MOAIAction self
	@opt	number throttle	Default value is 1.
	@out	MOAIAction self
*/
int MOAIAction::_throttle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	self->mThrottle = state.GetValue < float >( 2, 1.0f );
	state.CopyToTop ( 1 );
	
	return 1;
}

//================================================================//
// MOAIAction
//================================================================//

//----------------------------------------------------------------//
void MOAIAction::AddChild ( MOAIAction& action ) {

	if ( action.mParent == this ) return;

	this->LuaRetain ( action );
	action.Stop ();
	
	this->mChildren.PushBack ( action.mLink );
	action.mParent = this;
	
	action.mNew = true;
	action.mPass = MOAIActionMgr::Get ().GetNextPass ();
	
	action.OnStart ();
}

//----------------------------------------------------------------//
void MOAIAction::ClearChildren () {

	while ( ChildIt actionIt = this->mChildren.Head ()) {
		this->RemoveChild ( *actionIt->Data ());
	}
}

//----------------------------------------------------------------//
bool MOAIAction::IsActive () {

	return ( this->mParent != 0 );
}

//----------------------------------------------------------------//
bool MOAIAction::IsBusy () {

	return ( this->IsActive () && ( !this->IsDone ()));
}

//----------------------------------------------------------------//
bool MOAIAction::IsCurrent () {

	if ( MOAIActionMgr::IsValid ()) {
		return ( MOAIActionMgr::Get ().GetCurrentAction () == this );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIAction::IsDone () {

	return ( this->mChildren.Count () == 0 );
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mNew ( true ),
	mPass ( 0 ),
	mParent ( 0 ),
	mChildIt ( 0 ),
	mThrottle ( 1.0f ) {

	this->mLink.Data ( this );

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIInstanceEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAction::~MOAIAction () {

	this->ClearChildren ();
}

//----------------------------------------------------------------//
void MOAIAction::OnStart () {
}

//----------------------------------------------------------------//
void MOAIAction::OnStop () {

	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( EVENT_STOP, state )) {
			state.DebugCall ( 1, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIAction::OnUnblock () {

	if ( MOAIActionMgr::IsValid ()) {
		this->mNew = true;
		this->mPass = MOAIActionMgr::Get ().GetNextPass ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::OnUpdate ( float step ) {
	UNUSED ( step );
}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_STOP", ( u32 )EVENT_STOP );
}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addChild",			_addChild },
		{ "clear",				_clear },
		{ "isActive",			_isActive },
		{ "isBusy",				_isBusy },
		{ "isDone",				_isDone },
		{ "start",				_start },
		{ "stop",				_stop },
		{ "throttle",			_throttle },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAction::RemoveChild ( MOAIAction& action ) {

	if ( action.mParent == this ) {
	
		if ( this->mChildIt == &action.mLink ) {
			this->mChildIt = this->mChildIt->Next ();
			if ( this->mChildIt ) {
				this->mChildIt->Data ()->Retain ();
			}
			action.Release ();
		}
		
		this->mChildren.Remove ( action.mLink );
		
		action.UnblockSelf ();
		action.UnblockAll ();
		action.mParent = 0;
		action.OnStop ();
		
		this->LuaRelease ( action );
	}
}

//----------------------------------------------------------------//
void MOAIAction::Start () {

	MOAIActionMgr::Get ().StartAction ( *this );
}

//----------------------------------------------------------------//
void MOAIAction::Start ( MOAIAction& parent ) {

	if ( !this->mParent ) {
		parent.AddChild ( *this );
	}
}

//----------------------------------------------------------------//
void MOAIAction::Stop () {

	if ( this->mParent ) {
		this->Retain ();
		this->mParent->RemoveChild ( *this );
		this->Release ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::Update ( float step, u32 pass, bool checkPass ) {

	bool profilingEnabled = MOAIActionMgr::Get ().GetProfilingEnabled ();

	if ( this->IsBlocked ()) return;
	if (( checkPass ) && ( pass < this->mPass )) return;

	double t0 = 0.0;
	if ( profilingEnabled ) {
		USDeviceTime::GetTimeInSeconds ();
	}

	step *= this->mThrottle;
	
	if ( this->mNew ) {
		step = 0.0f;
		checkPass = false;
	}
	
	if (( checkPass == false ) || ( pass == this->mPass )) {
		MOAIActionMgr::Get ().SetCurrentAction ( this );
		this->OnUpdate ( step );
	}

	if ( profilingEnabled ) {
		double elapsed = USDeviceTime::GetTimeInSeconds () - t0;
		if ( elapsed >= 0.005 ) {
			MOAILog ( 0, MOAILogMessages::MOAIAction_Profile_PSFF, this, this->TypeName (), step * 1000, elapsed * 1000 );
		}
	}

	this->mPass = 0;
	this->mNew = false;
	
	// the trick below is to alway retain the current child plus the
	// *next* child in the list. each child is processed once and 
	// released after processing, so all the children should be 
	// retain/release'd exactly once.
	
	// we retain the head child in the list (if any)
	// here because the first child retained inside the loop (below)
	// is the *second* child in the list
	this->mChildIt = this->mChildren.Head ();
	if ( this->mChildIt ) {
		this->mChildIt->Data ()->Retain ();
	}
	
	MOAIAction* child = 0;
	while ( this->mChildIt ) {
		
		child = this->mChildIt->Data ();
		
		// retain the *next* child in the list (if any)
		this->mChildIt = this->mChildIt->Next ();
		if ( this->mChildIt ) {
			this->mChildIt->Data ()->Retain ();
		}
		
		if ( child->mParent ) {
			child->Update ( step, pass, checkPass );
		}
		
		// release the *current* child
		child->Release ();
	}
	
	this->mChildIt = 0;
	
	if ( this->IsDone ()) {
		this->Stop ();
	}
}
