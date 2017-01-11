// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHITMASK_H
#define	MOAIHITMASK_H

//================================================================//
// MOAIMaterial
//================================================================//
class MOAIHitMask :
	public MOAIImage {
private:

	u32		mHitColorScalar;
	u32		mHitColorThreshold;

public:

	//----------------------------------------------------------------//
	bool		LoadGfxState			( MOAIMaterial* fallback, u32 defaultShader );
				MOAIHitMask				();
				~MOAIHitMask			();

	void		SetHitMaskScalar		( u32 idx, u32 scalar );
	void		SetHitMaskScalar		( MOAILuaState& state, u32 idx );
	void		SetHitMaskThreshold		( u32 idx, u32 threshold );
	void		SetHitMaskThreshold		( MOAILuaState& state, u32 idx );
	
	bool		TestHit					( float x, float y ); // in local (uv) space of the mask
	bool		TestHit					( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
};

#endif
