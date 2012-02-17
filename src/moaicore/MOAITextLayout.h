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

	friend class MOAITextDesigner;
	friend class MOAITextLayout;
	
	float				mX;
	float				mY;
	u32					mIdx;		// index in source text stream
	u32					mRGBA;
	const MOAIGlyph*	mGlyph;

public:

	//----------------------------------------------------------------//
	bool operator < ( const MOAITextSprite& rhs ) const {
	
		return this->mIdx < rhs.mIdx;
	}
};

//================================================================//
// MOAITextLine
//================================================================//
class MOAITextLine {
private:

	friend class MOAITextDesigner;
	friend class MOAITextLayout;
	
	u32			mStart;		// index in sprite stack
	u32			mSize;		// number of sprites in line;
	USRect		mRect;		// tight bounds of line
	float		mAscent;	// offset to the text baseline

public:

};

//================================================================//
// MOAITextLayout
//================================================================//
class MOAITextLayout {
private:

	friend class MOAITextDesigner;
	
	USLeanStack < MOAITextSprite, 64 >	mSprites;
	USLeanStack < MOAITextLine, 8 >		mLines;
	bool								mMore;

	//----------------------------------------------------------------//
	void		PushGlyph			( const MOAIGlyph* glyph, u32 idx, float x, float y, u32 rgba );
	void		PushLine			( u32 start, u32 size, const USRect& rect, float ascent );

public:

	//----------------------------------------------------------------//
	void		Draw				( u32 reveal );
	void		DrawDebugBaselines	();
	void		DrawDebugLayout		();
	bool		GetBoundsForRange	( u32 idx, u32 size, USRect& rect );
	bool		More				( u32 reveal );
	void		Reset				();
	void		SetColorForRange	( u32 idx, u32 size, u32 rgba );
	u32			Size				();
};

#endif
