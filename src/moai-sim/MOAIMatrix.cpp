// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMatrix.h>
#include <moai-sim/MOAISim.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMatrix::_getMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMatrix, "U" )
	
	state.Push ( self->m [ ZLAffine3D::C0_R0 ]);
	state.Push ( self->m [ ZLAffine3D::C1_R0 ]);
	state.Push ( self->m [ ZLAffine3D::C2_R0 ]);
	state.Push ( self->m [ ZLAffine3D::C3_R0 ]);
	
	state.Push ( self->m [ ZLAffine3D::C0_R1 ]);
	state.Push ( self->m [ ZLAffine3D::C1_R1 ]);
	state.Push ( self->m [ ZLAffine3D::C2_R1 ]);
	state.Push ( self->m [ ZLAffine3D::C3_R1 ]);
	
	state.Push ( self->m [ ZLAffine3D::C0_R2 ]);
	state.Push ( self->m [ ZLAffine3D::C1_R2 ]);
	state.Push ( self->m [ ZLAffine3D::C2_R2 ]);
	state.Push ( self->m [ ZLAffine3D::C3_R2 ]);
	
	return 12;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMatrix::_setMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMatrix, "U" )
	
	ZLMatrix4x4 mtx;
	
	mtx.Ident ();
	
	mtx = state.GetValue < ZLMatrix4x4 >( 2, mtx );
	
	mtx.Print ();
	
	self->Init ( mtx );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIMatrix
//================================================================//

//----------------------------------------------------------------//
void MOAIMatrix::BuildLocalToWorldMtx ( ZLAffine3D& localToWorldMtx ) {

	localToWorldMtx = *this;
}

//----------------------------------------------------------------//
MOAIMatrix::MOAIMatrix () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
	
	this->Ident ();
}

//----------------------------------------------------------------//
MOAIMatrix::~MOAIMatrix () {
}

//----------------------------------------------------------------//
void MOAIMatrix::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIMatrix::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getMatrix",			_getMatrix },
		{ "setMatrix",			_setMatrix },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMatrix::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMatrix::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}
