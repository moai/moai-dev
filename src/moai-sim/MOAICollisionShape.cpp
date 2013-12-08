// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
const ZLRect& MOAICollisionShape::GetRect () const {

	return *( ZLRect* )this->mRectBuffer;
}

//----------------------------------------------------------------//
const ZLQuad& MOAICollisionShape::GetQuad () const {

	return *( ZLQuad* )this->mQuadBuffer;
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
void MOAICollisionShape::Set ( const ZLRect& rect ) {

	memcpy ( this->mRectBuffer, &rect, sizeof ( ZLRect ));
	this->mType = RECT;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const ZLQuad& quad ) {

	memcpy ( this->mQuadBuffer, &quad, sizeof ( ZLQuad ));
	this->mType = QUAD;
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const ZLRect& s0, const ZLRect& s1 ) {

	return s0.Overlap ( s1 );
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const ZLQuad& s0, const ZLRect& s1 ) {

	return s0.Overlap ( s1 );
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const ZLQuad& s0, const ZLQuad& s1 ) {

	return s0.Overlap ( s1 );
}