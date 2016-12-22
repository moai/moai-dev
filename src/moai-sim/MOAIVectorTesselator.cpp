// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGeometryWriter.h>
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorEllipse.h>
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorRect.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorTesselatorWriter
//================================================================//

//----------------------------------------------------------------//
MOAIVectorTesselatorWriter::MOAIVectorTesselatorWriter () :
	mFlushStyle ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVectorTesselatorWriter::~MOAIVectorTesselatorWriter () {
}

//----------------------------------------------------------------//
MOAIVectorShape* MOAIVectorTesselatorWriter::ReadShape ( ZLStream& stream ) {

	#define CMD_CASE(cmd, type, member, fallback)										\
		case MOAIVectorTesselatorWriter::cmd:											\
			this->mStyle.member = stream.Read < type >(( type )fallback );				\
			break;

	u8 cmd;

	do {
		
		cmd = stream.Read < u8 >( MOAIVectorTesselatorWriter::VECTOR_CMD_DONE ).Value ();
		
		switch ( cmd ) {
			
			CMD_CASE ( STYLE_CMD_FILL,					u8,				mFillStyle,				MOAIVectorStyle::FILL_SOLID )
			CMD_CASE ( STYLE_CMD_FILL_COLOR,			u32,			mFillColor,				0xffffffff )
			CMD_CASE ( STYLE_CMD_LINE,					u8,				mLineStyle,				MOAIVectorStyle::LINE_NONE )
			CMD_CASE ( STYLE_CMD_LINE_COLOR,			u32,			mLineColor,				0xffffffff )
			CMD_CASE ( STYLE_CMD_LINE_WIDTH,			float,			mLineWidth,				1.0f )
			CMD_CASE ( STYLE_CMD_STROKE,				u8,				mStrokeStyle,			MOAIVectorStyle::STROKE_NONE )
			CMD_CASE ( STYLE_CMD_STROKE_COLOR,			u32,			mStrokeColor,			0xffffffff )
			CMD_CASE ( STYLE_CMD_STROKE_WIDTH,			float,			mStrokeWidth,			1.0f )
			CMD_CASE ( STYLE_CMD_STROKE_DEPTH_BIAS,		float,			mStrokeDepthBias,		0.0f )
			CMD_CASE ( STYLE_CMD_JOIN,					u8,				mJoinStyle,				MOAIVectorStyle::JOIN_MITER )
			CMD_CASE ( STYLE_CMD_CAP,					u8,				mCapStyle,				MOAIVectorStyle::CAP_BUTT )
			CMD_CASE ( STYLE_CMD_MITER_LIMIT,			float,			mMiterLimit,			5.0f )
			CMD_CASE ( STYLE_CMD_WINDING_RULE,			u8,				mWindingRule,			TESS_WINDING_ODD )
			CMD_CASE ( STYLE_CMD_CIRCLE_RESOLUTION,		u16,			mCircleResolution,		MOAIVectorStyle::DEFAULT_CIRCLE_RESOLUTION )
			CMD_CASE ( STYLE_CMD_EXTRUDE,				float,			mExtrude,				0.0f )
			CMD_CASE ( STYLE_CMD_Z_OFFSET,				float,			mZOffset,				0.0f )
			CMD_CASE ( STYLE_CMD_LIGHT_VEC,				ZLVec3D,		mLightVec,				ZLVec3D::ORIGIN )
			CMD_CASE ( STYLE_CMD_LIGHT_COLOR,			u32,			mLightColor,			0xffffffff )
			CMD_CASE ( STYLE_CMD_LIGHT_CURVE,			u8,				mLightCurve,			ZLInterpolate::kLinear )
			CMD_CASE ( STYLE_CMD_SHADOW_COLOR,			u32,			mShadowColor,			0xff000000 )
			CMD_CASE ( STYLE_CMD_SHADOW_CURVE,			u8,				mShadowCurve,			ZLInterpolate::kLinear )
			CMD_CASE ( STYLE_CMD_FILL_EXTRA_ID,			u32,			mFillExtraID,			0 )
			CMD_CASE ( STYLE_CMD_STROKE_EXTRA_ID,		u32,			mStrokeExtraID,			0 )
			CMD_CASE ( STYLE_CMD_MERGE_NORMALS,			float,			mMergeNormals,			0.0f )
			
			case MOAIVectorTesselatorWriter::STYLE_CMD_DRAWING_TO_WORLD:
			
				this->mStyle.mDrawingToWorld = stream.Read ( this->mStyle.mDrawingToWorld );
				this->mStyle.mWorldToDrawing.Inverse ( this->mStyle.mDrawingToWorld );
				
				break;
			
			case MOAIVectorTesselatorWriter::VECTOR_CMD_SHAPE: {
				
				u8 shapeType = stream.Read < u8 >( MOAIVectorShape::UNKNOWN );
				MOAIVectorShape* shape = MOAIVectorShape::Create ( shapeType );
				assert ( shape );
				
				shape->mStyle = this->mStyle; // MUST DO THIS BEFORE READ!
				shape->Read ( stream, *this );
				
				return shape;
			}
			default:
				break;
		}
	}
	while ( cmd != MOAIVectorTesselatorWriter::VECTOR_CMD_DONE );
	
	return 0;
}

