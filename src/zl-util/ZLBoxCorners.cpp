// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBox.h>
#include <zl-util/ZLBoxCorners.h>
#include <zl-util/ZLPrism.h>
#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLBoxCorners
//================================================================//

//----------------------------------------------------------------//
void ZLBoxCorners::Project ( const ZLMatrix4x4& mtx ) {

	mtx.Project ( this->mPoints [ MIN_LT_POINT ]);
	mtx.Project ( this->mPoints [ MIN_RT_POINT ]);
	mtx.Project ( this->mPoints [ MIN_RB_POINT ]);
	mtx.Project ( this->mPoints [ MIN_LB_POINT ]);
	mtx.Project ( this->mPoints [ MAX_LT_POINT ]);
	mtx.Project ( this->mPoints [ MAX_RT_POINT ]);
	mtx.Project ( this->mPoints [ MAX_RB_POINT ]);
	mtx.Project ( this->mPoints [ MAX_LB_POINT ]);
}

//----------------------------------------------------------------//
void ZLBoxCorners::Transform ( const ZLMatrix4x4& mtx ) {

	mtx.Transform ( this->mPoints [ MIN_LT_POINT ]);
	mtx.Transform ( this->mPoints [ MIN_RT_POINT ]);
	mtx.Transform ( this->mPoints [ MIN_RB_POINT ]);
	mtx.Transform ( this->mPoints [ MIN_LB_POINT ]);
	mtx.Transform ( this->mPoints [ MAX_LT_POINT ]);
	mtx.Transform ( this->mPoints [ MAX_RT_POINT ]);
	mtx.Transform ( this->mPoints [ MAX_RB_POINT ]);
	mtx.Transform ( this->mPoints [ MAX_LB_POINT ]);
}

//----------------------------------------------------------------//
ZLBoxCorners::ZLBoxCorners () {
}

//----------------------------------------------------------------//
ZLBoxCorners::ZLBoxCorners ( const ZLBox& box ) {

	this->mPoints [ MIN_LT_POINT ] = ZLVec3D ( box.mMin.mX, box.mMax.mY, box.mMin.mZ );
	this->mPoints [ MIN_RT_POINT ] = ZLVec3D ( box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	this->mPoints [ MIN_RB_POINT ] = ZLVec3D ( box.mMax.mX, box.mMin.mY, box.mMin.mZ );
	this->mPoints [ MIN_LB_POINT ] = box.mMin;
	
	this->mPoints [ MAX_LT_POINT ] = ZLVec3D ( box.mMin.mX, box.mMax.mY, box.mMax.mZ );
	this->mPoints [ MAX_RT_POINT ] = box.mMax;
	this->mPoints [ MAX_RB_POINT ] = ZLVec3D ( box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->mPoints [ MAX_LB_POINT ] = ZLVec3D ( box.mMin.mX, box.mMin.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
ZLBoxCorners::ZLBoxCorners ( const ZLPrism& prism ) {

	this->mPoints [ MIN_LT_POINT ] = prism.mLoc + prism.mYAxis;
	this->mPoints [ MIN_RT_POINT ] = this->mPoints [ MIN_LT_POINT ] + prism.mXAxis;
	this->mPoints [ MIN_RB_POINT ] = prism.mLoc + prism.mXAxis;
	this->mPoints [ MIN_LB_POINT ] = prism.mLoc;
	
	this->mPoints [ MAX_LT_POINT ] = this->mPoints [ MIN_LT_POINT ] + prism.mZAxis;
	this->mPoints [ MAX_RT_POINT ] = this->mPoints [ MIN_RT_POINT ] + prism.mZAxis;
	this->mPoints [ MAX_RB_POINT ] = this->mPoints [ MIN_RB_POINT ] + prism.mZAxis;
	this->mPoints [ MAX_LB_POINT ] = this->mPoints [ MIN_LB_POINT ] + prism.mZAxis;
}
