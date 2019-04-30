// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-box2d/MOAIBox2DDebugDraw.h>

#include <cstdio>
#include <cstdarg>
#include <cstring>

//================================================================//
// MOAIBox2DDebugDraw
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawAABB ( b2AABB* aabb, const b2Color& c ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();

	gfxState.SetPenColor ( c.r, c.g, c.b, 1.0f );
	gfxState.BeginPrim ( ZGL_PRIM_LINE_LOOP, 4 );
		this->WriteVtx ( gfxState, aabb->lowerBound.x, aabb->lowerBound.y );
		this->WriteVtx ( gfxState, aabb->upperBound.x, aabb->lowerBound.y );
		this->WriteVtx ( gfxState, aabb->upperBound.x, aabb->upperBound.y );
		this->WriteVtx ( gfxState, aabb->lowerBound.x, aabb->upperBound.y );
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawCircle ( const b2Vec2& center, float32 radius, const b2Color& color ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();
	
	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );

	const u32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	
	gfxState.BeginPrim ( ZGL_PRIM_LINE_LOOP, k_segments );
	float32 theta = 0.0f;
	for ( int32 i = 0; i < k_segments; ++i ) {
		b2Vec2 v = center + radius * b2Vec2 ( cosf ( theta ), sinf ( theta ));
		this->WriteVtx ( gfxState, v.x, v.y );
		theta += k_increment;
	}
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawPoint ( const b2Vec2& p, float32 size, const b2Color& color ) {
	UNUSED ( size );

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();

	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );
	gfxState.BeginPrim ( ZGL_PRIM_POINTS, 1 );
	this->WriteVtx ( gfxState, p.x, p.y );
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawPolygon ( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();
	
	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxState.BeginPrim ( ZGL_PRIM_LINE_LOOP, vertexCount );
	for ( int32 i = 0; i < vertexCount; ++i ) {
		this->WriteVtx ( gfxState, vertices [ i ].x, vertices [ i ].y );
	}
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSegment ( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();
	
	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxState.BeginPrim ( ZGL_PRIM_LINES, 2 );
		this->WriteVtx ( gfxState, p1.x, p1.y );
		this->WriteVtx ( gfxState, p2.x, p2.y );
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSolidCircle ( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();
	
	gfxState.SetBlendMode ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	gfxState.SetPenColor ( 0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f );

	const u32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	
	gfxState.BeginPrim ( ZGL_PRIM_TRIANGLE_FAN, k_segments );
	float32 theta = 0.0f;
	for (int32 i = 0; i < k_segments; ++i) {
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		this->WriteVtx ( gfxState, v.x, v.y );
		theta += k_increment;
	}
	gfxState.EndPrim ();

	gfxState.SetBlendMode ();
	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxState.BeginPrim ( ZGL_PRIM_LINE_LOOP, k_segments );
	theta = 0.0f;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		this->WriteVtx ( gfxState, v.x, v.y );
		theta += k_increment;
	}
	gfxState.EndPrim ();

	b2Vec2 p = center + radius * axis;
	gfxState.BeginPrim ( ZGL_PRIM_LINES, 2 );
		this->WriteVtx ( gfxState, center.x, center.y );
		this->WriteVtx ( gfxState, p.x, p.y );
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSolidPolygon ( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) {
	
	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();
	
	gfxState.SetBlendMode ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	
	gfxState.SetPenColor ( 0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f );

	gfxState.BeginPrim ( ZGL_PRIM_TRIANGLE_FAN, vertexCount );
	for ( int32 i = 0; i < vertexCount; ++i ) {
		this->WriteVtx ( gfxState, vertices [ i ].x, vertices [ i ].y );
	}
	gfxState.EndPrim ();
	
	gfxState.SetBlendMode ();
	gfxState.SetPenColor ( color.r, color.g, color.b, 1.0f );
	
	gfxState.BeginPrim ( ZGL_PRIM_LINE_LOOP, vertexCount );
	for (int32 i = 0; i < vertexCount; ++i) {
		this->WriteVtx ( gfxState, vertices [ i ].x, vertices [ i ].y );
	}
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawTransform ( const b2Transform& xf ) {

	ZLGfxMgrGL& gfxState = MOAIGfxMgrGL::Get ();

	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	
	gfxState.SetPenColor ( 1.0f, 0.0f, 0.0f, 1.0f );
	gfxState.BeginPrim ( ZGL_PRIM_LINES, 2 );
	
		this->WriteVtx ( gfxState, p1.x, p1.y );
		
		p2 = p1 + k_axisScale * xf.q.GetXAxis ();
		this->WriteVtx ( gfxState, p2.x, p2.y );

	gfxState.EndPrim();

	gfxState.SetPenColor ( 0.0f, 1.0f, 0.0f, 1.0f );
	gfxState.BeginPrim ( ZGL_PRIM_LINES, 2 );

		this->WriteVtx ( gfxState, p1.x, p1.y );
		
		p2 = p1 + k_axisScale * xf.q.GetYAxis ();
		this->WriteVtx ( gfxState, p2.x, p2.y );

	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::WriteVtx ( ZLGfxMgrGL& gfxState, float x, float y ) {

	ZLVec3D vtx;
	vtx.mX = x * this->mScale;
	vtx.mY = y * this->mScale;
	vtx.mZ = 0.0f;
	gfxState.WriteVtx ( vtx );
	gfxState.WritePenColor4b ();
}
