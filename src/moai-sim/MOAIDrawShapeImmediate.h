// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWSHAPEIMMEDIATE_H
#define	MOAIDRAWSHAPEIMMEDIATE_H

#include <moai-sim/MOAIAbstractDrawShape.h>

//================================================================//
// MOAIDrawShapeImmediate
//================================================================//
// TODO: doxygen
class MOAIDrawShapeImmediate :
	public virtual MOAIAbstractDrawShape {
protected:

	//----------------------------------------------------------------//
	void		MOAIAbstractDrawShape_DrawLine				( float x0, float y0, float z0, float x1, float y1, float z1 );
	void		MOAIAbstractDrawShape_DrawPoint				( float x, float y, float z = 0.0f );
	void		MOAIAbstractDrawShape_DrawTriangleFill		( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void		MOAIAbstractDrawShape_SetPenColor			( u32 color );
	void		MOAIAbstractDrawShape_SetPenWidth			( float width );

public:

	//----------------------------------------------------------------//
				MOAIDrawShapeImmediate					();
				~MOAIDrawShapeImmediate					();
};

#endif
