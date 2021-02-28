// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIMatrix.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMatrix::_getMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMatrix, "U" )
	
	state.Push ( ZLMatrix4x4 ( *( ZLAffine3D* )self ));
	
	return 16;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMatrix::_invert ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMatrix, "U" )
	
	self->Inverse ();
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMatrix::_setMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMatrix, "U" )
	
	size_t size = state.GetTop () - 1;
	
	switch ( size ) {
			
		case 9:
			*( ZLAffine3D* )self = ZLAffine3D ( state.GetValue < ZLMatrix3x3 >( 2, ZLMatrix3x3::IDENT ));
			break;
			
		case 12:
			*( ZLAffine3D* )self = state.GetValue < ZLAffine3D >( 2, ZLAffine3D::IDENT );
			break;
			
		case 16:
			*( ZLAffine3D* )self = ZLAffine3D ( state.GetValue < ZLMatrix4x4 >( 2, ZLMatrix4x4::IDENT ));
			break;
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIMatrix
//================================================================//

//----------------------------------------------------------------//
MOAIMatrix::MOAIMatrix ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAINode ( context ),
	MOAIAbstractBaseTransform ( context ),
	MOAIAbstractChildTransform ( context ) {
	
	RTTI_BEGIN ( MOAIMatrix )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMatrix >)
		RTTI_EXTEND ( MOAIAbstractChildTransform )
	RTTI_END
	
	this->Ident ();
}

//----------------------------------------------------------------//
MOAIMatrix::~MOAIMatrix () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMatrix::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "ATTR_MATRIX",	AttrID::Pack ( ATTR_MATRIX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIMatrix::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getMatrix",			_getMatrix },
		{ "invert",				_invert },
		{ "setMatrix",			_setMatrix },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMatrix::MOAIAbstractBaseTransform_BuildLocalToWorldMtx ( ZLAffine3D& localToWorldMtx ) {

	localToWorldMtx = *this;
}

//----------------------------------------------------------------//
bool MOAIMatrix::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	// TODO: these values may need to be cached for performance reasons
	if ( AttrID::Check ( attrID )) {

		switch ( attrID.Unpack ()) {
			
			case ATTR_MATRIX:
			
				*( ZLAffine3D* )this = attr.ApplyNoAdd < ZLAffine3D >( *( ZLAffine3D* )this, op, ZLAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return MOAIAbstractChildTransform::MOAINode_ApplyAttrOp ( attrID, attr, op );
}
