/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "pch.h"
#include <moaicore/MOAIBox2DDebugDraw.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIVertexFormatMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

#include <cstdio>
#include <cstdarg>
#include <cstring>

//================================================================//
// MOAIBox2DDebugDraw
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawPolygon ( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	for ( int32 i = 0; i < vertexCount; ++i ) {
		this->WriteVtx ( gfxDevice, vertices [ i ].x, vertices [ i ].y );
	}
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSolidPolygon ( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetBlendMode ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	gfxDevice.SetPenColor ( 0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f );

	gfxDevice.BeginPrim ( GL_TRIANGLE_FAN );
	for ( int32 i = 0; i < vertexCount; ++i ) {
		this->WriteVtx ( gfxDevice, vertices [ i ].x, vertices [ i ].y );
	}
	gfxDevice.EndPrim ();
	
	gfxDevice.SetBlendMode ();
	gfxDevice.SetPenColor ( color.r, color.g, color.b, 1.0f );
	
	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	for (int32 i = 0; i < vertexCount; ++i) {
		this->WriteVtx ( gfxDevice, vertices [ i ].x, vertices [ i ].y );
	}
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawCircle ( const b2Vec2& center, float32 radius, const b2Color& color ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetPenColor ( color.r, color.g, color.b, 1.0f );

	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	
	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	float32 theta = 0.0f;
	for (int32 i = 0; i < k_segments; ++i) {
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		this->WriteVtx ( gfxDevice, v.x, v.y );
		theta += k_increment;
	}
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSolidCircle ( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetBlendMode ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	gfxDevice.SetPenColor ( 0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f );

	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	
	gfxDevice.BeginPrim ( GL_TRIANGLE_FAN );
	float32 theta = 0.0f;
	for (int32 i = 0; i < k_segments; ++i) {
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		this->WriteVtx ( gfxDevice, v.x, v.y );
		theta += k_increment;
	}
	gfxDevice.EndPrim ();

	gfxDevice.SetBlendMode ();
	gfxDevice.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	theta = 0.0f;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		this->WriteVtx ( gfxDevice, v.x, v.y );
		theta += k_increment;
	}
	gfxDevice.EndPrim ();

	b2Vec2 p = center + radius * axis;
	gfxDevice.BeginPrim ( GL_LINES );
	this->WriteVtx ( gfxDevice, center.x, center.y );
	this->WriteVtx ( gfxDevice, p.x, p.y );
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawSegment ( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetPenColor ( color.r, color.g, color.b, 1.0f );

	gfxDevice.BeginPrim ( GL_LINES );
	this->WriteVtx ( gfxDevice, p1.x, p1.y );
	this->WriteVtx ( gfxDevice, p2.x, p2.y );
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawTransform ( const b2Transform& xf ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	
	gfxDevice.BeginPrim(GL_LINES);
	
		gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
		this->WriteVtx(gfxDevice, p1.x, p1.y);
		
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		this->WriteVtx(gfxDevice, p2.x, p2.y);

	gfxDevice.EndPrim();

	gfxDevice.BeginPrim(GL_LINES);
	
		gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 1.0f);
		this->WriteVtx(gfxDevice, p1.x, p1.y);
		
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		this->WriteVtx(gfxDevice, p2.x, p2.y);

	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawPoint ( const b2Vec2& p, float32 size, const b2Color& color ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPointSize(size);
	gfxDevice.BeginPrim(GL_POINTS);
	gfxDevice.SetPenColor(color.r, color.g, color.b, 1.0f);
	this->WriteVtx(gfxDevice, p.x, p.y);
	gfxDevice.EndPrim();
	gfxDevice.SetPointSize(1.0f);
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::DrawAABB ( b2AABB* aabb, const b2Color& c ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPenColor(c.r, c.g, c.b, 1.0f);
	gfxDevice.BeginPrim(GL_LINE_LOOP);
	this->WriteVtx(gfxDevice, aabb->lowerBound.x, aabb->lowerBound.y);
	this->WriteVtx(gfxDevice, aabb->upperBound.x, aabb->lowerBound.y);
	this->WriteVtx(gfxDevice, aabb->upperBound.x, aabb->upperBound.y);
	this->WriteVtx(gfxDevice, aabb->lowerBound.x, aabb->upperBound.y);
	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIBox2DDebugDraw::WriteVtx ( MOAIGfxDevice& gfxDevice, float x, float y ) {

	USVec3D vtx;
	vtx.mX = x * this->mScale;
	vtx.mY = y * this->mScale;
	vtx.mZ = 0.0f;
	gfxDevice.WriteVtx ( vtx );
	gfxDevice.WriteFinalColor4b ();
}

#endif