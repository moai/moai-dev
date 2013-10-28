// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORDRAWING_H
#define	MOAIVECTORDRAWING_H

#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVectorStyle.h>
#include <moai-sim/MOAIVertexBuffer.h>

class MOAIVectorShape;
struct TESStesselator;

//================================================================//
// MOAIVectorLineJoin
//================================================================//
class MOAIVectorLineJoin {
public:

		ZLVec2D		mVertex;		// the join vertex
		ZLVec2D		mEdgeVec;		// vector of the next edge
		ZLVec2D		mEdgeNorm;		// normal of preceding edge
		ZLVec2D		mJointNorm;		// avg normal (same as N0 or N1 if path not closed)
		bool		mIsCap;
};

//================================================================//
// MOAIVectorDrawing
//================================================================//
/**	@name	MOAIVectorImage
	@text	Display object made of vector shapes.
*/
class MOAIVectorDrawing :
	public MOAILuaObject {
private:

	static const u32 DEFAULT_CIRCLE_RESOLUTION = 32;

	ZLLeanStack < MOAIVectorShape*, 64 >	mDirectory; // TODO: should use a chunked array or something
	ZLLeanStack < MOAIVectorShape*, 16 >	mShapeStack; // TODO: ditto
	ZLLeanStack < ZLVec2D, 32 >				mVertexStack;

	ZLMemStream			mIdxStream;
	ZLMemStream			mVtxStream;

	MOAIIndexBuffer		mIdxBuffer;
	MOAIVertexBuffer	mVtxBuffer;
	
	MOAIVectorStyle		mStyle;
	
	u32					mCircleResolution;

	//----------------------------------------------------------------//
	static int		_finish					( lua_State* L );
	static int		_pushCombo				( lua_State* L );
	static int		_pushEllipse			( lua_State* L );
	static int		_pushPath				( lua_State* L );
	static int		_pushPolygon			( lua_State* L );
	static int		_pushRect				( lua_State* L );
	static int		_pushVertex				( lua_State* L );
	static int		_setCapStyle			( lua_State* L );
	static int		_setCircleResolution	( lua_State* L );
	static int		_setFillColor			( lua_State* L );
	static int		_setFillStyle			( lua_State* L );
	static int		_setJoinStyle			( lua_State* L );
	static int		_setLineColor			( lua_State* L );
	static int		_setLineStyle			( lua_State* L );
	static int		_setLineWidth			( lua_State* L );
	static int		_setMiterLimit			( lua_State* L );
	static int		_setStrokeStyle			( lua_State* L );
	static int		_setWindingRule			( lua_State* L );

	//----------------------------------------------------------------//
	u32				PushShape				( MOAIVectorShape* shape );
	int				StrokeWedge				( ZLVec2D*& verts, const ZLVec2D& origin, const ZLVec2D& n0, const ZLVec2D& n1, float width );
	void			Tessalate				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVectorDrawing )
	
	GET_SET ( MOAIVectorStyle&, Style, mStyle )
	GET_SET ( u32, CircleResolution, mCircleResolution )
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			ComputeLineJoins		( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior );
	u32				CopyVertexStack			( ZLVec2D* vertices, u32 total );
	u32				CountVertices			();
	void			Draw					();
	void			Finish					();
	u32				GetResolutionForWedge	( float radians );
					MOAIVectorDrawing		();
					~MOAIVectorDrawing		();
	
	void			PushCombo				();
	void			PushEllipse				( float x, float y, float xRad, float yRad );
	void			PushPath				( ZLVec2D* vertices, u32 total );
	void			PushPolygon				( ZLVec2D* vertices, u32 total );
	void			PushRect				( const ZLRect& rect );
	void			PushVertex				( float x, float y );
	
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	int				StrokeLine				( ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact );
	void			WriteContourIndices		( TESStesselator* tess, u32 base );
	void			WriteTriangleIndices	( TESStesselator* tess, u32 base );
	void			WriteVertices			( TESStesselator* tess, u32 color );
};

#endif
