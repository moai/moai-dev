// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeMgr.h>

//================================================================//
// MOAIAttrLink
//================================================================//

//----------------------------------------------------------------//
MOAIAttrLink::MOAIAttrLink () :
	mSourceNode ( 0 ),
	mSourceAttrID ( MOAINode::NULL_ATTR ),
	mDestNode ( 0 ),
	mDestAttrID ( MOAINode::NULL_ATTR ),
	mDestAttrExists ( false ),
	mNextInSource ( 0 ),
	mNextInDest ( 0 ) {
}

//----------------------------------------------------------------//
MOAIAttrLink::~MOAIAttrLink () {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearAttrLink
	@text	Clears an attribute *pull* link - call this from the node
			receiving the attribute value.
	
	@in		MOAINode self
	@in		number attrID
	@out	nil
*/
int MOAINode::_clearAttrLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );
	self->ClearAttrLink ( attrID );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	clearDependency
	@text	Clears a dependency on a foreign node.
	
	@in		MOAINode self
	@in		MOAINode sourceNode
	@out	nil
*/
int MOAINode::_clearDependency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UU" );

	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 2 );
	if ( !srcNode ) return 0;
	
	self->ClearDependency ( *srcNode );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	forceUpdate
	@text	Evaluates the dependency graph for this node. Typically,
			the entire active dependency graph is evaluated once per
			frame, but in some cases it may be desirable to force
			evaluation of a node to make sure source dependencies
			are propagated to it immediately.
	
	@in		MOAINode self
	@out	nil
*/
int MOAINode::_forceUpdate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "U" );

	self->ForceUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAttr
	@text	Returns the value of the attribute if it exists or nil if it doesn't.
	
	@in		MOAINode self
	@in		number attrID
	@out	number value
*/
int MOAINode::_getAttr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );

	MOAI_ERROR_UNLESS ( self->AttrExists ( attrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );

	USAttrGetter getter;
	self->ApplyAttrOp ( attrID, getter );
	
	if ( getter.IsValid ()) {
		lua_pushnumber ( state, getter.Get < float >());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveAttr
	@text	Animate the attribute by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAINode self
	@in		number attrID	ID of the attribute to animate.
	@in		number delta	Total change to be added to attribute.
	@in		number length	Length of animation in seconds.
	@opt	number mode		The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
							MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAINode::_moveAttr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	u32 attrID		= state.GetValue < u32 >( 2, 0 );
	float value		= state.GetValue < float >( 3, 0.0f );
	float length	= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	MOAI_ERROR_UNLESS ( self->AttrExists ( attrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );
	
	action->SetLink ( 0, self, attrID, value, mode );
	action->SetLength ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	scheduleUpdate
	@text	Schedule the node for an update next time the dependency graph
			is processed. Any depdendent nodes will also be updated.
	
	@in		MOAINode self
	@out	nil
*/
int MOAINode::_scheduleUpdate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "U" );

	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekAttr
	@text	Animate the attribute by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAINode self
	@in		number attrID	ID of the attribute to animate.
	@in		number goal		Desired resulting value for attribute.
	@in		number length	Length of animation in seconds.
	@opt	number mode		The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
							MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAINode::_seekAttr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	MOAI_ERROR_UNLESS ( self->AttrExists ( attrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );
	
	USAttrGetter getter;
	self->ApplyAttrOp ( attrID, getter );
	if ( !getter.IsValid ()) return 0;
	
	float value		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, attrID, value - getter.Get < float >(), mode );
	
	action->SetLength ( delay );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setAttr
	@text	Sets the value of an attribute.
	
	@in		MOAINode self
	@in		number attrID
	@in		number value
	@out	nil
*/
int MOAINode::_setAttr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UNN" );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	float value = state.GetValue < float >( 3, 0.0f );

	MOAI_ERROR_UNLESS ( self->AttrExists ( attrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );

	USAttrSetter setter;
	setter.Set ( value );
	
	self->ClearAttrLink ( attrID );
	self->ApplyAttrOp ( attrID, setter );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAttrLink
	@text	Sets a *pull* attribute connecting an attribute in the
			note to an attribute in a foreign node.
	
	@in		MOAINode self
	@in		number attrID			ID of attribute to become dependent of foreign node.
	@in		MOAINode sourceNode		Foreign node.
	@in		number sourceAttrID		Attribute in foreign node to control value of attribue.
	@out	nil
*/
int MOAINode::_setAttrLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UNUN" );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	MOAI_ERROR_UNLESS ( self->AttrExists ( attrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );
	
	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 3 );
	if ( !srcNode ) return 0;

	u32 srcAttrID = state.GetValue < u32 >( 4, 0 );
	MOAI_ERROR_UNLESS ( srcNode->AttrExists ( srcAttrID ), state, MOAILogMessages::MOAINode_AttributeNotFound );

	self->SetAttrLink ( attrID, srcNode, srcAttrID );
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDependency
	@text	Creates a dependency between the node and a foreign node
			without the use of attributes; if the foreign node is updated,
			the dependent node will be updated after.
	
	@in		MOAINode self
	@in		MOAINode sourceNode
	@out	nil
*/
int MOAINode::_setDependency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UU" );
	
	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 2 );
	if ( !srcNode ) return 0;
	
	self->SetDependency ( *srcNode );
	
	return 0;
}

