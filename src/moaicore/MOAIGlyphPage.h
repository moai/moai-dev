// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHPAGE_H
#define	MOAIGLYPHPAGE_H

#include <moaicore/MOAIImage.h>
#include <moaicore/MOAISpanList.h>

class MOAIGlyph;

//================================================================//
// MOAIGlyphPage
//================================================================//
class MOAIGlyphPage {
private:

	friend class MOAIFont;

	typedef MOAISpanList < MOAIGlyph* > GlyphList;
	typedef MOAISpanList < MOAIGlyph* >::Span GlyphSpan;
	
	typedef MOAISpanList < GlyphList > RowList;
	typedef MOAISpanList < GlyphList >::Span RowSpan;

	RowList mRows;

	MOAIImage mImage;
	
	MOAIGlyphPage* mNext;

	float mThreshold;

	//----------------------------------------------------------------//
	void			AffirmCanvas				();
	GlyphSpan*		Alloc						( MOAIGlyph& glyph );
	RowSpan*		AllocRow					( u32 height );
	bool			ExpandToNextPowerofTwo		();

public:
	
	//----------------------------------------------------------------//
					MOAIGlyphPage				();
					~MOAIGlyphPage				();
};

#endif
