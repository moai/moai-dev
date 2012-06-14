// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICollisionShape.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
const USRect& MOAICollisionShape::GetRect () const {

	return *( USRect* )this->mRectBuffer;
}

//----------------------------------------------------------------//
const USQuad& MOAICollisionShape::GetQuad () const {

	return *( USQuad* )this->mQuadBuffer;
}

//----------------------------------------------------------------//
MOAICollisionShape::MOAICollisionShape () :
	mType ( NONE ) {
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const MOAICollisionShape& shape ) const {

	switch ( this->mType ) {
	
		case RECT: {
			switch ( shape.mType ) {
				case RECT: MOAICollisionShape::Overlap ( this->GetRect (), shape.GetRect ());
				case QUAD: MOAICollisionShape::Overlap ( shape.GetQuad (), this->GetRect ());
			}
			break;
		}
		case QUAD: {
			switch ( shape.mType ) {
				case RECT: MOAICollisionShape::Overlap ( this->GetQuad (), shape.GetRect ());
				case QUAD: MOAICollisionShape::Overlap ( this->GetQuad (), shape.GetQuad ());
			}
			break;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const USRect& rect ) {

	memcpy ( this->mRectBuffer, &rect, sizeof ( USRect ));
	this->mType = RECT;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const USQuad& quad ) {

	memcpy ( this->mQuadBuffer, &quad, sizeof ( USQuad ));
	this->mType = QUAD;
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const USRect& s0, const USRect& s1 ) {

	return s0.Overlap ( s1 );
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const USQuad& s0, const USRect& s1 ) {

	return s0.Overlap ( s1 );
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const USQuad& s0, const USQuad& s1 ) {

	return s0.Overlap ( s1 );
}