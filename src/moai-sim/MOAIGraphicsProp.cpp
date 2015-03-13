// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getScissorRect
	@text	Retrieve the prop's scissor rect.
	
	@in		MOAIProp self
	@out	MOAIScissorRect scissorRect 	Or nil if none exists.
*/
int MOAIGraphicsProp::_getScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	if ( self->mScissorRect ) {
		self->mScissorRect->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTexture
	@text	Returns the texture.
	
	@in		MOAIProp self
	@out	MOAITexture texture
*/
int MOAIGraphicsProp::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isVisible
	@text	Returns true if the given prop is visible. An optional
			LOD factor may be passed in to test the prop's LOD
			settings.
	
	@in		MOAIGraphicsProp self
	@opt	number lod
	@out	boolean is visible
*/
int	MOAIGraphicsProp::_isVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		float lod = state.GetValue < float >( 2, 0.0f );
		lua_pushboolean ( state, self->IsVisible ( lod ));
	}
	else {
		lua_pushboolean ( state, self->IsVisible ());
	}
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsProp::_setBillboard ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	if ( state.IsType ( 2, LUA_TBOOLEAN )) {
		bool billboard = state.GetValue < bool >( 2, false );
		self->mBillboard = billboard ? BILLBOARD_NORMAL : BILLBOARD_NONE;
	}
	else {
		self->mBillboard = state.GetValue < u32 >( 2, BILLBOARD_NONE );
	}
	return 0;
}

//----------------------------------------------------------------//
/** @lua	setBlendEquation
	@text	Set the blend equation. This determines how the srcFactor and dstFactor values set with setBlendMode are interpreted.

	@overload	Reset the blend function to GL_FUNC_ADD.

		@in		MOAIGraphicsProp self
		@out	nil

	@overload	Set the blend equation.

		@in		MOAIGraphicsProp self
		@in		number equation				One of GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT.
		@out	nil
	
*/
int MOAIGraphicsProp::_setBlendEquation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		u32 equation = state.GetValue < u32 >( 2, ZGL_BLEND_MODE_ADD );
		self->mBlendMode.SetBlendEquation ( equation );
	}
	else {
		self->mBlendMode.SetBlendEquation ( ZGL_BLEND_MODE_ADD );
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/** @lua	setBlendMode
	@text	Set the blend mode.

	@overload	Reset the blend mode to MOAIGraphicsProp.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA). This will reset the blend function to GL_FUNC_ADD.

		@in		MOAIGraphicsProp self
		@out	nil

	@overload	Set blend mode using one of the Moai presets. This will reset the blend function to GL_FUNC_ADD.

		@in		MOAIGraphicsProp self
		@in		number mode					One of MOAIGraphicsProp.BLEND_NORMAL, MOAIGraphicsProp.BLEND_ADD, MOAIGraphicsProp.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIGraphicsProp.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAIGraphicsProp self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAIGraphicsProp::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		if ( state.IsType ( 3, LUA_TNUMBER )) {
		
			u32 srcFactor = state.GetValue < u32 >( 2, 0 );
			u32 dstFactor = state.GetValue < u32 >( 3, 0 );
			self->mBlendMode.SetBlend ( srcFactor, dstFactor );
		}
		else {
			
			u32 blendMode = state.GetValue < u32 >( 2, MOAIBlendMode::BLEND_NORMAL );
			self->mBlendMode.SetBlend ( blendMode );
		}
	}
	else {
		self->mBlendMode.SetBlend ( MOAIBlendMode::BLEND_NORMAL );
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIGraphicsProp self
	@opt	number cullMode			Default value is MOAIGraphicsProp.CULL_NONE.
	@out	nil
*/
int MOAIGraphicsProp::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	self->mCullMode = state.GetValue < int >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIGraphicsProp self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIGraphicsProp::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	self->mDepthMask = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIGraphicsProp self
	@opt	number depthFunc		Default value is MOAIGraphicsProp.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIGraphicsProp::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	self->mDepthTest = state.GetValue < int >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsProp::_setLODLimits ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	u32 flags = 0;
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->mLODMin = state.GetValue < float >( 2, 0.0f );
		flags |= LOD_FLAGS_MIN_LIMIT;
	}
	
	if ( state.IsType ( 3, LUA_TNUMBER )) {
		self->mLODMax = state.GetValue < float >( 3, 0.0f );
		flags |= LOD_FLAGS_MAX_LIMIT;
	}

	self->mLODFlags = flags;	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIGraphicsProp self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( PACK_ATTR ( MOAIColor, INHERIT_COLOR ), parent, PACK_ATTR ( MOAIColor, COLOR_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAITransformBase, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAIGraphicsProp, INHERIT_VISIBLE ), parent, PACK_ATTR ( MOAIGraphicsProp, ATTR_VISIBLE ));
	
	//MOAILog ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setScissorRect
	@text	Set or clear the prop's scissor rect.
	
	@in		MOAIGraphicsProp self
	@opt	MOAIScissorRect scissorRect		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	MOAIScissorRect* scissorRect = state.GetLuaObject < MOAIScissorRect >( 2, true );
	self->mScissorRect.Set ( *self, scissorRect );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets or clears the prop's shader. The prop's shader takes
			precedence over any shader specified by the deck or its
			elements.
	
	@in		MOAIGraphicsProp self
	@opt	MOAIShader shader	Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )
	
	MOAIShader* shader = state.GetLuaObject < MOAIShader >( 2, true );
	self->SetDependentMember < MOAIShader >( self->mShader, shader );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Set or load a texture for this prop. The prop's texture will
			override the deck's texture.
	
	@in		MOAIGraphicsProp self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIGraphicsProp::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, 2 );
	self->mTexture.Set ( *self, texture );

	if ( texture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIGraphicsProp self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp::_setUVTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
	self->SetDependentMember < MOAITransformBase >( self->mUVTransform, transform );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIGraphicsProp self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIGraphicsProp::_setVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsProp, "U" )

	bool visible = state.GetValue < bool >( 2, true );
	self->SetVisible ( visible );

	return 0;
}

