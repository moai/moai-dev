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
	void		MOAIDrawShape_DrawLine					( float x0, float y0, float z0, float x1, float y1, float z1 );
	void		MOAIDrawShape_DrawPoint					( float x, float y, float z = 0.0f );
	void		MOAIDrawShape_DrawTriangleFill			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void		MOAIDrawShape_SetPenColor				( u32 color );
	void		MOAIDrawShape_SetPenWidth				( float width );

public:

	//----------------------------------------------------------------//
				MOAIDrawShapeImmediate					();
				~MOAIDrawShapeImmediate					();
};

#endif
