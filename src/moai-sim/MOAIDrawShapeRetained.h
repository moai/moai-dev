// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWSHAPERETAINED_H
#define	MOAIDRAWSHAPERETAINED_H

#include <moai-sim/MOAIAbstractDrawShape.h>

//================================================================//
// MOAIDrawShapeRetained
//================================================================//
// TODO: doxygen
class MOAIDrawShapeRetained :
	public virtual MOAIAbstractDrawShape {
protected:

	enum {
		CMD_DONE,
		CMD_LINE,
		CMD_PEN_COLOR,
		CMD_PEN_WIDTH,
		CMD_POINT,
		CMD_TRIANGLE,
	};

	ZLMemStream		mStream;

	//----------------------------------------------------------------//
	void		MOAIDrawShape_DrawLine					( float x0, float y0, float z0, float x1, float y1, float z1 );
	void		MOAIDrawShape_DrawPoint					( float x, float y, float z = 0.0f );
	void		MOAIDrawShape_DrawTriangleFill			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void		MOAIDrawShape_SetPenColor				( u32 color );
	void		MOAIDrawShape_SetPenWidth				( float width );

public:

	//----------------------------------------------------------------//
	void		DrawShapeStream				();
	size_t		GetShapeStreamSize			();
				MOAIDrawShapeRetained		();
				~MOAIDrawShapeRetained		();
	void		ResetShapeStream			();
};

#endif
