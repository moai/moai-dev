// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

#include <moai-sim/MOAIVectorTesselator.h>

class MOAIVertexFormat;
class SafeTesselator;

#define MOAI_VECTOR_SHAPE_DECL_TYPE(type)	\
	u8 GetType () const { return type; }

//================================================================//
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	static const int	NVP						= 3;

	MOAIVectorStyle		mStyle;
	bool				mCanGroup;

	//----------------------------------------------------------------//
	void				CopyBoundaries				( SafeTesselator& dest, SafeTesselator* src );
	void				Stroke						( SafeTesselator& tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior );
	void				StrokeBoundaries			( SafeTesselator& tess, SafeTesselator& outline, float width, bool forward, bool interior );

public:

	enum {
		COMBO,
		ELLIPSE,
		POLY,
		RECT,
		UNKNOWN,
	};

	friend class MOAIVectorTesselator;
	friend class MOAIVectorTesselatorWriter;

	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	
	//----------------------------------------------------------------//
	virtual int					AddFillContours				( SafeTesselator& tess ); // TODO: should add status codes to make the meaning ot these return values clear
	virtual int					AddStrokeContours			( SafeTesselator& tess, bool inside, bool outside );
	bool						CanGroup					();
	
	static MOAIVectorShape*		Create						( u32 type );
	
	virtual u8					GetType						() const = 0;
	
	virtual bool				GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool				IsClosed					() = 0;
								MOAIVectorShape				();
	virtual						~MOAIVectorShape			();
	
	virtual void				Read						( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) = 0;
	
	virtual bool				SetVertices					( const ZLVec2D* vertices, u32 total, bool closed );
	
	virtual int					Tesselate					( MOAIVectorTesselator& drawing, MOAIRegion& region, u32 flags );
	virtual int					Tesselate					( MOAIVectorTesselator& drawing, SafeTesselator& tess, u32 flags );
	virtual int					Tesselate					( MOAIVectorTesselator& drawing, ZLStream& vertexStream, ZLStream& indexStream, MOAIVertexFormat& format, u32 flags );
	
	virtual void				Write						( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) const = 0;
};

#endif
