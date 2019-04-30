// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATWEBP_H
#define	MOAIIMAGEFORMATWEBP_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatWebP
//================================================================//
class MOAIImageFormatWebP :
	public ZLImageFormat {
private:

	static const u32 HEADER_SIZE = 12;

	//----------------------------------------------------------------//
	void			ReadImageWebP				( ZLImage& image, u8 const* data, size_t dataSize, int width, int height, bool hasAlpha, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	size_t			GetHeaderSize				();
					MOAIImageFormatWebP			();
					~MOAIImageFormatWebP		();
	bool			ReadImage					( ZLImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const ZLImage& image, ZLStream& stream );
};

#endif
