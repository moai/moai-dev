// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFREETYPEFONTRIPPER_H
#define	MOAIFREETYPEFONTRIPPER_H
#if USE_FREETYPE

#include <moaicore/MOAIFont.h>

class MOAIImage;

//================================================================//
// MOAIFreetypeFontRipper
//================================================================//
namespace MOAIFreetypeFontRipper {

	void	RipFromTTF		( cc8* filename, MOAIFont& font, MOAIImage& image, cc8* chars, float points, u32 dpi );
};

#endif
#endif
