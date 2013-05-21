// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeMgr.h>

//================================================================//
// MOAINodeMgr
//================================================================//

//----------------------------------------------------------------//
void MOAINodeMgr::InsertAfter ( MOAINode& cursor, MOAINode& node ) {

	if ( cursor.mNext ) {
	
		node.mPrev = &cursor;
		node.mNext = cursor.mNext;
		
		node.mPrev->mNext = &node;
		node.mNext->mPrev = &node;
		
		node.Retain ();
	}
	else {
		this->PushBack ( node );
	}
}

//----------------------------------------------------------------//
void MOAINodeMgr::InsertBefore ( MOAINode& cursor, MOAINode& node ) {

	if ( cursor.mPrev ) {
		node.mPrev = cursor.mPrev;
		node.mNext = &cursor;
		
		node.mPrev->mNext = &node;
		node.mNext->mPrev = &node;
		
		node.Retain ();
	}
	else {
		this->PushFront ( node );
	}
}

//----------------------------------------------------------------//
void MOAINodeMgr::PushBack ( MOAINode& node ) {

	node.mNext = 0;
	node.mPrev = 0;

	if ( !this->mUpdateListHead ) {
		this->mUpdateListHead = &node;
		this->mUpdateListTail = &node;
	}
	else {
		node.mPrev = this->mUpdateListTail;
		this->mUpdateListTail->mNext = &node;
		this->mUpdateListTail = &node;
	}
	
	node.Retain ();
}

//----------------------------------------------------------------//
void MOAINodeMgr::PushFront ( MOAINode& node ) {

	node.mNext = 0;
	node.mPrev = 0;

	if ( !this->mUpdateListHead ) {
		this->mUpdateListHead = &node;
		this->mUpdateListTail = &node;
	}
	else {
		node.mNext = this->mUpdateListHead;
		this->mUpdateListHead->mPrev = &node;
		this->mUpdateListHead = &node;
	}
	
	node.Retain ();
}

//----------------------------------------------------------------//
void MOAINodeMgr::Remove ( MOAINode& node ) {

	if ( node.mNext ) {
		node.mNext->mPrev = node.mPrev;
	}
	else {
		this->mUpdateListTail = node.mPrev;
	}

	if ( node.mPrev ) {
		node.mPrev->mNext = node.mNext;
	}
	else {
		this->mUpdateListHead = node.mNext;
	}
	
	node.Release ();
}

//----------------------------------------------------------------//
void MOAINodeMgr::Update () {

	MOAINode* node = this->mUpdateListHead;
	for ( ; node ; node = node->mNext ) {
		node->DepNodeUpdate ();
	}
	
	// TODO: fix this up later
	node = this->mUpdateListHead;
	while ( node ) {
		
		MOAINode* temp = node;
		node = node->mNext;
		
		temp->mState = MOAINode::STATE_IDLE;
		temp->Release ();
	}
	
	this->mUpdateListHead = 0;
	this->mUpdateListTail = 0;
}

//----------------------------------------------------------------//
MOAINodeMgr::MOAINodeMgr () :
	mUpdateListHead ( 0 ),
	mUpdateListTail ( 0 ) {
}

//----------------------------------------------------------------//
MOAINodeMgr::~MOAINodeMgr () {

	MOAINode* cursor = this->mUpdateListHead;
	while ( cursor ) {
		MOAINode* node = cursor;
		cursor = cursor->mNext;
		
		node->mState = MOAINode::STATE_IDLE;
		node->Release ();
	}
}
