// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATPVR_H
#define	MOAIIMAGEFORMATPVR_H

#include <moai-sim/MOAIImageFormat.h>

class MOAIPvrHeader;
class MOAIPvrMipLevelInfo;

//================================================================//
// MOAIImageFormatPvr
//================================================================//
class MOAIImageFormatPvr :
	public MOAIImageFormat {
private:

	//----------------------------------------------------------------//
	bool			Decompress					( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, MOAIImage& image, ZLStream& stream );
	static bool		Decompress					( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, void* buffer, size_t bufferSize, ZLStream& stream );
	static bool		Decompress					( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, void* buffer, size_t bufferSize, const void* srcBuffer, size_t srcBufferSize );

public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	bool			CreateTexture				( MOAITextureBase& texture, const void* data, size_t size );
	size_t			GetHeaderSize				();
	bool			GetTextureInfo				( ZLStream& stream, MOAITextureInfo& info );
					MOAIImageFormatPvr			();
					~MOAIImageFormatPvr			();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
