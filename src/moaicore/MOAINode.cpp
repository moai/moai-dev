// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIForceAction.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeHarness.h>
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
/**	@brief <tt>clearAttrLink ( self, attributeID )</tt>\n
\n
	Clears an attribute link.
	@param self (in)
	@param attributeID (in)
*/
int MOAINode::_clearAttrLink ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );
	self->ClearAttrLink ( attrID );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAINode::_clearDependency ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UU" );

	MOAINode* srcNode = state.GetLuaData < MOAINode >( 2 );
	if ( !srcNode ) return 0;
	
	self->ClearDependency ( *srcNode );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAINode::_countAttrs ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "U" );

	lua_pushnumber ( state, self->CountAttributes ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( value ) getAttr ( self, attributeID )</tt>\n
\n
	Returns the value of the attribute if it exists or nil if it doesn't.
	@param self (in)
	@param attributeID (in)
	@param value (out)
*/
int MOAINode::_getAttr ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );

	USAttrGetter getter;
	self->ApplyAttrOp ( attrID, getter );
	
	if ( getter.IsValid ()) {
		lua_pushnumber ( state, getter.Get < float >());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( action ) moveAttr ( self, attributeID, value, delay, [mode])</tt>\n
\n
	Returns a MOAIForceAction configured to add a value to an attribute over a delay.
	@param self (in)
	@param attributeID (in)
	@param value (in)
	@param delay (in)
	@param [mode] (in)
	@param action (out)
*/
int MOAINode::_moveAttr ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UNNN" )

	MOAIForceAction* action = new MOAIForceAction ();
	action->ReserveForces ( 1 );
	
	u32 attrID		= state.GetValue < u32 >( 2, 0 );
	float value		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, attrID, value, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAINode::_scheduleUpdate ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "U" );

	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( action ) moveAttr ( self, attributeID, value, delay, [mode])</tt>\n
\n
	Returns a MOAIForceAction configured to add a delta to an attribute over a delay to reach a value.
	@param self (in)
	@param attributeID (in)
	@param value (in)
	@param delay (in)
	@param [mode] (in)
	@param action (out)
*/
int MOAINode::_seekAttr ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UNNN" )

	MOAIForceAction* action = new MOAIForceAction ();
	action->ReserveForces ( 1 );
	
	u32 attrID		= state.GetValue < u32 >( 2, 0 );
	
	USAttrGetter getter;
	self->ApplyAttrOp ( attrID, getter );
	if ( !getter.IsValid ()) return 0;
	
	float value		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, attrID, value - getter.Get < float >(), mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setAttr( attributeID, value )</tt>\n
	\n
	Sets an attribute of a Moai object.
	@param attributeID Property of this object to set.
	@param value Value to set to.
*/
int MOAINode::_setAttr ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UNN" );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	float value = state.GetValue < float >( 3, 0.0f );

	USAttrSetter setter;
	setter.Set ( value );
	
	self->ClearAttrLink ( attrID );
	self->ApplyAttrOp ( attrID, setter );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setAttrLink ( attributeID, sourceNode, sourceAttribute )</tt>\n
	\n
	Sets an attribute of a Moai object.
	@param attributeID Property of this object to link to.
	\li \c ATTR_X_LOC - Location of the object along its X-axis.
	\li \c ATTR_Y_LOC - Location of the object along its Y-axis.
	\li \c ATTR_Z_ROT - Rotation of the object about its origin.
	\li \c ATTR_X_SCL - Scaling of the object along its X-axis.
	\li \c ATTR_Y_SCL - Scaling of the object along its Y-axis.
	\li \c ATTR_BRUSH_ID - Index of a MOAIBrush in a MOAISpriteLibrary.
	@param sourceNode Object to link to.
	@param sourceAttribute Attribute of the object you are linking to to connect to.
	\li \c ATTR_TIME - The time of a MOAITimer object.
	\li \c ATTR_VALUE - The value parameter of a MOAIAnimCurve key.
*/
int MOAINode::_setAttrLink ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UNUN" );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	
	MOAINode* srcNode = state.GetLuaData < MOAINode >( 3 );
	if ( !srcNode ) return 0;

	u32 srcAttrID = state.GetValue < u32 >( 4, 0 );

	self->SetAttrLink ( attrID, srcNode, srcAttrID );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAINode::_setDependency ( lua_State* L ) {
	LUA_SETUP ( MOAINode, "UU" );
	
	MOAINode* srcNode = state.GetLuaData < MOAINode >( 2 );
	if ( !srcNode ) return 0;
	
	self->SetDependency ( *srcNode );
	
	return 0;
}

//================================================================//
// MOAINode
//================================================================//

//----------------------------------------------------------------//
void MOAINode::Activate ( MOAINode& activator ) {

	// bail if we're already active
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
bool MOAINode::CanHarness () {

	return false;
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
}

//----------------------------------------------------------------//
void MOAINode::ExtendUpdate () {

	MOAIAttrLink* link = this->mPushAttrLinks;
	for ( ; link ; link = link->mNextInSource ) {
		link->mDestNode->ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
void MOAINode::Harness ( MOAINodeHarness& harness ) {
	UNUSED ( harness );
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
	
	RTTI_SINGLE ( USLuaData )
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
	
	LuaReg regTable [] = {
		{ "clearAttrLink",			_clearAttrLink },
		{ "clearDependency",		_clearDependency },
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
	
	// bail if we're already active or scheduled
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