//----------------------------------------------------------------//
void MOAIVectorTesselatorWriter::WriteShape ( ZLStream& stream, const MOAIVectorShape& shape ) {

	bool forceWrite = this->mFlushStyle;
	this->mFlushStyle = false;
	
	#define WRITE_STYLE_CMD(cmd, type, member)										\
		if ( forceWrite || ( shape.mStyle.member != this->mStyle.member )) {		\
			stream.Write < u8 >(( u8 )MOAIVectorTesselatorWriter::cmd );			\
			stream.Write < type >(( type )shape.mStyle.member );					\
			this->mStyle.member = shape.mStyle.member;								\
		}
	
	#define WRITE_STYLE_CMD_FORCE(cmd, type, member)								\
		stream.Write < u8 >(( u8 )MOAIVectorTesselatorWriter::cmd );				\
		stream.Write < type >(( type )shape.mStyle.member );						\
		this->mStyle.member = shape.mStyle.member;									\

	WRITE_STYLE_CMD ( STYLE_CMD_FILL,					u8,				mFillStyle )
	WRITE_STYLE_CMD ( STYLE_CMD_FILL_COLOR,				u32,			mFillColor )
	WRITE_STYLE_CMD ( STYLE_CMD_LINE,					u8,				mLineStyle )
	WRITE_STYLE_CMD ( STYLE_CMD_LINE_COLOR,				u32,			mLineColor )
	WRITE_STYLE_CMD ( STYLE_CMD_LINE_WIDTH,				float,			mLineWidth )
	WRITE_STYLE_CMD ( STYLE_CMD_STROKE,					u8,				mStrokeStyle )
	WRITE_STYLE_CMD ( STYLE_CMD_STROKE_COLOR,			u32,			mStrokeColor )
	WRITE_STYLE_CMD ( STYLE_CMD_STROKE_WIDTH,			float,			mStrokeWidth )
	WRITE_STYLE_CMD ( STYLE_CMD_STROKE_DEPTH_BIAS,		float,			mStrokeDepthBias )
	WRITE_STYLE_CMD ( STYLE_CMD_JOIN,					u8,				mJoinStyle )
	WRITE_STYLE_CMD ( STYLE_CMD_CAP,					u8,				mCapStyle )
	WRITE_STYLE_CMD ( STYLE_CMD_MITER_LIMIT,			float,			mMiterLimit )
	WRITE_STYLE_CMD ( STYLE_CMD_WINDING_RULE,			u8,				mWindingRule )
	WRITE_STYLE_CMD ( STYLE_CMD_CIRCLE_RESOLUTION,		u16,			mCircleResolution )
	WRITE_STYLE_CMD ( STYLE_CMD_EXTRUDE,				float,			mExtrude )
	WRITE_STYLE_CMD ( STYLE_CMD_Z_OFFSET,				float,			mZOffset )
	WRITE_STYLE_CMD ( STYLE_CMD_LIGHT_VEC,				ZLVec3D,		mLightVec )
	WRITE_STYLE_CMD ( STYLE_CMD_LIGHT_COLOR,			u32,			mLightColor )
	WRITE_STYLE_CMD ( STYLE_CMD_LIGHT_CURVE,			u8,				mLightCurve )
	WRITE_STYLE_CMD ( STYLE_CMD_SHADOW_COLOR,			u32,			mShadowColor )
	WRITE_STYLE_CMD ( STYLE_CMD_SHADOW_CURVE,			u8,				mShadowCurve )
	WRITE_STYLE_CMD ( STYLE_CMD_DRAWING_TO_WORLD,		ZLAffine2D,		mDrawingToWorld )
	WRITE_STYLE_CMD ( STYLE_CMD_FILL_EXTRA_ID,			u32,			mFillExtraID )
	WRITE_STYLE_CMD ( STYLE_CMD_STROKE_EXTRA_ID,		u32,			mStrokeExtraID )
	WRITE_STYLE_CMD ( STYLE_CMD_MERGE_NORMALS,			float,			mMergeNormals )
	
	stream.Write < u8 >( MOAIVectorTesselatorWriter::VECTOR_CMD_SHAPE );
	stream.Write < u8 >( shape.GetType ());
	shape.Write ( stream, *this );
}

