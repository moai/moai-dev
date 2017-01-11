// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGraphicsPropBase.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIRenderMgr.h>
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
// TODO: doxygen
int MOAIGraphicsPropBase::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	u32 subPrimID		= state.GetValue < u32 >( 2, MOAIPartitionHull::NO_SUBPRIM_ID );
	float lodFactor		= state.GetValue < float >( 3, 1.0f );
	
	self->Draw ( subPrimID, lodFactor );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getBillboard ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mBillboard );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScissorRect
	@text	Retrieve the prop's scissor rect.
	
	@in		MOAIGraphicsPropBase self
	@out	MOAIScissorRect scissorRect 	Or nil if none exists.
*/
int MOAIGraphicsPropBase::_getScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	if ( self->mScissorRect ) {
		self->mScissorRect->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isVisible
	@text	Returns true if the given prop is visible. An optional
			LOD factor may be passed in to test the prop's LOD
			settings.
	
	@in		MOAIGraphicsPropBase self
	@opt	number lod
	@out	boolean isVisible		Indicates whether the prop is visible.
*/
int	MOAIGraphicsPropBase::_isVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

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
/**	@lua	setBillboard
	@text	Sets the prop's billboarding mode. One of BILLBOARD_NORMAL,
			BILLBOARD_ORTHO, BILLBOARD_COMPASS, BILLBOARD_SCREEN,
			BILLBOARD_NONE.
	
	@override
	
		@in		MOAIGraphicsPropBase self
		@in		boolean billboard			true == BILLBOARD_NORMAL, false == BILLBOARD_NONE
		@out	nil
	
	@override
	
		@in		MOAIGraphicsPropBase self
		@in		number mode
		@out	nil
*/
int MOAIGraphicsPropBase::_setBillboard ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

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
// TODO: doxygen
int MOAIGraphicsPropBase::_setLODLimits ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
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
	
	@in		MOAIGraphicsPropBase self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIGraphicsPropBase::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( PACK_ATTR ( MOAIColor, INHERIT_COLOR ), parent, PACK_ATTR ( MOAIColor, COLOR_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAITransformBase, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAIGraphicsPropBase, INHERIT_VISIBLE ), parent, PACK_ATTR ( MOAIGraphicsPropBase, ATTR_VISIBLE ));
	
	//MOAILogF ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setScissorRect
	@text	Set or clear the prop's scissor rect.
	
	@in		MOAIGraphicsPropBase self
	@opt	MOAIScissorRect scissorRect		Default value is nil.
	@out	nil
*/
int MOAIGraphicsPropBase::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	MOAIScissorRect* scissorRect = state.GetLuaObject < MOAIScissorRect >( 2, true );
	self->mScissorRect.Set ( *self, scissorRect );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIGraphicsPropBase self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIGraphicsPropBase::_setUVTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
	self->SetDependentMember < MOAITransformBase >( self->mUVTransform, transform );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIGraphicsPropBase self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIGraphicsPropBase::_setVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	bool visible = state.GetValue < bool >( 2, true );
	self->SetVisible ( visible );

	return 0;
}

