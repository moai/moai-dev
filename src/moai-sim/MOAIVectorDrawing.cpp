// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorEllipse.h>
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPath.h>
#include <moai-sim/MOAIVectorPolygon.h>
#include <moai-sim/MOAIVectorRect.h>
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
int MOAIVectorDrawing::_pushEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	float x			= state.GetValue < float >( 2, 0.0f );
	float y			= state.GetValue < float >( 3, 0.0f );
	float xRad		= state.GetValue < float >( 4, 0.0f );
	float yRad		= state.GetValue < float >( 5, xRad );
	
	self->PushEllipse ( x, y, xRad, yRad );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_pushPath ( lua_State* L ) {
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
	self->PushPath ( vertices, total );
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
int MOAIVectorDrawing::_pushRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	ZLRect rect = state.GetRect < float >( 2 );
	self->PushRect ( rect );
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
int MOAIVectorDrawing::_setCapStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mStyle.mCapStyle = state.GetValue < u32 >( 2, MOAIVectorStyle::CAP_BUTT );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setCircleResolution ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mCircleResolution = state.GetValue < u32 >( 2, DEFAULT_CIRCLE_RESOLUTION );
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
int MOAIVectorDrawing::_setJoinStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mStyle.mJoinStyle = state.GetValue < u32 >( 2, MOAIVectorStyle::JOIN_MITER );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setLineColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mLineColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
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
int MOAIVectorDrawing::_setMiterLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )

	self->mStyle.mMiterLimit = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::_setStrokeStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawing, "U" )
	
	self->mStyle.mStrokeStyle = state.GetValue < u32 >( 2, MOAIVectorStyle::STROKE_CENTER );
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
void MOAIVectorDrawing::ComputeLineJoins ( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior ) {
	
	int top = nVerts - 1;
	float scale = interior ? -1.0f : 1.0f;
	
	if ( forward ) {
		for ( int i = 0; i < nVerts; ++i ) {
			joins [ i ].mVertex = verts [ i ];
		}
	}
	else {
		for ( int i = 0; i < nVerts; ++i ) {
			joins [ i ].mVertex = verts [ top - i ];
		}
	}
	
	for ( int i = 0; i < nVerts; ++i ) {
		
		ZLVec2D v0 = joins [ i ].mVertex;
		ZLVec2D v1 = joins [( i + 1 ) % nVerts ].mVertex;
		
		ZLVec2D n = v1;
		
		n.Sub ( v0 );
		n.Norm ();
		
		joins [ i ].mEdgeVec = n;
		
		n.Rotate90Anticlockwise ();
		n.Scale ( scale );
		
		joins [ i ].mEdgeNorm = n;
		joins [ i ].mIsCap = false;
	}
	
	int start = 0;
	int max = nVerts;
	
	if ( open ) {
		
		joins [ 0 ].mIsCap = true;
		joins [ 0 ].mJointNorm = joins [ 0 ].mEdgeNorm;
		
		joins [ top ].mIsCap = true;
		joins [ top ].mEdgeVec = joins [ top - 1 ].mEdgeVec;
		joins [ top ].mEdgeNorm = joins [ top - 1 ].mEdgeNorm;
		joins [ top ].mJointNorm = joins [ top ].mEdgeNorm;
		
		start = 1;
		max = top;
	}
	
	for ( int i = start; i < max; ++i ) {
		
		ZLVec2D n = joins [( i + top ) % nVerts ].mEdgeNorm;
		n.Add ( joins [ i ].mEdgeNorm );
		n.Norm ();
		
		joins [ i ].mJointNorm = n;
	}
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
u32 MOAIVectorDrawing::GetResolutionForWedge ( float radians ) {

	u32 resolution = ( u32 )( this->mCircleResolution * ( radians / ( float )TWOPI ));
	return resolution < 1 ? 1 : resolution;
}

//----------------------------------------------------------------//
MOAIVectorDrawing::MOAIVectorDrawing () :
	mCircleResolution ( DEFAULT_CIRCLE_RESOLUTION ) {
	
	this->mStyle.SetFillStyle ( MOAIVectorStyle::FILL_SOLID );
	this->mStyle.SetLineStyle ( MOAIVectorStyle::LINE_NONE );
	this->mStyle.SetStrokeStyle ( MOAIVectorStyle::STROKE_CENTER );
	this->mStyle.SetJoinStyle ( MOAIVectorStyle::JOIN_MITER );
	this->mStyle.SetCapStyle ( MOAIVectorStyle::CAP_BUTT );
	
	this->mStyle.SetLineWidth ( 1.0f );
	this->mStyle.SetMiterLimit ( 0.0f );
	
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
void MOAIVectorDrawing::PushEllipse ( float x, float y, float xRad, float yRad ) {

	MOAIVectorEllipse* ellipse = new MOAIVectorEllipse ();
	ellipse->Init ( x, y, xRad, yRad );
	this->PushShape ( ellipse );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushPath ( ZLVec2D* vertices, u32 total ) {

	MOAIVectorPath* path = new MOAIVectorPath ();
	path->SetVertices ( vertices, total );
	this->PushShape ( path );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushPolygon ( ZLVec2D* vertices, u32 total ) {

	MOAIVectorPolygon* polygon = new MOAIVectorPolygon ();
	polygon->SetVertices ( vertices, total );
	this->PushShape ( polygon );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::PushRect ( const ZLRect& rect ) {

	MOAIVectorRect* vectorRect = new MOAIVectorRect ();
	vectorRect->Init ( rect );
	this->PushShape ( vectorRect );
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
	
	state.SetField ( -1, "FILL_NONE",					( u32 )MOAIVectorStyle::FILL_NONE );
	state.SetField ( -1, "FILL_SOLID",					( u32 )MOAIVectorStyle::FILL_SOLID );
	
	state.SetField ( -1, "LINE_NONE",					( u32 )MOAIVectorStyle::LINE_NONE );
	state.SetField ( -1, "LINE_STROKE",					( u32 )MOAIVectorStyle::LINE_STROKE );
	state.SetField ( -1, "LINE_VECTOR",					( u32 )MOAIVectorStyle::LINE_VECTOR );
	
	state.SetField ( -1, "STROKE_CENTER",				( u32 )MOAIVectorStyle::STROKE_CENTER );
	state.SetField ( -1, "STROKE_INTERIOR",				( u32 )MOAIVectorStyle::STROKE_INTERIOR );
	state.SetField ( -1, "STROKE_EXTERIOR",				( u32 )MOAIVectorStyle::STROKE_EXTERIOR );
	
	state.SetField ( -1, "JOIN_BEVEL",					( u32 )MOAIVectorStyle::JOIN_BEVEL );
	state.SetField ( -1, "JOIN_MITER",					( u32 )MOAIVectorStyle::JOIN_MITER );
	state.SetField ( -1, "JOIN_ROUND",					( u32 )MOAIVectorStyle::JOIN_ROUND );
	
	state.SetField ( -1, "CAP_BUTT",					( u32 )MOAIVectorStyle::CAP_BUTT );
	state.SetField ( -1, "CAP_ROUND",					( u32 )MOAIVectorStyle::CAP_ROUND );
	state.SetField ( -1, "CAP_POINTY",					( u32 )MOAIVectorStyle::CAP_POINTY );
	state.SetField ( -1, "CAP_SQUARE",					( u32 )MOAIVectorStyle::CAP_SQUARE );
	
	state.SetField ( -1, "TESS_WINDING_ODD",			( u32 )TESS_WINDING_ODD );
	state.SetField ( -1, "TESS_WINDING_NONZERO",		( u32 )TESS_WINDING_NONZERO );
	state.SetField ( -1, "TESS_WINDING_POSITIVE",		( u32 )TESS_WINDING_POSITIVE );
	state.SetField ( -1, "TESS_WINDING_NEGATIVE",		( u32 )TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "TESS_WINDING_ABS_GEQ_TWO",	( u32 )TESS_WINDING_ABS_GEQ_TWO );
}

//----------------------------------------------------------------//
void MOAIVectorDrawing::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "finish",					_finish },
		{ "pushCombo",				_pushCombo },
		{ "pushEllipse",			_pushEllipse },
		{ "pushPath",				_pushPath },
		{ "pushPolygon",			_pushPolygon },
		{ "pushRect",				_pushRect },
		{ "pushVertex",				_pushVertex },
		{ "setCapStyle",			_setCapStyle },
		{ "setCircleResolution",	_setCircleResolution },
		{ "setFillColor",			_setFillColor },
		{ "setFillStyle",			_setFillStyle },
		{ "setJoinStyle",			_setJoinStyle },
		{ "setLineColor",			_setLineColor },
		{ "setLineStyle",			_setLineStyle },
		{ "setLineWidth",			_setLineWidth },
		{ "setMiterLimit",			_setMiterLimit },
		{ "setStrokeStyle",			_setStrokeStyle },
		{ "setWindingRule",			_setWindingRule },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::StrokeLine ( ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact ) {

	if ( exact ) {
		if ( verts ) {
			for ( int i = 0; i < nJoins; ++i ) {
				*( verts++ ) = joins [ i ].mVertex;
			}
		}
		return nJoins;
	}

	u32 count = 0;
	for ( int i = 0; i < nJoins; ++i ) {
		
		int j0 = ( i + nJoins -1 ) % nJoins;
		int j1 = i;
		
		const MOAIVectorLineJoin& join = joins [ j1 ];
		
		float d = join.mEdgeNorm.Dot ( join.mJointNorm );
		float miter = width / d;
		
		u32 joinStyle = this->mStyle.mJoinStyle;
		u32 capStyle = this->mStyle.mCapStyle;
		
		if ( join.mIsCap ) {
			
			switch ( capStyle ) {
				
				case MOAIVectorStyle::CAP_BUTT: {
					
					if ( verts ) {
						ZLVec2D v = join.mVertex;
						v.Add ( join.mJointNorm, width );
						*( verts++ ) = v;
					}
					count = count + 1;
					break;
				}
				case MOAIVectorStyle::CAP_ROUND: {
					
					if ( i == 0 ) {
					
						ZLVec2D n0 = join.mEdgeVec;
						n0.Scale ( -1.0f );
					
						count = count + this->StrokeWedge ( verts, join.mVertex, n0, join.mEdgeNorm, width );
					}
					else {
						count = count + this->StrokeWedge ( verts, join.mVertex, join.mEdgeNorm, join.mEdgeVec, width );
					}
					break;
				}
				case MOAIVectorStyle::CAP_POINTY: {
				
					if ( verts ) {
					
						ZLVec2D v = join.mVertex;
					
						if ( i == 0 ) {
							
							v.Add ( join.mEdgeVec, -width );
							*( verts++ ) = v;
							
							v = join.mVertex;
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
						}
						else {
							
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
							
							v = join.mVertex;
							v.Add ( join.mEdgeVec, width );
							*( verts++ ) = v;
						}
					}
					count = count + 2;
					break;
				}
				case MOAIVectorStyle::CAP_SQUARE: {
				
					if ( verts ) {
					
						ZLVec2D v = join.mVertex;
					
						if ( i == 0 ) {
							
							v.Add ( join.mEdgeVec, -width );
							*( verts++ ) = v;
							
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
						}
						else {
							
							v.Add ( join.mEdgeNorm, width );
							v.Add ( join.mEdgeVec, width );
							*( verts++ ) = v;
							
							v.Add ( join.mEdgeNorm, -width );
							*( verts++ ) = v;
						}
					}
					count = count + 2;
					break;
				}
			}
		}
		else {
			
			const MOAIVectorLineJoin& prev = joins [ j0 ];
			
			if ( join.mJointNorm.Dot ( prev.mEdgeVec ) <= 0.0f ) {
				joinStyle = MOAIVectorStyle::JOIN_MITER;
			}
			else if ( joinStyle == MOAIVectorStyle::JOIN_MITER ) {
				if (( miter / width ) > this->mStyle.mMiterLimit ) {
					joinStyle = MOAIVectorStyle::JOIN_BEVEL;
				}
			}
			
			switch ( joinStyle ) {
				
				case MOAIVectorStyle::JOIN_BEVEL: {
					
					if ( verts ) {
					
						ZLVec2D v0 = prev.mEdgeNorm;
						v0.Scale ( width );
						v0.Add ( join.mVertex );
						*( verts++ ) = v0;
						
						ZLVec2D v1 = join.mEdgeNorm;
						v1.Scale ( width );
						v1.Add ( join.mVertex );
						*( verts++ ) = v1;
					}
					count = count + 2;
					break;
				}
				
				case MOAIVectorStyle::JOIN_MITER: {
				
					if ( verts ) {
						ZLVec2D v = join.mJointNorm;
						v.Scale ( miter );
						v.Add ( join.mVertex );
						*( verts++ ) = v;
					}
					count = count + 1;
					break;
				}
				
				case MOAIVectorStyle::JOIN_ROUND: {
					
					count = count + this->StrokeWedge ( verts, join.mVertex, prev.mEdgeNorm, join.mEdgeNorm, width );
					break;
				}
			}
		}
	}
	
	return count;
}

//----------------------------------------------------------------//
int MOAIVectorDrawing::StrokeWedge ( ZLVec2D*& verts, const ZLVec2D& origin, const ZLVec2D& n0, const ZLVec2D& n1, float width ) {
	
	float wedge = n0.Radians ( n1 ); // angle between two normals
	
	u32 steps = this->GetResolutionForWedge ( wedge );
	
	if ( verts ) {
		
		float angle = n0.Radians (); // angle of first normal against x axis
		float angleStep = wedge / ( float )steps;
		
		for ( u32 i = 0; i <= steps; ++i, angle += angleStep ) {
			ZLVec2D v;
			v.mX = origin.mX + ( Cos ( angle ) * width );
			v.mY = origin.mY + ( Sin ( angle ) * width );
			*( verts++ ) = v;
		}
	}
	return ( int )( steps + 1 );
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
