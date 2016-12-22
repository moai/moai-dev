// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORTESSELATOR_H
#define	MOAIVECTORTESSELATOR_H

#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIVectorUtil.h>

class MOAIGfxBuffer;
class MOAIVectorShape;
class MOAIVertexFormat;

class SafeTesselator;

//================================================================//
// MOAIVectorTesselatorWriter
//================================================================//
// TODO: doxygen
class MOAIVectorTesselatorWriter :
	public MOAILuaObject {
private:

	friend class MOAIVectorTesselator;

	enum {
		STYLE_CMD_FILL,
		STYLE_CMD_FILL_COLOR,
		STYLE_CMD_LINE,
		STYLE_CMD_LINE_COLOR,
		STYLE_CMD_LINE_WIDTH,
		STYLE_CMD_STROKE,
		STYLE_CMD_STROKE_COLOR,
		STYLE_CMD_STROKE_WIDTH,
		STYLE_CMD_STROKE_DEPTH_BIAS,
		STYLE_CMD_JOIN,
		STYLE_CMD_CAP,
		STYLE_CMD_MITER_LIMIT,
		STYLE_CMD_WINDING_RULE,
		STYLE_CMD_CIRCLE_RESOLUTION,
		STYLE_CMD_EXTRUDE,
		STYLE_CMD_Z_OFFSET,
		STYLE_CMD_LIGHT_VEC,
		STYLE_CMD_LIGHT_COLOR,
		STYLE_CMD_LIGHT_CURVE,
		STYLE_CMD_SHADOW_COLOR,
		STYLE_CMD_SHADOW_CURVE,
		STYLE_CMD_DRAWING_TO_WORLD,
		STYLE_CMD_FILL_EXTRA_ID,
		STYLE_CMD_STROKE_EXTRA_ID,
		STYLE_CMD_MERGE_NORMALS,
		
		VECTOR_CMD_SHAPE,
		VECTOR_CMD_DONE,
	};

	bool				mFlushStyle;
	MOAIVectorStyle		mStyle;

public:

	DECL_LUA_OPAQUE ( MOAIVectorTesselatorWriter )

	//----------------------------------------------------------------//
							MOAIVectorTesselatorWriter			();
							~MOAIVectorTesselatorWriter			();
	MOAIVectorShape*		ReadShape							( ZLStream& stream );
	void					WriteShape							( ZLStream& stream, const MOAIVectorShape& shape );
};

