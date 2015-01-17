// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATWEBP_H
#define	MOAIIMAGEFORMATWEBP_H

#if MOAI_WITH_LIBWEBP

#include <moai-sim/MOAIImageFormat.h>

//================================================================//
// MOAIImageFormatWebP
//================================================================//
class MOAIImageFormatWebP :
	public MOAIImageFormat {
private:

	static const u32 HEADER_SIZE = 12;

	//----------------------------------------------------------------//
	void			ReadImageWebP				( MOAIImage& image, u8 const* data, size_t dataSize, int width, int height, bool hasAlpha, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	bool			CreateTexture				( MOAITextureBase& texture, const void* data, size_t size );
	size_t			GetHeaderSize				();
					MOAIImageFormatWebP			();
					~MOAIImageFormatWebP		();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
#endif
