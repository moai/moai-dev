// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATPNG_H
#define	MOAIIMAGEFORMATPNG_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatPng
//================================================================//
class MOAIImageFormatPng :
	public ZLImageFormat {
private:

	static const u32 HEADER_SIZE = 8;

	//----------------------------------------------------------------//
	void			ReadImagePng				( ZLImage& image, void* pngParam, void* pngInfoParam, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	size_t			GetHeaderSize				();
					MOAIImageFormatPng			();
					~MOAIImageFormatPng			();
	bool			ReadImage					( ZLImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const ZLImage& image, ZLStream& stream );
};

#endif
