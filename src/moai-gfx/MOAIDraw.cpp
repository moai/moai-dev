// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAICamera.h>
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>
#include <moai-gfx/MOAIViewport.h>

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawAnimCurve ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAnimCurve& param ) {

	u32 resolution = param.mResolution;
	MOAIAnimCurve* animCurve = param.mAnimCurve;

	MOAIAnimCurveFloat* animCurveFloat = MOAICast < MOAIAnimCurveFloat >( animCurve );
	MOAIAnimCurveIndex* animCurveIndex = MOAICast < MOAIAnimCurveIndex >( animCurve );

	if ( !( animCurveFloat || animCurveIndex )) return;

	// TODO: this isn't entirely correct. the value of each key frame should be drawn
	// and then the spans between keys should be filled in with an approximation of
	// the resolution.

	ZLReal length = animCurve->GetLength ();
	ZLReal step = length / ( ZLReal )resolution;

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_STRIP, resolution );

	for ( ZLSize i = 0; i < ( resolution + 1 ); ++i ) {

		ZLReal t = step * ( ZLReal )i;
		ZLReal v = animCurveFloat ? animCurveFloat->GetValue ( t ) : ( float )animCurveIndex->GetValue ( t );

		gfxMgr.WriteVtx ( t, v, 0.0 );
		gfxMgr.WritePenColor4b ();
	}
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawAxis2D ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAxis2D& param ) {

	ZLVec2D loc = param.mV0;
	ZLVec2D vec = param.mD;
	
	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	
	ZLMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );
	
	ZLRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	ZLVec2D p0;
	ZLVec2D p1;
	
	if ( viewRect.GetIntersection ( loc, vec, p0, p1 )) {
		
		invMtx.Transform ( p0 );
		invMtx.Transform ( p1 );
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );
		
			gfxMgr.WriteVtx ( p0.mX, p0.mY, 0.0f );
			gfxMgr.WritePenColor4b ();
			
			gfxMgr.WriteVtx ( p1.mX, p1.mY, 0.0f );
			gfxMgr.WritePenColor4b ();

		gfxMgr.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawAxisGrid2D ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAxisGrid2D& param ) {

	ZLVec2D loc = param.mV0;
	ZLVec2D vec = param.mD;
	float size = param.mSize;

	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	
	ZLMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	// Set the axis to the grid length so we can get the length back post-transform
	vec.SetLength ( size );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );

	// Get the axis unit vector
	ZLVec2D norm = vec;
	size = norm.NormSafe ();
	
	// Get the axis normal
	ZLVec2D perpNorm ( norm.mY, -norm.mX );
	
	// Project the corners of the viewport onto the axis to get the mix/max bounds
	float dot;
	float min;
	float max;
	
	ZLVec2D corner;
	
	// left, top
	corner.Init ( -1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = dot;
	max = dot;
	
	// right, top
	corner.Init ( 1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// right, bottom
	corner.Init ( 1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// left, bottom
	corner.Init ( -1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// Get the start andstop grids
	s32 start = ( s32 )( min / size ) - 1;
	s32 stop = ( s32 )( max / size ) + 1;
	
	// Set the pen to the first...
	ZLVec2D pen = norm;
	pen.Scale (( float )start * size );
	pen.Add ( loc );
	
	// Step along the axis to draw perpendicular grid lines
	ZLRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	for ( ; start < stop; ++start ) {
		
		ZLVec2D p0;
		ZLVec2D p1;
		
		if ( viewRect.GetIntersection ( pen, perpNorm, p0, p1 )) {
			
			invMtx.Transform ( p0 );
			invMtx.Transform ( p1 );
			
			gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );
			
				gfxMgr.WriteVtx ( p0.mX, p1.mY, 0.0f );
				gfxMgr.WritePenColor4b ();
				
				gfxMgr.WriteVtx ( p0.mX, p1.mY, 0.0f  );
				gfxMgr.WritePenColor4b ();
			
			gfxMgr.EndPrim ();
		}
		
		pen.Add ( vec );
	}
}

////----------------------------------------------------------------//
//void MOAIDraw::ExecuteDrawPrims ( MOAIGfxBufferGL* vtxBuffer, MOAIVertexFormat* vtxFormat, u32 count ) {
//
//	// TODO: fix this?
//
//	UNUSED ( vtxBuffer );
//	UNUSED ( vtxFormat );
//	UNUSED ( count );
//
////	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
////
////	MOAIGfxMgr::Get ().UnbindBufferedDrawing ();
////
////	vtxBuffer->Bind ();
////	vtxFormat->Bind ( 0 );
////
////	gfxMgr.SetVertexMtxMode ( MOAIGfxMgr::VTX_STAGE_MODEL, MOAIGfxMgr::VTX_STAGE_MODEL );
////
////	gfxMgr.UpdateShaderGlobals ();
////
////	// TODO: use gfxMgr to cache buffers
////
////	zglDrawArrays ( MOAIGfxTopologyEnum::TRIANGLE_LIST, 0, count );
////	vtxBuffer->Unbind ();
////	vtxFormat->Unbind ();
//}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawLine ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawLine& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );
	
		gfxMgr.WriteVtx ( param.mV0.mX, param.mV0.mY, param.mV0.mZ );
		gfxMgr.WritePenColor4b ();
		
		gfxMgr.WriteVtx ( param.mV1.mX, param.mV1.mY, param.mV1.mZ  );
		gfxMgr.WritePenColor4b ();
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawPoint ( MOAIGfxMgr& gfxMgr, const ZLVec3D& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::POINT_LIST, 1 );
		gfxMgr.WriteVtx ( param.mX, param.mY, param.mZ );
		gfxMgr.WritePenColor4b ();
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawRay ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawRay& param ) {

	float x = param.mLoc.mX;
	float y = param.mLoc.mY;

	float dx = param.mVec.mX;
	float dy = param.mVec.mY;

	ZLVec2D loc ( x, y );
	ZLVec2D vec ( dx, dy );

	ZLMatrix4x4 mtx = gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );

	ZLMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );

	mtx.Transform ( loc );
	mtx.TransformVec ( vec );

	ZLRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );

	ZLVec2D p0;
	ZLVec2D p1;

	if ( viewRect.GetIntersection ( loc, vec, p0, p1 )) {

		invMtx.Transform ( p0 );
		invMtx.Transform ( p1 );

		gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );

			gfxMgr.WriteVtx ( p0.mX, p0.mY, 0.0f );
			gfxMgr.WritePenColor4b ();

			gfxMgr.WriteVtx ( p1.mX, p1.mY, 0.0f );
			gfxMgr.WritePenColor4b ();

		gfxMgr.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::ExecuteDrawTriangle ( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawTriangle& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::TRIANGLE_LIST, 3 );
	
		gfxMgr.WriteVtx ( param.mV0.mX, param.mV0.mY, param.mV0.mZ );
		gfxMgr.WritePenColor4b ();
	
		gfxMgr.WriteVtx ( param.mV1.mX, param.mV1.mY, param.mV1.mZ );
		gfxMgr.WritePenColor4b ();
	
		gfxMgr.WriteVtx ( param.mV2.mX, param.mV2.mY, param.mV2.mZ );
		gfxMgr.WritePenColor4b ();

	gfxMgr.EndPrim ();
}