//================================================================//
// MOAIVectorTesselator
//================================================================//
/**	@lua	MOAIVectorTessalator
	@text	Convert vector primitives into triangles.
*/
class MOAIVectorTesselator :
	public MOAILuaObject {
private:

	enum {
		VERTEX_SIZE = 16,
	};

	ZLLeanStack < MOAIVectorShape*, 16 >	mShapeStack;
	
	ZLLeanStack < ZLVec2D, 32 >				mVertexStack;
	bool									mPolyClosed;
	
	float				mDepthBias;
	float				mDepthOffset;
	
	MOAIVectorStyle		mStyle;
	
	bool				mVerbose;

	ZLLeanStack < ZLAffine2D, 16 > mMatrixStack;

	size_t					mVtxExtraSize;
	ZLLeanArray < void* >	mVtxExtras;

	//----------------------------------------------------------------//
	static int		_clearShapes			( lua_State* L );
	static int		_clearStyles			( lua_State* L );
	static int		_clearTransforms		( lua_State* L );
	static int		_drawingToWorld			( lua_State* L );
	static int		_drawingToWorldVec		( lua_State* L );
	static int		_finish					( lua_State* L );
	static int		_getExtrude				( lua_State* L );
	static int		_getTransform			( lua_State* L );
	static int		_openWriter				( lua_State* L );
	static int		_pushBezierVertices		( lua_State* L );
	static int		_pushCombo				( lua_State* L );
	static int		_pushEllipse			( lua_State* L );
	static int		_pushPoly				( lua_State* L );
	static int		_pushRect				( lua_State* L );
    static int      _pushRegion             ( lua_State* L );
	static int		_pushRotate				( lua_State* L );
	static int		_pushScale				( lua_State* L );
	static int		_pushSkew				( lua_State* L );
	static int		_pushTransform			( lua_State* L );
	static int		_pushTranslate			( lua_State* L );
	static int		_pushVertex				( lua_State* L );
	static int		_readShapes				( lua_State* L );
	static int		_reserveVertexExtras	( lua_State* L );
	static int		_setCapStyle			( lua_State* L );
	static int		_setCircleResolution	( lua_State* L );
	static int		_setDepthBias			( lua_State* L );
	static int		_setExtrude				( lua_State* L );
	static int		_setFillColor			( lua_State* L );
	static int		_setFillStyle			( lua_State* L );
	static int		_setFillExtra			( lua_State* L );
	static int		_setJoinStyle			( lua_State* L );
	static int		_setLightColor			( lua_State* L );
	static int		_setLightCurve			( lua_State* L );
	static int		_setLightVec			( lua_State* L );
	static int		_setLineColor			( lua_State* L );
	static int		_setLineStyle			( lua_State* L );
	static int		_setLineWidth			( lua_State* L );
	static int		_setMergeNormals		( lua_State* L );
	static int		_setMiterLimit			( lua_State* L );
	static int		_setPolyClosed			( lua_State* L );
	static int		_setShadowColor			( lua_State* L );
	static int		_setShadowCurve			( lua_State* L );
	static int		_setStrokeColor			( lua_State* L );
	static int		_setStrokeDepthBias		( lua_State* L );
	static int		_setStrokeExtra			( lua_State* L );
	static int		_setStrokeStyle			( lua_State* L );
	static int		_setStrokeWidth			( lua_State* L );
	static int		_setVerbose				( lua_State* L );
	static int		_setVertexExtra			( lua_State* L );
	static int		_setWindingRule			( lua_State* L );
	static int		_tesselate				( lua_State* L );
	static int		_worldToDrawing			( lua_State* L );
	static int		_worldToDrawingVec		( lua_State* L );
	static int		_writeShapes			( lua_State* L );

	//----------------------------------------------------------------//
	void			PushShape				( MOAIVectorShape* shape );
		
public:

	static const u32	TESSELATE_FILLS			= 0x01;
	static const u32	TESSELATE_STROKES		= 0x02;
	static const u32	TESSELATE_ALL			= TESSELATE_FILLS | TESSELATE_STROKES;
	
	DECL_LUA_FACTORY ( MOAIVectorTesselator )
	
	GET_SET ( MOAIVectorStyle&, Style, mStyle )
	GET_SET ( bool, Verbose, mVerbose )
	GET_SET ( bool, PolyClosed, mPolyClosed )
	GET_SET ( float, DepthBias, mDepthBias )
	
	//----------------------------------------------------------------//
	void				Clear						();
	void				ClearShapes					();
	void				ClearTransforms				();
	u32					CountVertices				( const MOAIVertexFormat& format, ZLStream& vtxStream );
	int					Finish						();
						MOAIVectorTesselator		();
						~MOAIVectorTesselator		();
	void				PopTransform				();
	void				PushBezierVertices			( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2, const ZLVec2D& p3 );
	void				PushCombo					();
	void				PushEllipse					( float x, float y, float xRad, float yRad );
	void				PushPoly					( const ZLVec2D* vertices, u32 total, bool closed );
	void				PushRect					( float xMin, float yMin, float xMax, float yMax );
	void				PushRegion					( const MOAIRegion& region );
	void				PushRotate					( float x, float y, float r );
	void				PushScale					( float x, float y );
	void				PushSkew					( float yx, float xy );
	void				PushTransform				( const ZLAffine2D& transform );
	void				PushTransform				( float a, float b, float c, float d, float tx, float ty );
	void				PushTranslate				( float x, float y);
	void				PushVertex					( float x, float y );
	void				ReadShapes					( ZLStream& stream );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveVertexExtras			( u32 total, size_t size );
	void				SetVertexExtra				( u32 idx, void* extra, size_t size );
	int					Tesselate					( SafeTesselator& tess, u32 flags = TESSELATE_ALL );
	int					Tesselate					( MOAIRegion& region, u32 flags = TESSELATE_ALL );
	int					Tesselate					( ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, u32 flags = TESSELATE_ALL );
	int					Tesselate					( MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, MOAIVertexFormat& format, u32 idxSizeInBytes, u32 flags = TESSELATE_ALL );
	void				WriteShapes					( ZLStream& stream, MOAIVectorTesselatorWriter* writer = 0 );
	void				WriteSkirt					( SafeTesselator& tess, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, const MOAIVectorStyle& style, const ZLColorVec& fillColor, u32 vertexExtraID );
	void				WriteTriangles				( SafeTesselator& tess, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, const MOAIVectorStyle& style, float z, u32 color, u32 vertexExtraID );
	void				WriteVertex					( ZLStream& stream, MOAIVertexFormat& format, float x, float y, float z, float xn, float yn, float zn, u32 color, u32 vertexExtraID );
};

#endif
