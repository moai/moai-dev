// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_PVR

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIImageFormatPvr.h>

//================================================================//
// MOAIPvrHeader
//================================================================//
class MOAIPvrHeader {
public:
	
	static const u32 HEADER_SIZE		= 52;
	static const u32 PVR_FILE_MAGIC		= 0x21525650; // 'P' 'V' 'R' '!'
	static const u32 PF_MASK			= 0xff;
	
	enum {
		OGL_RGBA_4444		= 0x10,
		OGL_RGBA_5551,
		OGL_RGBA_8888,
		OGL_RGB_565,
		OGL_RGB_555,
		OGL_RGB_888,
		OGL_I_8,
		OGL_AI_88,
		OGL_PVRTC2,
		OGL_PVRTC4,
		OGL_BGRA_8888,
		OGL_A_8,
	};
	
	u32 mHeaderSize;	// size of the structure
	u32 mHeight;		// height of surface to be created
	u32 mWidth;			// width of input surface
	u32 mMipMapCount;	// number of MIP-map levels requested
	u32 mPFFlags;		// pixel format flags
	u32 mDataSize;		// Size of the compress data
	u32 mBitCount;		// number of bits per pixel
	u32 mRBitMask;		// mask for red bit
	u32 mGBitMask;		// mask for green bits
	u32 mBBitMask;		// mask for blue bits
	u32 mAlphaBitMask;	// mask for alpha channel
	u32 mPVR;			// should be 'P' 'V' 'R' '!'
	u32 mNumSurfs;

	////----------------------------------------------------------------//
	static const void* GetFileData ( const void* data, size_t size ) {
	
		if ( data && ( size >= HEADER_SIZE )) {
			return ( const void* )(( size_t )data + HEADER_SIZE );
		}
		return 0;
	}

	//----------------------------------------------------------------//
	size_t GetTotalSize () {
	
		return HEADER_SIZE + this->mDataSize;
	}

	//----------------------------------------------------------------//
	bool IsValid () {
		return this->mPVR == PVR_FILE_MAGIC;
	}

	//----------------------------------------------------------------//
	bool Load ( ZLStream& stream ) {
		
		assert ( HEADER_SIZE <= sizeof ( MOAIPvrHeader ));
		
		this->mPVR = 0;
		size_t size = stream.PeekBytes ( this, HEADER_SIZE );
		return ( size == HEADER_SIZE ) && this->IsValid ();
	}

	//----------------------------------------------------------------//
	static MOAIPvrHeader* GetHeader ( const void* data, size_t size ) {
	
		if ( data && ( size >= HEADER_SIZE )) {
			MOAIPvrHeader* header = ( MOAIPvrHeader* )data;
			if ( header->mPVR == PVR_FILE_MAGIC ) {
				return header;
			}
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	MOAIPvrHeader () {
		this->mPVR = 0;
	}
};

//================================================================//
// MOAIImageFormatPvr
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::CheckHeader ( const void* buffer ) {

	return MOAIPvrHeader::GetHeader ( buffer, MOAIPvrHeader::HEADER_SIZE ) != 0;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );

	#if ZGL_DEVCAPS_PVR_TEXTURE

		if ( !MOAIGfxDevice::Get ().GetHasContext ()) return false;
		MOAIGfxDevice::Get ().ClearErrors ();

		MOAIPvrHeader* header = MOAIPvrHeader::GetHeader ( data, size );
		if ( !header ) return false;
		
		bool compressed = false;
		bool hasAlpha = header->mAlphaBitMask ? true : false;
	
		int internalFormat = 0;
		int pixelType;
	
		switch ( header->mPFFlags & MOAIPvrHeader::PF_MASK ) {
			
			case MOAIPvrHeader::OGL_RGBA_4444:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_RGBA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
				break;
		
			case MOAIPvrHeader::OGL_RGBA_5551:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_RGBA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
				break;
			
			case MOAIPvrHeader::OGL_RGBA_8888:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_RGBA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_RGB_565:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_RGB;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
				break;
			
			// NO IMAGE FOR THIS
			case MOAIPvrHeader::OGL_RGB_555:
				return false;
				break;
			
			case MOAIPvrHeader::OGL_RGB_888:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_RGB;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_I_8:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_LUMINANCE;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_AI_88:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_PVRTC2:
				compressed = true;
				internalFormat = hasAlpha ?
					ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG :
					ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;
			
			case MOAIPvrHeader::OGL_PVRTC4:
				compressed = true;
				internalFormat = hasAlpha ?
					ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG :
					ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;
			
			case MOAIPvrHeader::OGL_BGRA_8888:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_BGRA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_A_8:
				compressed = false;
				internalFormat = ZGL_PIXEL_FORMAT_ALPHA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
		}
	
		u32 glTexID = zglCreateTexture ();
		size_t textureSize = 0;
	
		if ( glTexID ) {

			zglBindTexture ( glTexID );
			
			textureSize = 0;
			
			int width = header->mWidth;
			int height = header->mHeight;
			char* imageData = (char*)(header->GetFileData ( data, size));
			if ( header->mMipMapCount == 0 ) {
				
				u32 currentSize = std::max ( 32u, width * height * header->mBitCount / 8u );
				textureSize += currentSize;
				
				if ( compressed ) {
					zglCompressedTexImage2D ( 0, internalFormat, width, height, currentSize, imageData );
				}
				else {
					zglTexImage2D ( 0, internalFormat, width, height, internalFormat, pixelType, imageData );
				}
				
				if ( zglGetError () != ZGL_ERROR_NONE ) {
					this->CleanupTexture ( texture );
					return false;
				}
			}
			else {
				for ( int level = 0; width > 0 && height > 0; ++level ) {
					u32 currentSize = std::max ( 32u, width * height * header->mBitCount / 8u );
				
					if ( compressed ) {
						zglCompressedTexImage2D ( level, internalFormat, width, height, currentSize, imageData );
					}
					else {
						zglTexImage2D( level, internalFormat, width, height, internalFormat, pixelType, imageData );
					}
					
					if ( zglGetError () != ZGL_ERROR_NONE ) {
						this->CleanupTexture ( texture );
						return false;
					}
					
					imageData += currentSize;
					textureSize += currentSize;
					
					width >>= 1;
					height >>= 1;
				}	
			}
	
			this->SetTextureID ( texture, glTexID, internalFormat, pixelType, textureSize );
			return true;
		}

	#endif
	
	return false;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatPvr::GetHeaderSize () {

	return MOAIPvrHeader::HEADER_SIZE;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::GetTextureInfo ( ZLStream& stream, MOAITextureInfo& info ) {
	
	MOAIPvrHeader header;
	if ( header.Load ( stream )) {
		info.mWidth = header.mWidth;
		info.mHeight = header.mHeight;
		info.mSize = header.mHeaderSize + header.mDataSize;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::IsTextureFormat () {

	#if ZGL_DEVCAPS_PVR_TEXTURE // TODO: MOAI_WITH_PVR_TEXTURE
		return true;
	#else
		return false;
	#endif
}

//----------------------------------------------------------------//
MOAIImageFormatPvr::MOAIImageFormatPvr () {
}

//----------------------------------------------------------------//
MOAIImageFormatPvr::~MOAIImageFormatPvr () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {
	UNUSED ( image );
	UNUSED ( stream );
	UNUSED ( transform );
	
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	
	return false;
}

#endif