//================================================================//
// MOAIGraphicsPropBase
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGraphicsPropBase::GetWorldDrawingMtx () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIRenderMgr& renderMgr = MOAIRenderMgr::Get ();

	ZLMatrix4x4 worldDrawingMtx;

	switch ( this->mBillboard ) {
	
		case BILLBOARD_NORMAL: {
			
			ZLAffine3D billboardMtx ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::INVERSE_VIEW_MTX )); // inv view mtx sans translation
			
			billboardMtx.m [ ZLAffine3D::C3_R0 ] = 0.0f;
			billboardMtx.m [ ZLAffine3D::C3_R1 ] = 0.0f;
			billboardMtx.m [ ZLAffine3D::C3_R2 ] = 0.0f;
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetBillboardMtx ( billboardMtx ));
			break;
		}
		
		case BILLBOARD_ORTHO: {
			
			const ZLMatrix4x4& proj				= gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::PROJ_MTX );
			const ZLMatrix4x4& invViewProj		= gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::INVERSE_VIEW_PROJ_MTX );
			const ZLMatrix4x4& invWindowMtx		= gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::INVERSE_WINDOW_MTX );
			
			ZLMatrix4x4 view					= gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::VIEW_MTX );
			ZLMatrix4x4 windowMtx				= gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WINDOW_MTX );
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
			
			// world space location for prop
			ZLVec3D worldLoc;
			worldLoc.mX = worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ];
			worldLoc.mY = worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ];
			worldLoc.mZ = worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ];
			
			// view and project the location
			view.Transform ( worldLoc );	// perspective view
			proj.Project ( worldLoc );		// perspective projection
			
			// now we're going to "unproject" the poing back, going from window space to *ortho* space
			// this is as though we had projected the point using a 2D window matrix instead of the camera matrix
			
			invWindowMtx.Transform ( worldLoc );

			// prop will start at the origin; will be moved by camera after rotation
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ] = 0.0f;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ] = 0.0f;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;

			// keep camera rotation, but replace translation with "projected" location of prop
			view.m [ ZLMatrix4x4::C3_R0 ] = worldLoc.mX;
			view.m [ ZLMatrix4x4::C3_R1 ] = worldLoc.mY;
			view.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
			
			worldDrawingMtx.Append ( view );
			
			// apply the 2D "window" projection to get our final transform
			windowMtx.m [ ZLMatrix4x4::C2_R2 ] = 0.0f;
			worldDrawingMtx.Append ( windowMtx );
			
			// now roll everything back given the current view/proj
			// this will be applied once more by the pipeline, thus cancelling itself out and leaving us with the previous transform
			worldDrawingMtx.Append ( invViewProj );
			
			break;
		}
		
		case BILLBOARD_COMPASS: {
		
			//const ZLAffine3D& cameraMtx = camera->GetLocalToWorldMtx (); // inv view mtx
			ZLAffine3D cameraMtx ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::INVERSE_VIEW_MTX ));
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
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
			worldDrawingMtx.Prepend ( billboardMtx );
		
			break;
		}
		
		case BILLBOARD_SCREEN: {
			
			//MOAIGfxMgr::Get ().GetWorldToWndMtx ();
			
			//ZLMatrix4x4 viewProjMtx = camera->GetWorldToWndMtx ( *viewport );
			
			ZLMatrix4x4 viewProjMtx;
			viewProjMtx.Ident (); // TODO
			
			ZLMatrix4x4 localToWorldMtx ( this->GetLocalToWorldMtx ());
			
			// TODO: check that pivot is supported correctly
			ZLVec3D loc;
			worldDrawingMtx.GetTranslation ( loc );
			viewProjMtx.Project ( loc );
			
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ] = loc.mX;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ] = loc.mY;
			worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ] = loc.mZ;
			
			viewProjMtx.Inverse ();
			worldDrawingMtx.Append ( viewProjMtx );
			
			break;
		}
		
		case BILLBOARD_NONE:
		default:
		
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
	}
	
	return worldDrawingMtx;
}

//----------------------------------------------------------------//
bool MOAIGraphicsPropBase::IsVisible () {
	return (( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) && ( this->mDisplayFlags & FLAGS_VISIBLE ));
}

//----------------------------------------------------------------//
bool MOAIGraphicsPropBase::IsVisible ( float lod ) {

	if ( this->IsVisible ()) {
	
		if (( this->mLODFlags & LOD_FLAGS_MIN_LIMIT ) && ( lod < this->mLODMin )) return false;
		if (( this->mLODFlags & LOD_FLAGS_MAX_LIMIT ) && ( lod > this->mLODMax )) return false;
	
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::LoadUVTransform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( this->mUVTransform ) {
		ZLAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::UV_MTX, uvMtx );
	}
	else {
		gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::UV_MTX );
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::LoadVertexTransform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::WORLD_MTX, this->GetWorldDrawingMtx ());
}

//----------------------------------------------------------------//
MOAIGraphicsPropBase::MOAIGraphicsPropBase () :
	mBillboard ( BILLBOARD_NONE ),
	mLODFlags ( DEFAULT_LOD_FLAGS ),
	mLODMin ( 0.0f ),
	mLODMax ( 0.0f ){
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIColor )
		RTTI_EXTEND ( MOAIRenderable )
		RTTI_EXTEND ( MOAIAbstractDrawable )
		RTTI_EXTEND ( MOAIMaterialBatchHolder )
	RTTI_END
	
	this->mDisplayFlags = DEFAULT_FLAGS;
}