//================================================================//
// MOAIVectorDrawingVertexWriter
//================================================================//
class MOAIVectorDrawingVertexWriter :
	public ZLAbstractVertexWriter2D {
private:

	MOAIVectorTesselator* mDrawing;

public:

	SET ( MOAIVectorTesselator*, Drawing, mDrawing )

	//----------------------------------------------------------------//
	void WriteVertex ( const ZLVec2D& v ) {
		assert ( this->mDrawing );
		this->mDrawing->PushVertex ( v.mX, v.mY );
	}
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorTesselator::_clearShapes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->ClearShapes ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_clearStyles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.Default ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_clearTransforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->ClearTransforms ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_drawingToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "UNN" )
	
	ZLVec2D vec;
	vec.mX = state.GetValue ( 2, 0.0f );
	vec.mY = state.GetValue ( 3, 0.0f );

	self->mStyle.mDrawingToWorld.Transform ( vec );
	
	state.Push ( vec.mX );
	state.Push ( vec.mY );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_drawingToWorldVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "UNN" )
	
	ZLVec2D vec;
	vec.mX = state.GetValue ( 2, 0.0f );
	vec.mY = state.GetValue ( 3, 0.0f );

	self->mStyle.mDrawingToWorld.TransformVec ( vec );
	
	state.Push ( vec.mX );
	state.Push ( vec.mY );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_finish ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	int error = self->Finish ();
	
	bool hasContent = self->mShapeStack.GetTop () > 0;
	
	state.Push ( error );
	state.Push ( hasContent );
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_getExtrude ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	state.Push ( self->mStyle.mExtrude );
	state.Push ( self->mStyle.mZOffset );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_getTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	const ZLAffine2D& drawingToWorld = self->mStyle.mDrawingToWorld;
	
	state.Push ( drawingToWorld.m [ ZLAffine2D::C0_R0 ]);
	state.Push ( drawingToWorld.m [ ZLAffine2D::C0_R1 ]);
	
	state.Push ( drawingToWorld.m [ ZLAffine2D::C1_R0 ]);
	state.Push ( drawingToWorld.m [ ZLAffine2D::C1_R1 ]);
	
	state.Push ( drawingToWorld.m [ ZLAffine2D::C2_R0 ]);
	state.Push ( drawingToWorld.m [ ZLAffine2D::C2_R1 ]);
	
	return 6;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_openWriter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	MOAIVectorTesselatorWriter* writer = new MOAIVectorTesselatorWriter ();
	state.Push ( writer );
		
	return 1;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushBezierVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	ZLVec2D p0;
	ZLVec2D p1;
	ZLVec2D p2;
	ZLVec2D p3;
	
	p0.mX			= state.GetValue < float >( 2, 0.0f );
	p0.mY			= state.GetValue < float >( 3, 0.0f );
	
	p1.mX			= state.GetValue < float >( 4, 0.0f );
	p1.mY			= state.GetValue < float >( 5, 0.0f );
	
	p2.mX			= state.GetValue < float >( 6, 0.0f );
	p2.mY			= state.GetValue < float >( 7, 0.0f );
	
	p3.mX			= state.GetValue < float >( 8, 0.0f );
	p3.mY			= state.GetValue < float >( 9, 0.0f );
	
	self->PushBezierVertices ( p0, p1, p2, p3 );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushCombo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->PushCombo ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float x			= state.GetValue < float >( 2, 0.0f );
	float y			= state.GetValue < float >( 3, 0.0f );
	float xRad		= state.GetValue < float >( 4, 0.0f );
	float yRad		= state.GetValue < float >( 5, xRad );
	
	self->PushEllipse ( x, y, xRad, yRad );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushPoly ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	ZLVec2D* vertices = 0;
	u32 total = ( state.GetTop () - 1 ) >> 1;
	bool closed = state.GetValue < bool >( -1, true );
	
	if ( total ) {
		vertices = ( ZLVec2D* )alloca ( total * sizeof ( ZLVec2D ));
		
		for ( u32 i = 0; i < total; ++i ) {
			vertices [ i ].mX = state.GetValue < float >(( i << 1 ) + 2, 0 );
			vertices [ i ].mY = state.GetValue < float >(( i << 1 ) + 3, 0 );
		}
	}
	
	self->PushPoly ( vertices, total, closed );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	ZLRect rect = state.GetRect < float >( 2 );
	self->PushRect ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushRegion ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 2, true );
	
	if ( region ) {
		self->PushCombo ();
		self->PushRegion ( *region );
		self->Finish ();
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushRotate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );

	self->PushRotate ( x, y, r * ( float )D2R );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float x			= state.GetValue < float >( 2, 1.0f );
	float y			= state.GetValue < float >( 3, 1.0f );
	
	self->PushScale ( x, y );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushSkew ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float yx		= state.GetValue < float >( 2, 0.0f );
	float xy		= state.GetValue < float >( 3, 0.0f );
	
	self->PushSkew ( yx, xy );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float a			= state.GetValue < float >( 2, 1.0f );
	float b			= state.GetValue < float >( 3, 0.0f );
	float c			= state.GetValue < float >( 4, 0.0f );
	float d			= state.GetValue < float >( 5, 1.0f );
	
	float tx		= state.GetValue < float >( 6, 0.0f );
	float ty		= state.GetValue < float >( 7, 0.0f );
	
	self->PushTransform ( a, b, c, d, tx, ty );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushTranslate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float x			= state.GetValue < float >( 2, 1.0f );
	float y			= state.GetValue < float >( 3, 0.0f );
	
	self->PushTranslate ( x, y );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_pushVertex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	
	self->PushVertex ( x, y );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_readShapes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, false );
	
	if ( stream ) {
		self->ReadShapes ( *stream );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_reserveVertexExtras ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	u32 total		= state.GetValue < u32 >( 2, 0 );
	size_t size		= state.GetValue < u32 >( 3, 0 );
	
	self->ReserveVertexExtras ( total, size );
	return 0;	
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setCapStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mCapStyle = ( u8 )state.GetValue < u32 >( 2, MOAIVectorStyle::CAP_BUTT );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setCircleResolution ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mCircleResolution = ( u16 )state.GetValue < u32 >( 2, MOAIVectorStyle::DEFAULT_CIRCLE_RESOLUTION );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setDepthBias ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mDepthBias = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setExtrude ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mExtrude = state.GetValue < float >( 2, 0.0f );
	self->mStyle.mZOffset = state.GetValue < float >( 3, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setFillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mFillColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setFillStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mFillStyle = ( u8 )state.GetValue < u32 >( 2, MOAIVectorStyle::FILL_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setFillExtra ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mFillExtraID = state.GetValue < u32 >( 2, 1 ) - 1;
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setJoinStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mJoinStyle = ( u8 )state.GetValue < u32 >( 2, MOAIVectorStyle::JOIN_MITER );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLightColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mLightColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLightCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mLightCurve = ( u8 )state.GetValue < u32 >( 2, ZLInterpolate::kLinear );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLightVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mLightVec.mX = state.GetValue < float >( 2, 0.0f );
	self->mStyle.mLightVec.mY = state.GetValue < float >( 3, 0.0f );
	self->mStyle.mLightVec.mZ = state.GetValue < float >( 4, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLineColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mLineColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLineStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mLineStyle = ( u8 )state.GetValue < u32 >( 2, MOAIVectorStyle::LINE_NONE );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setLineWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mLineWidth = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setMergeNormals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mMergeNormals = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setMiterLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mMiterLimit = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setPolyClosed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mPolyClosed = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setShadowColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mShadowColor = state.GetColor ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setShadowCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mShadowCurve = ( u8 )state.GetValue < u32 >( 2, ZLInterpolate::kLinear );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setStrokeColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mStyle.mStrokeColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setStrokeDepthBias ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mStrokeDepthBias = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setStrokeExtra ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mStrokeExtraID =  state.GetValue < u32 >( 2, 1 ) - 1;
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setStrokeStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mStrokeStyle = ( u8 )state.GetValue < u32 >( 2, MOAIVectorStyle::STROKE_CENTER );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setStrokeWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mStrokeWidth = state.GetValue < float >( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setVerbose ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )

	self->mVerbose = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setVertexExtra ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 )- 1;
	
	size_t len;
	void* extra = ( void* )lua_tolstring ( state, 3, &len );
	
	self->SetVertexExtra ( idx, extra, len );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_setWindingRule ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	self->mStyle.mWindingRule = ( u8 )state.GetValue < u32 >( 2, ( u32 )TESS_WINDING_ODD );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_tesselate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	u32 base = 0;
	int totalElements = 0;
	
	MOAIVertexBuffer* vtxBuffer		= state.GetLuaObject < MOAIVertexBuffer >( 2, false );
	MOAIIndexBuffer* idxBuffer		= state.GetLuaObject < MOAIIndexBuffer >( 3, false );

	if ( vtxBuffer && idxBuffer ) {
	
		u32 idxSizeInBytes			= state.GetValue < u32 >( 4, 4 );
		MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 5, false );
		u32 flags					= state.GetValue < u32 >( 6, MOAIVectorTesselator::TESSELATE_ALL );
		
		base = ( u32 )( idxBuffer->GetCursor () / idxSizeInBytes ); // TODO: cast
		totalElements = self->Tesselate ( *vtxBuffer, *idxBuffer, *format, idxSizeInBytes, flags );
	}
	else {
		
		MOAIStream* vtxStream		= state.GetLuaObject < MOAIStream >( 2, false );
		MOAIStream* idxStream		= state.GetLuaObject < MOAIStream >( 3, false );
		MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 4, false );
		u32 flags					= state.GetValue < u32 >( 5, MOAIVectorTesselator::TESSELATE_ALL );
		
		if ( vtxStream && idxStream && format ) {
			base = ( u32 )( idxStream->GetCursor () / 4 ); // TODO: cast
			totalElements = self->Tesselate ( *vtxStream, *idxStream, *format, flags );
		}
	}
	
	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 2, false );
	
	if ( region ) {
		u32 flags		= state.GetValue < u32 >( 3, MOAIVectorTesselator::TESSELATE_ALL );
		totalElements	= self->Tesselate ( *region, flags );
	}
	
	state.Push ( totalElements );
	state.Push ( base );
	state.Push ( base + totalElements );
	return 3;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_worldToDrawing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "UNN" )
	
	ZLVec2D vec;
	vec.mX = state.GetValue ( 2, 0.0f );
	vec.mY = state.GetValue ( 3, 0.0f );

	self->mStyle.mWorldToDrawing.Transform ( vec );
	
	state.Push ( vec.mX );
	state.Push ( vec.mY );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_worldToDrawingVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "UNN" )
	
	ZLVec2D vec;
	vec.mX = state.GetValue ( 2, 0.0f );
	vec.mY = state.GetValue ( 3, 0.0f );

	self->mStyle.mWorldToDrawing.TransformVec ( vec );
	
	state.Push ( vec.mX );
	state.Push ( vec.mY );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::_writeShapes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorTesselator, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, false );
	MOAIVectorTesselatorWriter* writer = state.GetLuaObject < MOAIVectorTesselatorWriter >( 3, false );
	
	if ( writer ) {
		self->WriteShapes ( *stream, writer );
	}
	return 0;
}

