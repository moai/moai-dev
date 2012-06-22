//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAIMultiShape2D.h>

//================================================================//
// MOAIMultiShape2D
//================================================================//

//----------------------------------------------------------------//
void MOAIMultiShape2D::Clear () {

	this->mType = NONE;
}

//----------------------------------------------------------------//
bool MOAIMultiShape2D::Intersect ( const MOAIMultiShape2D& shape, USRect& result ) const {

	if ( this->mType == QUAD ) {
	
		if ( shape.mType == QUAD ) {
			return this->mQuad.Intersect ( shape.mQuad, result );
		}
		
		if ( shape.mType == RECT ) {
			return this->mQuad.Intersect ( shape.mRect, result );
		}
	}
	
	if ( this->mType == RECT ) {
	
		if ( shape.mType == QUAD ) {
			return shape.mQuad.Intersect ( this->mRect, result );
		}
		
		if ( shape.mType == RECT ) {
			return this->mRect.Intersect ( shape.mRect, result );
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
MOAIMultiShape2D::MOAIMultiShape2D () :
	mType ( NONE ) {
}

//----------------------------------------------------------------//
MOAIMultiShape2D::~MOAIMultiShape2D () {
}

//----------------------------------------------------------------//
bool MOAIMultiShape2D::Overlap ( const MOAIMultiShape2D& shape ) const {
	
	if ( this->mType == QUAD ) {
	
		if ( shape.mType == QUAD ) {
			return this->mQuad.Overlap ( shape.mQuad );
		}
		
		if ( shape.mType == RECT ) {
			return this->mQuad.Overlap ( shape.mRect );
		}
	
	}
	
	if ( this->mType == RECT ) {
	
		if ( shape.mType == QUAD ) {
			return shape.mQuad.Overlap ( this->mRect );
		}
		
		if ( shape.mType == RECT ) {
			return this->mRect.Overlap ( shape.mRect );
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
void MOAIMultiShape2D::SetQuad ( const USQuad& quad ) {

	this->mType = QUAD;
	this->mQuad = quad;
}

//----------------------------------------------------------------//
void MOAIMultiShape2D::SetRect ( const USRect& rect ) {

	this->mType = RECT;
	this->mRect = rect;
}
