// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractDrawingAPICallback.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIDrawingCommand.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIDrawingCommand
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawingCommand::Execute ( MOAIAbstractDrawingAPICallback* callback, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawingCmdEnum::CALL:
		
			if ( callback ) {
				callback->Call ();
			}
			break;
		
		case MOAIDrawingCmdEnum::CALL_FROM_SHADER:
		
			if ( callback ) {
				
				MOAIShader* shader = gfxMgr.GetShader ();
				MOAIAbstractGfxScript* gfxScript = shader ? shader->GetGfxScript () : NULL;
			
				if ( gfxScript ) {
					gfxScript->RunScript ( callback, MOAIDrawingCmdEnum::CALL );
				}
				else {
					callback->Call ();
				}
			}
			break;
		
		case MOAIDrawingCmdEnum::CLEAR_SURFACE:
			break;
		
		case MOAIDrawingCmdEnum::DRAW_AXIS_2D: {
			MOAIDrawingParam::DrawAxis2D param = *( const MOAIDrawingParam::DrawAxis2D* )rawParam;
			MOAIDrawingCommand::ExecuteDrawAxis2D ( gfxMgr, param );
			break;
		}
		
		case MOAIDrawingCmdEnum::DRAW_LINE: {
			MOAIDrawingParam::DrawLine param = *( const MOAIDrawingParam::DrawLine* )rawParam;
			MOAIDrawingCommand::ExecuteDrawLine ( gfxMgr, param );
			break;
		}
		
		case MOAIDrawingCmdEnum::DRAW_POINT: {
			ZLVec3D param = *( const ZLVec3D* )rawParam;
			MOAIDrawingCommand::ExecuteDrawPoint ( gfxMgr, param );
			break;
		}
	
		case MOAIDrawingCmdEnum::DRAW_TRIANGLE: {
			MOAIDrawingParam::DrawTriangle param = *( const MOAIDrawingParam::DrawTriangle* )rawParam;
			MOAIDrawingCommand::ExecuteDrawTriangle ( gfxMgr, param );
			break;
		}
		
		case MOAIDrawingCmdEnum::POP_GFX_STATE:
			gfxMgr.PopState ();
			break;
		
		case MOAIDrawingCmdEnum::PUSH_GFX_STATE:
			gfxMgr.PushState ();
			break;
		
		case MOAIDrawingCmdEnum::SET_BLEND_MODE:
			gfxMgr.SetBlendMode ( *( const MOAIBlendMode* )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_CULL_FUNC:
			gfxMgr.SetCullFunc ( *( const MOAICullFuncEnum::_* )rawParam );
			break;
			
		case MOAIDrawingCmdEnum::SET_DEPTH_FUNC:
			gfxMgr.SetDepthFunc ( *( const MOAIDepthFuncEnum::_* )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_DEPTH_MASK:
			gfxMgr.SetDepthMask ( *( const bool* )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_FRAME_BUFFER:
			gfxMgr.SetFrameBuffer ( *( MOAIFrameBuffer** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_INDEX_BUFFER:
			gfxMgr.SetIndexBuffer ( *( MOAIIndexBuffer** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_MATRIX: {
			const MOAIDrawingParam::SetMatrix* param = ( const MOAIDrawingParam::SetMatrix* )rawParam;
			gfxMgr.SetMtx ( param->mMatrixID, param->mMatrix );
			break;
		}
		
		case MOAIDrawingCmdEnum::SET_PEN_COLOR:
			gfxMgr.SetPenColor ( *( u32* )rawParam );
			break;
					
		case MOAIDrawingCmdEnum::SET_PEN_WIDTH:
			gfxMgr.SetPenWidth ( *( float* )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_SCISSOR_RECT:
			gfxMgr.SetScissorRect ( *( ZLRect* )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_SHADER:
			gfxMgr.SetShader ( *( MOAIShader** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_TEXTURE: {
			const MOAIDrawingParam::SetTexture* param = ( const MOAIDrawingParam::SetTexture* )rawParam;
			gfxMgr.SetTexture ( param->mTexture, param->mTextureUnit );
			break;
		}
		
		case MOAIDrawingCmdEnum::SET_UNIFORM: {
			const MOAIDrawingParam::SetUniform* param = ( const MOAIDrawingParam::SetUniform* )rawParam;
			MOAIDrawingCommand::ExecuteSetUniform ( gfxMgr, *param );
			break;
		}
		
		case MOAIDrawingCmdEnum::SET_VERTEX_ARRAY:
			gfxMgr.SetVertexArray ( *( MOAIVertexArray** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_VERTEX_BUFFER:
			gfxMgr.SetVertexBuffer ( *( MOAIVertexBuffer** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_VERTEX_FORMAT:
			gfxMgr.SetVertexFormat ( *( MOAIVertexFormat** )rawParam );
			break;
		
		case MOAIDrawingCmdEnum::SET_VIEW_RECT:
			gfxMgr.SetViewRect ( *( ZLRect* )rawParam );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIDrawingCommand::ExecuteDrawAxis2D ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxis2D& param ) {

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
void MOAIDrawingCommand::ExecuteDrawAxisGrid2D ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxisGrid2D& param ) {

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
void MOAIDrawingCommand::ExecuteDrawLine ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawLine& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );
	
		gfxMgr.WriteVtx ( param.mV0.mX, param.mV0.mY, param.mV0.mZ );
		gfxMgr.WritePenColor4b ();
		
		gfxMgr.WriteVtx ( param.mV1.mX, param.mV1.mY, param.mV1.mZ  );
		gfxMgr.WritePenColor4b ();
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawingCommand::ExecuteDrawPoint ( MOAIGfxMgr& gfxMgr, const ZLVec3D& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::POINT_LIST, 1 );
		gfxMgr.WriteVtx ( param.mX, param.mY, param.mZ );
		gfxMgr.WritePenColor4b ();
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawingCommand::ExecuteDrawTriangle ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawTriangle& param ) {

	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::TRIANGLE_LIST, 3 );
	
		gfxMgr.WriteVtx ( param.mV0.mX, param.mV0.mY, param.mV0.mZ );
		gfxMgr.WritePenColor4b ();
	
		gfxMgr.WriteVtx ( param.mV1.mX, param.mV1.mY, param.mV1.mZ );
		gfxMgr.WritePenColor4b ();
	
		gfxMgr.WriteVtx ( param.mV2.mX, param.mV2.mY, param.mV2.mZ );
		gfxMgr.WritePenColor4b ();

	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawingCommand::ExecuteSetUniform ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::SetUniform& param ) {

	MOAIShader* shader = gfxMgr.GetShader ();
	if ( !shader ) return;
	
	MOAIUniformHandle uniform = shader->GetUniformHandle ( param.mTargetUniformID, param.mTargetUniformIndex );
	if ( !uniform.IsValid ()) return;

	if ( param.mPipelineGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {

		uniform.SetValue ( gfxMgr.GetMtx ( param.mPipelineGlobalID ));
	}
	else {

		switch (( ZLSize )param.mPipelineGlobalID ) {

			case MOAIGfxMgr::PEN_COLOR:

				uniform.SetValue ( gfxMgr.GetFinalColor ());
				break;

			case MOAIGfxMgr::VIEW_HALF_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height () * 0.5f );
				break;

			case MOAIGfxMgr::VIEW_HALF_WIDTH: {

				uniform.SetValue ( gfxMgr.GetViewRect ().Width () * 0.5f );
				break;
			}
			case MOAIGfxMgr::VIEW_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height ());
				break;

			case MOAIGfxMgr::VIEW_WIDTH:

				uniform.SetValue ( gfxMgr.GetViewRect ().Width ());
				break;
		}
	}
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
