// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATTGA_H
#define	MOAIIMAGEFORMATTGA_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatPng
//================================================================//
class MOAIImageFormatTga :
	public MOAIImageFormat {
private:

public:

	//----------------------------------------------------------------//
	bool			CreateTexture				( MOAISingleTexture& texture, const void* data, size_t size );
	size_t			GetHeaderGuessSize			();
	bool			GuessHeader					( const void* buffer );
					MOAIImageFormatTga			();
					~MOAIImageFormatTga			();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
