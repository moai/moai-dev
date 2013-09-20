// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIActionMgr.h>

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
	
	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2, true );
	
	if ( action ) {
		action->Attach ( self );
	}
	state.CopyToTop ( 1 );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	attach
	@text	Attaches a child to a parent action. The child will receive
			updates from the parent only if the parent is in the action tree.

	@in		MOAIAction self
	@opt	MOAIAction parent		Default value is nil; same effect as calling detach ().
	@out	MOAIAction self
*/
int MOAIAction::_attach ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	MOAIAction* parent = state.GetLuaObject < MOAIAction >( 2, true );
	self->Attach ( parent );
	state.CopyToTop ( 1 );
	
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
/**	@name	detach
	@text	Detaches an action from its parent (if any) thereby removing
			it from the action tree. Same effect as calling stop ().

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_detach ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	self->Attach ();
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
/**	@name	pause
	@text	Leaves the action in the action tree but prevents it from
			receiving updates. Call pause ( false ) or start () to unpause.

	@in		MOAIAction self
	@opt	bool pause			Default value is 'true.'
	@out	nil
*/
int MOAIAction::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	self->mIsPaused = state.GetValue < bool >( 2, true );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAction::_setAutoStop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );
	self->mAutoStop = state.GetValue < bool >( 2, false );
	return 0;
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

	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2, true );
	
	if ( !action ) {
		action = MOAIActionMgr::Get ().AffirmRoot ();
	}

	self->Attach ( action );
	state.CopyToTop ( 1 );
	self->mIsPaused = false;

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

	self->Attach ();
	state.CopyToTop ( 1 );
	self->mIsPaused = false;

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
void MOAIAction::Attach ( MOAIAction* parent ) {

	MOAIAction* oldParent = this->mParent;
	if ( oldParent == parent ) return;

	this->Retain ();

	if ( parent ) {
		parent->LuaRetain ( this );
	}
	
	if ( oldParent ) {
		
		oldParent->mChildren.Remove ( this->mLink );
		
		this->UnblockSelf ();
		this->UnblockAll ();
		this->mParent = 0;
		
		oldParent->LuaRelease ( this );
	}
	
	if ( oldParent && ( !parent )) {
		this->OnStop ();
	}
	
	if ( parent ) {
		// TODO: there are some edge cases that may lead to the action
		// getting two updates in a frame or missing an update. additional
		// state may need to be introduced to handle this. the TODO is
		// to investigate the edge cases and (possibly) provide a fix.
		parent->mChildren.PushBack ( this->mLink );
		this->mParent = parent;
	}
	
	if (( !oldParent ) && parent ) {
		this->mNew = true;
		this->mPass = MOAIActionMgr::Get ().GetNextPass ();
		if ( !this->mIsPaused ) {
			this->OnStart ();
		}
	}
	
	this->Release ();
}

//----------------------------------------------------------------//
void MOAIAction::ClearChildren () {

	while ( ChildIt actionIt = this->mChildren.Head ()) {
		actionIt->Data ()->Attach ();
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

	return ( this->mAutoStop && ( this->mChildren.Count () == 0 ));
}

//----------------------------------------------------------------//
STLString MOAIAction::GetDebugInfo() const {
	return TypeName();
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mNew ( true ),
	mPass ( 0 ),
	mParent ( 0 ),
	mThrottle ( 1.0f ),
	mIsPaused ( false ),
	mAutoStop ( true ) {

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
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
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
		{ "attach",				_attach },
		{ "clear",				_clear },
		{ "detach",				_detach },
		{ "isActive",			_isActive },
		{ "isBusy",				_isBusy },
		{ "isDone",				_isDone },
		{ "pause",				_pause },
		{ "setAutoStop",		_setAutoStop },
		{ "start",				_start },
		{ "stop",				_stop },
		{ "throttle",			_throttle },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAction::Update ( float step, u32 pass, bool checkPass ) {

	bool profilingEnabled = MOAIActionMgr::Get ().GetProfilingEnabled ();

	if ( this->mIsPaused || this->IsBlocked ()){
		if ( this->mNew ) { 		//avoid edge case that a new-created-paused action cannot receive further update
			step = 0.0f;
			checkPass = false;
			this->mPass = 0;
			this->mNew = false;
		}
		return;
	} 
	if (( checkPass ) && ( pass < this->mPass )) return;

	double t0 = 0.0;
	if ( profilingEnabled ) {
		t0 = ZLDeviceTime::GetTimeInSeconds ();
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
		double elapsed = ZLDeviceTime::GetTimeInSeconds () - t0;
		if ( elapsed >= 0.005 ) {
			STLString debugInfo = this->GetDebugInfo();
			MOAILog ( 0, MOAILogMessages::MOAIAction_Profile_PSFF, this, this->TypeName (), debugInfo.c_str(), step * 1000, elapsed * 1000 );
		}
	}

	this->mPass = 0;
	this->mNew = false;
	
	ChildIt childIt = this->mChildren.Head ();
	while ( childIt ) {
		
		MOAIAction* child = childIt->Data ();
		childIt = childIt->Next ();
		
		if ( child->mParent ) {
			child->Update ( step, pass, checkPass );
		}
	}
	
	if ( this->IsDone ()) {
		this->Attach ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::Start () {

	MOAIAction* root = MOAIActionMgr::Get ().AffirmRoot ();
	this->Attach ( root );
	this->mIsPaused = false;
}

//----------------------------------------------------------------//
void MOAIAction::Stop () {

	this->Attach ( 0 );
	this->mIsPaused = false;
}