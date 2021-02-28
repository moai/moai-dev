// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIScissorRect.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Return the extents of the scissor rect.
	
	@in		MOAIScissorRect self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIScissorRect::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	state.Push ( self->mRect );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Sets the extents of the scissor rect.
	
	@in		number x1					The X coordinate of the rect's upper-left point.
	@in		number y1					The Y coordinate of the rect's upper-left point.
	@in		number x2					The X coordinate of the rect's lower-right point.
	@in		number y2					The Y coordinate of the rect's lower-right point.
	@out	nil
*/
int MOAIScissorRect::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	self->mRect = state.GetRect < float >( 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setScissorRect
	@text	Set or clear the parent scissor rect.
	
	@in		MOAIScissorRect self
	@opt	MOAIScissorRect parent		Default value is nil.
	@out	nil
*/
int MOAIScissorRect::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScissorRect, "U" )
	
	MOAIScissorRect* scissorRect = state.GetLuaObject < MOAIScissorRect >( 2, true );
	self->mScissorRect.Set ( *self, scissorRect );
	
	return 0;
}

//================================================================//
// MOAIScissorRect
//================================================================//

//----------------------------------------------------------------//
ZLRect MOAIScissorRect::GetScissorRect ( const ZLMatrix4x4& worldToWndMtx ) const {

	ZLVec3D vtx3D [ 4 ];

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

	ZLMatrix4x4 mtx ( this->GetLocalToWorldMtx ());
	mtx.Append ( worldToWndMtx );
	
	mtx.Project ( vtx3D [ 0 ]);
	mtx.Project ( vtx3D [ 1 ]);
	mtx.Project ( vtx3D [ 2 ]);
	mtx.Project ( vtx3D [ 3 ]);
	
	ZLRect scissorRect;

	scissorRect.Init ( vtx3D [ 0 ]);
	scissorRect.Grow ( vtx3D [ 1 ]);
	scissorRect.Grow ( vtx3D [ 2 ]);
	scissorRect.Grow ( vtx3D [ 3 ]);

	if ( this->mScissorRect ) {
		ZLRect parentRect = this->mScissorRect->GetScissorRect ( worldToWndMtx );
		parentRect.Clip ( scissorRect );
	}

	return scissorRect;
}

//----------------------------------------------------------------//
MOAIScissorRect::MOAIScissorRect ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAINode ( context ),
	MOAIAbstractBaseTransform ( context ),
	MOAIAbstractChildTransform ( context ),
	MOAITransform ( context ),
 	mRect ( ZLRect::EMPTY ) {
	
	RTTI_BEGIN ( MOAIScissorRect )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIScissorRect >)
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIScissorRect::~MOAIScissorRect () {

	this->mScissorRect.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIScissorRect::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIScissorRect::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getRect",			_getRect },
		{ "setRect",			_setRect },
		{ "setScissorRect",		_setScissorRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

