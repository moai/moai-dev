// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORRECT_H
#define	MOAIVECTORRECT_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorRect
//================================================================//
class MOAIVectorRect :
	public MOAIVectorShape {
private:

	ZLRect			mRect;

	enum {
		FASTTRACK_OK,
		FASTTRACK_FALLBACK,
		FASTTRACK_SKIP,
	};

	//----------------------------------------------------------------//
	int				CheckFastTrack			( MOAIVectorTesselator& drawing, u32 flags );

public:
	
	MOAI_VECTOR_SHAPE_DECL_TYPE ( MOAIVectorShape::RECT )
	
	//----------------------------------------------------------------//
	int				AddFillContours			( SafeTesselator& tess );
	int				AddStrokeContours		( SafeTesselator& tess, bool inside, bool outside );
	void			Init					( float xMin, float yMin, float xMax, float yMax );
	bool			IsClosed				();
					MOAIVectorRect			();
					~MOAIVectorRect			();
	void			Read					( ZLStream& stream, MOAIVectorTesselatorWriter& writer );
	int				Tesselate				( MOAIVectorTesselator& drawing, MOAIRegion& region, u32 flags );
	int				Tesselate				( MOAIVectorTesselator& drawing, ZLStream& vertexStream, ZLStream& indexStream, MOAIVertexFormat& format, u32 flags );
	void			Write					( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) const;
};

#endif
