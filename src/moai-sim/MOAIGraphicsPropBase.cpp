// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGraphicsPropBase.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// MOAIGraphicsPropBaseCallable
//================================================================//
class MOAIGraphicsPropBaseCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	MOAIGraphicsPropBase*		mProp;
	int							mSubPrimID;

	//----------------------------------------------------------------//
	void MOAIAbstractGfxScriptCallback_Call () {
	
		this->mProp->MOAIGraphicsPropBase_Draw ( this->mSubPrimID );
	}
};

//================================================================//
// lua
//================================================================//

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
	@out	boolean isVisible		Indicates whether the prop is visible.
*/
int	MOAIGraphicsPropBase::_isVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		lua_pushboolean ( state, self->IsVisible ());
	}
	else {
		lua_pushboolean ( state, self->IsVisible ());
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setBillboard
	@text	Sets the prop's billboarding mode. One of BILLBOARD_NORMAL,
			BILLBOARD_ORTHO, BILLBOARD_COMPASS, BILLBOARD_COMPASS_SCALE,
			BILLBOARD_SCREEN, BILLBOARD_NONE.
	
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
	MOAI_LUA_RETURN_SELF
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
	
	self->SetAttrLink ( MOAIColor::AttrID_INHERIT_COLOR (),							parent, MOAIColor::AttrID_COLOR_TRAIT ());
	self->SetAttrLink ( MOAIAbstractChildTransform::AttrID_INHERIT_TRANSFORM (),	parent, MOAIAbstractBaseTransform::AttrID_TRANSFORM_TRAIT ());
	self->SetAttrLink ( AttrID_INHERIT_VISIBLE (),									parent, AttrID_ATTR_VISIBLE ());
		
	MOAI_LUA_RETURN_SELF
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
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIGraphicsPropBase self
	@opt	MOAIAbstractChildTransform transform	Default value is nil.
	@out	nil
*/
int MOAIGraphicsPropBase::_setUVTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
	self->SetDependentMember < MOAIAbstractChildTransform >( self->mUVTransform, transform );

	MOAI_LUA_RETURN_SELF
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

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIGraphicsPropBase
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGraphicsPropBase::GetWorldDrawingMtx () const {

	return this->MOAIGraphicsPropBase_GetWorldDrawingMtx ();
}

//----------------------------------------------------------------//
bool MOAIGraphicsPropBase::IsVisible () {
	return (( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) && ( this->mDisplayFlags & FLAGS_VISIBLE ));
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::LoadUVTransform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( this->mUVTransform ) {
		ZLAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_UV_MTX, uvMtx );
	}
	else {
		gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_UV_MTX );
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::LoadVertexTransform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX, this->GetWorldDrawingMtx ());
}

//----------------------------------------------------------------//
MOAIGraphicsPropBase::MOAIGraphicsPropBase () :
	mBillboard ( BILLBOARD_NONE ) {
	
	RTTI_BEGIN ( MOAIGraphicsPropBase )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGraphicsPropBase >)
		RTTI_EXTEND ( MOAIAbstractProp )
		RTTI_EXTEND ( MOAIColor )
		RTTI_EXTEND ( MOAIRenderNode )
	RTTI_END
	
	this->mDisplayFlags = DEFAULT_FLAGS;
}

