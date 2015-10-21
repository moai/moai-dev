// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATJPG_H
#define	MOAIIMAGEFORMATJPG_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatJpg
//================================================================//
class MOAIImageFormatJpg :
	public MOAIImageFormat {
private:

	static const u32 HEADER_SIZE = 8;

	//----------------------------------------------------------------//
	void			ReadImageJpg				( MOAIImage& image, void* jpgInfoParam, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	bool			CreateTexture				( MOAISingleTexture& texture, const void* data, size_t size );
	size_t			GetHeaderSize				();
					MOAIImageFormatJpg			();
					~MOAIImageFormatJpg			();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
