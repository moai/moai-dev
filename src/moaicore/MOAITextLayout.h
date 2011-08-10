// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUT_H
#define	MOAITEXTLAYOUT_H

class MOAIGlyph;

//================================================================//
// MOAITextSprite
//================================================================//
class MOAITextSprite {
private:

	friend class MOAITextLayout;
	friend class MOAIFont;
	friend class MOAITextFrame;
	
	float				mX;
	float				mY;
	u32					mIdx;		// index in source text stream
	u32					mRGBA;
	const MOAIGlyph*	mGlyph;
	float				mPoints;

public:

	//----------------------------------------------------------------//
	bool operator < ( const MOAITextSprite& rhs ) const {
	
		return this->mIdx < rhs.mIdx;
	}
};

//================================================================//
// MOAITextLayout
//================================================================//
class MOAITextLayout :
	public USLeanStack < MOAITextSprite, 64 > {
public:

	//----------------------------------------------------------------//
	void		Draw				( u32 reveal );
	bool		GetBoundsForRange	( u32 idx, u32 size, USRect& rect );
	void		PushGlyph			( const MOAIGlyph* glyph, u32 idx, float x, float y, float points, u32 rgba );
	void		SetColorForRange	( u32 idx, u32 size, u32 rgba );
};

#endif
