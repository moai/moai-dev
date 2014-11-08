// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIActionTree.h>
#include <moai-sim/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	addChild
	@text	Attaches a child action for updating.

	@in		MOAIAction self
	@in		MOAIAction child
	@out	MOAIAction self
*/
int MOAIAction::_addChild ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )
	
	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2, true );
	
	if ( action ) {
		action->Attach ( self );
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
/**	@lua	detach
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
/**	@lua	start
	@text	Adds the action to a parent action or the root of the action tree.

	@in		MOAIAction self
	@opt	MOAIAction parent		Default value is MOAIActionMgr.getRoot ()
	@out	MOAIAction self
*/
int MOAIAction::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAction, "U" )

	MOAIAction* action = state.GetLuaObject < MOAIAction >( 2, true );
	
	if ( !action ) {
		action = MOAISim::Get ().GetActionMgr ().GetDefaultParent ();
	}

	self->Attach ( action );
	state.CopyToTop ( 1 );
	self->mIsPaused = false;

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

	self->Attach ();
	state.CopyToTop ( 1 );
	self->mIsPaused = false;

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
void MOAIAction::Attach ( MOAIAction* parent ) {

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
		if ( oldParent->mChildIt == &this->mLink ) {
			oldParent->mChildIt = oldParent->mChildIt->Next ();
			if ( oldParent->mChildIt ) {
				oldParent->mChildIt->Data ()->Retain ();
			}
			this->Release ();
		}
		
		oldParent->mChildren.Remove ( this->mLink );
		
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
		parent->mChildren.PushBack ( this->mLink );
		this->mParent = parent;
		this->SetTree ( parent->mTree );
	}
	
	if (( !oldParent ) && parent ) {
		this->mBasePass = this->mTree ? this->mTree->mPass : 0; // whenever we move it, if there is a tree we get bumped to the current pass
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

	return ( this->mTree && ( this->mTree->GetCurrentAction () == this ));
}

//----------------------------------------------------------------//
bool MOAIAction::IsDone () {

	return ( this->mAutoStop && ( this->mChildren.Count () == 0 ));
}

//----------------------------------------------------------------//
bool MOAIAction::IsPaused () {
	// TODO: better to do this with a state?
	return this->mIsPaused || (this->mParent && this->mParent->IsPaused ());
}

//----------------------------------------------------------------//
STLString MOAIAction::GetDebugInfo() const {
	return TypeName();
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mBasePass ( 0 ),
	mIsDefaultParent ( 0 ),
	mParent ( 0 ),
	mTree ( 0 ),
	mChildIt ( 0 ),
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

	this->InvokeListenerWithSelf ( EVENT_START );
}

//----------------------------------------------------------------//
void MOAIAction::OnStop () {

	this->InvokeListenerWithSelf ( EVENT_STOP );
}

//----------------------------------------------------------------//
void MOAIAction::OnUnblock () {

	// TODO: does this make sense?
	this->mBasePass = 0;
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
void MOAIAction::ResetPass () {

	this->mBasePass = 0;
	
	this->mChildIt = this->mChildren.Head ();
	for ( ; this->mChildIt; this->mChildIt->Next ()) {
		this->mChildIt->Data ()->ResetPass ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::SetTree ( MOAIActionTree* tree ) {

	if ( this->mTree != tree ) {
		this->mTree = tree;

		this->mChildIt = this->mChildren.Head ();
		for ( ; this->mChildIt; this->mChildIt->Next ()) {
			this->mChildIt->Data ()->SetTree ( tree );
		}
	}
}

//----------------------------------------------------------------//
void MOAIAction::Update ( double step ) {

	if ( this->IsPaused () || this->IsBlocked ()) return;
	if ( this->mTree && ( this->mTree->mPass <= this->mBasePass )) return;

	MOAIActionTree* tree = this->mTree;

	double t0 = 0.0;
	bool profilingEnabled = false;

	if ( tree ) {
		profilingEnabled = tree->GetProfilingEnabled ();
		if ( profilingEnabled ) {
			t0 = ZLDeviceTime::GetTimeInSeconds ();
		}
		tree->SetCurrentAction ( this );
		tree->SetDefaultParent ( 0 );
	}
	
	step *= this->mThrottle;
	
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
		
		if ( child->mParent ) {
			child->Update ( step );
		}
		
		// release the *current* child
		child->Release ();
	}
	
	this->mChildIt = 0;
	
	if ( this->IsDone ()) {
		this->Attach ();
	}
}

//----------------------------------------------------------------//
void MOAIAction::Start ( MOAIActionTree& tree ) {

	MOAIAction* defaultParent = tree.GetDefaultParent ();
	this->Attach ( defaultParent );
	this->mIsPaused = false;
}

//----------------------------------------------------------------//
void MOAIAction::Stop () {

	this->Attach ( 0 );
	this->mIsPaused = false;
}