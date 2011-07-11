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
	@out	nil
*/
int MOAIAction::_addChild ( lua_State* L ) {
	
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UU" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;
	
	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2 );
	if ( !action ) return 0;
	
	self->AddChild ( *action );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	clear
	@text	Removes all child actions.

	@in		MOAIAction self
	@out	nil
*/
int MOAIAction::_clear ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;

	self->ClearChildren ();
	
	return 0;
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
	@text	Adds the action to the root of the action tree for updating.

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_start ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;

	self->Start ();

	state.CopyToTop ( 1 );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Removed the action from its parent action; action will
			stop being updated.

	@in		MOAIAction self
	@out	nil
*/
int MOAIAction::_stop ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;

	self->Stop ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	throttle
	@text	Sets the actions throttle. Throttle is a scalar on time.
			Is is passed to the action's children.
	
	@in		MOAIAction self
	@opt	number throttle	Default value is 1.
	@out	nil
*/
int MOAIAction::_throttle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	self->mThrottle = state.GetValue < float >( 2, 1.0f );
	
	return 0;
}

//================================================================//
// MOAIAction
//================================================================//

//----------------------------------------------------------------//
void MOAIAction::AddChild ( MOAIAction& action ) {

	action.Retain ();
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
bool MOAIAction::IsDone () {

	return ( this->mChildren.Count () == 0 );
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mNew ( true ),
	mPass ( 0 ),
	mParent ( 0 ),
	mThrottle ( 1.0f ) {

	this->mLink.Data ( this );

	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
		RTTI_EXTEND ( MOAIEventSource )
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

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( EVENT_STOP, state )) {
		state.DebugCall ( 1, 0 );
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
void MOAIAction::RegisterLuaClass ( USLuaState& state ) {

	MOAIEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_STOP", ( u32 )EVENT_STOP );
}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIEventSource::RegisterLuaFuncs ( state );

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
		this->mChildren.Remove ( action.mLink );
		
		action.UnblockSelf ();
		action.UnblockAll ();
		action.mParent = 0;
		action.Release ();
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
		this->OnStop ();
		this->Release ();
	}
}

//----------------------------------------------------------------//
STLString MOAIAction::ToString () {

	STLString repr;
	return repr;
}

//----------------------------------------------------------------//
void MOAIAction::Update ( float step, u32 pass, bool checkPass ) {

	if ( this->IsBlocked ()) return;
	if (( checkPass ) && ( pass < this->mPass )) return;
	
	step *= this->mThrottle;
	
	if ( this->mNew ) {
		step = 0.0f;
		checkPass = false;
	}
	
	if (( checkPass == false ) || ( pass == this->mPass )) {
		MOAIActionMgr::Get ().SetCurrentAction ( this );
		this->OnUpdate ( step );
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
	ChildIt childIt = this->mChildren.Head ();
	if ( childIt ) {
		childIt->Data ()->Retain ();
	}
	
	MOAIAction* child = 0;
	while ( childIt ) {
		
		child = childIt->Data ();
		
		// retain the *next* child in the list (if any)
		childIt = childIt->Next ();
		if ( childIt ) {
			childIt->Data ()->Retain ();
		}
		
		child->Update ( step, pass, checkPass );
		
		// release the *current* child
		child->Release ();
	}
	
	if ( this->IsDone ()) {
		this->Stop ();
	}
}
