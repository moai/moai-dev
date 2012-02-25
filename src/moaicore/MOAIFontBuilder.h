// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTBUILDER_H
#define	MOAIFONTBUILDER_H

#include <moaicore/MOAILua.h>

class MOAIFont;

//================================================================//
// MOAIFontBuilder
//================================================================//
class MOAIFontBuilder :
	public MOAILuaObject {
public:

	//----------------------------------------------------------------//
						MOAIFontBuilder			();
						~MOAIFontBuilder		();
	virtual void		ProcessGlyphs			( MOAIFont& font ) = 0;
	virtual void		RebuildKerning			( MOAIFont& font ) = 0;
	virtual void		RebuildKerning			( MOAIFont& font, float points ) = 0;
};

#endif
