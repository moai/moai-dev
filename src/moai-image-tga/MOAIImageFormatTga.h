// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATTGA_H
#define	MOAIIMAGEFORMATTGA_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatPng
//================================================================//
class MOAIImageFormatTga :
	public ZLImageFormat {
private:

public:

	//----------------------------------------------------------------//
	size_t			GetHeaderGuessSize			();
	bool			GuessHeader					( const void* buffer );
					MOAIImageFormatTga			();
					~MOAIImageFormatTga			();
	bool			ReadImage					( ZLImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const ZLImage& image, ZLStream& stream );
};

#endif
