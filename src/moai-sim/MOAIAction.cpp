// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIActionTree.h>
#include <moai-sim/MOAISim.h>

//================================================================//
// MOAIActionStackMgr
//================================================================//
	
//----------------------------------------------------------------//
MOAIAction* MOAIActionStackMgr::GetCurrent () {

	size_t top = this->mStack.GetTop ();
	return top > 0 ? this->mStack [ top - 1 ] : 0;
}

//----------------------------------------------------------------//
MOAIAction* MOAIActionStackMgr::GetDefaultParent () {

	MOAIAction* defaultParent = 0;

	MOAIAction* cursor = this->GetCurrent ();
	for ( ; cursor; cursor = cursor->mParent ) {
		defaultParent = cursor->GetDefaultParent ();
		if ( defaultParent ) {
			return defaultParent;
		}
	}
	return MOAISim::Get ().GetActionMgr ().GetDefaultParent ();
}

//----------------------------------------------------------------//
MOAIActionStackMgr::MOAIActionStackMgr () {
}

//----------------------------------------------------------------//
MOAIActionStackMgr::~MOAIActionStackMgr () {
}

//----------------------------------------------------------------//
void MOAIActionStackMgr::Pop () {

	this->mStack.Pop ();
}

//----------------------------------------------------------------//
void MOAIActionStackMgr::Push ( MOAIAction& action ) {

	this->mStack.Push ( &action );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	addChild
	@text	Attaches a child action for updating.

	@in		MOAIAction self
	@in		MOAIAction child
	@opt	boolean defer		Default value is 'false.'
	@out	MOAIAction self
*/
int MOAIAction::_addChild ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	MOAIAction* action		= state.GetLuaObject < MOAIAction >( 2, true );
	bool defer				= state.GetValue < bool >( 3, false );
	
	if ( action ) {
		action->Attach ( self, defer );
	}
	state.CopyToTop ( 1 );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	attach
	@text	Attaches a child to a parent action. The child will receive
			updates from the parent only if the parent is in the action tree.

	@in		MOAIAction self
	@opt	MOAIAction parent		Default value is nil; same effect as calling detach ().
	@opt	boolean defer			Default value is 'false.'
	@out	MOAIAction self
*/
int MOAIAction::_attach ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	MOAIAction* parent		= state.GetLuaObject < MOAIAction >( 2, true );
	bool defer				= state.GetValue < bool >( 3, false );
	
	self->Attach ( parent, defer );
	state.CopyToTop ( 1 );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	clear
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
// TODO: doxygen
int MOAIAction::_defer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	bool defer = state.GetValue < bool >( 2, false );
	self->Defer ( defer );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	detach
	@text	Detaches an action from its parent (if any) thereby removing
			it from the action tree. Same effect as calling stop ().

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_detach ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	self->Detach ();
	state.CopyToTop ( 1 );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isActive
	@text	Checks to see if an action is currently in the action tree.

	@in		MOAIAction self
	@out	boolean isActive
*/
int MOAIAction::_isActive ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsActive ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isBusy
	@text	Checks to see if an action is currently busy. An action is
			'busy' only if it is 'active' and not 'done.'

	@in		MOAIAction self
	@out	boolean isBusy
*/
int MOAIAction::_isBusy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsBusy ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isDone
	@text	Checks to see if an action is 'done.' Definition of 'done'
			is up to individual action implementations.

	@in		MOAIAction self
	@out	boolean isDone
*/
int MOAIAction::_isDone ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	lua_pushboolean ( state, self->IsDone ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isPaused
	@text	Checks to see if an action is 'paused.'
 
	@in		MOAIAction self
	@out	bool isPaused
*/
int MOAIAction::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );
	
	lua_pushboolean ( state, self->IsPaused ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	pause
	@text	Leaves the action in the action tree but prevents it from
			receiving updates. Call pause ( false ) or start () to unpause.

	@in		MOAIAction self
	@opt	boolean pause			Default value is 'true.'
	@out	nil
*/
int MOAIAction::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );

	self->mActionFlags = state.GetValue < bool >( 2, false ) ? self->mActionFlags | FLAGS_IS_PAUSED : self->mActionFlags & ~FLAGS_IS_PAUSED;
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAction::_setAutoStop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" );
	self->mActionFlags = state.GetValue < bool >( 2, false ) ? self->mActionFlags | FLAGS_AUTO_STOP : self->mActionFlags & ~FLAGS_AUTO_STOP;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	start
	@text	Adds the action to a parent action or the root of the action tree.

	@in		MOAIAction self
	@opt	MOAIAction parent		Default value is MOAIActionMgr.getRoot ()
	@opt	boolean defer			Action will first run during the next sim step, even if it visited during the current sim step. Default value is 'false.'
	@out	MOAIAction self
*/
int MOAIAction::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	MOAIAction* action		= state.GetLuaObject < MOAIAction >( 2, true );
	bool defer				= state.GetValue < bool >( 3, false );
	
	if ( !action ) {
		action = MOAISim::Get ().GetActionMgr ().GetDefaultParent ();
	}

	self->Attach ( action, defer );
	state.CopyToTop ( 1 );
	self->mActionFlags &= ~FLAGS_IS_PAUSED;

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	stop
	@text	Removed the action from its parent action; action will
			stop being updated.

	@in		MOAIAction self
	@out	MOAIAction self
*/
int MOAIAction::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	self->Detach ();
	state.CopyToTop ( 1 );
	self->mActionFlags &= ~FLAGS_IS_PAUSED;

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	throttle
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
void MOAIAction::Attach ( MOAIAction* parent, bool defer ) {

	MOAIAction* oldParent = this->mParent;
	if ( oldParent == parent ) return;

	this->Retain ();

	if ( parent ) {
		parent->LuaRetain ( this );
	}
	
	if ( oldParent ) {
	
		// if we're detaching the action while the parent action is updating
		// then we need to handle the edge case where the action is referenced
		// by mChildIt
		if (( oldParent->mActionFlags & FLAGS_IS_UPDATING ) && ( oldParent->mChildIt == &this->mLink )) {
			oldParent->mChildIt = oldParent->mChildIt->Next ();
			if ( oldParent->mChildIt ) {
				oldParent->mChildIt->Data ()->Retain ();
			}
			this->Release ();
		}
		
		oldParent->mChildren.Remove ( this->mLink );
		oldParent->OnLostChild ( this );
		
		// TODO: hmmm...
		this->UnblockSelf ();
		this->UnblockAll ();
		this->mParent = 0;
		
		oldParent->LuaRelease ( this );
	}
	
	if ( oldParent && ( !parent )) {
		this->OnStop ();
	}
	
	if ( parent ) {
	
		this->mParent = parent;
		parent->mChildren.PushBack ( this->mLink );
		this->ResetPass ( defer ? parent->mPass + 1 : parent->mPass );
		
		if ( !parent->mChildIt ) {
			parent->mChildIt = &this->mLink;
		}
	}
	
	if (( !oldParent ) && parent ) {
		if ( !( this->mActionFlags & FLAGS_IS_PAUSED )) {
			this->OnStart ();
		}
	}

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIAction::ClearChildren () {

	while ( ChildIt actionIt = this->mChildren.Head ()) {
		actionIt->Data ()->Detach ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::Defer ( bool defer ) {

	if ( this->mParent ) {
		this->mPass = defer ? this->mParent->mPass + 1 : this->mParent->mPass;
	}
}

//----------------------------------------------------------------//
void MOAIAction::Detach () {

	this->Attach ( 0, false );
}

//----------------------------------------------------------------//
MOAIAction* MOAIAction::GetDefaultParent () {

	return 0;
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

	return (( this->mActionFlags & FLAGS_AUTO_STOP ) && ( this->mChildren.Count () == 0 ));
}

//----------------------------------------------------------------//
bool MOAIAction::IsPaused () {

	return ( this->mActionFlags & FLAGS_IS_PAUSED ) || (this->mParent && this->mParent->IsPaused ());
}

//----------------------------------------------------------------//
STLString MOAIAction::GetDebugInfo() const {
	return TypeName();
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mPass ( 0 ),
	mParent ( 0 ),
	mChildIt ( 0 ),
	mThrottle ( 1.0f ),
	mActionFlags ( FLAGS_AUTO_STOP ) {

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
void MOAIAction::OnLostChild ( MOAIAction* child ) {
	UNUSED ( child );
}

//----------------------------------------------------------------//
void MOAIAction::OnStart () {

	this->InvokeListenerWithSelf ( EVENT_START );
}

//----------------------------------------------------------------//
void MOAIAction::OnStop () {

	this->InvokeListenerWithSelf ( EVENT_STOP );
}

//----------------------------------------------------------------//
void MOAIAction::OnUnblock () {

	// TODO: does this make sense?
	this->mPass = 0;
}

//----------------------------------------------------------------//
void MOAIAction::OnUpdate ( double step ) {
	UNUSED ( step );
}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_ACTION_PRE_UPDATE",		( u32 )EVENT_ACTION_PRE_UPDATE );
	state.SetField ( -1, "EVENT_ACTION_POST_UPDATE",	( u32 )EVENT_ACTION_POST_UPDATE );
	state.SetField ( -1, "EVENT_START",					( u32 )EVENT_START );
	state.SetField ( -1, "EVENT_STOP",					( u32 )EVENT_STOP );

}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addChild",				_addChild },
		{ "attach",					_attach },
		{ "clear",					_clear },
		{ "defer",					_defer },
		{ "detach",					_detach },
		{ "isActive",				_isActive },
		{ "isBusy",					_isBusy },
		{ "isDone",					_isDone },
		{ "isPaused",				_isPaused },
		{ "pause",					_pause },
		{ "setAutoStop",			_setAutoStop },
		{ "start",					_start },
		{ "stop",					_stop },
		{ "throttle",				_throttle },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAction::ResetPass ( u32 pass ) {

	if ( this->mPass > pass ) {
		ChildIt childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = this->mChildIt->Next ()) {
			this->mChildIt->Data ()->ResetPass ( pass );
		}
	}
	this->mPass = pass;
}

//----------------------------------------------------------------//
void MOAIAction::Update ( MOAIActionTree& tree, double step ) {

	if ( this->IsPaused () || this->IsBlocked ()) return;

	MOAIActionStackMgr::Get ().Push ( *this );

	double t0 = 0.0;
	bool profilingEnabled = false;

	profilingEnabled = tree.GetProfilingEnabled ();
	if ( profilingEnabled ) {
		t0 = ZLDeviceTime::GetTimeInSeconds ();
	}
	
	this->mPass = this->mParent ? this->mParent->mPass : this->mPass + 1;
	
	// handles the case when Moai has been running continuously for approx. 136 years at 60 fps
	if ( this->mPass == 0xffffffff ) {
		this->ResetPass ();
	}
	
	step *= this->mThrottle;
	
	this->mActionFlags |= FLAGS_IS_UPDATING;
	
	this->InvokeListenerWithSelf ( EVENT_ACTION_PRE_UPDATE );
	this->OnUpdate ( step );
	this->InvokeListenerWithSelf ( EVENT_ACTION_POST_UPDATE );

	if ( profilingEnabled ) {
		double elapsed = ZLDeviceTime::GetTimeInSeconds () - t0;
		if ( elapsed >= 0.005 ) {
			STLString debugInfo = this->GetDebugInfo();
			MOAILog ( 0, MOAILogMessages::MOAIAction_Profile_PSFF, this, this->TypeName (), debugInfo.c_str(), step * 1000, elapsed * 1000 );
		}
	}
	
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
		
		if ( child->mParent && ( child->mPass <= this->mPass )) {
			child->Update ( tree, step );
		}
		
		// release the *current* child
		child->Release ();
	}
	
	this->mActionFlags &= ~FLAGS_IS_UPDATING;
	this->mChildIt = 0;
	
	if ( this->IsDone ()) {
		this->Detach ();
	}
	
	MOAIActionStackMgr::Get ().Pop ();
}

//----------------------------------------------------------------//
void MOAIAction::Start ( MOAIActionTree& tree, bool defer ) {

	MOAIAction* defaultParent = tree.GetDefaultParent ();
	this->Attach ( defaultParent, defer );
	this->mActionFlags &= ~FLAGS_IS_PAUSED;
}

//----------------------------------------------------------------//
void MOAIAction::Stop () {

	this->Detach ();
	this->mActionFlags &= ~FLAGS_IS_PAUSED;
}