// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORDRAWING_H
#define	MOAIVECTORDRAWING_H

#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVectorStyle.h>
#include <moai-sim/MOAIVertexBuffer.h>

class MOAIVectorShape;
struct TESStesselator;

#define DEFAULT_DEPTH_BIAS 0.001f

//================================================================//
// MOAIVectorDrawing
//================================================================//
/**	@name	MOAIVectorImage
	@text	Display object made of vector shapes.
*/
class MOAIVectorDrawing :
	public MOAILuaObject {
private:

	ZLLeanStack < MOAIVectorShape*, 64 >	mDirectory; // TODO: should use a chunked array or something
	ZLLeanStack < MOAIVectorShape*, 16 >	mShapeStack; // TODO: ditto
	ZLLeanStack < ZLVec2D, 32 >				mVertexStack;

	ZLMemStream			mIdxStream;
	ZLMemStream			mVtxStream;

	MOAIIndexBuffer		mIdxBuffer;
	MOAIVertexBuffer	mVtxBuffer;
	
	float				mDepthBias;
	float				mDepthOffset;
	
	MOAIVectorStyle		mStyle;
	
	bool				mVerbose;

	//----------------------------------------------------------------//
	static int		_finish					( lua_State* L );
	static int		_pushCombo				( lua_State* L );
	static int		_pushEllipse			( lua_State* L );
	static int		_pushPath				( lua_State* L );
	static int		_pushPoly				( lua_State* L );
	static int		_pushRect				( lua_State* L );
	static int		_pushVertex				( lua_State* L );
	static int		_setCapStyle			( lua_State* L );
	static int		_setCircleResolution	( lua_State* L );
	static int		_setExtrude				( lua_State* L );
	static int		_setFillColor			( lua_State* L );
	static int		_setFillStyle			( lua_State* L );
	static int		_setJoinStyle			( lua_State* L );
	static int		_setLightColor			( lua_State* L );
	static int		_setLightCurve			( lua_State* L );
	static int		_setLightVec			( lua_State* L );
	static int		_setLineColor			( lua_State* L );
	static int		_setLineStyle			( lua_State* L );
	static int		_setLineWidth			( lua_State* L );
	static int		_setMiterLimit			( lua_State* L );
	static int		_setShadowColor			( lua_State* L );
	static int		_setShadowCurve			( lua_State* L );
	static int		_setStrokeColor			( lua_State* L );
	static int		_setStrokeDepthBias		( lua_State* L );
	static int		_setStrokeStyle			( lua_State* L );
	static int		_setStrokeWidth			( lua_State* L );
	static int		_setVerbose				( lua_State* L );
	static int		_setWindingRule			( lua_State* L );

	//----------------------------------------------------------------//
	u32				PushShape				( MOAIVectorShape* shape );
	void			Tessalate				();
	void			WriteVertex				( float x, float y, float z, u32 color );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVectorDrawing )
	
	GET_SET ( MOAIVectorStyle&, Style, mStyle )
	GET_SET ( bool, Verbose, mVerbose )
	
	GET ( const ZLBox&, Bounds, mVtxBuffer.GetBounds ())
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				CopyVertexStack			( ZLVec2D* vertices, u32 total );
	u32				CountVertices			();
	void			Draw					();
	void			Finish					();
					MOAIVectorDrawing		();
					~MOAIVectorDrawing		();
	
	void			PushCombo				();
	void			PushEllipse				( float x, float y, float xRad, float yRad );
	void			PushPath				( ZLVec2D* vertices, u32 total );
	void			PushPoly				( ZLVec2D* vertices, u32 total );
	void			PushRect				( float xMin, float yMin, float xMax, float yMax );
	void			PushVertex				( float x, float y );
	
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			WriteContourIndices		( TESStesselator* tess, u32 base );
	void			WriteSkirt				( TESStesselator* tess, const MOAIVectorStyle& style, const ZLColorVec& fillColor );
	void			WriteTriangleIndices	( TESStesselator* tess, u32 base );
	void			WriteVertices			( TESStesselator* tess, float z, u32 color );
};

#endif
