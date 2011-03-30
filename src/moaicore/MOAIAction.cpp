// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAction.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name addChild
	@param1 self @type userdata
	@param2 child @type userdata
	@text Attaches a child action updating.
	@return nil
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
/**	@name clear
	@param1 self @type userdata
	@text Removes all child actions.
	@return nil
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
/**	@name isBusy
	@param1 self @type userdata
	@text Checks to see if an action is currently active.
	@return Whether the action is active. @type boolean
*/
int MOAIAction::_isBusy ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;

	lua_pushboolean ( state, self->IsBusy ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name start
	@param1 self @type userdata
	@text Starts processing the action.
	@return This object. @type userdata
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
/**	@name stop
	@param1 self @type userdata
	@text Stops processing of the action.
	@return nil
*/
int MOAIAction::_stop ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* self = state.GetLuaObject < MOAIAction >( 1 );
	if ( !self ) return 0;

	self->Stop ();

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

	return ( this->mChildren.Count () > 0 );
}

//----------------------------------------------------------------//
MOAIAction::MOAIAction () :
	mNew ( true ),
	mPass ( 0 ),
	mParent ( 0 ) {

	this->mLink.Data ( this );

	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
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
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAction::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "addChild",			_addChild },
		{ "clear",				_clear },
		{ "isBusy",				_isBusy },
		{ "start",				_start },
		{ "stop",				_stop },
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
		action.OnStop ();
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
		this->mParent->RemoveChild ( *this );
	}
}

//----------------------------------------------------------------//
STLString MOAIAction::ToString () {

	STLString repr;

	PrettyPrint ( repr, "mBusy", this->IsBusy () );

	return repr;
}

//----------------------------------------------------------------//
void MOAIAction::Update ( float step, u32 pass, bool checkPass ) {

	if ( this->IsBlocked ()) return;

	if (( checkPass ) && ( pass < this->mPass )) return;
	
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
	
	ChildIt childIt = this->mChildren.Head ();
	while ( childIt ) {
		
		MOAIAction* child = childIt->Data ();
		childIt = childIt->Next ();
		
		child->Retain ();
		child->Update ( step, pass, checkPass );
		child->Release ();
	}
	
	if ( !this->IsBusy ()) {
		this->Stop ();
	}
}

//================================================================//
// MOAIActionMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIActionMgr::Clear () {

	this->mRoot.ClearChildren ();
}

//----------------------------------------------------------------//
u32 MOAIActionMgr::GetNextPass () {

	this->mTotalPasses = this->mPass + 2;
	return this->mPass + 1;
}

//----------------------------------------------------------------//
MOAIActionMgr::MOAIActionMgr () :
	mPass ( RESET_PASS ),
	mCurrentAction ( 0 ) {
}

//----------------------------------------------------------------//
MOAIActionMgr::~MOAIActionMgr () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIActionMgr::StartAction ( MOAIAction& action ) {

	this->mRoot.AddChild ( action );
}

//----------------------------------------------------------------//
void MOAIActionMgr::Update ( float step ) {

	this->GetNextPass ();

	for ( this->mPass = 0; this->mPass < this->mTotalPasses; ++this->mPass ) {
		this->mRoot.Update ( step, this->mPass, true );
	}

	this->mPass = RESET_PASS;
	this->mCurrentAction = 0;
}
