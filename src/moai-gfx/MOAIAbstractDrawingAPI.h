// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPI_H
#define	MOAIDRAWINGAPI_H

#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIDrawingCommand.h>

class MOAIBlendMode;
class MOAIFrameBuffer;
class MOAIIndexBuffer;
class MOAIShader;
class MOAITexture;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIAbstractDrawingAPI
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPI {
protected:

	static const u32 DEFAULT_ELLIPSE_STEPS			= 64;

	//----------------------------------------------------------------//
	void				DrawArcFill					( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps ); // TODO: not ready for prime time; see notes in implementation
	void				DrawArcStroke				( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset );
	void				DrawLuaArray				( lua_State* L, MOAIGfxTopologyEnum::_ primType );
	void				DrawLuaParams				( lua_State* L, MOAIGfxTopologyEnum::_ primType );
	void				SubmitCommand				( MOAIDrawingCmdEnum::_ cmd );
	void				RetainObject				( ZLRefCountedObject* object );

	//----------------------------------------------------------------//
//	virtual void		MOAIAbstractDrawingAPI_Call					() = 0;
	virtual void		MOAIAbstractDrawingAPI_RetainObject			( ZLRefCountedObject* object ) = 0;
	virtual void		MOAIAbstractDrawingAPI_SubmitCommand		( MOAIDrawingCmdEnum::_ cmd, const void* param, ZLSize size ) = 0;

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SubmitCommand ( MOAIDrawingCmdEnum::_ cmd, const TYPE& param ) {
		this->MOAIAbstractDrawingAPI_SubmitCommand ( cmd, &param, sizeof ( TYPE ));
	}

public:

	//----------------------------------------------------------------//
	void 				BindVectorDrawing			();
	void				Call						();
	void				CallFromShader				();
	void				ClearSurface				();
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
	void				DrawRay						( float x, float y, float dx, float dy, float length );
	void				DrawRectEdges				( ZLRect rect, u32 edges );
	void				DrawRectFill				( ZLRect rect );
	void				DrawRectFill				( float left, float top, float right, float bottom );
	void				DrawRectOutline				( const ZLRect& rect );
	void				DrawRectOutline				( float left, float top, float right, float bottom );
	void				DrawRoundedRectFill			( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps );
	void				DrawRoundedRectStroke		( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset );
	void				DrawTriangleFill			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void				DrawTriangleOutline			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
						MOAIAbstractDrawingAPI		();
	virtual				~MOAIAbstractDrawingAPI		();
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
	void				SetScissorRect				( const ZLRect& rect );
	void				SetShader					( MOAIShader* shader );
	void				SetTexture					( MOAITexture* texture, ZLIndex textureUnit );
	void				SetVertexArray				( MOAIVertexArray* vertexArray );
	void				SetVertexBuffer				( MOAIVertexBuffer* vertexBuffer );
	void				SetVertexFormat				( MOAIVertexFormat* vertexFormat );
	void				SetViewProj					( MOAIViewport* viewport, MOAICamera* camera );
	void				SetViewRect					( const ZLRect& rect );
	void				SetViewRect					( MOAIViewport* viewport );
};

#endif