//================================================================//
// MOAINode
//================================================================//

//----------------------------------------------------------------//
void MOAINode::Activate ( MOAINode& activator ) {

	// bail if we're already in the list
	if ( this->mState != STATE_IDLE ) return;

	// insert before activator
	MOAINodeMgr::Get ().InsertBefore ( activator, *this );
	this->mState = STATE_ACTIVE;

	// activate source nodes
	MOAIAttrLink* link = this->mPullAttrLinks;
	for ( ; link ; link = link->mNextInDest ) {
		link->mSourceNode->Activate ( *this );
	}
}

//----------------------------------------------------------------//
void MOAINode::ActivateOnLink ( MOAINode& srcNode ) {

	if ( this->mState != STATE_IDLE ) {
	
		if ( srcNode.mState == STATE_IDLE ) {
			srcNode.Activate ( *this );
		}
		else {
			MOAINodeMgr& depNodeMgr = MOAINodeMgr::Get ();
			
			if ( srcNode.IsNodeUpstream ( this )) {
				
				depNodeMgr.Remove ( *this );
				depNodeMgr.InsertAfter ( srcNode, *this );
			}
		}
	}
}

//----------------------------------------------------------------//
MOAIAttrLink* MOAINode::AffirmDependency ( MOAINode& srcNode ) {

	MOAIAttrLink* link = this->mPullAttrLinks;
	for ( ; link; link = link->mNextInDest ) {
		if (( link->mSourceAttrID == NULL_ATTR ) && ( link->mSourceNode == &srcNode )) return link;
	}
	
	link = new MOAIAttrLink ();
	
	link->mSourceNode = &srcNode;
	link->mSourceAttrID = NULL_ATTR;
	
	link->mDestNode = this;
	link->mDestAttrID = NULL_ATTR;
	link->mDestAttrExists = false;
	
	// add the link to the list of pull links
	link->mNextInDest = this->mPullAttrLinks;
	this->mPullAttrLinks = link;
	
	// add the link to the source node's list of push links
	link->mNextInSource = srcNode.mPushAttrLinks;
	srcNode.mPushAttrLinks = link;
	
	return link;
}

