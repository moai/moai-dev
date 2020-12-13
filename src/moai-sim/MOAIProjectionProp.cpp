// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIProjectionProp.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProjectionProp::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProjectionProp, "UUUU" );
	
	MOAIAbstractGraphicsProp* sourceProp = state.GetLuaObject < MOAIAbstractGraphicsProp >( 2, true );
	if ( !sourceProp ) return 0;
	
	MOAIAbstractViewLayer* sourceLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 3, true );
	if ( !sourceLayer ) return 0;
	
	MOAIAbstractViewLayer* destLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 4, true );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceProp, sourceProp );
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIProjectionProp
//================================================================//

//----------------------------------------------------------------//
MOAIProjectionProp::MOAIProjectionProp () :
	mFront ( 1.0 ) {
	
	RTTI_BEGIN ( MOAIProjectionProp )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIProjectionProp >)
		RTTI_EXTEND ( MOAIAbstractRenderNode )
		RTTI_EXTEND ( MOAIPartitionHull )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIProjectionProp::~MOAIProjectionProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIProjectionProp::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	MOAIDebugLinesMgr::Get ().ReserveStyleSet < MOAIProjectionProp >( TOTAL_DEBUG_LINE_STYLES );
	
	state.SetField ( -1, "DEBUG_DRAW_WORLD_BOUNDS",		MOAIDebugLinesMgr::Pack < MOAIProjectionProp >( DEBUG_DRAW_WORLD_BOUNDS ));
	state.SetField ( -1, "ATTR_FRONT",					AttrID::Pack ( ATTR_FRONT ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIProjectionProp::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIAbstractRenderNode_RenderInner ( u32 renderPhase ) {
	UNUSED ( renderPhase );

	if ( this->GetWorldBounds ().IsEmpty ()) return;

	MOAIDebugLinesMgr& debugLines = MOAIDebugLinesMgr::Get ();
	if ( !( debugLines.IsVisible () && debugLines.SelectStyleSet < MOAIProjectionProp >())) return;
	if ( !debugLines.Bind ( DEBUG_DRAW_WORLD_BOUNDS )) return;

	MOAIGfxMgr::Get ().SetVertexTransform ( MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	draw.BindVectorPresets ();
	draw.DrawBoxOutline ( this->GetWorldBounds ().mAABB );
}

//----------------------------------------------------------------//
bool MOAIProjectionProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		switch ( attrID.Unpack ()) {
			case ATTR_FRONT:
				attr.Apply ( this->mFront, op, ZLAttribute::ATTR_READ );
				return true;
		}
	}
	
	return MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAINode_Update () {
	
	if ( !( this->mSourceProp && this->mSourceLayer && this->mDestLayer )) return;
	
	ZLMatrix4x4 modelToWnd = this->mSourceProp->GetWorldDrawingMtx ();
	modelToWnd.Append ( this->mSourceLayer->GetWorldToWndMtx ());
	
	const ZLMatrix4x4& wndToWorldMtx = this->mDestLayer->GetWndToWorldMtx ();

	ZLBounds sourceBounds = this->mSourceProp->GetModelBounds ();
	ZLBounds projectedBounds = ZLBounds::EMPTY;

	ZLReal front = 0.0;

	if ( sourceBounds.HasGeometry ()) {
	
		// get the corners of the source AABB and project them
		ZLBoxCorners corners ( sourceBounds.mAABB );
		corners.Project ( modelToWnd );
		corners.Transform ( wndToWorldMtx );
		projectedBounds.Init ( ZLBox ( corners ));
		
		// Z component is at the back of the NDC's near plane
		ZLReal z = projectedBounds.mAABB.mMax.mZ - projectedBounds.mAABB.mMin.mZ;
		front = z < -1.0 ? 0.0 : 1.0;
	}

	this->UpdateWorldBounds ( projectedBounds );
	this->mFront = front;
}
