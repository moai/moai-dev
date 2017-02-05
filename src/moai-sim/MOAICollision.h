// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISION_H
#define	MOAICOLLISION_H

class MOAIContactPointAccumulator2D;

//================================================================//
// MOAICollision
//================================================================//
namespace MOAICollision {

	//----------------------------------------------------------------//
	void		FindContactPoints		( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, MOAIContactPointAccumulator2D& accumulator );
	bool		FindOverlapInterval		( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, ZLVec2D& interval );
	//bool		Collide					( ZLVec2D* poly0, size_t nPoly0, ZLVec2D* poly1, size_t nPoly1, ZLVec2D velocity, ZLVec2D& n, float& t );
};

#endif
