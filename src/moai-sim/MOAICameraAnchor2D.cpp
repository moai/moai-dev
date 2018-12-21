// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractChildTransform.h>
#include <moai-sim/MOAICameraAnchor2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	Attach the anchor to a transform.
	
	@in		MOAICameraAnchor2D self
	@opt	MOAINode parent
	@out	nil
*/
int MOAICameraAnchor2D::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "U" )
	
	MOAIAbstractChildTransform* parent = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
	self->SetAttrLink ( AttrID::Pack ( INHERIT_LOC ), parent, MOAIAbstractChildTransform::AttrID::Pack ( MOAIAbstractChildTransform::TRANSFORM_TRAIT ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set the dimensions (in world units) of the anchor.
	
	@in		MOAICameraAnchor2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAICameraAnchor2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mRect.Init ( x0, y0, x1, y1 );
	
	return 0;
}

//================================================================//
// MOAICameraAnchor2D
//================================================================//

//----------------------------------------------------------------//
ZLRect MOAICameraAnchor2D::GetRect () {

	ZLRect rect = this->mRect;
	rect.Offset ( this->mLoc.mX, this->mLoc.mY );
	rect.Bless ();
	
	return rect;
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::MOAICameraAnchor2D () :
	mRect ( ZLRect::EMPTY ),
	mLoc ( ZLVec3D::ORIGIN ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::~MOAICameraAnchor2D () {
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );

	state.SetField ( -1, "INHERIT_LOC", AttrID::Pack ( INHERIT_LOC ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setParent",			_setParent },
		{ "setRect",			_setRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAICameraAnchor2D::MOAINode_Update () {
	
	const ZLAffine3D inherit = this->GetLinkedValue ( AttrID::Pack ( INHERIT_LOC ), ZLAffine3D::IDENT );
	this->mLoc = inherit.GetTranslation ();
}