//----------------------------------------------------------------//
MOAIAttrLink* MOAINode::AffirmPullLink ( int attrID ) {

	MOAIAttrLink* link = this->mPullAttrLinks;
	for ( ; link; link = link->mNextInDest ) {
		if ( link->mDestAttrID == ( u32 )attrID ) return link;
	}
	
	link = new MOAIAttrLink ();
	
	link->mDestNode = this;
	link->mDestAttrID = attrID;
	link->mDestAttrExists = this->AttrExists ( attrID );
	
	link->mNextInDest = this->mPullAttrLinks;
	this->mPullAttrLinks = link;
	
	return link;
}

//----------------------------------------------------------------//
void MOAINode::AffirmPushLink ( MOAIAttrLink& link, int attrID ) {

	if ( link.mSourceNode ) {
		if ( link.mSourceNode == this ) return;
		link.mSourceNode->ClearPushLink ( link );
	}

	if ( !this->AttrExists ( attrID )) {
		attrID = NULL_ATTR;
	}

	link.mSourceNode = this;
	link.mSourceAttrID = attrID;
	
	link.mNextInSource = this->mPushAttrLinks;
	this->mPushAttrLinks = &link;
}

//----------------------------------------------------------------//
void MOAINode::ClearAttrLink ( int attrID ) {

	this->ClearPullLink ( attrID );
}

