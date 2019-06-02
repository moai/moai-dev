// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-vr/MOAIHeadTransform.h>
#include <moai-vr/MOAIVrMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_LIBGVR

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHeadTransform::_pauseTracking ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHeadTransform, "U" )
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHeadTransform::_recenterTracking ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHeadTransform, "U" )
	
	return 0;
}

//================================================================//
// MOAIHeadTransform
//================================================================//

//----------------------------------------------------------------//
MOAIHeadTransform::MOAIHeadTransform () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND ( MOAIAbstractBaseTransform )
	RTTI_END
	
	this->Ident ();
}

//----------------------------------------------------------------//
MOAIHeadTransform::~MOAIHeadTransform () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHeadTransform::MOAIAbstractBaseTransform_BuildLocalToWorldMtx ( ZLAffine3D& localToWorldMtx ) {

	localToWorldMtx = *this;
}

//----------------------------------------------------------------//
bool MOAIHeadTransform::MOAIAction_IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAIHeadTransform::MOAIAction_Update ( double step ) {
	UNUSED ( step );
	
	*( ZLAffine3D* )this = ZLAffine3D ( MOAIVrMgr::Get ().GetHeadTransform ());
	this->ScheduleUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHeadTransform::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractBaseTransform, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIHeadTransform::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAction, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractBaseTransform, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "pauseTracking",				_pauseTracking },
		{ "recenterTracking",			_recenterTracking },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
