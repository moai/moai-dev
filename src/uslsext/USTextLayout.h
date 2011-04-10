// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USTEXTLAYOUT_H
#define	USTEXTLAYOUT_H

class USGlyph;

//================================================================//
// USTextSprite
//================================================================//
class USTextSprite {
private:

	friend class USTextLayout;
	friend class USFont;
	friend class USTextFrame;
	
	float			mX;
	float			mY;
	u32				mIdx;		// index in source text stream
	u32				mRGBA;
	const USGlyph*	mGlyph;
	float			mPoints;

public:

	//----------------------------------------------------------------//
	bool operator < ( const USTextSprite& rhs ) const {
	
		return this->mIdx < rhs.mIdx;
	}
};

//================================================================//
// USTextLayout
//================================================================//
class USTextLayout :
	public USLeanStack < USTextSprite, 64 > {
public:

	//----------------------------------------------------------------//
	void		Draw				( u32 reveal );
	bool		GetBoundsForRange	( u32 idx, u32 size, USRect& rect );
	void		PushGlyph			( const USGlyph* glyph, u32 idx, float x, float y, float points, u32 rgba );
};

#endif
