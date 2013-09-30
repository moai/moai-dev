// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPolygon.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <tesselator.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorDrawing::_addDrawCommand ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	self->AddCommand ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushCombo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	u32 top = self->mStack.GetTop ();
	
	if ( top == 1 ) {
		state.Push ( self->mStack [ 0 ]);
		return 1;
	}
	
	if ( top ) {
		
		MOAIVectorCombo* combo = new MOAIVectorCombo ();
		combo->Reserve ( top );
		
		for ( u32 i = 0; i < top; ++i ) {
			MOAIVectorShape* shape = self->mDirectory [ self->mStack [ i ]];
			combo->SetShape ( i, shape );
		}
		
		self->mStack.Reset ();
		
		u32 tag = self->PushShape ( combo );
		state.Push ( tag );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	MOAIVectorPolygon* polygon = new MOAIVectorPolygon ();
	u32 tag = self->PushShape ( polygon );

	u32 total = ( state.GetTop () - 1 ) >> 1;
	USVec2D* vertices = ( USVec2D* )alloca ( total * sizeof ( USVec2D ));
	
	for ( u32 i = 0; i < total; ++i ) {
		vertices [ i ].mX = state.GetValue < float >(( i << 1 ) + 2, 0 );
		vertices [ i ].mY = state.GetValue < float >(( i << 1 ) + 3, 0 );
	}
	
	polygon->SetVertices ( vertices, total );
	state.Push ( tag );
	return 1;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushStroke ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushTag ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setFillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mFillColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setFillStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mFillStyle = state.GetValue < u32 >( 2, MOAIVectorShape::FILL_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mLineColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mLineStyle = state.GetValue < u32 >( 2, MOAIVectorShape::LINE_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setWindingRule ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mWindingRule = state.GetValue < u32 >( 2, ( u32 )TESS_WINDING_ODD );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_tessalate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->Tessalate ();
	return 0;
}

//================================================================//
// MOAIVectorDrawing
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorDrawing::AddCommand () {

	u32 top = this->mStack.GetTop ();
	if ( !top ) return;
	this->mCommands.Push ( this->mStack [ top - 1 ]);
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::Clear () {

	for ( u32 i = 0; i < this->mDirectory.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mDirectory [ i ];
		if ( shape ) {
			delete shape;
		}
	}
	
	this->mDirectory.Clear ();
	this->mStack.Clear ();
	
	this->mIdxStream.Clear ();
	this->mVtxStream.Clear ();
}

//----------------------------------------------------------------//
u32 MOAIVectorDrawing::CountVertices () {

	return ( this->mVtxStream.GetLength () / MOAIVertexFormatMgr::Get ().GetVertexSize ( MOAIVertexFormatMgr::XYZC ));
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::Draw () {

	if ( this->mVtxBuffer.Bind ()) {
		if ( this->mIdxBuffer.LoadGfxState ()) {
			zglDrawElements ( ZGL_PRIM_TRIANGLES, this->mIdxBuffer.GetIndexCount (), ZGL_TYPE_UNSIGNED_INT, 0 );
		}
	}
}

//----------------------------------------------------------------//
MOAIVectorDrawing::MOAIVectorDrawing () :
	mFillStyle ( MOAIVectorShape::FILL_SOLID ),
	mLineStyle ( MOAIVectorShape::LINE_NONE ),
	mWindingRule (( u32 )TESS_WINDING_ODD ) {
	
	this->mFillColor.SetWhite ();
	this->mLineColor.SetWhite ();

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVectorDrawing::~MOAIVectorDrawing () {

	this->Clear ();
}

//----------------------------------------------------------------//
u32 MOAIVectorDrawing::PushShape ( MOAIVectorShape* shape ) {

	shape->SetFillColor ( this->mFillColor );
	shape->SetFillStyle ( this->mFillStyle );
	shape->SetLineColor ( this->mLineColor );
	shape->SetLineStyle ( this->mLineStyle );
	shape->SetWindingRule ( this->mWindingRule );

	u32 tag = this->mDirectory.GetTop ();

	this->mDirectory.Push ( shape );
	this->mStack.Push ( tag );
	
	return tag;
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "LINE_NONE",					( u32 )MOAIVectorShape::LINE_NONE );
	state.SetField ( -1, "LINE_VECTOR",					( u32 )MOAIVectorShape::LINE_VECTOR );
	state.SetField ( -1, "FILL_NONE",					( u32 )MOAIVectorShape::FILL_NONE );
	state.SetField ( -1, "FILL_SOLID",					( u32 )MOAIVectorShape::FILL_SOLID );
	
	state.SetField ( -1, "TESS_WINDING_ODD",			( u32 )TESS_WINDING_ODD );
	state.SetField ( -1, "TESS_WINDING_NONZERO",		( u32 )TESS_WINDING_NONZERO );
	state.SetField ( -1, "TESS_WINDING_POSITIVE",		( u32 )TESS_WINDING_POSITIVE );
	state.SetField ( -1, "TESS_WINDING_NEGATIVE",		( u32 )TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "TESS_WINDING_ABS_GEQ_TWO",	( u32 )TESS_WINDING_ABS_GEQ_TWO );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "addDrawCommand",		_addDrawCommand },
		{ "pushCombo",			_pushCombo },
		{ "pushPolygon",		_pushPolygon },
		{ "pushStroke",			_pushStroke },
		{ "pushTag",			_pushTag },
		{ "setFillColor",		_setFillColor },
		{ "setFillStyle",		_setFillStyle },
		{ "setLineColor",		_setLineColor },
		{ "setLineStyle",		_setLineStyle },
		{ "setWindingRule",		_setWindingRule },
		{ "tessalate",			_tessalate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::Tessalate () {

	for ( u32 i = 0; i < this->mCommands.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mDirectory [ this->mCommands [ i ]];
		shape->Tessalate ( *this );
	}
	
	this->mIdxStream.Seek ( 0, SEEK_SET );
	this->mVtxStream.Seek ( 0, SEEK_SET );
	
	this->mIdxBuffer.ReserveIndices ( this->mIdxStream.GetLength () >> 2 );
	this->mIdxBuffer.GetStream ().WriteStream ( this->mIdxStream );
	
	this->mVtxBuffer.SetDefaultFormat ( MOAIVertexFormatMgr::XYZC );
	this->mVtxBuffer.Reserve ( this->mVtxStream.GetLength ());
	this->mVtxBuffer.GetStream ().WriteStream ( this->mVtxStream );
	
	this->mIdxStream.Clear ();
	this->mVtxStream.Clear ();
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::WriteContourIndices ( TESStesselator* tess, u32 base ) {

	const int* elems = tessGetElements ( tess );
	const int nelems = tessGetElementCount ( tess );
	
	int totalLines = 0;
	for ( int i = 0; i < nelems; ++i ) {
		totalLines += elems [( i * 2 ) + 1 ];
	}
	
	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		
		for ( int j = 0; j < n; ++j ) {
			this->mIdxStream.Write < u32 >( base + b + j );
			this->mIdxStream.Write < u32 >( base + b + (( j + 1 ) % n ));
		}
	}
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::WriteTriangleIndices ( TESStesselator* tess, u32 base ) {

	const int* elems = tessGetElements ( tess );
	const int nelems = tessGetElementCount ( tess );
	
	for ( int i = 0; i < nelems; ++i ) {
		const int* tri = &elems [ i * 3 ];
		this->mIdxStream.Write < u32 >( base + tri [ 0 ]);
		this->mIdxStream.Write < u32 >( base + tri [ 1 ]);
		this->mIdxStream.Write < u32 >( base + tri [ 2 ]);
	}
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::WriteVertices ( TESStesselator* tess, u32 color ) {

	const float* verts = tessGetVertices ( tess );
	const int nverts = tessGetVertexCount ( tess );
	
	for ( int i = 0; i < nverts; ++i ) {
		this->mVtxStream.WriteBytes ( &verts [ i * 2 ], 2 * sizeof ( float ));
		this->mVtxStream.Write < float >( 0.0f );
		this->mVtxStream.Write < u32 >( color );
	}
}
