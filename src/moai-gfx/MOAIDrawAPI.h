// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWAPI_H
#define	MOAIDRAWAPI_H

#include <moai-gfx/MOAIGfxConsts.h>

typedef MOAIRetainedCmdStream MOAIGfxScript;

class MOAIAbstractChildTransform;
class MOAIBlendMode;
class MOAICamera;
class MOAIFrameBuffer;
class MOAIIndexBuffer;
class MOAIShader;
class MOAITexture;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;
class MOAIViewport;

//================================================================//
// MOAIDrawAPIParam
//================================================================//
namespace MOAIDrawAPIParam {

	//----------------------------------------------------------------//
	struct Compose {
		ZLVec2D 			mV0;
		ZLVec2D 			mD;
	};

	//----------------------------------------------------------------//
	struct DrawAxis2D {
		ZLVec2D 			mV0;
		ZLVec2D 			mD;
	};
	
	//----------------------------------------------------------------//
	struct DrawAnimCurve {
		MOAIAnimCurve*		mAnimCurve;
		u32					mResolution;
	};
	
	//----------------------------------------------------------------//
	struct DrawAxisGrid2D {
		ZLVec2D 			mV0;
		ZLVec2D 			mD;
		float				mSize;
	};

	//----------------------------------------------------------------//
	struct DrawLine {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
	};
	
	//----------------------------------------------------------------//
	struct DrawRay {
		ZLVec2D 			mLoc;
		ZLVec2D 			mVec;
	};

	//----------------------------------------------------------------//
	struct DrawTriangle {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
		ZLVec3D 			mV2;
	};

	//----------------------------------------------------------------//
	struct SetMatrix {
		u32 				mMatrixID;
		ZLMatrix4x4 		mMatrix;
	};

	//----------------------------------------------------------------//
	struct SetMatrixFromTransform {
		u32 							mMatrixID;
		MOAIAbstractChildTransform*		mTransform;
	};

	//----------------------------------------------------------------//
	struct SetPenColor {
		u32 				mColor;
	};

	//----------------------------------------------------------------//
	struct SetPenWidth {
		float 				mWidth;
	};
	
	//----------------------------------------------------------------//
	struct SetTexture {
		MOAITexture* 		mTexture;
		ZLIndex 			mTextureUnit;
	};
	
	//----------------------------------------------------------------//
	struct SetViewProj {
		MOAICamera*			mCamera;
		MOAIViewport*		mViewport;
	};
};

