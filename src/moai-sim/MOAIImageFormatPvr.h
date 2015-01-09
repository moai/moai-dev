// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATPVR_H
#define	MOAIIMAGEFORMATPVR_H

#if MOAI_WITH_PVR

#include <moai-sim/MOAIImageFormat.h>

//================================================================//
// MOAIImageFormatPvr
//================================================================//
class MOAIImageFormatPvr :
	public MOAIImageFormat {
private:
public:

	//----------------------------------------------------------------//
	bool			CheckHeader					( const void* buffer );
	bool			CreateTexture				( MOAITextureBase& texture, const void* data, size_t size );
	size_t			GetHeaderSize				();
	bool			GetTextureInfo				( ZLStream& stream, MOAITextureInfo& info );
	bool			IsTextureFormat				();
					MOAIImageFormatPvr			();
					~MOAIImageFormatPvr			();
	bool			ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	bool			WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
#endif
