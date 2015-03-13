// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAISim.h>

// TODO: remove when setParent is removed
#include <moai-sim/MOAIProp.h>

//================================================================//
// MOAIDepLink
//================================================================//
class MOAIDepLink {
private:

	friend class MOAINode;

	// don't need smart pointers; either node's destructor will delete link
	MOAINode*					mSourceNode;
	MOAINode*					mDestNode;

	// sibling pointers for the two singly linked lists
	MOAIDepLink*				mNextInSource;
	MOAIDepLink*				mNextInDest;

	// the attribute mapping
	u32							mSourceAttrID;
	u32							mDestAttrID;

	// cached flag indicating it's safe to pull from source to dest (attribute flags match)
	bool						mPullable;

	//----------------------------------------------------------------//
	MOAIDepLink () :
		mSourceNode ( 0 ),
		mDestNode ( 0 ),
		mNextInSource ( 0 ),
		mNextInDest ( 0 ),
		mSourceAttrID ( MOAIAttrOp::NULL_ATTR ),
		mDestAttrID ( MOAIAttrOp::NULL_ATTR ),
		mPullable ( false ) {
	}

	//----------------------------------------------------------------//
	~MOAIDepLink () {
	}

	//----------------------------------------------------------------//
	void Update () {
		this->mPullable =
			( mSourceAttrID & MOAIAttrOp::ATTR_READ ) &&
			( mDestAttrID & MOAIAttrOp::ATTR_WRITE ) &&
			( mSourceAttrID != MOAIAttrOp::NULL_ATTR );
	}
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clearAttrLink
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

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	clearNodeLink
	@text	Clears a dependency on a foreign node.
	
	@in		MOAINode self
	@in		MOAINode sourceNode
	@out	nil
*/
int MOAINode::_clearNodeLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UU" );

	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 2, true );
	if ( !srcNode ) return 0;
	
	self->ClearNodeLink ( *srcNode );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	forceUpdate
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
/**	@lua	getAttr
	@text	Returns the value of the attribute if it exists or nil if it doesn't.
	
	@in		MOAINode self
	@in		number attrID
	@out	number value
*/
int MOAINode::_getAttr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );

	MOAIAttrOp getter;
	self->ApplyAttrOp ( attrID, getter, MOAIAttrOp::GET );
	
	if ( getter.IsValid ()) {
		lua_pushnumber ( state, getter.GetValue ( 0.0f ));
		return 1;
	}
	
	MOAILog ( L, MOAILogMessages::MOAINode_AttributeNotFound );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getAttrLink
	@text	Returns the link if it exists or nil if it doesn't.
	
	@in		MOAINode self
	@in		number attrID
	@out	MOAINode sourceNode
	@out	number sourceAttrID