////----------------------------------------------------------------//
//void MOAIDraw::DrawTexture ( float left, float top, float right, float bottom, MOAITexture* texture ) {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//
//	if ( texture && texture->IsReadyForUse ()) {
//
////		gfxMgr.SetBlendMode ( MOAIBlendFactorEnum::ONE, MOAIBlendFactorEnum::ZERO );
//		gfxMgr.SetTexture ( texture );
//		gfxMgr.SetShader ( MOAIShaderPresetEnum::DECK2D_SHADER );
//
//		const ZLColorVec& orgColor = gfxMgr.GetPenColor ();
//		gfxMgr.SetPenColor ( 1, 1, 1, 1 );
//
//		MOAIQuadBrush::BindVertexFormat ();
//
//		MOAIQuadBrush quad;
//		quad.SetVerts ( left, top, right, bottom );
//		quad.SetUVs ( 0, 0, 1, 1 );
//		quad.Draw ();
//
////		gfxMgr.SetBlendMode ();
//		gfxMgr.SetPenColor ( orgColor );
//
//		MOAIDraw::Bind ();
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIDraw::DrawVertexArray ( const ZLVec3D* verts, u32 count, u32 color, MOAIGfxTopologyEnum::_ primType ) {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//
//	gfxMgr.SetPenColor ( color );
//	gfxMgr.BeginPrim ( primType, count );
//
//	for ( u32 i = 0; i < count; ++i ) {
//		const ZLVec3D& vtx = verts [ i ];
//		gfxMgr.WriteVtx ( vtx );
//		gfxMgr.WritePenColor4b ();
//	}
//
//	gfxMgr.EndPrim ();
//}
//
////----------------------------------------------------------------//
//void MOAIDraw::DrawVertexArray2D ( const float* verts, u32 count, u32 color, MOAIGfxTopologyEnum::_ primType ) {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//
//	gfxMgr.SetPenColor ( color );
//	gfxMgr.BeginPrim ( primType, count );
//
//	for ( u32 i = 0; i < count; ++i ) {
//		u32 v = i << 1;
//		gfxMgr.WriteVtx ( verts [ v ], verts [ v + 1 ], 0.0f );
//		gfxMgr.WritePenColor4b ();
//	}
//
//	gfxMgr.EndPrim ();
//}

//----------------------------------------------------------------//
MOAIDraw::MOAIDraw () {

	RTTI_BEGIN ( MOAIDraw )
		RTTI_EXTEND ( MOAIAbstractCmdHandler )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDraw::~MOAIDraw () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::MOAIAbstractCmdHandler_HandleCommand ( u32 cmd, const void* param ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
		
		case MOAIDrawAPI::BIND_VECTOR_PRESETS:
			
			gfxMgr.SetTexture ();
			gfxMgr.SetShader ( MOAIShaderPresetEnum::LINE_SHADER );
			gfxMgr.SetVertexFormat ( MOAIVertexFormatPresetEnum::XYZWC );
			
			gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
			gfxMgr.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );

			gfxMgr.SetCullFunc ();
			break;
		
		case MOAIDrawAPI::CLEAR_SURFACE: {

			// TODO: handle properly setting clear color and flags
			gfxMgr.SetClearColor ( ZLColorVec::CLEAR );
			gfxMgr.SetClearFlags (
				MOAIClearFlagsEnum::COLOR_BUFFER_BIT |
				MOAIClearFlagsEnum::DEPTH_BUFFER_BIT |
				MOAIClearFlagsEnum::STENCIL_BUFFER_BIT
			);
			gfxMgr.SetClearFlags (
				MOAIClearFlagsEnum::COLOR_BUFFER_BIT
			);
			gfxMgr.ClearSurface ();
			break;
		}
		
		case MOAIDrawAPI::DISABLE_SCISSOR_RECT:
			gfxMgr.SetScissorRect ();
			break;
			
		case MOAIDrawAPI::DISABLE_VIEW_RECT:
			gfxMgr.SetViewRect ();
			break;
		
		case MOAIDrawAPI::DRAW:
			assert ( false );
			break;
		
		case MOAIDrawAPI::DRAW_ANIM_CURVE: {
			MOAIDraw::ExecuteDrawAnimCurve ( gfxMgr, *( const MOAIDrawAPIParam::DrawAnimCurve* )param );
			break;
		}
		
		case MOAIDrawAPI::DRAW_AXIS_2D: {
			MOAIDraw::ExecuteDrawAxis2D ( gfxMgr, *( const MOAIDrawAPIParam::DrawAxis2D* )param );
			break;
		}
		
		case MOAIDrawAPI::DRAW_LINE: {
			MOAIDraw::ExecuteDrawLine ( gfxMgr, *( const MOAIDrawAPIParam::DrawLine* )param );
			break;
		}
		
		case MOAIDrawAPI::DRAW_POINT: {
			MOAIDraw::ExecuteDrawPoint ( gfxMgr, *( const ZLVec3D* )param );
			break;
		}
	
		case MOAIDrawAPI::DRAW_RAY: {
			MOAIDraw::ExecuteDrawRay ( gfxMgr, *( const MOAIDrawAPIParam::DrawRay* )param );
			break;
		}
	
		case MOAIDrawAPI::DRAW_TRIANGLE: {
			MOAIDraw::ExecuteDrawTriangle ( gfxMgr, *( const MOAIDrawAPIParam::DrawTriangle* )param );
			break;
		}
		
		case MOAIDrawAPI::POP_GFX_STATE:
			gfxMgr.PopState ();
			break;
		
		case MOAIDrawAPI::PUSH_GFX_STATE:
			gfxMgr.PushState ();
			break;
		
		case MOAIDrawAPI::SET_BLEND_MODE:
			gfxMgr.SetBlendMode ( *( const MOAIBlendMode* )param );
			break;
		
		case MOAIDrawAPI::SET_CULL_FUNC:
			gfxMgr.SetCullFunc ( *( const MOAICullFuncEnum::_* )param );
			break;
			
		case MOAIDrawAPI::SET_DEPTH_FUNC:
			gfxMgr.SetDepthFunc ( *( const MOAIDepthFuncEnum::_* )param );
			break;
		
		case MOAIDrawAPI::SET_DEPTH_MASK:
			gfxMgr.SetDepthMask ( *( const bool* )param );
			break;
		
		case MOAIDrawAPI::SET_FRAME_BUFFER: {
			gfxMgr.SetFrameBuffer ( *( MOAIFrameBuffer** )param );
			break;
		}
		
		case MOAIDrawAPI::SET_INDEX_BUFFER:
			gfxMgr.SetIndexBuffer ( *( MOAIIndexBuffer** )param );
			break;
		
		case MOAIDrawAPI::SET_MATRIX: {
			const MOAIDrawAPIParam::SetMatrix* cast = ( const MOAIDrawAPIParam::SetMatrix* )param;
			gfxMgr.SetMtx ( cast->mMatrixID, cast->mMatrix );
			break;
		}
		
		case MOAIDrawAPI::SET_MATRIX_FROM_TRANSFORM: {
			const MOAIDrawAPIParam::SetMatrixFromTransform* cast = ( const MOAIDrawAPIParam::SetMatrixFromTransform* )param;
			gfxMgr.SetMtx ( cast->mMatrixID, cast->mTransform->GetLocalToWorldMtx ());
			break;
		}
		
		case MOAIDrawAPI::SET_PEN_COLOR:
			gfxMgr.SetPenColor ( *( u32* )param );
			break;
					
		case MOAIDrawAPI::SET_PEN_WIDTH:
			gfxMgr.SetPenWidth ( *( float* )param );
			break;
		
		case MOAIDrawAPI::SET_SCISSOR_RECT:
			gfxMgr.SetScissorRect ( *( ZLRect* )param );
			break;
		
		case MOAIDrawAPI::SET_SCISSOR_RECT_FROM_VIEWPORT:
			gfxMgr.SetScissorRect ( *( MOAIViewport** )param );
			break;
		
		case MOAIDrawAPI::SET_SHADER:
			gfxMgr.SetShader ( *( MOAIShader** )param );
			break;
		
		case MOAIDrawAPI::SET_TEXTURE: {
			const MOAIDrawAPIParam::SetTexture* cast = ( const MOAIDrawAPIParam::SetTexture* )param;
			gfxMgr.SetTexture ( cast->mTexture, cast->mTextureUnit );
			break;
		}
		
		case MOAIDrawAPI::SET_VERTEX_ARRAY:
			gfxMgr.SetVertexArray ( *( MOAIVertexArray** )param );
			break;
		
		case MOAIDrawAPI::SET_VERTEX_BUFFER:
			gfxMgr.SetVertexBuffer ( *( MOAIVertexBuffer** )param );
			break;
		
		case MOAIDrawAPI::SET_VERTEX_FORMAT:
			gfxMgr.SetVertexFormat ( *( MOAIVertexFormat** )param );
			break;
		
		case MOAIDrawAPI::SET_VIEW_PROJ: {
			const MOAIDrawAPIParam::SetViewProj* cast = ( const MOAIDrawAPIParam::SetViewProj* )param;
			gfxMgr.SetViewProj ( cast->mViewport, cast->mCamera );
			break;
		}
		
		case MOAIDrawAPI::SET_VIEW_RECT:
			gfxMgr.SetViewRect ( *( ZLRect* )param );
			break;
		
		case MOAIDrawAPI::SET_VIEW_RECT_FROM_VIEWPORT:
			gfxMgr.SetViewRect ( *( MOAIViewport** )param );
			break;
	}
}
