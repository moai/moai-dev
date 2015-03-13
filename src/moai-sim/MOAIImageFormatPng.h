// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATPNG_H
#define	MOAIIMAGEFORMATPNG_H

#if MOAI_WITH_LIBPNG

#include <moai-sim/MOAIImageFormat.h>

//================================================================//
// MOAIImageFormatPng
//================================================================//
class MOAIImageFormatPng :
	public MOAIImageFormat {
private:

	static const u32 HEADER_SIZE = 8;

	//----------------------------------------------------------------//
	void			ReadImagePng				( MOAIImage& image, void* pngParam, void* pngInfoParam, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	bool			CreateTexture				( MOAITextureBase& texture, const void* data, size_t size );
	size_t			GetHeaderSize				();
					MOAIImageFormatPng			();
					~MOAIImageFormatPng			();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
#endif