*/
int MOAINode::_getAttrLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UN" );

	u32 attrID = state.GetValue < u32 >( 2, 0 );

	MOAIDepLink* link = self->FindAttrLink ( attrID );
	if ( link && link->mSourceNode ) {
		state.Push ( link->mSourceNode );
		if ( link->mSourceAttrID != MOAIAttrOp::NULL_ATTR ) {
			state.Push ( link->mSourceAttrID );
			return 2;
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAINode::_getNodeState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "U" );

	state.Push ( self->mState );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	moveAttr
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
	u32 mode		= state.GetValue < u32 >( 5, ZLInterpolate::kSmooth );
	
	if ( self->CheckAttrExists ( attrID )) {
	
		action->SetLink ( 0, self, attrID, value, mode );
		action->SetSpan ( length );
		action->Start ( MOAISim::Get ().GetActionMgr (), false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	MOAILog ( L, MOAILogMessages::MOAINode_AttributeNotFound );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	scheduleUpdate
	@text	Schedule the node for an update next time the dependency graph
			is processed. Any dependent nodes will also be updated.
	
	@in		MOAINode self
	@out	nil
*/
int MOAINode::_scheduleUpdate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "U" );

	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	seekAttr
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
	if ( self->CheckAttrExists ( attrID )) {
	
		MOAIAttrOp getter;
		self->ApplyAttrOp ( attrID, getter, MOAIAttrOp::GET );
		if ( !getter.IsValid ()) return 0;
		
		float value		= state.GetValue < float >( 3, 0.0f );
		float delay		= state.GetValue < float >( 4, 0.0f );
		u32 mode		= state.GetValue < u32 >( 5, ZLInterpolate::kSmooth );
		
		action->SetLink ( 0, self, attrID, value - getter.GetValue ( 0.0f ), mode );
		
		action->SetSpan ( delay );
		action->Start ( MOAISim::Get ().GetActionMgr (), false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	MOAILog ( L, MOAILogMessages::MOAINode_AttributeNotFound );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setAttr
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
	
	if ( self->CheckAttrExists ( attrID )) {
	
		MOAIAttrOp setter;
		setter.SetValue ( value, MOAIAttrOp::ATTR_TYPE_FLOAT );
	
		self->ClearAttrLink ( attrID );
		self->ApplyAttrOp ( attrID, setter, MOAIAttrOp::SET );
		self->ScheduleUpdate ();
	}
	else {
		MOAILog ( L, MOAILogMessages::MOAINode_AttributeNotFound );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setAttrLink
	@text	Sets a *pull* attribute connecting an attribute in the
			node to an attribute in a foreign node.
	
	@in		MOAINode self
	@in		number attrID				ID of attribute to become dependent of foreign node.
	@in		MOAINode sourceNode			Foreign node.
	@opt	number sourceAttrID			Attribute in foreign node to control value of attribue. Default value is attrID.
	@out	nil
*/
int MOAINode::_setAttrLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UNU" );
	
	u32 attrID = state.GetValue < u32 >( 2, 0 );
	
	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 3, true );
	if ( !srcNode ) return 0;

	u32 srcAttrID = state.GetValue < u32 >( 4, attrID );
	
	if ( srcNode->CheckAttrExists ( srcAttrID )) {
		self->SetAttrLink ( attrID, srcNode, srcAttrID );
		self->ScheduleUpdate ();
		return 0;
	}
	
	MOAILog ( L, MOAILogMessages::MOAINode_AttributeNotFound );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setNodeLink
	@text	Creates a dependency between the node and a foreign node
			without the use of attributes; if the foreign node is updated,
			the dependent node will be updated after.
	
	@in		MOAINode self
	@in		MOAINode sourceNode
	@out	nil
*/
int MOAINode::_setNodeLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAINode, "UU" );
	
	MOAINode* srcNode = state.GetLuaObject < MOAINode >( 2, true );
	if ( !srcNode ) return 0;
	
	self->SetNodeLink ( *srcNode );
	
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
	MOAIDepLink* link = this->mPullLinks;
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
			
			if ( srcNode.IsNodeUpstream ( this ) && this->mState != STATE_UPDATING ) {
				
				depNodeMgr.Remove ( *this );
				depNodeMgr.InsertAfter ( srcNode, *this );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAINode::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );
	UNUSED ( op );
	
	return false;
}

//----------------------------------------------------------------//
bool MOAINode::CheckAttrExists ( u32 attrID ) {

	if ( attrID == MOAIAttrOp::NULL_ATTR ) return false;

	MOAIAttrOp getter;
	this->ApplyAttrOp ( attrID, getter, MOAIAttrOp::CHECK );
	return getter.IsValid ();
}

//----------------------------------------------------------------//
void MOAINode::ClearAttrLink ( int attrID ) {

	MOAIDepLink* cursor = this->mPullLinks;
	this->mPullLinks = 0;

	while ( cursor ) {
		MOAIDepLink* link = cursor;
		cursor = cursor->mNextInDest;
		
		if (( link->mDestAttrID & ~MOAIAttrOp::ATTR_FLAGS_MASK ) == ( u32 )attrID ) {
			link->mSourceNode->RemoveDepLink ( *link );
			delete link;
			this->ScheduleUpdate ();
		}
		else {
			link->mNextInDest = this->mPullLinks;
			this->mPullLinks = link;
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::ClearNodeLink ( MOAINode& srcNode ) {
	
	MOAIDepLink* cursor = this->mPullLinks;
	this->mPullLinks = 0;

	while ( cursor ) {
		MOAIDepLink* link = cursor;
		cursor = cursor->mNextInDest;
		
		if (( link->mDestAttrID == MOAIAttrOp::NULL_ATTR ) && ( link->mSourceNode == &srcNode )) {
			link->mSourceNode->RemoveDepLink ( *link );
			delete link;
		}
		else {
			link->mNextInDest = this->mPullLinks;
			this->mPullLinks = link;
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::DepNodeUpdate () {
	
	if ( this->mState == STATE_SCHEDULED ) {
	
		this->mState = STATE_UPDATING;
		this->PullAttributes ();

		InvokeListenerWithSelf ( EVENT_NODE_PRE_UPDATE );
		this->OnDepNodeUpdate ();
		InvokeListenerWithSelf ( EVENT_NODE_POST_UPDATE );
		
		this->mState = STATE_ACTIVE;
	}
}

//----------------------------------------------------------------//
void MOAINode::ExtendUpdate () {

	MOAIDepLink* link = this->mPushLinks;
	for ( ; link ; link = link->mNextInSource ) {
		link->mDestNode->ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
MOAIDepLink* MOAINode::FindAttrLink ( int attrID ) {

	attrID = attrID & ~MOAIAttrOp::ATTR_FLAGS_MASK;

	MOAIDepLink* link = this->mPullLinks;
	for ( ; link; link = link->mNextInDest ) {
		if ( link->mDestAttrID == ( u32 )attrID ) break;
	}
	return link;
}

//----------------------------------------------------------------//
MOAIDepLink* MOAINode::FindNodeLink ( MOAINode& srcNode ) {

	MOAIDepLink* link = this->mPullLinks;
	for ( ; link; link = link->mNextInDest ) {
		if (( link->mSourceNode == &srcNode ) && ( link->mDestAttrID == MOAIAttrOp::NULL_ATTR )) break;
	}
	return link;
}

//----------------------------------------------------------------//
void MOAINode::ForceUpdate () {

	this->ScheduleUpdate ();
	this->DepNodeUpdate ();
}

//----------------------------------------------------------------//
u32 MOAINode::GetAttrFlags ( u32 attrID ) {

	MOAIAttrOp attrOp;
	this->ApplyAttrOp ( attrID, attrOp, MOAIAttrOp::CHECK );
	return attrOp.GetFlags ();
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
	mPullLinks ( 0 ),
	mPushLinks ( 0 ),
	mState ( STATE_IDLE ),
	mPrev ( 0 ),
	mNext ( 0 ) {
	
	RTTI_SINGLE ( MOAIInstanceEventSource )
}

//----------------------------------------------------------------//
MOAINode::~MOAINode () {
	
	while ( this->mPullLinks ) {
		MOAIDepLink* link = this->mPullLinks;
		this->mPullLinks = link->mNextInDest;
		link->mSourceNode->RemoveDepLink ( *link );
		delete link;
	}
	
	while ( this->mPushLinks ) {
		MOAIDepLink* link = this->mPushLinks;
		this->mPushLinks = link->mNextInSource;
		link->mDestNode->RemoveDepLink ( *link );
		delete link;
	}

	if (( this->mState != STATE_IDLE ) && ( MOAINodeMgr::IsValid ())) {
		MOAINodeMgr::Get ().Remove ( *this );
	}
}

//----------------------------------------------------------------//
void MOAINode::OnDepNodeUpdate () {
}

//----------------------------------------------------------------//
void MOAINode::PullAttributes () {

	MOAIAttrOp attrOp;

	MOAIDepLink* link = this->mPullLinks;	
	for ( ; link ; link = link->mNextInDest ) {
		
		if ( link->mSourceNode->mState == STATE_SCHEDULED ) {
			link->mSourceNode->DepNodeUpdate ();
		}

		if ( link->mPullable ) {
			link->mSourceNode->ApplyAttrOp ( link->mSourceAttrID, attrOp, MOAIAttrOp::GET );
			this->ApplyAttrOp ( link->mDestAttrID, attrOp, MOAIAttrOp::SET );
		}
	}
}

//----------------------------------------------------------------//
bool MOAINode::PullLinkedAttr ( u32 attrID, MOAIAttrOp& attrOp ) {

	MOAIDepLink* link = this->mPullLinks;	
	for ( ; link ; link = link->mNextInDest ) {
		if ((( link->mDestAttrID & ~MOAIAttrOp::ATTR_FLAGS_MASK ) == attrID ) && ( link->mSourceAttrID & MOAIAttrOp::ATTR_READ )) {
			link->mSourceNode->ApplyAttrOp ( link->mSourceAttrID, attrOp, MOAIAttrOp::GET );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAINode::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_UPDATE",		( u32 )EVENT_NODE_PRE_UPDATE ); // TODO: deprecate
	
	state.SetField ( -1, "EVENT_NODE_PRE_UPDATE",	( u32 )EVENT_NODE_PRE_UPDATE );
	state.SetField ( -1, "EVENT_NODE_POST_UPDATE",	( u32 )EVENT_NODE_POST_UPDATE );
}

//----------------------------------------------------------------//
void MOAINode::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "clearAttrLink",			_clearAttrLink },
		{ "clearNodeLink",			_clearNodeLink },
		{ "forceUpdate",			_forceUpdate },
		{ "getAttr",				_getAttr },
		{ "getAttrLink",			_getAttrLink },
		{ "getNodeState",			_getNodeState },
		{ "moveAttr",				_moveAttr },
		{ "scheduleUpdate",			_scheduleUpdate },
		{ "seekAttr",				_seekAttr },
		{ "setAttr",				_setAttr },
		{ "setAttrLink",			_setAttrLink },
		{ "setNodeLink",			_setNodeLink },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINode::RemoveDepLink ( MOAIDepLink& link ) {

	if ( link.mSourceNode == this ) {
	
		MOAIDepLink* cursor = this->mPushLinks;
		this->mPushLinks = 0;
		
		while ( cursor ) {
			MOAIDepLink* next = cursor->mNextInSource;
			if ( cursor != &link ) {
				cursor->mNextInSource = this->mPushLinks;
				this->mPushLinks = cursor;
			}
			cursor = next;
		}
		link.mNextInSource = 0;
	}
	else {
	
		MOAIDepLink* cursor = this->mPullLinks;
		this->mPullLinks = 0;
		
		while ( cursor ) {
			MOAIDepLink* next = cursor->mNextInDest;
			if ( cursor != &link ) {
				cursor->mNextInDest = this->mPullLinks;
				this->mPullLinks = cursor;
			}
			cursor = next;
		}
		link.mNextInDest = 0;
	}
	link.Update ();
}

//----------------------------------------------------------------//
void MOAINode::ScheduleUpdate () {
	
	if ( MOAINodeMgr::IsValid () ) {
	
		if (( this->mState == STATE_IDLE ) || ( this->mState == STATE_ACTIVE )) {
		
			// add to the list if not already in it
			if ( this->mState == STATE_IDLE ) {
				this->mState = STATE_SCHEDULED;

				// push us at the end of the list
				MOAINodeMgr::Get ().PushBack ( *this );
				
				// activate source nodes
				MOAIDepLink* link = this->mPullLinks;
				for ( ; link ; link = link->mNextInDest ) {
					link->mSourceNode->Activate ( *this );
				}
			}
			this->mState = STATE_SCHEDULED;

			this->ExtendUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAINode::SetAttrLink ( int attrID, MOAINode* srcNode, int srcAttrID ) {
	
	if ( attrID == ( int )MOAIAttrOp::NULL_ATTR ) return;
	
	if (( srcNode ) && ( !srcNode->CheckAttrExists ( srcAttrID ))) {
		srcNode = 0;
	}
	
	if (( !srcNode ) || ( srcAttrID == ( int )MOAIAttrOp::NULL_ATTR )) {
		attrID |= this->GetAttrFlags ( attrID );
		this->ClearAttrLink ( attrID );
		return;
	}
	
	attrID |= this->GetAttrFlags ( attrID );
	srcAttrID |= srcNode->GetAttrFlags ( srcAttrID );
	
	MOAIDepLink* link = this->FindAttrLink ( attrID );
	
	if ( link ) {
		if ( link->mSourceNode != srcNode ) {
			link->mSourceNode->RemoveDepLink ( *link );
			link->mNextInSource = srcNode->mPushLinks;
			srcNode->mPushLinks = link;
		}
	}
	else {
		link = new MOAIDepLink ();
		
		link->mDestNode = this;
		link->mDestAttrID = attrID;
		
		link->mNextInSource = srcNode->mPushLinks;
		srcNode->mPushLinks = link;
		
		link->mNextInDest = this->mPullLinks;
		this->mPullLinks = link;
	}

	link->mSourceNode = srcNode;
	link->mSourceAttrID = srcAttrID;
	link->Update ();
	
	this->ActivateOnLink ( *srcNode );
}

//----------------------------------------------------------------//
void MOAINode::SetNodeLink ( MOAINode& srcNode ) {
	
	if ( &srcNode == this ) return;
	
	MOAIDepLink* link = this->FindNodeLink ( srcNode );
	
	if ( !link ) {
		link = new MOAIDepLink ();

		link->mSourceNode = &srcNode;
		link->mDestNode = this;
		
		link->mNextInSource = srcNode.mPushLinks;
		srcNode.mPushLinks = link;
		
		link->mNextInDest = this->mPullLinks;
		this->mPullLinks = link;
		link->mPullable = true;
		
		this->ActivateOnLink ( srcNode );
	}
}
