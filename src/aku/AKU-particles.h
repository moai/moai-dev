//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_PARTICLES_H
#define AKU_PARTICLES_H

#include <aku/AKU.h>

//================================================================//
// AKUParticleSprite
//================================================================//
class AKUParticleSprite {
public:
	
	float		mXLoc;
	float		mYLoc;
	float		mZRot;
	float		mXScl;
	float		mYScl;
	float		mRed;
	float		mGreen;
	float		mBlue;
	float		mAlpha;
	int			mGfxID;
};

typedef void	( *AKUParticleInitFunc )		( float* particle, float* registers );
typedef void	( *AKUParticleRenderFunc )		( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term );

AKU_API void	AKUNewParticlePlugin			( lua_State* L, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size );
AKU_API void	AKUSetParticlePreset			( const char* presetTable, const char* presetName, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size );

#endif