//----------------------------------------------------------------//
MOAIGraphicsPropBase::~MOAIGraphicsPropBase () {
	
	this->mUVTransform.Set ( *this, 0 );
	this->mScissorRect.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::SetVisible ( bool visible ) {

	this->mDisplayFlags = visible ? this->mDisplayFlags | FLAGS_LOCAL_VISIBLE : this->mDisplayFlags & ~FLAGS_LOCAL_VISIBLE;
	this->ScheduleUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	MOAIDebugLinesMgr::Get ().ReserveStyleSet < MOAIGraphicsPropBase >( TOTAL_DEBUG_LINE_STYLES );
	
	state.SetField ( -1, "DEBUG_DRAW_GFX_PROP_MASTER",			MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( (u32) -1 ));
	state.SetField ( -1, "DEBUG_DRAW_PARTITION_CELLS",			MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_PARTITION_CELLS ));
	state.SetField ( -1, "DEBUG_DRAW_PARTITION_PADDED_CELLS",	MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_PARTITION_PADDED_CELLS ));
	state.SetField ( -1, "DEBUG_DRAW_AXIS",						MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_AXIS ));
	state.SetField ( -1, "DEBUG_DRAW_DIAGONALS",				MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_DIAGONALS ));
	state.SetField ( -1, "DEBUG_DRAW_MODEL_BOUNDS",				MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_MODEL_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_WORLD_BOUNDS",				MOAIDebugLinesMgr::Pack < MOAIGraphicsPropBase >( DEBUG_DRAW_WORLD_BOUNDS ));
	
	state.SetField ( -1, "ATTR_SCISSOR_RECT",			AttrID::Pack ( ATTR_SCISSOR_RECT ).ToRaw ());

	state.SetField ( -1, "ATTR_LOCAL_VISIBLE",			AttrID::Pack ( ATTR_LOCAL_VISIBLE ).ToRaw ());
	state.SetField ( -1, "ATTR_VISIBLE",				AttrID::Pack ( ATTR_VISIBLE ).ToRaw ());
	state.SetField ( -1, "INHERIT_VISIBLE",				AttrID::Pack ( INHERIT_VISIBLE ).ToRaw ());

	state.SetField ( -1, "INHERIT_FRAME",				AttrID::Pack ( INHERIT_FRAME ).ToRaw ());
	state.SetField ( -1, "FRAME_TRAIT",					AttrID::Pack ( FRAME_TRAIT ).ToRaw ());
	
	state.SetField ( -1, "GL_FUNC_ADD",					( u32 )MOAIBlendFuncEnum::ADD );
	state.SetField ( -1, "GL_FUNC_SUBTRACT",			( u32 )MOAIBlendFuncEnum::SUBTRACT );
	state.SetField ( -1, "GL_FUNC_REVERSE_SUBTRACT",	( u32 )MOAIBlendFuncEnum::REVERSE_SUBTRACT );
	
	state.SetField ( -1, "GL_ONE",						( u32 )MOAIBlendFactorEnum::ONE );
	state.SetField ( -1, "GL_ZERO",						( u32 )MOAIBlendFactorEnum::ZERO );
	state.SetField ( -1, "GL_DST_ALPHA",				( u32 )MOAIBlendFactorEnum::DST_ALPHA );
	state.SetField ( -1, "GL_DST_COLOR",				( u32 )MOAIBlendFactorEnum::DST_COLOR );
	state.SetField ( -1, "GL_SRC_COLOR",				( u32 )MOAIBlendFactorEnum::SRC_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_DST_ALPHA",		( u32 )MOAIBlendFactorEnum::ONE_MINUS_DST_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_DST_COLOR",		( u32 )MOAIBlendFactorEnum::ONE_MINUS_DST_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_ALPHA",		( u32 )MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_COLOR",		( u32 )MOAIBlendFactorEnum::ONE_MINUS_SRC_COLOR );
	state.SetField ( -1, "GL_SRC_ALPHA",				( u32 )MOAIBlendFactorEnum::SRC_ALPHA );
	state.SetField ( -1, "GL_SRC_ALPHA_SATURATE",		( u32 )MOAIBlendFactorEnum::SRC_ALPHA_SATURATE );
	
	state.SetField ( -1, "DEPTH_TEST_DISABLE",			( u32 )MOAIDepthFuncEnum::NONE );
	state.SetField ( -1, "DEPTH_TEST_NEVER",			( u32 )MOAIDepthFuncEnum::NEVER );
	state.SetField ( -1, "DEPTH_TEST_LESS",				( u32 )MOAIDepthFuncEnum::LESS );
	state.SetField ( -1, "DEPTH_TEST_EQUAL",			( u32 )MOAIDepthFuncEnum::EQUAL );
	state.SetField ( -1, "DEPTH_TEST_LESS_EQUAL",		( u32 )MOAIDepthFuncEnum::LEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER",			( u32 )MOAIDepthFuncEnum::GREATER );
	state.SetField ( -1, "DEPTH_TEST_NOTEQUAL",			( u32 )MOAIDepthFuncEnum::NOTEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER_EQUAL",	( u32 )MOAIDepthFuncEnum::GEQUAL );
	state.SetField ( -1, "DEPTH_TEST_ALWAYS",			( u32 )MOAIDepthFuncEnum::ALWAYS );
	
	state.SetField ( -1, "CULL_NONE",					( u32 )MOAICullFuncEnum::NONE );
	state.SetField ( -1, "CULL_ALL",					( u32 )MOAICullFuncEnum::ALL );
	state.SetField ( -1, "CULL_BACK",					( u32 )MOAICullFuncEnum::BACK );
	state.SetField ( -1, "CULL_FRONT",					( u32 )MOAICullFuncEnum::FRONT );
	
	state.SetField ( -1, "BILLBOARD_NONE",				( u32 )BILLBOARD_NONE );
	state.SetField ( -1, "BILLBOARD_NORMAL",			( u32 )BILLBOARD_NORMAL );
	state.SetField ( -1, "BILLBOARD_ORTHO",				( u32 )BILLBOARD_ORTHO );
	state.SetField ( -1, "BILLBOARD_COMPASS",			( u32 )BILLBOARD_COMPASS );
	state.SetField ( -1, "BILLBOARD_COMPASS_SCALE",		( u32 )BILLBOARD_COMPASS_SCALE );
	state.SetField ( -1, "BILLBOARD_SCREEN",			( u32 )BILLBOARD_SCREEN );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getBillboard",			_getBillboard },
		{ "getScissorRect",			_getScissorRect },
		{ "isVisible",				_isVisible },
		{ "setBillboard",			_setBillboard },
		{ "setParent",				_setParent },
		{ "setScissorRect",			_setScissorRect },
		{ "setUVTransform",			_setUVTransform },
		{ "setVisible",				_setVisible },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIDrawable_Draw ( int subPrimID ) {

	if ( !this->IsVisible ()) return;
	if ( this->IsClear ()) return;

	if ( this->MOAIGraphicsPropBase_LoadGfxState ()) {

		MOAIGfxScript* gfxScript = this->GetGfxScript ();
		
		if ( gfxScript ) {
			MOAIGraphicsPropBaseCallable callable;
			callable.mProp = this;
			callable.mSubPrimID = subPrimID;
			gfxScript->ExecuteBytecode ( &callable );
		}
		else {
			this->MOAIGraphicsPropBase_Draw ( subPrimID );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIDrawable_DrawDebug ( int subPrimID ) {

	this->MOAIGraphicsPropBase_DrawDebug ( subPrimID );

	if ( this->GetWorldBounds ().IsEmpty ()) return;

	MOAIDebugLinesMgr& debugLines = MOAIDebugLinesMgr::Get ();
	if ( !( debugLines.IsVisible () && debugLines.SelectStyleSet < MOAIGraphicsPropBase >())) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	draw.BindVectorPresets ();

	this->LoadVertexTransform ();

	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );

	ZLBounds modelBounds = this->GetModelBounds ();

	// TODO: check bounds status

	if ( debugLines.Bind ( DEBUG_DRAW_AXIS )) {
		draw.DrawBoxAxis ( modelBounds.mAABB );
	}

	if ( debugLines.Bind ( DEBUG_DRAW_DIAGONALS )) {
		draw.DrawBoxDiagonals ( modelBounds.mAABB );
	}

	if ( debugLines.Bind ( DEBUG_DRAW_MODEL_BOUNDS )) {
		draw.DrawBoxOutline ( modelBounds.mAABB );
	}

	// clear out the world transform (draw in world space)
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );

	if ( debugLines.Bind ( DEBUG_DRAW_WORLD_BOUNDS )) {
		draw.DrawBoxOutline ( this->GetWorldBounds ().mAABB );
	}

	if ( debugLines.IsVisible ( DEBUG_DRAW_PARTITION_CELLS ) || debugLines.IsVisible ( DEBUG_DRAW_PARTITION_CELLS )) {

		ZLRect cellRect;
		ZLRect paddedRect;

		if ( this->GetCellRect ( &cellRect, &paddedRect )) {

			if ( cellRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( DEBUG_DRAW_PARTITION_CELLS )) {
					draw.DrawRectOutline ( cellRect );
				}
			}

			if ( paddedRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( DEBUG_DRAW_PARTITION_PADDED_CELLS )) {
					draw.DrawRectOutline ( paddedRect );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIGraphicsPropBase_Draw ( int subPrimID ) {
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAIGraphicsPropBase_DrawDebug ( int subPrimID ) {
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGraphicsPropBase::MOAIGraphicsPropBase_GetWorldDrawingMtx () const {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	ZLMatrix4x4 worldDrawingMtx;

	switch ( this->mBillboard ) {
	
		case BILLBOARD_NORMAL: {
			
			ZLAffine3D billboardMtx ( gfxMgr.GetMtx ( MOAIGfxMgr::VIEW_TO_WORLD_MTX )); // inv view mtx sans translation
			
			billboardMtx.m [ ZLAffine3D::C3_R0 ] = 0.0f;
			billboardMtx.m [ ZLAffine3D::C3_R1 ] = 0.0f;
			billboardMtx.m [ ZLAffine3D::C3_R2 ] = 0.0f;
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetBillboardMtx ( billboardMtx ));
			break;
		}
		
		case BILLBOARD_ORTHO: {
			
			const ZLMatrix4x4& proj				= gfxMgr.GetMtx ( MOAIGfxMgr::VIEW_TO_CLIP_MTX );
			const ZLMatrix4x4& invViewProj		= gfxMgr.GetMtx ( MOAIGfxMgr::CLIP_TO_WORLD_MTX );
			const ZLMatrix4x4& invWindowMtx		= gfxMgr.GetMtx ( MOAIGfxMgr::WINDOW_TO_CLIP_MTX );
			
			ZLMatrix4x4 view					= gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_VIEW_MTX );
			ZLMatrix4x4 windowMtx				= gfxMgr.GetMtx ( MOAIGfxMgr::CLIP_TO_WINDOW_MTX );
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
			
			// world space location for prop
			ZLVec3D worldLoc;
			worldLoc.mX = worldDrawingMtx.m [ ZLMatrix4x4::C3_R0 ];
			worldLoc.mY = worldDrawingMtx.m [ ZLMatrix4x4::C3_R1 ];
			worldLoc.mZ = worldDrawingMtx.m [ ZLMatrix4x4::C3_R2 ];
			
			// view and project the location
			view.Transform ( worldLoc );	// perspective view
			proj.Project ( worldLoc );		// perspective projection
			
			// now we're going to "unproject" the point back, going from window space to *ortho* space
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
		
			ZLAffine3D cameraMtx ( gfxMgr.GetMtx ( MOAIGfxMgr::VIEW_TO_WORLD_MTX ));
			ZLVec3D	cameraY = cameraMtx.GetYAxis ();
			
			cameraY.mZ = 0.0f;
			cameraY.Norm ();
			
			ZLVec2D mapY ( cameraY.mX, cameraY.mY );
			ZLVec2D worldY ( 0.0f, 1.0f );
			
			float radians = -mapY.Radians ( worldY );
			
			if ( cameraY.mX < 0.0f ) {
				radians = -radians;
			}
		
			ZLMatrix4x4 billboardMtx;
			billboardMtx.Translate ( -this->mPiv.mX, -this->mPiv.mY, -this->mPiv.mZ );
			
			ZLMatrix4x4 mtx;
			mtx.RotateZ ( radians );
			billboardMtx.Append ( mtx );
			
			mtx.Translate ( this->mPiv.mX, this->mPiv.mY, this->mPiv.mZ );
			billboardMtx.Append ( mtx );
			
			// now that we've calculated the compass billboard, here's the tricky part.
			// we need to sandwich it into the local-to-world matrix, after the scale and rotate,
			// but before the translation. it's not sufficient just to prepend as we'd wind up
			// with compass-scale-rotate-translate. in that scenario, if scale != 1, then it will
			// mess up the compass transform. so we need to do scale-rotate-compass-translate instead.
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
			
			ZLVec4D localToWorldTranslation = worldDrawingMtx.GetColumn ( 3 ); // extract the translation
			worldDrawingMtx.SetColumn ( 3, ZLVec4D::ORIGIN ); // set translation back to origin
			worldDrawingMtx.Append ( billboardMtx ); // this gives us scale-rotate-billboard
			worldDrawingMtx.SetColumn ( 3, localToWorldTranslation ); // scale-rotate-billboard-translation
			
			break;
		}

		case BILLBOARD_COMPASS_SCALE: {
		
			ZLMatrix4x4 viewToClip = gfxMgr.GetMtx ( MOAIGfxMgr::VIEW_TO_CLIP_MTX );
			ZLMatrix4x4 worldToClip = gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
			ZLMatrix4x4 clipToWindow = gfxMgr.GetMtx ( MOAIGfxMgr::CLIP_TO_WINDOW_MTX );
			// TODO: The cache for VIEW_TO_WORLD matrix is not working, so manually inverting here, which not efficient
			ZLMatrix4x4 worldToView = gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_VIEW_MTX );
			ZLMatrix4x4 worldToViewInv;
			
			worldDrawingMtx = ZLMatrix4x4 ( this->GetLocalToWorldMtx ());
			
			bool worldToViewInvSuccess = worldToViewInv.Inverse(worldToView);
			
			if (!worldToViewInvSuccess) {
				break;
			}
			
			ZLAffine3D cameraMtx ( worldToViewInv );
			
			ZLVec3D	cameraY = cameraMtx.GetYAxis ();
			
			cameraY.mZ = 0.0f;
			cameraY.Norm ();
			
			ZLVec2D mapY ( cameraY.mX, cameraY.mY );
			ZLVec2D worldY ( 0.0f, 1.0f );
			
			float flipped = copysign(1.0f, worldDrawingMtx.GetYAxis().Dot(ZLVec3D(0.0f, 1.0f, 0.0f)));
			float radians = mapY.Radians ( worldY ) * flipped;
			
			if ( cameraY.mX < 0.0f ) {
				radians = -radians;
			}
			
			// Determine scale to match camera
			ZLVec3D loc;
			worldDrawingMtx.GetTranslation ( loc );
			ZLVec4D loc4 (loc.mX, loc.mY, loc.mZ, 1);
			worldToClip.Transform(loc4);
			float sx = loc4.mW * clipToWindow.GetXAxis().Length() / viewToClip.GetXAxis().Length();
			float sy = loc4.mW * clipToWindow.GetYAxis().Length() / viewToClip.GetYAxis().Length();
			
			ZLMatrix4x4 billboardMtx;
			billboardMtx.Translate ( -this->mPiv.mX, -this->mPiv.mY, -this->mPiv.mZ );
			
			ZLMatrix4x4 mtx;
			mtx.RotateZ ( -radians );
			billboardMtx.Append ( mtx );
			
			mtx.Scale ( sx, sy, 1 );
			billboardMtx.Append ( mtx );
			
			mtx.Translate ( this->mPiv.mX, this->mPiv.mY, this->mPiv.mZ );
			billboardMtx.Append ( mtx );
			
			worldDrawingMtx.Prepend ( billboardMtx );
			
			break;
		}
		
		case BILLBOARD_SCREEN: {
			
			//MOAIGfxMgr::Get ().GetWorldToWndMtx ();
			
			//ZLMatrix4x4 viewProjMtx = camera->GetWorldToWndMtx ( *viewport );
			ZLMatrix4x4 viewProjMtx = gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
			viewProjMtx.Append ( gfxMgr.GetMtx ( MOAIGfxMgr::CLIP_TO_WINDOW_MTX ));
			
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
bool MOAIGraphicsPropBase::MOAIGraphicsPropBase_LoadGfxState () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.SetPenColor ( this->mColor );
	
	if ( this->mScissorRect ) {
		ZLRect scissorRect = this->mScissorRect->GetScissorRect ( gfxMgr.GetWorldToWndMtx ());
		gfxMgr.SetScissorRect ( scissorRect );
	}
	else {
		gfxMgr.SetScissorRect ();
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		
		switch ( attrID.Unpack ()) {
				
			case ATTR_SCISSOR_RECT:
				this->mScissorRect.Set ( *this, attr.ApplyVariantNoAdd < MOAIScissorRect* >( this->mScissorRect, op, ZLAttribute::ATTR_READ_WRITE ));
				return true;
				
			case ATTR_LOCAL_VISIBLE:
				this->SetVisible ( ZLFloat::ToBoolean ( attr.ApplyNoAdd ( ZLFloat::FromBoolean (( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) != 0 ), op, ZLAttribute::ATTR_READ_WRITE )));
				return true;
				
			case ATTR_VISIBLE:
				attr.ApplyNoAdd ( ZLFloat::FromBoolean ( this->IsVisible ()), op , ZLAttribute::ATTR_READ );
				return true;
			
			//case FRAME_TRAIT:
			//	attr.Apply < ZLBox >( &this->mFrame, op, ZLAttribute::ATTR_READ );
			//	return true;
		}
	}
	
	if ( MOAIColor::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsPropBase::MOAINode_Update () {
	
	MOAIColor::MOAINode_Update ();
	MOAIAbstractProp::MOAINode_Update ();
	
	bool visible = ZLFloat::ToBoolean ( this->GetLinkedValue ( AttrID::Pack ( INHERIT_VISIBLE ), 1.0f ));
	this->mDisplayFlags = visible && ( this->mDisplayFlags & FLAGS_LOCAL_VISIBLE ) ? this->mDisplayFlags | FLAGS_VISIBLE : this->mDisplayFlags & ~FLAGS_VISIBLE ;
}

