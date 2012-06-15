// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTREADER_H
#define	MOAIFONTREADER_H

#include <moaicore/MOAILua.h>

class MOAIFont;
class MOAIGlyph;
class MOAIGlyphSet;
class MOAIKernVec;

//================================================================//
// MOAIFontReader
//================================================================//
class MOAIFontReader :
	public MOAILuaObject {
public:

	//----------------------------------------------------------------//
	virtual void		CloseFont				() = 0;
	virtual void		GetFaceMetrics			( MOAIGlyphSet& glyphSet ) = 0;
	virtual bool		GetKernVec				( MOAIGlyph& glyph0, MOAIGlyph& glyph1, MOAIKernVec& kernVec ) = 0;
	virtual bool		HasKerning				() = 0;
						MOAIFontReader			();
						~MOAIFontReader			();
	virtual void		OpenFont				( MOAIFont& font ) = 0;
	virtual void		RenderGlyph				( MOAIFont& font, MOAIGlyph& glyph ) = 0;
	virtual void		SetFaceSize				( float size ) = 0;
};

#endif