//================================================================//
// MOAIDrawAPI
//================================================================//
// TODO: doxygen
class MOAIDrawAPI :
	public virtual MOAIAbstractCmdAPI {
public:

	enum {
		BIND_VECTOR_PRESETS,
		CLEAR_SURFACE,
		COMPOSE,
		
		DEBUG_TRI,
		
		DISABLE_SCISSOR_RECT,
		DISABLE_VIEW_RECT,
		
		DRAW,
		
		DRAW_ANIM_CURVE,
		DRAW_AXIS_2D,
		DRAW_LINE,
		DRAW_POINT,
		DRAW_RAY,
		DRAW_TRIANGLE,
		
		POP_GFX_STATE,
		PUSH_GFX_STATE,
		
		SET_BLEND_MODE,
		SET_CULL_FUNC,
		SET_DEPTH_FUNC,
		SET_DEPTH_MASK,
		SET_FRAME_BUFFER,
		SET_INDEX_BUFFER,
		SET_MATRIX,
		SET_MATRIX_FROM_TRANSFORM,
		SET_PEN_COLOR,
		SET_PEN_WIDTH,
		SET_SCISSOR_RECT,
		SET_SCISSOR_RECT_FROM_VIEWPORT,
		SET_SHADER,
		SET_TEXTURE,
		SET_VERTEX_ARRAY,
		SET_VERTEX_BUFFER,
		SET_VERTEX_FORMAT,
		SET_VIEW_PROJ,
		SET_VIEW_RECT,
		SET_VIEW_RECT_FROM_VIEWPORT,
		
		TOTAL_BASE_COMMANDS,
	};

protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	static const u32 DEFAULT_ELLIPSE_STEPS			= 64;

	//----------------------------------------------------------------//
	static int			_bindVectorPresets			( lua_State* L );
	static int			_clearSurface				( lua_State* L );
	static int			_compose					( lua_State* L );
	static int			_debugTri					( lua_State* L );
	static int			_drawAnimCurve				( lua_State* L );
	static int			_drawBezierCurve			( lua_State* L );
	static int			_drawBoxOutline				( lua_State* L );
	static int			_drawCircle					( lua_State* L );
	static int			_drawCircleSpokes			( lua_State* L );
	static int			_drawElements				( lua_State* L );
	static int			_drawEllipse				( lua_State* L );
	static int			_drawEllipseSpokes			( lua_State* L );
	static int			_drawGrid					( lua_State* L );
	static int			_drawLine					( lua_State* L );
	static int			_drawPoints					( lua_State* L );
	static int			_drawRay					( lua_State* L );
	static int			_drawRect					( lua_State* L );
	static int			_fillCircle					( lua_State* L );
	static int			_fillEllipse				( lua_State* L );
	static int			_fillFan					( lua_State* L );
	static int			_fillRect					( lua_State* L );
	static int			_fillRoundedRect			( lua_State* L );
	static int			_drawAxis2D					( lua_State* L );
	static int			_popGfxState				( lua_State* L );
	static int			_pushGfxState				( lua_State* L );
	static int			_setBlendMode				( lua_State* L );
	static int			_setCullFunc				( lua_State* L );
	static int			_setDepthFunc				( lua_State* L );
	static int			_setDepthMask				( lua_State* L );
	static int			_setFrameBuffer				( lua_State* L );
	static int			_setIndexBuffer				( lua_State* L );
	static int			_setMatrix					( lua_State* L );
	static int			_setPenColor				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	static int			_setScissorRect				( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );
	static int			_setVertexArray				( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );
	static int			_setVertexFormat			( lua_State* L );
	static int			_setViewProj				( lua_State* L );
	static int			_setViewRect				( lua_State* L );
	static int			_strokeRoundedRect			( lua_State* L );

	//----------------------------------------------------------------//
	void				DrawArcFill					( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps ); // TODO: not ready for prime time; see notes in implementation
	void				DrawArcStroke				( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset );
	void				DrawLuaArray				( lua_State* L, MOAIGfxTopologyEnum::_ primType );
	void				DrawLuaParams				( lua_State* L, MOAIGfxTopologyEnum::_ primType );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIAbstractCmdAPI_RegisterLuaAPI		( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	void 				BindVectorPresets			();
	void				ClearSurface				();
	void				Compose						( MOAIShader* shader );
	void				DebugTri					();
	void				Draw						();
	void				DrawAnimCurve				( MOAIAnimCurve& curve, u32 resolution );
	void				DrawAxis2D					( float x, float y, float dx, float dy );
	void				DrawBezierCurve				( const ZLCubicBezier2D& bezier );
	void				DrawBoxAxis					( const ZLBox& box );
	void				DrawBoxDiagonals			( const ZLBox& box );
	void				DrawBoxOutline				( const ZLBox& box );
	void				DrawCircleFill				( float x, float y, float radius, u32 steps );
	void				DrawCircleOutline			( float x, float y, float radius, u32 steps );
	void				DrawEllipseFill				( const ZLRect& rect, u32 steps );
	void				DrawEllipseFill				( float x, float y, float xRad, float yRad, u32 steps );
	void				DrawEllipseOutline			( const ZLRect& rect, u32 steps );
	void				DrawEllipseOutline			( float x, float y, float xRad, float yRad, u32 steps );
	void				DrawEllipseSpokes			( float x, float y, float xRad, float yRad, u32 steps );
	void				DrawGrid					( const ZLRect& rect, u32 xCells, u32 yCells );
	void				DrawLine					( const ZLVec2D& v0, const ZLVec2D& v1 );
	void				DrawLine					( const ZLVec3D& v0, const ZLVec3D& v1 );
	void				DrawLine					( float x0, float y0, float x1, float y1 );
	void				DrawLine					( float x0, float y0, float z0, float x1, float y1, float z1 );
	void				DrawPoint					( const ZLVec2D& loc );
	void				DrawPoint					( float x, float y, float z = 0.0f );
	void				DrawPolyOutline				( const ZLPolygon2D& poly );
	void				DrawQuadOutline				( const ZLQuad& quad );
	void				DrawRay						( float x, float y, float dx, float dy );
	void				DrawRectEdges				( ZLRect rect, u32 edges );
	void				DrawRectFill				( ZLRect rect );
	void				DrawRectFill				( float left, float top, float right, float bottom );
	void				DrawRectOutline				( const ZLRect& rect );
	void				DrawRectOutline				( float left, float top, float right, float bottom );
	void				DrawRoundedRectFill			( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps );
	void				DrawRoundedRectStroke		( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset );
	void				DrawTriangleFill			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void				DrawTriangleOutline			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void				DrawVec						( float x, float y, float dx, float dy, float t );
	void				DrawVec						( float x, float y, float z, float dx, float dy, float dz, float t );
						MOAIDrawAPI					();
	virtual				~MOAIDrawAPI				();
	void 				PopGfxState					();
	void				PushGfxState				();
	void				SetBlendMode				( const MOAIBlendMode& blendMode );
	void				SetCullFunc					( MOAICullFuncEnum::_ cullFunc );
	void				SetDepthFunc				( MOAIDepthFuncEnum::_ depthFunc );
	void				SetDepthMask				( bool depthMask );
	void				SetFrameBuffer				( MOAIFrameBuffer* frameBuffer );
	void				SetIndexBuffer				( MOAIIndexBuffer* indexBuffer );
	void 				SetMatrix					( u32 matrixID, const ZLMatrix4x4& mtx );
	void 				SetMatrixFromTransform		( u32 matrixID, MOAIAbstractChildTransform& transform );
	void				SetPenColor					( u32 color );
	void				SetPenWidth					( float width );
	void				SetScissorRect				();
	void				SetScissorRect				( const ZLRect& rect );
	void				SetScissorRect				( MOAIViewport* viewport );
	void				SetShader					( MOAIShader* shader );
	void				SetTexture					( MOAITexture* texture, ZLIndex textureUnit );
	void				SetVertexArray				( MOAIVertexArray* vertexArray );
	void				SetVertexBuffer				( MOAIVertexBuffer* vertexBuffer );
	void				SetVertexFormat				( MOAIVertexFormat* vertexFormat );
	void				SetViewProj					( MOAIViewport* viewport, MOAICamera* camera );
	void				SetViewRect					();
	void				SetViewRect					( const ZLRect& rect );
	void				SetViewRect					( MOAIViewport* viewport );
};

#endif
