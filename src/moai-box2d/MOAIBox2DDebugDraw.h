// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBOX2DDEBUGDRAW_H
#define MOAIBOX2DDEBUGDRAW_H

#include <Box2D/Box2D.h>

class MOAIGfxMgr;
struct b2AABB;

//================================================================//
// MOAIBox2DDebugDraw
//================================================================//
// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class MOAIBox2DDebugDraw :
	public b2Draw {
private:

	friend class MOAIBox2DWorld;

	MOAIGfxMgr& 	mGfxMgr;
	float			mScale;

	//----------------------------------------------------------------//
	void		DrawAABB				( b2AABB* aabb, const b2Color& color );
	void		DrawCircle				( const b2Vec2& center, float32 radius, const b2Color& color );
	void		DrawPoint				( const b2Vec2& p, float32 size, const b2Color& color );
	void		DrawPolygon				( const b2Vec2* vertices, int32 vertexCount, const b2Color& color );
	void		DrawSegment				( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color );
	void		DrawSolidCircle			( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color );
	void		DrawSolidPolygon		( const b2Vec2* vertices, int32 vertexCount, const b2Color& color );
	void		DrawTransform			( const b2Transform& xf );
				MOAIBox2DDebugDraw		( MOAIGfxMgr& gfxMgr );
	void		WriteVtx				( MOAIGfxMgr& gfxMgr, float x, float y );
};

#endif
