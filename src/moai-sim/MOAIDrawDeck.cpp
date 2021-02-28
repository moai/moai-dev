// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDrawDeck.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawDeck::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )

	int top = state.GetTop ();

	switch ( top ) {
	
		case 5:
			self->mBounds.Init ( state.GetRect < float >( 2 ));
			break;
	
		case 7:
			self->mBounds.Init ( state.GetBox ( 2 ));
			break;
			
		default:
			self->mBounds = ZLBounds::GLOBAL;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBoundsCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( number index ).

	@in		MOAIDrawDeck self
	@in		function callback
	@out	nil
*/
int MOAIDrawDeck::_setBoundsCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )
	
	self->mOnBounds.SetRef ( *self, state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDrawCallback
	@text	Sets the callback to be issued when draw events occur.
			The callback's parameters are ( number index, number xOff, number yOff, number xScale, number yScale ).

	@in		MOAIDrawDeck self
	@in		function callback
	@out	nil
*/
int MOAIDrawDeck::_setDrawCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )
	
	self->mOnDraw.SetRef ( *self, state, 2 );
	return 0;
}

//================================================================//
// MOAIDrawDeck
//================================================================//

//----------------------------------------------------------------//
MOAIDrawDeck::MOAIDrawDeck ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIDeck ( context ),
	MOAIStretchDeck ( context ) {
	
	RTTI_BEGIN ( MOAIDrawDeck )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIDrawDeck >)
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIStretchDeck )
	RTTI_END
	
	this->mStretchFactor = 0.0;
	this->mBounds = ZLBounds::GLOBAL;
}

//----------------------------------------------------------------//
MOAIDrawDeck::~MOAIDrawDeck () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawDeck::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDrawDeck::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setBounds",				_setBounds },
		{ "setBoundsCallback",		_setBoundsCallback },
		{ "setDrawCallback",		_setDrawCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIDrawDeck::MOAIDeck_GetBounds () {

	return this->mBounds;
}

//----------------------------------------------------------------//
ZLBounds MOAIDrawDeck::MOAIDeck_GetBounds ( ZLIndex idx ) {
	
	if ( this->mOnBounds ) {
	
		MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
		
		this->mOnBounds.PushRef ( state );
		
		state.Push ( MOAILuaIndex ( idx ));
		state.DebugCall ( 1, 6 );
		
		ZLBounds bounds;
		bounds.Init ( state.GetBox ( -6 ));
		return bounds;
	}
	return this->mBounds;
}

//----------------------------------------------------------------//
void MOAIDrawDeck::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {
	
	if ( this->mOnDraw ) {
	
		MOAIDraw& draw = this->Get < MOAIDraw >();
		draw.BindVectorPresets ();
	
		ZLVec3D stretch = this->BindStretchVertexTransform ();
	
		MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
		this->mOnDraw.PushRef ( state );
	
		draw.PushCmdInterface ( state );
	
		state.Push ( MOAILuaIndex ( idx ));
		state.Push ( stretch.mX );
		state.Push ( stretch.mY );
		state.Push ( stretch.mZ );
		state.DebugCall ( 5, 0 );
	}
}