//================================================================//
// MOAIGraphicsProp
//================================================================//

//----------------------------------------------------------------//
u32 MOAIGraphicsProp::AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIGraphicsProp >();
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIGraphicsPropAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_SHADER:
				this->mShader.Set ( *this, attrOp.ApplyNoAdd < MOAIShader* >( this->mShader, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_VARIANT ));
				return true;
			case ATTR_BLEND_MODE:
				attrOp.ApplyNoAdd < MOAIBlendMode >( this->mBlendMode, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_VARIANT );
				return true;
			case ATTR_LOCAL_VISIBLE:
				this->SetVisible ( ZLFloat::ToBoolean ( attrOp.ApplyNoAdd ( ZLFloat::FromBoolean (( this->mFlags & FLAGS_LOCAL_VISIBLE ) != 0 ), op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT )));
				return true;
			case ATTR_VISIBLE:
				attrOp.ApplyNoAdd ( ZLFloat::FromBoolean ( this->IsVisible ()), op , MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			//case FRAME_TRAIT:
			//	attrOp.Apply < ZLBox >( &this->mFrame, op, MOAIAttrOp::ATTR_READ );
			//	return true;
		}
	}
	
	if ( MOAIColor::ApplyAttrOp ( attrID, attrOp, op )) return true;
	return MOAIProp::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );

	if ( !this->IsVisible ( lod )) return;
	if ( !this->mDeck ) return;

	this->LoadGfxState ();
	this->LoadVertexTransform ();
	this->LoadUVTransform ();
	
	if ( this->mGrid ) {
		this->DrawGrid ( subPrimID );
	}
	else {
		this->mDeck->Draw ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ));
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	this->LoadVertexTransform ();
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_AXIS )) {
		
		ZLBox bounds;
		u32 status = this->OnGetModelBounds ( bounds );
		if ( status == BOUNDS_OK ) {
			draw.DrawBoxAxis ( bounds );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_DIAGONALS )) {
		
		ZLBox bounds;
		u32 status = this->OnGetModelBounds ( bounds );
		if ( status == BOUNDS_OK ) {
			draw.DrawBoxDiagonals ( bounds );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_BOUNDS )) {
		
		ZLBox bounds;
		u32 status = this->OnGetModelBounds ( bounds );
		if ( status == BOUNDS_OK ) {
			draw.DrawBoxOutline ( bounds );
		}
	}
	
	// clear out the world transform (draw in world space)
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_WORLD_BOUNDS )) {
		draw.DrawBoxOutline ( this->GetBounds ());
	}
	
	if ( debugLines.IsVisible ( MOAIDebugLines::PARTITION_CELLS ) || debugLines.IsVisible ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
		
		ZLRect cellRect;
		ZLRect paddedRect;
		
		if ( this->GetCellRect ( &cellRect, &paddedRect )) {
			
			if ( cellRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( MOAIDebugLines::PARTITION_CELLS )) {
					draw.DrawRectOutline ( cellRect );
				}
			}
			
			if ( paddedRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
					draw.DrawRectOutline ( paddedRect );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::DrawGrid ( int subPrimID ) {

	MOAIGrid& grid = *this->mGrid;
	MOAICellCoord c0, c1;

	if ( subPrimID == MOAIProp::NO_SUBPRIM_ID ) {
		this->GetGridBoundsInView ( c0, c1 );
	} else {
		c0 = c1 = grid.GetCellCoord ( subPrimID );
	}
	grid.Draw ( this->mDeck, this->mRemapper, c0, c1 );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGraphicsProp::GetWorldDrawingMtx () {

	MOAIRenderMgr& renderMgr = MOAIRenderMgr::Get ();

	MOAIViewport* viewport = renderMgr.GetViewport ();
	MOAICamera* camera = renderMgr.GetCamera ();
	u32 billboard = camera ? this->mBillboard : BILLBOARD_NONE;

	ZLMatrix4x4 worldDrawingMtx;

	switch ( billboard ) {
	
		case BILLBOARD_NORMAL: {
			
			ZLAffine3D billboardMtx;
			billboardMtx.Init ( camera->GetBillboardMtx ());
			worldDrawingMtx.Init ( this->GetBillboardMtx ( billboardMtx ));
			
			break;
		}
		
		case BILLBOARD_ORTHO: {
		
			ZLMatrix4x4 view = camera->GetViewMtx ();
			ZLMatrix4x4 proj = camera->GetProjMtx ( *viewport );
			
			ZLMatrix4x4 invViewProj = view;
			invViewProj.Append ( proj );
			invViewProj.Inverse ();
			
			worldDrawingMtx.Init ( this->GetLocalToWorldMtx ());
			
			// world space location for prop
			ZLVec3D worldLoc;
			worldLoc.mX = worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ];
			worldLoc.mY = worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ];
			worldLoc.mZ = worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ];
			
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ] = 0.0f;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ] = 0.0f;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
			
			view.Transform ( worldLoc );
			proj.Project ( worldLoc );
			
			viewport->GetProjMtxInv ().Transform ( worldLoc );
			
			view.m [ ZLMatrix4x4::C3_R0 ] = worldLoc.mX;
			view.m [ ZLMatrix4x4::C3_R1 ] = worldLoc.mY;
			view.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
			
			proj = viewport->GetProjMtx ();
			proj.m [ ZLMatrix4x4::C2_R2 ] = 0.0f;
			
			worldDrawingMtx.Append ( view );
			worldDrawingMtx.Append ( proj );
			worldDrawingMtx.Append ( invViewProj );
			
			break;
		}
		
		case BILLBOARD_COMPASS: {
		
			const ZLAffine3D& cameraMtx = camera->GetLocalToWorldMtx ();
			//ZLVec3D cameraZ = cameraMtx.GetZAxis ();
			ZLVec3D	cameraY = cameraMtx.GetYAxis ();
			
			cameraY.mZ = 0.0f;
			cameraY.Norm ();
			
			ZLVec2D mapY ( cameraY.mX, cameraY.mY );
			ZLVec2D worldY ( 0.0f, 1.0f );
			
			float radians = mapY.Radians ( worldY );
			
			if ( cameraY.mX < 0.0f ) {
				radians = -radians;
			}
		
			ZLMatrix4x4 billboardMtx;
			billboardMtx.Translate ( -this->mPiv.mX, -this->mPiv.mY, -this->mPiv.mZ );
			
			ZLMatrix4x4 mtx;
			mtx.RotateZ ( -radians );
			billboardMtx.Append ( mtx );
			
			mtx.Translate ( this->mPiv.mX, this->mPiv.mY, this->mPiv.mZ );
			billboardMtx.Append ( mtx );
			
			worldDrawingMtx.Init ( this->GetLocalToWorldMtx ());
			worldDrawingMtx.Prepend ( billboardMtx );
		
			break;
		}
		
		case BILLBOARD_NONE:
		default:
		
			worldDrawingMtx.Init ( this->GetLocalToWorldMtx ());
	}
	
	return worldDrawingMtx;
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::IsVisible () {
	return (( this->mFlags & FLAGS_LOCAL_VISIBLE ) && ( this->mFlags & FLAGS_VISIBLE ));
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::IsVisible ( float lod ) {

	if ( this->IsVisible ()) {
	
		if (( this->mLODFlags & LOD_FLAGS_MIN_LIMIT ) && ( lod < this->mLODMin )) return false;
		if (( this->mLODFlags & LOD_FLAGS_MAX_LIMIT ) && ( lod > this->mLODMax )) return false;
	
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::LoadGfxState () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	MOAIDeckGfxState gfxState;

	if ( this->mDeck ) {
		this->mDeck->GetGfxState ( gfxState );
	}

	gfxState.SetShader ( this->mShader );
	gfxState.SetTexture ( this->mTexture );

	gfxDevice.SetShader ( gfxState.GetShader ());
	gfxDevice.SetGfxState ( gfxState.GetTexture ());

	gfxDevice.SetPenColor ( this->mColor );
	gfxDevice.SetCullFunc ( this->mCullMode );
	gfxDevice.SetDepthFunc ( this->mDepthTest );
	gfxDevice.SetDepthMask ( this->mDepthMask );
	gfxDevice.SetBlendMode ( this->mBlendMode );
	
	if ( this->mScissorRect ) {
		ZLRect scissorRect = this->mScissorRect->GetScissorRect ( gfxDevice.GetWorldToWndMtx ());		
		gfxDevice.SetScissorRect ( scissorRect );
	}
	else {
		gfxDevice.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::LoadUVTransform () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( this->mUVTransform ) {
		ZLAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		gfxDevice.SetUVTransform ( uvMtx );
	}
	else {
		gfxDevice.SetUVTransform ();
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::LoadVertexTransform () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetWorldDrawingMtx ());
}

//----------------------------------------------------------------//
MOAIGraphicsProp::MOAIGraphicsProp () :
	mBillboard ( BILLBOARD_NONE ),
	mCullMode ( 0 ),
	mDepthTest ( 0 ),
	mDepthMask ( true ),
	mLODFlags ( DEFAULT_LOD_FLAGS ),
	mLODMin ( 0.0f ),
	mLODMax ( 0.0f ){
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIColor )
		RTTI_EXTEND ( MOAIRenderable )
	RTTI_END
	
	this->mFlags = DEFAULT_FLAGS;
	//this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
}

//----------------------------------------------------------------//
MOAIGraphicsProp::~MOAIGraphicsProp () {
	
	this->mShader.Set ( *this, 0 );
	this->mTexture.Set ( *this, 0 );
	this->mUVTransform.Set ( *this, 0 );
	this->mScissorRect.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::OnDepNodeUpdate () {
	
	MOAIColor::OnDepNodeUpdate ();
	MOAIProp::OnDepNodeUpdate ();
	
	bool visible = ZLFloat::ToBoolean ( this->GetLinkedValue ( MOAIGraphicsPropAttr::Pack ( INHERIT_VISIBLE ), 1.0f ));
	this->mFlags = visible && ( this->mFlags & FLAGS_LOCAL_VISIBLE ) ? this->mFlags | FLAGS_VISIBLE : this->mFlags & ~FLAGS_VISIBLE ;	
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	MOAIColor::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_SHADER",					MOAIGraphicsPropAttr::Pack ( ATTR_SHADER ));
	state.SetField ( -1, "ATTR_BLEND_MODE",				MOAIGraphicsPropAttr::Pack ( ATTR_BLEND_MODE ));
	state.SetField ( -1, "ATTR_VISIBLE",				MOAIGraphicsPropAttr::Pack ( ATTR_VISIBLE ));

	state.SetField ( -1, "ATTR_LOCAL_VISIBLE",			MOAIGraphicsPropAttr::Pack ( ATTR_LOCAL_VISIBLE ));
	state.SetField ( -1, "ATTR_VISIBLE",				MOAIGraphicsPropAttr::Pack ( ATTR_VISIBLE ));
	state.SetField ( -1, "INHERIT_VISIBLE",				MOAIGraphicsPropAttr::Pack ( INHERIT_VISIBLE ));

	state.SetField ( -1, "INHERIT_FRAME",				MOAIGraphicsPropAttr::Pack ( INHERIT_FRAME ));
	state.SetField ( -1, "FRAME_TRAIT",					MOAIGraphicsPropAttr::Pack ( FRAME_TRAIT ));
	
	state.SetField ( -1, "BLEND_ADD",					( u32 )MOAIBlendMode::BLEND_ADD );
	state.SetField ( -1, "BLEND_MULTIPLY",				( u32 )MOAIBlendMode::BLEND_MULTIPLY );
	state.SetField ( -1, "BLEND_NORMAL",				( u32 )MOAIBlendMode::BLEND_NORMAL );
	
	state.SetField ( -1, "GL_FUNC_ADD",					( u32 )ZGL_BLEND_MODE_ADD );
	state.SetField ( -1, "GL_FUNC_SUBTRACT",			( u32 )ZGL_BLEND_MODE_SUBTRACT );
	state.SetField ( -1, "GL_FUNC_REVERSE_SUBTRACT",	( u32 )ZGL_BLEND_MODE_REVERSE_SUBTRACT );
	
	state.SetField ( -1, "GL_ONE",						( u32 )ZGL_BLEND_FACTOR_ONE );
	state.SetField ( -1, "GL_ZERO",						( u32 )ZGL_BLEND_FACTOR_ZERO );
	state.SetField ( -1, "GL_DST_ALPHA",				( u32 )ZGL_BLEND_FACTOR_DST_ALPHA );
	state.SetField ( -1, "GL_DST_COLOR",				( u32 )ZGL_BLEND_FACTOR_DST_COLOR );
	state.SetField ( -1, "GL_SRC_COLOR",				( u32 )ZGL_BLEND_FACTOR_SRC_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_DST_ALPHA",		( u32 )ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_DST_COLOR",		( u32 )ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_ALPHA",		( u32 )ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_COLOR",		( u32 )ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR );
	state.SetField ( -1, "GL_SRC_ALPHA",				( u32 )ZGL_BLEND_FACTOR_SRC_ALPHA );
	state.SetField ( -1, "GL_SRC_ALPHA_SATURATE",		( u32 )ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE );
	
	state.SetField ( -1, "DEPTH_TEST_DISABLE",			( u32 )0 );
	state.SetField ( -1, "DEPTH_TEST_NEVER",			( u32 )ZGL_DEPTH_NEVER );
	state.SetField ( -1, "DEPTH_TEST_LESS",				( u32 )ZGL_DEPTH_LESS );
	state.SetField ( -1, "DEPTH_TEST_EQUAL",			( u32 )ZGL_DEPTH_EQUAL );
	state.SetField ( -1, "DEPTH_TEST_LESS_EQUAL",		( u32 )ZGL_DEPTH_LEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER",			( u32 )ZGL_DEPTH_GREATER );
	state.SetField ( -1, "DEPTH_TEST_NOTEQUAL",			( u32 )ZGL_DEPTH_NOTEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER_EQUAL",	( u32 )ZGL_DEPTH_GEQUAL );
	state.SetField ( -1, "DEPTH_TEST_ALWAYS",			( u32 )ZGL_DEPTH_ALWAYS );
	
	state.SetField ( -1, "CULL_NONE",					( u32 )0 );
	state.SetField ( -1, "CULL_ALL",					( u32 )ZGL_CULL_ALL );
	state.SetField ( -1, "CULL_BACK",					( u32 )ZGL_CULL_BACK );
	state.SetField ( -1, "CULL_FRONT",					( u32 )ZGL_CULL_FRONT );
	
	state.SetField ( -1, "BILLBOARD_NONE",				( u32 )BILLBOARD_NONE );
	state.SetField ( -1, "BILLBOARD_NORMAL",			( u32 )BILLBOARD_NORMAL );
	state.SetField ( -1, "BILLBOARD_ORTHO",				( u32 )BILLBOARD_ORTHO );
	state.SetField ( -1, "BILLBOARD_COMPASS",			( u32 )BILLBOARD_COMPASS );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIColor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getScissorRect",		_getScissorRect },
		{ "getTexture",			_getTexture },
		{ "isVisible",			_isVisible },
		{ "setBillboard",		_setBillboard },
		{ "setBlendEquation",	_setBlendEquation },
		{ "setBlendMode",		_setBlendMode },
		{ "setCullMode",		_setCullMode },
		{ "setDepthMask",		_setDepthMask },
		{ "setDepthTest",		_setDepthTest },
		{ "setLODLimits",		_setLODLimits },
		{ "setParent",			_setParent },
		{ "setScissorRect",		_setScissorRect },
		{ "setShader",			_setShader },
		{ "setTexture",			_setTexture },
		{ "setUVTransform",		_setUVTransform },
		{ "setVisible",			_setVisible },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::Render () {

	this->Draw ( MOAIGraphicsProp::NO_SUBPRIM_ID, 0.0f );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIProp::SerializeIn ( state, serializer );
	MOAIColor::SerializeIn ( state, serializer );
	MOAIRenderable::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIProp::SerializeOut ( state, serializer );
	MOAIColor::SerializeOut ( state, serializer );
	MOAIRenderable::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SetVisible ( bool visible ) {

	this->mFlags = visible ? this->mFlags | FLAGS_LOCAL_VISIBLE : this->mFlags & ~FLAGS_LOCAL_VISIBLE;
	this->ScheduleUpdate ();
}