//================================================================//
// MOAIVectorTesselator
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorTesselator::Clear () {

	this->ClearShapes ();
	
	for ( u32 i = 0; i < this->mVtxExtras.Size (); ++i ) {
		free ( this->mVtxExtras [ i ]);
	}
	this->mVtxExtraSize = 0;
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::ClearShapes () {

	for ( u32 i = 0; i < this->mShapeStack.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mShapeStack [ i ];
		if ( shape ) {
			delete shape;
		}
	}
	
	this->mShapeStack.Reset ();
	this->mVertexStack.Reset ();
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::ClearTransforms () {

	this->mMatrixStack.Reset ();
	this->mStyle.mDrawingToWorld.Ident ();
	this->mStyle.mWorldToDrawing.Ident ();
}

//----------------------------------------------------------------//
u32 MOAIVectorTesselator::CountVertices ( const MOAIVertexFormat& format, ZLStream& vtxStream ) {

	return ( u32 )( vtxStream.GetCursor () / format.GetVertexSize ()); // TODO: cast
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::Finish () {

	// TODO: check overflow
	u32 vertsTop = ( u32 )this->mVertexStack.GetTop ();
	u32 shapesTop = ( u32 )this->mShapeStack.GetTop ();
	
	int error = 0;
	
	if ( vertsTop ) {
	
		assert ( shapesTop );
		MOAIVectorShape* shape = this->mShapeStack [ shapesTop - 1 ];
		bool result = shape->SetVertices ( this->mVertexStack, vertsTop, this->mPolyClosed );
		
		assert ( result );
		UNUSED ( result );
		
		this->mVertexStack.Reset ();
	}
	else {

		if ( shapesTop >= 2 ) {
			
			bool done = false;
			for ( int i = ( shapesTop - 2 ); ( i >= 0 ) && ( !done ); --i ) {
				int c = i + 1;
				MOAIVectorShape* shape = this->mShapeStack [ i ];
				if ( shape->CanGroup () && shape->GroupShapes ( &this->mShapeStack [ c ], shapesTop - c )) {
					this->mShapeStack.SetTop ( c );
					done = true;
				}
			}
		}
		
		if ( this->mShapeStack.GetTop () == shapesTop ) {
			this->mDepthOffset = 0.0f;
		}
	}
	
	return error;
}

//----------------------------------------------------------------//
MOAIVectorTesselator::MOAIVectorTesselator () :
	mDepthBias ( 0.0f ),
	mDepthOffset ( 0.0f ),
	mVerbose ( false ),
	mVtxExtraSize ( 0 ) {
	
	this->mStyle.Default ();
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVectorTesselator::~MOAIVectorTesselator () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PopTransform () {

	this->mMatrixStack.Pop ();
	
	ZLAffine2D transform;
	transform.Ident ();
	
	for ( u32 i = 0; i < this->mMatrixStack.GetTop (); ++i ) {
		transform.Prepend ( this->mMatrixStack [ i ]);
	}
	this->mStyle.mDrawingToWorld = transform;
	this->mStyle.mWorldToDrawing.Inverse ( this->mStyle.mDrawingToWorld );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushBezierVertices ( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2, const ZLVec2D& p3 ) {

	MOAIVectorDrawingVertexWriter writer;
	writer.SetDrawing ( this );

	ZLCubicBezier2D curve;
	curve.Init ( p0, p1, p2, p3 );
	curve.Flatten ( writer );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushCombo () {

	MOAIVectorCombo* combo = new MOAIVectorCombo ();
	this->PushShape ( combo );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushEllipse ( float x, float y, float xRad, float yRad ) {

	MOAIVectorEllipse* ellipse = new MOAIVectorEllipse ();
	this->PushShape ( ellipse );
	ellipse->Init ( x, y, xRad, yRad );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushPoly ( const ZLVec2D* vertices, u32 total, bool closed ) {

	this->mPolyClosed = closed;

	MOAIVectorPoly* poly = new MOAIVectorPoly ();
	this->PushShape ( poly );
	poly->SetVertices ( vertices, total, closed );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushRect ( float xMin, float yMin, float xMax, float yMax ) {

	MOAIVectorRect* vectorRect = new MOAIVectorRect ();
	this->PushShape ( vectorRect );
	vectorRect->Init ( xMin, yMin, xMax, yMax );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushRegion ( const MOAIRegion& region ) {

	size_t size = region.GetSize ();
	for ( size_t i = 0; i < size; ++i ) {
	
		const ZLPolygon2D& polygon = region.GetPolygon (( u32 )i );
		this->PushPoly ( polygon.GetVertices(), ( u32 )polygon.GetSize (), true ); // TODO: cast
	}
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushRotate ( float x, float y, float r ) {

	ZLAffine2D transform;

	if (( x != 0.0f ) || ( y != 0.0f )) {
		
		ZLAffine2D mtx;
		
		transform.Translate ( -x, -y );
		
		mtx.Rotate ( r );
		transform.Append ( mtx );
	
		mtx.Translate ( x, y );
		transform.Append ( mtx );
		
		this->PushTransform ( transform );
	}
	else {
		transform.Rotate ( r );
		this->PushTransform ( transform );
	}
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushScale ( float x, float y ) {
	
	ZLAffine2D transform;
	transform.Scale ( x, y );
	this->PushTransform ( transform );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushShape ( MOAIVectorShape* shape ) {

	shape->mStyle = this->mStyle;
	this->mShapeStack.Push ( shape );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushSkew ( float yx, float xy ) {

	ZLAffine2D transform;
	transform.Shear ( Tan ( yx ), Tan ( xy ));
	this->PushTransform ( transform );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushTransform ( const ZLAffine2D& transform ) {

	this->mMatrixStack.Push ( transform );
	this->mStyle.mDrawingToWorld.Prepend ( transform );
	this->mStyle.mWorldToDrawing.Inverse ( this->mStyle.mDrawingToWorld );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushTransform ( float a, float b, float c, float d, float tx, float ty ) {

	ZLAffine2D transform;
	
	transform.m [ ZLAffine2D::C0_R0 ] = a;
	transform.m [ ZLAffine2D::C0_R1 ] = b;
	
	transform.m [ ZLAffine2D::C1_R0 ] = c;
	transform.m [ ZLAffine2D::C1_R1 ] = d;
	
	transform.m [ ZLAffine2D::C2_R0 ] = tx;
	transform.m [ ZLAffine2D::C2_R1 ] = ty;
	
	this->PushTransform ( transform );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushTranslate ( float x, float y ) {

	ZLAffine2D transform;
	transform.Translate ( x, y );
	this->PushTransform ( transform );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::PushVertex ( float x, float y ) {

	ZLVec2D vertex ( x, y );
	this->mVertexStack.Push ( vertex );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::ReadShapes ( ZLStream& stream ) {

	this->ClearShapes ();

	MOAIVectorTesselatorWriter writer;
	writer.mStyle = this->mStyle;
	
	while ( MOAIVectorShape* shape = writer.ReadShape ( stream )) {
		this->mShapeStack.Push ( shape );
	}
	this->mStyle = writer.mStyle;
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "FILL_NONE",					( u32 )MOAIVectorStyle::FILL_NONE );
	state.SetField ( -1, "FILL_SOLID",					( u32 )MOAIVectorStyle::FILL_SOLID );
	
	state.SetField ( -1, "LINE_NONE",					( u32 )MOAIVectorStyle::LINE_NONE );
	state.SetField ( -1, "LINE_VECTOR",					( u32 )MOAIVectorStyle::LINE_VECTOR );
	
	state.SetField ( -1, "STROKE_NONE",					( u32 )MOAIVectorStyle::STROKE_NONE );
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
	
	state.SetField ( -1, "TESSELATE_FILLS",				( u32 )TESSELATE_FILLS );
	state.SetField ( -1, "TESSELATE_STROKES",			( u32 )TESSELATE_STROKES );
	state.SetField ( -1, "TESSELATE_ALL",				( u32 )TESSELATE_ALL );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clearShapes",			_clearShapes },
		{ "clearStyles",			_clearStyles },
		{ "clearTransforms",		_clearTransforms },
		{ "drawingToWorld",			_drawingToWorld },
		{ "drawingToWorldVec",		_drawingToWorldVec },
		{ "finish",					_finish },
		{ "getExtrude",				_getExtrude },
		{ "getTransform",			_getTransform },
		{ "openWriter",				_openWriter },
		{ "pushBezierVertices",		_pushBezierVertices },
		{ "pushCombo",				_pushCombo },
		{ "pushEllipse",			_pushEllipse },
		{ "pushPoly",				_pushPoly },
		{ "pushRect",				_pushRect },
		{ "pushRegion",				_pushRegion },
		{ "pushRotate",				_pushRotate },
		{ "pushScale",				_pushScale },
		{ "pushSkew",				_pushSkew },
		{ "pushTransform",			_pushTransform },
		{ "pushTranslate",			_pushTranslate },
		{ "pushVertex",				_pushVertex },
		{ "readShapes",				_readShapes },
		{ "reserveVertexExtras",	_reserveVertexExtras },
		{ "setCapStyle",			_setCapStyle },
		{ "setCircleResolution",	_setCircleResolution },
		{ "setDepthBias",			_setDepthBias },
		{ "setExtrude",				_setExtrude },
		{ "setFillColor",			_setFillColor },
		{ "setFillStyle",			_setFillStyle },
		{ "setFillExtra",			_setFillExtra },
		{ "setJoinStyle",			_setJoinStyle },
		{ "setLightColor",			_setLightColor },
		{ "setLightCurve",			_setLightCurve },
		{ "setLightVec",			_setLightVec },
		{ "setLineColor",			_setLineColor },
		{ "setLineStyle",			_setLineStyle },
		{ "setLineWidth",			_setLineWidth },
		{ "setMergeNormals",		_setMergeNormals },
		{ "setMiterLimit",			_setMiterLimit },
		{ "setPolyClosed",			_setPolyClosed },
		{ "setShadowColor",			_setShadowColor },
		{ "setShadowCurve",			_setShadowCurve },
		{ "setStrokeColor",			_setStrokeColor },
		{ "setStrokeDepthBias",		_setStrokeDepthBias },
		{ "setStrokeExtra",			_setStrokeExtra },
		{ "setStrokeStyle",			_setStrokeStyle },
		{ "setStrokeWidth",			_setStrokeWidth },
		{ "setVerbose",				_setVerbose },
		{ "setVertexExtra",			_setVertexExtra },
		{ "setWindingRule",			_setWindingRule },
		{ "tesselate",				_tesselate },
		{ "worldToDrawing",			_worldToDrawing },
		{ "worldToDrawingVec",		_worldToDrawingVec },
		{ "writeShapes",			_writeShapes },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::ReserveVertexExtras ( u32 total, size_t size ) {

	this->mVtxExtraSize = size;
	this->mVtxExtras.Init ( total );
	
	for ( u32 i = 0; i < this->mVtxExtras.Size (); ++i ) {
		this->mVtxExtras [ i ] = calloc ( 1, this->mVtxExtraSize );
	}
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::SetVertexExtra ( u32 idx, void* extra, size_t size ) {

	size = size <= this->mVtxExtraSize ? size : this->mVtxExtraSize;
	if ( idx < this->mVtxExtras.Size ()) {
		memcpy ( this->mVtxExtras [ idx ], extra, size );
	}
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::Tesselate ( SafeTesselator& tess, u32 flags ) {

	int error = this->Finish ();
	if ( error ) return error;

	for ( u32 i = 0; i < this->mShapeStack.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mShapeStack [ i ];
		error = shape->Tesselate ( *this, tess, flags );
		if ( error ) return error;
	}
	return ( int )this->mShapeStack.GetTop (); // TODO: fix cast
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::Tesselate ( MOAIRegion& region, u32 flags ) {

	region.Clear ();
	bool useTess = true;
	
	if ( this->mShapeStack.GetTop () == 1 ) {

		useTess = this->mShapeStack [ 0 ]->Tesselate ( *this, region, flags ) != 0;
	}

	if ( useTess ) {
		
		region.Clear ();
	
		SafeTesselator tess;
		if ( !this->Tesselate ( tess, flags )) return 0;
		
		int error = tess.Tesselate (( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0 );

		if ( !error ) {
			region.Copy ( tess );
			region.Transform ( region, this->mStyle.mDrawingToWorld );
			region.Bless ();
		}
	}
	
	return ( int )region.GetSize (); // TODO: cast
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::Tesselate ( ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, u32 flags ) {

	int error = this->Finish ();
	if ( error ) return -1;

	int base = ( int )idxStream.GetCursor(); // TODO: cast

	this->mDepthOffset = 0.0f;

	for ( u32 i = 0; i < this->mShapeStack.GetTop (); ++i ) {
		MOAIVectorShape* shape = this->mShapeStack [ i ];
		error = shape->Tesselate ( *this, vtxStream, idxStream, format, flags );
		assert ( error == 0 );
		if ( error ) return -1;
	}
	
	// idx stream is 32-bits, so divide by 4 to get total indices
	return ( int )(( idxStream.GetCursor () - base ) >> 2 ); // TODO: cast
}

//----------------------------------------------------------------//
int MOAIVectorTesselator::Tesselate ( MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, MOAIVertexFormat& format, u32 idxSizeInBytes, u32 flags ) {
	
	ZLMemStream vtxStream;
	ZLMemStream idxStream;
	
	int totalIndices = this->Tesselate ( vtxStream, idxStream, format, flags );
	
	if ( totalIndices > 0 ) {
		return MOAIGeometryWriter::GetMesh ( format, vtxStream, vtxStream.GetLength (), idxStream, idxStream.GetLength (), vtxBuffer, idxBuffer, idxSizeInBytes );
	}
	
	// something went wrong, return error code
	return totalIndices;
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::WriteShapes ( ZLStream& stream, MOAIVectorTesselatorWriter* writer ) {

	assert ( writer );

	MOAIVectorTesselatorWriter defaultWriter;
	writer = writer ? writer : &defaultWriter;
	
	u32 nShapes = this->mShapeStack.GetTop ();

	if ( nShapes ) {
		for ( u32 i = 0; i < nShapes; ++i ) {
			MOAIVectorShape* shape = this->mShapeStack [ i ];
			writer->WriteShape ( stream, *shape );
		}
	}
	stream.Write < u8 >( MOAIVectorTesselatorWriter::VECTOR_CMD_DONE );
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::WriteSkirt ( SafeTesselator& tess, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, const MOAIVectorStyle& style, const ZLColorVec& fillColor, u32 vertexExtraID ) {

	u32 base = this->CountVertices ( format, vtxStream );
	float z = style.GetExtrude ();
	float zOff = style.GetZOffset ();

	ZLVec3D lightVec3D = style.GetLightVec ();
	ZLVec2D lightVec ( lightVec3D.mX, lightVec3D.mY );
	
	float lightLen = lightVec.Norm ();
	bool doLighting = lightLen != 0.0f ? true : false;
	
	ZLColorVec lightColor = style.mLightColor;
	u32 lightCurve = style.mLightCurve;
	float lightAlpha = lightColor.mA;
	lightColor.mA = 1.0f;

	ZLColorVec shadowColor = style.mShadowColor;
	u32 shadowCurve = style.mShadowCurve;
	float shadowAlpha = shadowColor.mA;
	shadowColor.mA = 1.0f;

	float normalsMergeCos = style.GetMergeNormalsCosine ();

	u32 color32 = fillColor.PackRGBA ();

	const int* elems = tessGetElements ( tess.mTess );
	const int nelems = tessGetElementCount ( tess.mTess );
	const float* verts = tessGetVertices ( tess.mTess );
	
	for ( int i = 0; i < nelems; ++i ) {
		
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ]; // vertices
		
		ZLVec2D* vertices = ( ZLVec2D* )alloca ( n * sizeof ( ZLVec2D ));
		assert ( vertices );
		
		ZLVec2D* faceNormals = ( ZLVec2D* )alloca ( n * sizeof ( ZLVec2D ));
		assert ( faceNormals );
		
		for ( int j = 0; j < n; ++j ) {
			
			ZLVec2D v0 = (( ZLVec2D* )verts )[ b + j ];
			style.mDrawingToWorld.Transform ( v0 );
			vertices [ j ] = v0;
		}
		
		for ( int j = 0; j < n; ++j ) {
			
			const ZLVec2D& v0 = vertices [ j ];
			const ZLVec2D& v1 = vertices [( j + 1 ) % n ];
			
			ZLVec2D e = v1;
			e.Sub ( v0 );
			e.Rotate90Clockwise ();
			e.Norm ();
			
			faceNormals [ j ] = e;
		}
		
		for ( int j = 0; j < n; ++j ) {
			
			const ZLVec2D& v0 = vertices [ j ];
			const ZLVec2D& v1 = vertices [( j + 1 ) % n ];
			
			const ZLVec2D& n0 = faceNormals [( j + n - 1 ) % n ]; // prev
			const ZLVec2D& n1 = faceNormals [ j ]; // current
			const ZLVec2D& n2 = faceNormals [( j + 1 ) % n ]; // next

			ZLVec2D normal0 = n1;
			if ( n1.Dot ( n0 ) > normalsMergeCos ) {
				normal0.Add ( n0 );
				normal0.Norm ();
			}
			
			ZLVec2D normal1 = n1;
			if ( n1.Dot ( n2 ) > normalsMergeCos ) {
				normal1.Add ( n2 );
				normal1.Norm ();
			}
			
			u32 color0 = color32;
			u32 color1 = color32;
			
			if ( doLighting ) {
				
				ZLColorVec color;
				float dot;
				
				dot = lightVec.Dot ( normal0 );
				if ( dot < 0.0f ) {
					color.Lerp ( shadowCurve, fillColor, shadowColor, -dot * shadowAlpha );
				}
				else {
					color.Lerp ( lightCurve, fillColor, lightColor, dot * lightAlpha );
				}
				color0 = color.PackRGBA ();
				
				dot = lightVec.Dot ( normal1 );
				if ( dot < 0.0f ) {
					color.Lerp ( shadowCurve, fillColor, shadowColor, -dot * shadowAlpha );
				}
				else {
					color.Lerp ( lightCurve, fillColor, lightColor, dot * lightAlpha );
				}
				color1 = color.PackRGBA ();
			}
			
			this->WriteVertex ( vtxStream, format, v0.mX, v0.mY, zOff, normal0.mX, normal0.mY, 0.0f, color0, vertexExtraID );
			this->WriteVertex ( vtxStream, format, v1.mX, v1.mY, zOff, normal1.mX, normal1.mY, 0.0f, color1, vertexExtraID );
			this->WriteVertex ( vtxStream, format, v0.mX, v0.mY, z + zOff, normal0.mX, normal0.mY, 0.0f, color0, vertexExtraID );
			this->WriteVertex ( vtxStream, format, v1.mX, v1.mY, z + zOff, normal1.mX, normal1.mY, 0.0f, color1, vertexExtraID );
			
			idxStream.Write < u32 >( base + 0 );
			idxStream.Write < u32 >( base + 1 );
			idxStream.Write < u32 >( base + 3 );
			
			idxStream.Write < u32 >( base + 0 );
			idxStream.Write < u32 >( base + 3 );
			idxStream.Write < u32 >( base + 2 );
			
			base += 4;
		}
	}
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::WriteTriangles ( SafeTesselator& tess, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, const MOAIVectorStyle& style, float z, u32 color, u32 vertexExtraID ) {

	u32 base = this->CountVertices ( format, vtxStream );

	z = z != 0.0f ? z : this->mDepthOffset;

	STLString log;

	if ( this->mVerbose ) {
		log = "";
		log.write ( "WRITING VERTICES:\n" );
	}

	const float* verts = tessGetVertices ( tess.mTess );
	const int nverts = tessGetVertexCount ( tess.mTess );
	
	for ( int i = 0; i < nverts; ++i ) {
		
		ZLVec2D vert = (( const ZLVec2D* )verts )[ i ];
		
		if ( this->mVerbose ) {
			log.write ( "%d: %f, %f\n", i, vert.mX, vert.mY );
		}
		
		style.mDrawingToWorld.Transform ( vert );
		this->WriteVertex ( vtxStream, format, vert.mX, vert.mY, z, 0.0f, 0.0f, 1.0f, color, vertexExtraID );
	}
	
	if ( this->mVerbose ) {
		log.write ( "\n" );
		ZLLogF ( ZLLog::CONSOLE, log.c_str ());
	}

	this->mDepthOffset += this->mDepthBias;
	
	if ( this->mVerbose ) {
		ZLLogF ( ZLLog::CONSOLE, "WRITING INDICES:\n" );
	}

	const int* elems = tessGetElements ( tess.mTess );
	const int nelems = tessGetElementCount ( tess.mTess );
	
	for ( int i = 0; i < nelems; ++i ) {
		const int* tri = &elems [ i * 3 ];
		
		if ( this->mVerbose ) {
			ZLLogF ( ZLLog::CONSOLE, "%d: %d, %d, %d\n", i, tri [ 0 ], tri [ 1 ], tri [ 2 ]);
		}
		
		idxStream.Write < u32 >( base + tri [ 0 ]);
		idxStream.Write < u32 >( base + tri [ 1 ]);
		idxStream.Write < u32 >( base + tri [ 2 ]);
	}
	
	if ( this->mVerbose ) {
		ZLLogF ( ZLLog::CONSOLE, "\n" );
	}
}

//----------------------------------------------------------------//
void MOAIVectorTesselator::WriteVertex ( ZLStream& stream, MOAIVertexFormat& format, float x, float y, float z, float xn, float yn, float zn, u32 color, u32 vertexExtraID ) {

	size_t base = stream.GetCursor ();
	format.WriteAhead ( stream );

	size_t vertexSize = format.GetVertexSize ();
	if ( this->mVtxExtraSize && ( this->mVtxExtraSize < vertexSize )) {
	
		stream.SetCursor ( base + ( vertexSize - this->mVtxExtraSize ));
		
		vertexExtraID = vertexExtraID % this->mVtxExtras.Size ();
		stream.WriteBytes ( this->mVtxExtras [ vertexExtraID ], this->mVtxExtraSize );
		
		stream.SetCursor ( base );
	}
	
	format.WriteCoord ( stream, 0, x, y, z, 1.0f );
	format.WriteColor ( stream, 0, color );
	format.WriteNormal ( stream, 0, xn, yn, zn );
	
	// next vertex
	format.SeekVertex ( stream, base, 1 );
}
