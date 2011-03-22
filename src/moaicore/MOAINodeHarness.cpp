// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeHarness.h>

//================================================================//
// MOAIManipulator
//================================================================//

//----------------------------------------------------------------//
void MOAIManipulator::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_X_LOC:
			this->mLoc.mX = attrOp.Op ( this->mLoc.mX );
			return;
		case ATTR_Y_LOC:
			this->mLoc.mY = attrOp.Op ( this->mLoc.mY );
			return;
		case ATTR_Z_ROT:
			this->mAngle = attrOp.Op ( this->mAngle );
			return;
		case ATTR_X_SCL:
			this->mScale.mX = attrOp.Op ( this->mScale.mX );
			return;
		case ATTR_Y_SCL:
			this->mScale.mY = attrOp.Op ( this->mScale.mY );
			return;
	}
}

//----------------------------------------------------------------//
u32 MOAIManipulator::CountAttributes () {

	return MOAIManipulator::TOTAL_ATTR;
}

//----------------------------------------------------------------//
MOAIManipulator::MOAIManipulator () {
}

//----------------------------------------------------------------//
MOAIManipulator::~MOAIManipulator () {
}

//================================================================//
// MOAINodeHarness
//================================================================//

//----------------------------------------------------------------//
void MOAINodeHarness::BindNode ( MOAINode* node ) {

	this->UnbindNode ();

	if ( node ) {
		if ( node->CanHarness ()) {
			this->mMOAINode = node;
			node->Harness ( *this );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAINodeHarness::CountAttributes () {

	if ( this->mMOAINode ) {
		return this->mMOAINode->CountAttributes ();
	}
	return 0;
}


//----------------------------------------------------------------//
MOAINodeHarness::MOAINodeHarness () :
	mMOAINode ( 0 ) {
}

//----------------------------------------------------------------//
MOAINodeHarness::~MOAINodeHarness () {
}

//----------------------------------------------------------------//
void MOAINodeHarness::MapManipulatorAttr ( u32 nodeAttrID, u32 manipAttrID ) {

	this->mManipulatorAttrMap.MapAttr ( nodeAttrID, manipAttrID );
}

//----------------------------------------------------------------//
void MOAINodeHarness::ReadManipulator ( MOAIManipulator& manipulator ) {

	if ( this->mMOAINode ) {
		this->mManipulatorAttrMap.ReadAttributes ( *this, manipulator );
	}
}

//----------------------------------------------------------------//
void MOAINodeHarness::SetFlags ( u32 flags ) {
	UNUSED ( flags );
}

//----------------------------------------------------------------//
void MOAINodeHarness::UnbindNode () {

	this->Clear ();
	this->mManipulatorAttrMap.Clear ();
	this->mMOAINode = 0;
}

//----------------------------------------------------------------//
void MOAINodeHarness::WriteManipulator ( MOAIManipulator& manipulator ) {

	if ( this->mMOAINode ) {
		this->mManipulatorAttrMap.WriteAttributes ( *this, manipulator );
	}
}

//----------------------------------------------------------------//
STLString MOAINodeHarness::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mMOAINode );

	return repr;
}
