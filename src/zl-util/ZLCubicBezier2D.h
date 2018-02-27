// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCUBICBEZIER2D_H
#define	ZLCUBICBEZIER2D_H

#include <zl-util/ZLCurve.h>

//================================================================//
// ZLAbstractVertexWriter2D
//================================================================//
// TODO: this should be moved someplace more general
class ZLAbstractVertexWriter2D {
public:

	//----------------------------------------------------------------//
	virtual void		WriteVertex					( const ZLVec2D& v ) = 0;
						ZLAbstractVertexWriter2D	() {};
	virtual				~ZLAbstractVertexWriter2D	() {};
};

//================================================================//
// ZLCubicBezier2D
//================================================================//
class ZLCubicBezier2D :
	public ZLCubicCurve2D {
private:

	//----------------------------------------------------------------//
	void				FlattenProgressive		( ZLAbstractVertexWriter2D& writer, ZLReal flatness = 0.125, ZLReal angle = 15.0 ) const;

public:

	enum {
		NONE,
		ONE_INFLECTION,
		TWO_INFLECTIONS,
		ONE_CUSP,
		DEGENERATE,
	};

	//----------------------------------------------------------------//
	ZLReal				Angle					();
	void				Bless					();
	ZLVec2D				Evaluate				( ZLReal t ) const;
	void				FindInflectionDomain	( ZLReal t, ZLReal& t0, ZLReal& t1, ZLReal flatness = 0.25 ) const;
	u32					FindInflections			( ZLReal& t0, ZLReal& t1 ) const;
	void				Flatten					( ZLAbstractVertexWriter2D& writer, ZLReal flatness = 0.125, ZLReal angle = 15.0 ) const;
	ZLReal				GetFlattenedLength		( ZLReal flatness = 0.125, ZLReal angle = 15.0 );
	size_t				GetFlattenedSize		( ZLReal flatness = 0.125, ZLReal angle = 15.0 );
	void				Split					( ZLReal t, ZLCubicBezier2D& left, ZLCubicBezier2D& right ) const;
	ZLCubicBezier2D		Split					( ZLReal t0, ZLReal t1 ) const;
};

//================================================================//
// ZLCubicBezierFlattener2D
//================================================================//
class ZLCubicBezierFlattener2D {
private:

	friend class ZLCubicBezier2D;

	static const u32		MAX_CURVES		= 3;
	static const u32		MAX_VERTICES	= 6;
	static const u32		MAX_COMMANDS	= 9;

	enum {
		COMMAND_CURVE,
		COMMAND_VERTEX,
	};

	ZLCubicBezier2D		mCurves [ MAX_CURVES ];
	ZLVec2D				mVertices [ MAX_VERTICES ];
	u32					mCommands [ MAX_COMMANDS ];
	
	u32					mCurveIdx;
	u32					mVertexIdx;
	u32					mCommandIdx;
	
	u32					mTotalCommands;
	
	ZLReal				mFlatness;
	ZLReal				mAngleInRadians;
	
	bool				mIsProcessingCurve;
	ZLCubicBezier2D		mCurve;
	
	ZLVec2D				mLastVertex;

	//----------------------------------------------------------------//
	void				PushCommand			( const ZLCubicBezier2D& curve );
	void				PushCommand			( const ZLVec2D& vertex );
	
public:

	//----------------------------------------------------------------//
	void				Init							( const ZLCubicBezier2D& curve, ZLReal flatness = 0.125, ZLReal angle = 15.0 );
	bool				More							();
	ZLVec2D				Next							();
						ZLCubicBezierFlattener2D		();
						~ZLCubicBezierFlattener2D		();
};

#endif
