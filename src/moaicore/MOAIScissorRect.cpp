// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIScissorRect.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIScissorRect::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	state.Push ( self->mRect );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIScissorRect::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	self->mRect = state.GetRect < float >( 2 );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIScissorRect::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	self->mScissorRect = state.GetLuaObject < MOAIScissorRect >( 2 );
	
	return 0;
}

//================================================================//
// MOAIScissorRect
//================================================================//

//----------------------------------------------------------------//
USRect MOAIScissorRect::GetScissorRect ( const USMatrix4x4& worldToWndMtx ) const {

	USVec3D vtx3D [ 4 ];

	vtx3D [ 0 ].mX = this->mRect.mXMin;
	vtx3D [ 0 ].mY = this->mRect.mYMin;
	vtx3D [ 0 ].mZ = 0.0f;

	vtx3D [ 1 ].mX = this->mRect.mXMin;
	vtx3D [ 1 ].mY = this->mRect.mYMax;
	vtx3D [ 1 ].mZ = 0.0f;
	
	vtx3D [ 2 ].mX = this->mRect.mXMax;
	vtx3D [ 2 ].mY = this->mRect.mYMax;
	vtx3D [ 2 ].mZ = 0.0f;

	vtx3D [ 3 ].mX = this->mRect.mXMax;
	vtx3D [ 3 ].mY = this->mRect.mYMin;
	vtx3D [ 3 ].mZ = 0.0f;

	USMatrix4x4 mtx;
	
	mtx.Init ( this->GetLocalToWorldMtx ());
	mtx.Append ( worldToWndMtx );
	
	mtx.Project ( vtx3D [ 0 ]);
	mtx.Project ( vtx3D [ 1 ]);
	mtx.Project ( vtx3D [ 2 ]);
	mtx.Project ( vtx3D [ 3 ]);
	
	USRect scissorRect;

	scissorRect.Init ( vtx3D [ 0 ]);
	scissorRect.Grow ( vtx3D [ 1 ]);
	scissorRect.Grow ( vtx3D [ 2 ]);
	scissorRect.Grow ( vtx3D [ 3 ]);

	if ( this->mScissorRect ) {
		USRect parentRect = this->mScissorRect->GetScissorRect ( worldToWndMtx );
		parentRect.Clip ( scissorRect );
	}

	return scissorRect;
}

//----------------------------------------------------------------//
MOAIScissorRect::MOAIScissorRect () :
	mScissorRect ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIScissorRect::~MOAIScissorRect () {
}

//----------------------------------------------------------------//
void MOAIScissorRect::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScissorRect::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITransform::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getRect",			_getRect },
		{ "setRect",			_setRect },
		{ "setScissorRect",		_setScissorRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