//----------------------------------------------------------------//
MOAIGraphicsPropBase::~MOAIGraphicsPropBase () {
	
	this->mUVTransform.Set ( *this, 0 );
	this->mScissorRect.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::PopGfxState () {

	MOAIMaterialStackMgr::Get ().Pop ();
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::PushGfxState () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mGfxState.SetPenColor ( this->mColor );

	MOAIMaterialStackMgr::Get ().Push ( this->GetMaterial ());
	
	if ( this->mScissorRect ) {
		ZLRect scissorRect = this->mScissorRect->GetScissorRect ( gfxMgr.mGfxState.GetWorldToWndMtx ());
		gfxMgr.mGfxState.SetScissorRect ( scissorRect );
	}
	else {
		gfxMgr.mGfxState.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
	MOAIColor::RegisterLuaClass ( state );
	MOAIMaterialBatchHolder::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_SCISSOR_RECT",			MOAIGraphicsPropBaseAttr::Pack ( ATTR_SCISSOR_RECT ));

	state.SetField ( -1, "ATTR_LOCAL_VISIBLE",			MOAIGraphicsPropBaseAttr::Pack ( ATTR_LOCAL_VISIBLE ));
	state.SetField ( -1, "ATTR_VISIBLE",				MOAIGraphicsPropBaseAttr::Pack ( ATTR_VISIBLE ));
	state.SetField ( -1, "INHERIT_VISIBLE",				MOAIGraphicsPropBaseAttr::Pack ( INHERIT_VISIBLE ));

	state.SetField ( -1, "INHERIT_FRAME",				MOAIGraphicsPropBaseAttr::Pack ( INHERIT_FRAME ));
	state.SetField ( -1, "FRAME_TRAIT",					MOAIGraphicsPropBaseAttr::Pack ( FRAME_TRAIT ));
	
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
	state.SetField ( -1, "BILLBOARD_SCREEN",			( u32 )BILLBOARD_SCREEN );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	MOAIColor::RegisterLuaFuncs ( state );
	MOAIMaterialBatchHolder::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "draw",					_draw },
		{ "getBillboard",			_getBillboard },
		{ "getScissorRect",			_getScissorRect },
		{ "isVisible",				_isVisible },
		{ "setBillboard",			_setBillboard },
		{ "setLODLimits",			_setLODLimits },
		{ "setParent",				_setParent },
		{ "setScissorRect",			_setScissorRect },
		{ "setUVTransform",			_setUVTransform },
		{ "setVisible",				_setVisible },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::Render () {

	this->Draw ( MOAIPartitionHull::NO_SUBPRIM_ID, 0.0f );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIColor::SerializeIn ( state, serializer );
	MOAIRenderable::SerializeIn ( state, serializer );
	MOAIPartitionHull::SerializeIn ( state, serializer );
	MOAIMaterialBatchHolder::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIColor::SerializeOut ( state, serializer );
	MOAIRenderable::SerializeOut ( state, serializer );
	MOAIPartitionHull::SerializeOut ( state, serializer );
	MOAIMaterialBatchHolder::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::SetVisible ( bool visible ) {

	this->mDisplayFlags = visible ? this->mDisplayFlags | FLAGS_LOCAL_VISIBLE : this->mDisplayFlags & ~FLAGS_LOCAL_VISIBLE;
	this->ScheduleUpdate ();
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIAbstractDrawable_DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );

	if ( this->GetBoundsStatus () != BOUNDS_OK ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	this->LoadVertexTransform ();
	
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX ));
	
	ZLBox modelBounds;
	this->GetModelBounds ( modelBounds );
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_AXIS )) {
		draw.DrawBoxAxis ( modelBounds );
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_DIAGONALS )) {
		draw.DrawBoxDiagonals ( modelBounds );
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_BOUNDS )) {
		draw.DrawBoxOutline ( modelBounds );
	}
	
	// clear out the world transform (draw in world space)
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::VIEW_PROJ_MTX ));
	
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
bool MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIGraphicsPropBaseAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
				
			case ATTR_SCISSOR_RECT:
				this->mScissorRect.Set ( *this, attr.ApplyVariantNoAdd < MOAIScissorRect* >( this->mScissorRect, op, MOAIAttribute::ATTR_READ_WRITE ));
				return true;
				
			case ATTR_LOCAL_VISIBLE:
				this->SetVisible ( ZLFloat::ToBoolean ( attr.ApplyNoAdd ( ZLFloat::FromBoolean (( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) != 0 ), op, MOAIAttribute::ATTR_READ_WRITE )));
				return true;
				
			case ATTR_VISIBLE:
				attr.ApplyNoAdd ( ZLFloat::FromBoolean ( this->IsVisible ()), op , MOAIAttribute::ATTR_READ );
				return true;
			
			//case FRAME_TRAIT:
			//	attr.Apply < ZLBox >( &this->mFrame, op, MOAIAttribute::ATTR_READ );
			//	return true;
		}
	}
	
	if ( MOAIColor::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAINode_Update () {
	
	MOAIColor::MOAINode_Update ();
	MOAIPartitionHull::MOAINode_Update ();
	
	bool visible = ZLFloat::ToBoolean ( this->GetLinkedValue ( MOAIGraphicsPropBaseAttr::Pack ( INHERIT_VISIBLE ), 1.0f ));
	this->mDisplayFlags = visible && ( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) ? this->mDisplayFlags | FLAGS_VISIBLE : this->mDisplayFlags & ~FLAGS_VISIBLE ;
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetBounds ());
}

//----------------------------------------------------------------//
u32 MOAIGraphicsPropBase::MOAIPartitionHull_AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIAbstractDrawable >();
}
