// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

struct TESStesselator;
class MOAIVectorDrawing;

//================================================================//
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	ZLColorVec		mFillColor;
	ZLColorVec		mLineColor;
	
	u32				mFillStyle;
	u32				mLineStyle;
	
	u32				mWindingRule;
	bool			mOpen;

public:

	friend class MOAIVectorDrawing;

	enum {
		LINE_NONE,
		LINE_VECTOR,
	};
	
	enum {
		FILL_NONE,
		FILL_SOLID,
	};
	
	GET_SET ( ZLColorVec&, FillColor, mFillColor )
	GET_SET ( ZLColorVec&, LineColor, mLineColor )
	
	GET_SET ( u32, FillStyle, mFillStyle )
	GET_SET ( u32, LineStyle, mLineStyle )
	
	GET_SET ( u32, WindingRule, mWindingRule );
	GET_SET ( bool, Open, mOpen );
	
	//----------------------------------------------------------------//
	virtual bool		GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool		GroupVertices				( MOAIVectorDrawing& drawing, u32 total );
						MOAIVectorShape				();
	virtual				~MOAIVectorShape			();
	virtual void		Tessalate					( MOAIVectorDrawing& drawing );
	virtual void		ToOutline					( TESStesselator* outline ) = 0;
	virtual void		ToTriangles					( TESStesselator* outline, TESStesselator* triangles );
};

#endif
