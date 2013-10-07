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
int MOAIVectorDrawing::_finish ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->Finish ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushCombo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->PushCombo ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	u32 total = ( state.GetTop () - 1 ) >> 1;
	
	ZLVec2D* vertices = 0;
	
	if ( total ) {
		vertices = ( ZLVec2D* )alloca ( total * sizeof ( ZLVec2D ));
		
		for ( u32 i = 0; i < total; ++i ) {
			vertices [ i ].mX = state.GetValue < float >(( i << 1 ) + 2, 0 );
			vertices [ i ].mY = state.GetValue < float >(( i << 1 ) + 3, 0 );
		}
	}
	self->PushPolygon ( vertices, total );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushStroke ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushVertex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	
	self->PushVertex ( x, y );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setFillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mStyle.mFillColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setFillStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mStyle.mFillStyle = state.GetValue < u32 >( 2, MOAIVectorStyle::FILL_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mLineColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mLineOffset = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mLineStyle = state.GetValue < u32 >( 2, MOAIVectorStyle::LINE_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mLineWidth = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setWindingRule ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mWindingRule = state.GetValue < u32 >( 2, ( u32 )TESS_WINDING_ODD );
	return 0;
}

//================================================================//
// MOAIVectorDrawing
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorDrawing::Clear () {

	for ( u32 i = 0; i < this->mDirectory.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mDirectory [ i ];
		if ( shape ) {
			delete shape;
		}
	}
	
	this->mDirectory.Clear ();
	this->mShapeStack.Clear ();
	
	this->mIdxStream.Clear ();
	this->mVtxStream.Clear ();
}

//----------------------------------------------------------------//
u32 MOAIVectorDrawing::CopyVertexStack ( ZLVec2D* vertices, u32 total ) {

	u32 top = this->mVertexStack.GetTop ();
	total = total < top ? total : top;
	for ( u32 i = 0; i < total; ++i ) {
		vertices [ i ] = this->mVertexStack [ i ];
	}
	return total;
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
void MOAIVectorDrawing::Finish () {

	u32 vertsTop = this->mVertexStack.GetTop ();
	u32 shapesTop = this->mShapeStack.GetTop ();
	
	if ( vertsTop ) {
	
		assert ( shapesTop );
		MOAIVectorShape* shape = this->mShapeStack [ shapesTop - 1 ];
		bool result = shape->GroupVertices ( *this, vertsTop );
		shape->mOpen = false;
		
		UNUSED ( result );
		assert ( result );
		
		this->mVertexStack.Reset ();
	}
	else {

		if ( shapesTop >= 2 ) {
			
			bool done = false;
			for ( int i = ( shapesTop - 2 ); ( i >= 0 ) && ( !done ); --i ) {
				int c = i + 1;
				MOAIVectorShape* shape = this->mShapeStack [ i ];
				if ( shape->mOpen && shape->GroupShapes ( &this->mShapeStack [ c ], shapesTop - c )) {
					this->mShapeStack.SetTop ( c );
					done = true;
				}
				shape->mOpen = false;
			}
		}
		
		if ( this->mShapeStack.GetTop () == shapesTop ) {
			this->mVertexStack.Clear ();
			this->Tessalate ();
		}
	}	
}

//----------------------------------------------------------------//
MOAIVectorDrawing::MOAIVectorDrawing () {
	
	this->mStyle.SetFillStyle ( MOAIVectorStyle::FILL_SOLID );
	this->mStyle.SetLineStyle ( MOAIVectorStyle::LINE_NONE );
	this->mStyle.SetLineWidth ( 1.0f );
	this->mStyle.SetLineOffset ( 0.0f );
	this->mStyle.SetWindingRule (( u32 )TESS_WINDING_ODD );
	
	this->mStyle.mFillColor.SetWhite ();
	this->mStyle.mLineColor.SetWhite ();
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVectorDrawing::~MOAIVectorDrawing () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushCombo () {

	MOAIVectorCombo* combo = new MOAIVectorCombo ();
	this->PushShape ( combo );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushPolygon ( ZLVec2D* vertices, u32 total ) {

	MOAIVectorPolygon* polygon = new MOAIVectorPolygon ();
	polygon->SetVertices ( vertices, total );
	this->PushShape ( polygon );
}

//----------------------------------------------------------------//
u32 MOAIVectorDrawing::PushShape ( MOAIVectorShape* shape ) {

	shape->mStyle = this->mStyle;

	u32 tag = this->mDirectory.GetTop ();

	this->mDirectory.Push ( shape );
	this->mShapeStack.Push ( shape );
	
	return tag;
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushVertex ( float x, float y ) {

	ZLVec2D vertex ( x, y );
	this->mVertexStack.Push ( vertex );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "LINE_NONE",					( u32 )MOAIVectorStyle::LINE_NONE );
	state.SetField ( -1, "LINE_STROKE",					( u32 )MOAIVectorStyle::LINE_STROKE );
	state.SetField ( -1, "LINE_VECTOR",					( u32 )MOAIVectorStyle::LINE_VECTOR );
	state.SetField ( -1, "FILL_NONE",					( u32 )MOAIVectorStyle::FILL_NONE );
	state.SetField ( -1, "FILL_SOLID",					( u32 )MOAIVectorStyle::FILL_SOLID );
	
	state.SetField ( -1, "TESS_WINDING_ODD",			( u32 )TESS_WINDING_ODD );
	state.SetField ( -1, "TESS_WINDING_NONZERO",		( u32 )TESS_WINDING_NONZERO );
	state.SetField ( -1, "TESS_WINDING_POSITIVE",		( u32 )TESS_WINDING_POSITIVE );
	state.SetField ( -1, "TESS_WINDING_NEGATIVE",		( u32 )TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "TESS_WINDING_ABS_GEQ_TWO",	( u32 )TESS_WINDING_ABS_GEQ_TWO );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "finish",				_finish },
		{ "pushCombo",			_pushCombo },
		{ "pushPolygon",		_pushPolygon },
		{ "pushStroke",			_pushStroke },
		{ "pushVertex",			_pushVertex },
		{ "setFillColor",		_setFillColor },
		{ "setFillStyle",		_setFillStyle },
		{ "setLineColor",		_setLineColor },
		{ "setLineOffset",		_setLineOffset },
		{ "setLineStyle",		_setLineStyle },
		{ "setLineWidth",		_setLineWidth },
		{ "setWindingRule",		_setWindingRule },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::Tessalate () {

	this->mIdxStream.Clear ();
	this->mVtxStream.Clear ();

	this->mIdxBuffer.Clear ();
	this->mVtxBuffer.Clear ();

	for ( u32 i = 0; i < this->mShapeStack.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mShapeStack [ i ];
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
