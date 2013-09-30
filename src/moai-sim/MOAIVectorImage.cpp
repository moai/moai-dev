// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPolygonShape.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVectorImage.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <tesselator.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_addContour ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAIPolygonShape* polygon = MOAICast < MOAIPolygonShape >( self->mShapes [ idx ]);
	if ( !polygon ) return 0;

	u32 total = ( state.GetTop () - 2 ) >> 1;
	USVec2D* verts = ( USVec2D* )alloca ( total * sizeof ( USVec2D ));
	
	for ( u32 i = 0; i < total; ++i ) {
	
		verts [ i ].mX = state.GetValue < float >(( i << 1 ) + 3, 0 );
		verts [ i ].mY = state.GetValue < float >(( i << 1 ) + 4, 0 );
	}
	
	polygon->AddContour ( verts, total );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_reserveShapes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mShapes.Init ( total );
	self->mShapes.Fill ( 0 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_setFillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	ZLColorVec color	= state.GetColor ( 3, 1.0f, 1.0f, 1.0f, 1.0f );
	
	MOAIShape* shape = self->mShapes [ idx ];
	shape->SetFillColor ( color );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_setFillStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 style	= state.GetValue < u32 >( 3, MOAIShape::LINE_NONE );
	
	MOAIShape* shape = self->mShapes [ idx ];
	shape->SetFillStyle ( style );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_setLineColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	ZLColorVec color	= state.GetColor ( 3, 1.0f, 1.0f, 1.0f, 1.0f );
	
	MOAIShape* shape = self->mShapes [ idx ];
	shape->SetLineColor ( color );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_setLineStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 style	= state.GetValue < u32 >( 3, MOAIShape::LINE_NONE );
	
	MOAIShape* shape = self->mShapes [ idx ];
	shape->SetLineStyle ( style );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorImage::_setPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;

	MOAIPolygonShape* polygon = new MOAIPolygonShape ();
	self->SetShape ( idx, polygon );

	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorImage::_tessalate ( lua_State* L ) {
// TODO: doxygen
	MOAI_LUA_SETUP ( MOAIVectorImage, "U" )
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 windingRule		= state.GetValue < u32 >( 3, ( u32 )TESS_WINDING_ODD );
	bool preTessalate	= state.GetValue < bool >( 4, true );
	
	MOAIPolygonShape* polygon = MOAICast < MOAIPolygonShape >( self->mShapes [ idx ]);
	if ( polygon ) {
		polygon->Tesselate ( windingRule, preTessalate );
	}
	return 0;
}

//================================================================//
// MOAIVectorImage
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorImage::Clear () {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		MOAIShape* shape = this->mShapes [ i ];
		if ( shape ) {
			delete shape;
		}
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
ZLBox MOAIVectorImage::ComputeMaxBounds () {
	ZLBox bounds;
	bounds.Init ( -50, 50, 50, -50, 0, 0 );
	return bounds;
}

//----------------------------------------------------------------//
ZLBox MOAIVectorImage::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );
	ZLBox bounds;
	bounds.Init ( -50, 50, 50, -50, 0, 0 );
	return bounds;
}

//----------------------------------------------------------------//
void MOAIVectorImage::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( idx );
	UNUSED ( xOff );
	UNUSED ( yOff );
	UNUSED ( zOff );
	UNUSED ( xScl );
	UNUSED ( yScl );
	UNUSED ( zScl );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	gfxDevice.SetTexture ();
	
	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		this->mShapes [ 0 ]->Render ();
	}
}

//----------------------------------------------------------------//
MOAIVectorImage::MOAIVectorImage () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->mDefaultShaderID = MOAIShaderMgr::LINE_SHADER_3D;
}

//----------------------------------------------------------------//
MOAIVectorImage::~MOAIVectorImage () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVectorImage::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	
	state.SetField ( -1, "LINE_NONE",					( u32 )MOAIShape::LINE_NONE );
	state.SetField ( -1, "LINE_VECTOR",					( u32 )MOAIShape::LINE_VECTOR );
	state.SetField ( -1, "FILL_NONE",					( u32 )MOAIShape::FILL_NONE );
	state.SetField ( -1, "FILL_SOLID",					( u32 )MOAIShape::FILL_SOLID );
	
	state.SetField ( -1, "TESS_WINDING_ODD",			( u32 )TESS_WINDING_ODD );
	state.SetField ( -1, "TESS_WINDING_NONZERO",		( u32 )TESS_WINDING_NONZERO );
	state.SetField ( -1, "TESS_WINDING_POSITIVE",		( u32 )TESS_WINDING_POSITIVE );
	state.SetField ( -1, "TESS_WINDING_NEGATIVE",		( u32 )TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "TESS_WINDING_ABS_GEQ_TWO",	( u32 )TESS_WINDING_ABS_GEQ_TWO );
}

//----------------------------------------------------------------//
void MOAIVectorImage::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addContour",			_addContour },
		{ "reserveShapes",		_reserveShapes },
		{ "setFillColor",		_setFillColor },
		{ "setFillStyle",		_setFillStyle },
		{ "setLineColor",		_setLineColor },
		{ "setLineStyle",		_setLineStyle },
		{ "setPolygon",			_setPolygon },
		{ "tessalate",			_tessalate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVectorImage::SetShape ( u32 idx, MOAIShape* shape ) {

	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	this->mShapes [ idx ] = shape;
}
