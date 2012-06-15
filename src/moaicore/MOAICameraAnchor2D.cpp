// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setParent
	@text	Attach the anchor to a transform.
	
	@in		MOAICameraAnchor2D self
	@opt	MOAITransformBase parent
	@out	nil
*/
int MOAICameraAnchor2D::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "U" )
	
	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	self->SetAttrLink ( PACK_ATTR ( MOAICameraAnchor2D, INHERIT_LOC ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set the dinemsions (in world units) of the anchor.
	
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
USRect MOAICameraAnchor2D::GetRect () {

	USRect rect = this->mRect;
	rect.Offset ( this->mLoc.mX, this->mLoc.mY );
	rect.Bless ();
	
	return rect;
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::MOAICameraAnchor2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mLoc.Init ( 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::~MOAICameraAnchor2D () {
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::OnDepNodeUpdate () {
	
	const USAffine3D* inherit = this->GetLinkedValue < USAffine3D* >( MOAICameraAnchor2DAttr::Pack ( INHERIT_LOC ), 0 );
	if ( inherit ) {
		this->mLoc = inherit->GetTranslation ();
	}
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );

	state.SetField ( -1, "INHERIT_LOC", MOAICameraAnchor2DAttr::Pack ( INHERIT_LOC ));

	luaL_Reg regTable [] = {
		{ "setParent",			_setParent },
		{ "setRect",			_setRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

