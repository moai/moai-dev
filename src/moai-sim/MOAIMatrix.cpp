// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMatrix.h>
#include <moai-sim/MOAISim.h>

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
MOAIMatrix::MOAIMatrix () {
	
	RTTI_BEGIN ( MOAIMatrix )
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
void MOAIMatrix::MOAIAbstractBaseTransform_BuildLocalToWorldMtx ( ZLAffine3D& localToWorldMtx ) {

	localToWorldMtx = *this;
}

//----------------------------------------------------------------//
void MOAIMatrix::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractChildTransform, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "ATTR_MATRIX",	AttrID::Pack ( ATTR_MATRIX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIMatrix::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractChildTransform, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "getMatrix",			_getMatrix },
		{ "invert",				_invert },
		{ "setMatrix",			_setMatrix },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMatrix::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMatrix::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
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
