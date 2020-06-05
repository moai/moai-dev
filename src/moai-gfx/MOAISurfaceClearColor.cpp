// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAISurfaceClearColor.h>
#include <moai-gfx/MOAIColor.h>
#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setClearColor
	@text	At the start of each frame the device will by default automatically
			render a background color.  Using this function you can set the
			background color that is drawn each frame.  If you specify no arguments
			to this function, then automatic redraw of the background color will
			be turned off (i.e. the previous render will be used as the background).

	@overload

		@in		MOAIAbstractLayer self
		@opt	number red			The red value of the color.
		@opt	number green		The green value of the color.
		@opt	number blue			The blue value of the color.
		@opt	number alpha		The alpha value of the color.
		@out	nil
	
	@overload
		
		@in		MOAIAbstractLayer self
		@in		MOAIColor color
		@out	nil
*/
int MOAISurfaceClearColor::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceClearColor, "U" )
	
	MOAIColor* color = state.GetLuaObject < MOAIColor >( 2, true );
	if ( color ) {
		self->SetClearColor ( color );
		self->mClearFlags |= MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
		return 0;
	}
	
	// don't clear the color
	self->mClearFlags &= ~MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
	self->SetClearColor ( 0 );

	if ( state.GetTop () > 1 ) {
	
		float r = state.GetValue < float >( 2, 0.0f );
		float g = state.GetValue < float >( 3, 0.0f );
		float b = state.GetValue < float >( 4, 0.0f );
		float a = state.GetValue < float >( 5, 1.0f );
		
		self->mClearColor = ZLColor::PackRGBA ( r, g, b, a );
		self->mClearFlags |= MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setClearDepth
	@text	At the start of each frame the buffer will by default automatically
			clear the depth buffer.  This function sets whether or not the depth
			buffer should be cleared at the start of each frame.

	@in		MOAIAbstractLayer self
	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
*/
int MOAISurfaceClearColor::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceClearColor, "U" )
	
	bool clearDepth = state.GetValue < bool >( 2, false );
	
	if ( clearDepth ) {
		self->mClearFlags |= MOAIClearFlagsEnum::DEPTH_BUFFER_BIT;
	}
	else {
		self->mClearFlags &= ~MOAIClearFlagsEnum::DEPTH_BUFFER_BIT;
	}
	return 0;
}

//================================================================//
// MOAIRenderBatch
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceClearColor::ClearSurface () const {

	if ( !this->mClearFlags ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	if ( this->mClearFlags & MOAIClearFlagsEnum::COLOR_BUFFER_BIT ) {
		ZLColorVec colorVec = this->GetClearColorVec ();
		gfxMgr.SetClearColor ( colorVec );
	}
	gfxMgr.SetClearFlags ( this->mClearFlags );
	gfxMgr.ClearSurface ();
}

//----------------------------------------------------------------//
ZLColorVec MOAISurfaceClearColor::GetClearColorVec () const {

	if ( this->mClearColorNode ) {
		return this->mClearColorNode->GetColorTrait ();
	}
	return ZLColorVec ( this->mClearColor );
}

//----------------------------------------------------------------//
MOAISurfaceClearColor::MOAISurfaceClearColor () :
	mClearFlags ( MOAIClearFlagsEnum::COLOR_BUFFER_BIT | MOAIClearFlagsEnum::DEPTH_BUFFER_BIT ),
	mClearColor ( ZLColor::PackRGBA ( 0.0f, 0.0f, 0.0f, 1.0f )) {
	
	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAISurfaceClearColor >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISurfaceClearColor::~MOAISurfaceClearColor () {
}

//----------------------------------------------------------------//
void MOAISurfaceClearColor::SetClearColor ( MOAIColor* color ) {

	if ( this->mClearColorNode != color ) {
		this->mClearColorNode.Set ( *this, color );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceClearColor::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAISurfaceClearColor::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