//----------------------------------------------------------------//
void MOAINode::ClearDependency ( MOAINode& srcNode ) {

	MOAIAttrLink* cursor = this->mPullAttrLinks;
	this->mPullAttrLinks = 0;
	
	while ( cursor ) {
		
		MOAIAttrLink* link = cursor;
		cursor = cursor->mNextInDest;
		
		if (( link->mSourceAttrID == NULL_ATTR ) && ( link->mSourceNode == &srcNode )) {
			link->mSourceNode->ClearPushLink ( *link );
			delete link;
		}
		else {
			link->mNextInDest = this->mPullAttrLinks;
			this->mPullAttrLinks = link;
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::ClearPullLink ( int attrID ) {

	MOAIAttrLink* cursor = this->mPullAttrLinks;
	this->mPullAttrLinks = 0;
	
	while ( cursor ) {
		
		MOAIAttrLink* link = cursor;
		cursor = cursor->mNextInDest;
		
		if ( link->mDestAttrID == ( u32 )attrID ) {
			link->mSourceNode->ClearPushLink ( *link );
			delete link;
		}
		else {
			link->mNextInDest = this->mPullAttrLinks;
			this->mPullAttrLinks = link;
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::ClearPullLink ( MOAIAttrLink& link ) {

	MOAIAttrLink* cursor = this->mPullAttrLinks;
	this->mPullAttrLinks = 0;
	
	while ( cursor ) {
		MOAIAttrLink* next = cursor->mNextInDest;
		
		if ( cursor != &link ) {
			cursor->mNextInDest = this->mPullAttrLinks;
			this->mPullAttrLinks = cursor;
		}
		cursor = next;
	}
}

//----------------------------------------------------------------//
void MOAINode::ClearPushLink ( MOAIAttrLink& link ) {

	MOAIAttrLink* cursor = this->mPushAttrLinks;
	this->mPushAttrLinks = 0;
	
	while ( cursor ) {
		MOAIAttrLink* next = cursor->mNextInSource;
		
		if ( cursor != &link ) {
			cursor->mNextInSource = this->mPushAttrLinks;
			this->mPushAttrLinks = cursor;
		}
		cursor = next;
	}
}

//----------------------------------------------------------------//
void MOAINode::DepNodeUpdate () {
	
	if ( this->mState == STATE_SCHEDULED ) {
		this->PullAttributes ();
		this->OnDepNodeUpdate ();
		this->ExtendUpdate ();
	}
	// now we are done
	this->mState = STATE_ACTIVE;
}

//----------------------------------------------------------------//
void MOAINode::ExtendUpdate () {

	MOAIAttrLink* link = this->mPushAttrLinks;
	for ( ; link ; link = link->mNextInSource ) {
		link->mDestNode->ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
void MOAINode::ForceUpdate () {

	this->DepNodeUpdate ();
}

//----------------------------------------------------------------//
bool MOAINode::IsNodeUpstream ( MOAINode* node ) {

	MOAINode* cursor = this->mPrev;
	for ( ; cursor; cursor = cursor->mPrev ) {
		if ( cursor == node ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
MOAINode::MOAINode () :
	mPullAttrLinks ( 0 ),
	mPushAttrLinks ( 0 ),
	mState ( STATE_IDLE ),
	mPrev ( 0 ),
	mNext ( 0 ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAINode::~MOAINode () {

	while ( this->mPullAttrLinks ) {
		MOAIAttrLink* link = this->mPullAttrLinks;
		this->mPullAttrLinks = link->mNextInDest;
		
		if ( link->mSourceNode ) {
			link->mSourceNode->ClearPushLink ( *link );
		}
		delete link;
	}
	
	while ( this->mPushAttrLinks ) {
		MOAIAttrLink* link = this->mPushAttrLinks;
		this->mPushAttrLinks = link->mNextInSource;
		
		if ( link->mDestNode ) {
			link->mDestNode->ClearPullLink ( *link );
		}
		delete link;
	}
}

//----------------------------------------------------------------//
void MOAINode::OnDepNodeUpdate () {
}

//----------------------------------------------------------------//
void MOAINode::PullAttributes () {

	USAttrGetter getter;
	USAttrSetter setter;

	MOAIAttrLink* link = this->mPullAttrLinks;	
	for ( ; link ; link = link->mNextInDest ) {
		
		if ( link->mSourceNode->mState == STATE_SCHEDULED ) {
			link->mSourceNode->DepNodeUpdate ();
		}
		
		if ( link->mDestAttrExists && ( link->mSourceAttrID != NULL_ATTR )) {
			link->mSourceNode->ApplyAttrOp ( link->mSourceAttrID, getter );
			setter.Copy ( getter );
			this->ApplyAttrOp ( link->mDestAttrID, setter );
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAINode::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "clearAttrLink",			_clearAttrLink },
		{ "clearDependency",		_clearDependency },
		{ "forceUpdate",			_forceUpdate },
		{ "getAttr",				_getAttr },
		{ "moveAttr",				_moveAttr },
		{ "scheduleUpdate",			_scheduleUpdate },
		{ "seekAttr",				_seekAttr },
		{ "setAttr",				_setAttr },
		{ "setAttrLink",			_setAttrLink },
		{ "setDependency",			_setDependency },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINode::ScheduleUpdate () {
	
	// add to the list if not already in it
	if ( this->mState == STATE_IDLE ) {

		// push us at the end of the list
		MOAINodeMgr::Get ().PushBack ( *this );
		
		// activate source nodes
		MOAIAttrLink* link = this->mPullAttrLinks;
		for ( ; link ; link = link->mNextInDest ) {
			link->mSourceNode->Activate ( *this );
		}
	}
	this->mState = STATE_SCHEDULED;
}

//----------------------------------------------------------------//
void MOAINode::SetAttrLink ( int attrID, MOAINode* srcNode, int srcAttrID ) {

	if ( attrID == ( int )NULL_ATTR ) return;
	
	// if no source node, clear the link and return
	if ( !srcNode ) {
		this->ClearAttrLink ( attrID );
		return;
	}
	
	// affirm the link
	MOAIAttrLink* link = this->AffirmPullLink ( attrID );
	srcNode->AffirmPushLink ( *link, srcAttrID );
	
	// handle activation edge case
	this->ActivateOnLink ( *srcNode );
}

//----------------------------------------------------------------//
void MOAINode::SetDependency ( MOAINode& srcNode ) {

	// bail if bad source node
	if ( &srcNode == this ) return;
	
	// affirm the link
	this->AffirmDependency ( srcNode );
	
	// handle activation edge case
	this->ActivateOnLink ( srcNode );
}
