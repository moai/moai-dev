// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATJPG_H
#define	MOAIIMAGEFORMATJPG_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIImageFormatJpg
//================================================================//
class MOAIImageFormatJpg :
	public ZLImageFormat {
private:

	static const u32 HEADER_SIZE = 8;

	//----------------------------------------------------------------//
	void			ReadImageJpg				( ZLImage& image, void* jpgInfoParam, u32 transform );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	size_t			GetHeaderSize				();
					MOAIImageFormatJpg			();
					~MOAIImageFormatJpg			();
	bool			ReadImage					( ZLImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const ZLImage& image, ZLStream& stream );
};

#endif
