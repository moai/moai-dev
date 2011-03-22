// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFTFONTRIPPER_H
#define	MOAIFTFONTRIPPER_H
#if USE_FREETYPE

//================================================================//
// MOAIFtFontRipper
//================================================================//
namespace MOAIFtFontRipper {

	void	RipFromTTF		( cc8* filename, USFont& font, USImage& image, cc8* chars, float points, u32 dpi );
};

#endif
#endif
