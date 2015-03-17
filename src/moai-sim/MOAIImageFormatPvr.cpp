// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIImageFormatPvr.h>

#include <PVRTTexture.h>
#include <PVRTDecompress.h>

//================================================================//
// MOAIPvrMipLevelInfo
//================================================================//
class MOAIPvrMipLevelInfo {
public:

	u32		mLevel;
	
	u32		mWidth;
	u32		mHeight;
	
	size_t	mSizeCompressed;
	size_t	mSizeDecompressed;
};

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
	
	u32		mHeaderSize;	// size of the structure
	u32		mHeight;		// height of surface to be created
	u32		mWidth;			// width of input surface
	u32		mMipMapCount;	// number of MIP-map levels requested
	u32		mPFFlags;		// pixel format flags
	u32		mDataSize;		// Size of the compress data
	u32		mBitCount;		// number of bits per pixel
	u32		mRBitMask;		// mask for red bit
	u32		mGBitMask;		// mask for green bits
	u32		mBBitMask;		// mask for blue bits
	u32		mAlphaBitMask;	// mask for alpha channel
	u32		mPVR;			// should be 'P' 'V' 'R' '!'
	u32		mNumSurfs;

	//----------------------------------------------------------------//
	static const void*			GetFileData					( const void* data, size_t size );
	static MOAIPvrHeader*		GetHeader					( const void* data, size_t size );
	MOAIPvrMipLevelInfo			GetMipLevelInfo				( int mipLevel );
	size_t						GetPixelSizeDecompressed	();
	size_t						GetTotalSize				();
	bool						HasAlpha					();
	bool						IsCompressed				();
	bool						IsTwoBit					();
	bool						IsValid						();
	bool						Load						( ZLStream& stream );
								MOAIPvrHeader				();
								~MOAIPvrHeader				();

};

//----------------------------------------------------------------//
const void* MOAIPvrHeader::GetFileData ( const void* data, size_t size ) {

	if ( data && ( size >= HEADER_SIZE )) {
		return ( const void* )(( size_t )data + HEADER_SIZE );
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIPvrHeader* MOAIPvrHeader::GetHeader ( const void* data, size_t size ) {

	if ( data && ( size >= HEADER_SIZE )) {
		MOAIPvrHeader* header = ( MOAIPvrHeader* )data;
		if ( header->mPVR == PVR_FILE_MAGIC ) {
			return header;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIPvrMipLevelInfo MOAIPvrHeader::GetMipLevelInfo ( int mipLevel ) {

	MOAIPvrMipLevelInfo info;
	info.mLevel = mipLevel;
	
	const size_t factor = 1 << mipLevel;

	info.mWidth	= MAX ( this->mWidth / factor, 1 );
	info.mHeight	= MAX ( this->mHeight / factor, 1 );

	info.mSizeDecompressed	= info.mWidth * info.mHeight * this->GetPixelSizeDecompressed ();

	if ( this->IsCompressed ()) {
	
		bool isTwoBit				= this->IsTwoBit ();
		const size_t minTexWidth	= isTwoBit ? PVRTC2_MIN_TEXWIDTH : PVRTC4_MIN_TEXWIDTH;
		const size_t minTexHeight	= isTwoBit ? PVRTC2_MIN_TEXHEIGHT : PVRTC4_MIN_TEXHEIGHT;
		info.mSizeCompressed		= ( MAX ( info.mWidth, minTexWidth ) * MAX ( info.mHeight, minTexHeight ) * this->mBitCount ) / 8; // TODO: this right for two bit?
	}
	else {
		info.mSizeCompressed		= info.mSizeDecompressed;
	}
	return info;
}

//----------------------------------------------------------------//
size_t MOAIPvrHeader::GetPixelSizeDecompressed () {

	switch ( this->mPFFlags & MOAIPvrHeader::PF_MASK ) {
			
		case OGL_A_8:
		case OGL_I_8:
			return 1;
		
		case OGL_RGBA_4444:
		case OGL_RGBA_5551:
		case OGL_RGB_555:
		case OGL_RGB_565:
		case OGL_AI_88:
			return 2;

		case OGL_RGB_888:
			return 3;
		
		case OGL_RGBA_8888:
		case OGL_BGRA_8888:
		case OGL_PVRTC2:
		case OGL_PVRTC4:
			return 4;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t MOAIPvrHeader::GetTotalSize () {

	return HEADER_SIZE + this->mDataSize;
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::HasAlpha () {

	return this->mAlphaBitMask ? true : false;
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::IsCompressed () {

	return (( this->mPFFlags & PF_MASK ) == OGL_PVRTC2 ) || (( this->mPFFlags & PF_MASK ) == OGL_PVRTC4 );
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::IsTwoBit () {

	return (( this->mPFFlags & PVRTEX_PIXELTYPE ) == OGL_PVRTC2 );
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::IsValid () {
	return this->mPVR == PVR_FILE_MAGIC;
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::Load ( ZLStream& stream ) {
	
	assert ( HEADER_SIZE <= sizeof ( MOAIPvrHeader ));
	
	this->mPVR = 0;
	size_t size = stream.PeekBytes ( this, HEADER_SIZE );
	return ( size == HEADER_SIZE ) && this->IsValid ();
}

//----------------------------------------------------------------//
MOAIPvrHeader::MOAIPvrHeader () :
	mPVR ( 0 ) {
}

//----------------------------------------------------------------//
MOAIPvrHeader::~MOAIPvrHeader () {
}

//================================================================//
// MOAIImageFormatPvr
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::CheckHeader ( const void* buffer ) {

	return MOAIPvrHeader::GetHeader ( buffer, MOAIPvrHeader::HEADER_SIZE ) != 0;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {

	if ( !MOAIGfxDevice::Get ().GetHasContext ()) return false;
	MOAIGfxDevice::Get ().ClearErrors ();

	MOAIPvrHeader* header = MOAIPvrHeader::GetHeader ( data, size );
	if ( !header ) return false;
	
	bool isCompressed = header->IsCompressed ();
	
	#if !( ZGL_DEVCAPS_PVR_TEXTURE || MOAI_WITH_LIBPVR )
		if ( isCompressed ) return false;
	#endif
	
	bool hasAlpha = header->HasAlpha ();

	int internalFormat = 0;
	int pixelType = 0;

	switch ( header->mPFFlags & MOAIPvrHeader::PF_MASK ) {
		
		case MOAIPvrHeader::OGL_RGBA_4444:
			internalFormat = ZGL_PIXEL_FORMAT_RGBA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
			break;
	
		case MOAIPvrHeader::OGL_RGBA_5551:
			internalFormat = ZGL_PIXEL_FORMAT_RGBA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
			break;
		
		case MOAIPvrHeader::OGL_RGBA_8888:
			internalFormat = ZGL_PIXEL_FORMAT_RGBA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case MOAIPvrHeader::OGL_RGB_565:
			internalFormat = ZGL_PIXEL_FORMAT_RGB;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
			break;
		
		case MOAIPvrHeader::OGL_RGB_888:
			internalFormat = ZGL_PIXEL_FORMAT_RGB;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case MOAIPvrHeader::OGL_I_8:
			internalFormat = ZGL_PIXEL_FORMAT_LUMINANCE;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case MOAIPvrHeader::OGL_AI_88:
			internalFormat = ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		#if ZGL_DEVCAPS_PVR_TEXTURE
		
			case MOAIPvrHeader::OGL_PVRTC2:
				internalFormat = hasAlpha ? ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;
			
			case MOAIPvrHeader::OGL_PVRTC4:
				internalFormat = hasAlpha ? ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;
		
		#else
		
			case MOAIPvrHeader::OGL_PVRTC2:
			case MOAIPvrHeader::OGL_PVRTC4:
				internalFormat = ZGL_PIXEL_FORMAT_RGBA;
				pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
				break;
		
		#endif
		
		case MOAIPvrHeader::OGL_BGRA_8888:
			internalFormat = ZGL_PIXEL_FORMAT_BGRA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case MOAIPvrHeader::OGL_A_8:
			internalFormat = ZGL_PIXEL_FORMAT_ALPHA;
			pixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		// TODO: more formats? more platforms?
		//case MOAIPvrHeader::OGL_RGB_555:
		
		default:
			return false;
	}

	u32 glTexID = zglCreateTexture ();
	if ( glTexID ) {

		zglBindTexture ( glTexID );
		
		size_t textureSize = 0;
		
		const void* imageData = header->GetFileData ( data, size );
		
		int nLevels = header->mMipMapCount + 1;
		for ( int level = 0; level < nLevels; ++level ) {
		
			MOAIPvrMipLevelInfo info = header->GetMipLevelInfo ( level );
		
			int width = info.mWidth;
			int height = info.mHeight;
		
			if ( isCompressed ) {
			
				#if ZGL_DEVCAPS_PVR_TEXTURE
				
					zglCompressedTexImage2D ( level, internalFormat, width, height, info.mSizeCompressed, imageData );
				
				#elif MOAI_WITH_LIBPVR
				
					ZLLeanArray < char > buffer;
					buffer.Init ( info.mSizeDecompressed );
					if ( !this->Decompress ( *header, info, buffer.Data (), info.mSizeDecompressed, imageData, info.mSizeCompressed )) {
						this->CleanupTexture ( texture );
						return false;
					}
					zglTexImage2D ( level, internalFormat, width, height, internalFormat, pixelType, buffer.Data ());
				
				#endif
			}
			else {
				zglTexImage2D ( level, internalFormat, width, height, internalFormat, pixelType, imageData );
			}
			
			if ( MOAIGfxDevice::Get ().LogErrors ()) {
				this->CleanupTexture ( texture );
				return false;
			}
			
			imageData = ( const void* )(( size_t )imageData + info.mSizeCompressed );
			textureSize += info.mSizeCompressed;
		}

		this->SetTextureID ( texture, glTexID, internalFormat, pixelType, textureSize );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::Decompress ( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, MOAIImage& image, ZLStream& stream ) {

	bool isCompressed = header.IsCompressed ();
	
	this->SetDimensions ( image, info.mWidth, info.mHeight, 0 );

	switch ( header.mPFFlags & PVRTEX_PIXELTYPE ) {
		
		case OGL_A_8:
			this->SetColorFormat ( image, ZLColor::A_8 );
			break;
		
		case OGL_RGBA_4444:
			this->SetColorFormat ( image, ZLColor::RGBA_4444 );
			break;
		
		case OGL_RGBA_5551:
			this->SetColorFormat ( image, ZLColor::RGBA_5551 );
			break;
		
		case OGL_RGB_565:
			this->SetColorFormat ( image, ZLColor::RGB_565 );
			break;
		
		case OGL_RGB_888:
			this->SetColorFormat ( image, ZLColor::RGB_888 );
			break;
		
		case OGL_PVRTC2:
		case OGL_PVRTC4:
		case OGL_RGBA_8888:
			this->SetColorFormat ( image, ZLColor::RGBA_8888 );
			break;
		
		// TODO: Support more formats
		//case OGL_RGB_555:
		//case OGL_I_8:
		//case OGL_AI_88:
		//case OGL_BGRA_8888:
		
		default:
			ZLLog::LogF ( ZLLog::CONSOLE, "Unsupported texture format\n" ); // TODO: Support more formats
			return false;
	}

	this->SetPixelFormat ( image, MOAIImage::TRUECOLOR );
	this->Alloc ( image );
	
	if ( !MOAIImageFormatPvr::Decompress ( header, info, this->GetBitmap ( image ), image.GetBitmapSize (), stream )) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Error loading or decompressing PVR at mip level %d\n", info.mLevel );
		return false;
	}

	return true;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::Decompress ( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, void* buffer, size_t bufferSize, ZLStream& stream ) {

	ZLLeanArray < char > srcBufferArray;
	void* srcBuffer = buffer;
	
	bool isCompressed = header.IsCompressed ();
	
	if ( isCompressed ) {
		srcBufferArray.Init ( info.mSizeCompressed );
		srcBuffer = srcBufferArray.Data ();
	}

	if ( stream.ReadBytes ( srcBuffer, info.mSizeCompressed ) != ( size_t )info.mSizeCompressed ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "End of file, pixel data incomplete\n" );
		return false;
	}
	
	return isCompressed ? MOAIImageFormatPvr::Decompress ( header, info, buffer, bufferSize, srcBuffer, info.mSizeCompressed ) : true;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::Decompress ( MOAIPvrHeader& header, const MOAIPvrMipLevelInfo& info, void* buffer, size_t bufferSize, const void* srcBuffer, size_t srcBufferSize ) {

	bool isCompressed = header.IsCompressed ();

	if ( srcBufferSize < info.mSizeCompressed ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Source buffer not large enough to hold compressed bitmap\n" );
		return false;
	}

	if ( bufferSize < info.mSizeDecompressed ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Buffer not large enough to hold decompressed bitmap\n" );
		return false;
	}

	if ( isCompressed ) {
	
		int resultSize = 0;
		
		#if MOAI_WITH_LIBPVR
			resultSize = PVRTDecompressPVRTC ( srcBuffer, header.IsTwoBit (), info.mWidth, info.mHeight, ( unsigned char* )buffer );
		#endif
	
		if ( resultSize != info.mSizeCompressed ) {
			ZLLog::LogF ( ZLLog::CONSOLE, "Error decompressing PVR at mip level %d\n", info.mLevel );
			return false;
		}
	}
	else {
		memcpy ( buffer, srcBuffer, info.mSizeDecompressed );
	}
	return true;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatPvr::GetHeaderSize () {

	return MOAIPvrHeader::HEADER_SIZE;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::GetTextureInfo ( ZLStream& stream, MOAITextureInfo& info ) {
	
	MOAIPvrHeader header;
	if ( header.Load ( stream )) {
	
		#if !( ZGL_DEVCAPS_PVR_TEXTURE || MOAI_WITH_LIBPVR )
			if ( header.IsCompressed ()) return false;
		#endif
	
		info.mWidth		= header.mWidth;
		info.mHeight	= header.mHeight;
		info.mSize		= header.mHeaderSize + header.mDataSize;
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
MOAIImageFormatPvr::MOAIImageFormatPvr () {
}

//----------------------------------------------------------------//
MOAIImageFormatPvr::~MOAIImageFormatPvr () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {

	// Read the file header
	MOAIPvrHeader header;
	stream.ReadBytes ( &header, MOAIPvrHeader::HEADER_SIZE );
	
	// Perform checks for old PVR psPVRHeader
	if ( header.mHeaderSize != sizeof ( MOAIPvrHeader )) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Bad PVR file\n" );
		return false;
	}

	#if !MOAI_WITH_LIBPVR
		if ( header.IsCompressed ()) {
			ZLLog::LogF ( ZLLog::CONSOLE, "Software PVR decompression not supported; build Moai SDK using MOAI_WITH_LIBPVR\n" );
			return false;
		}
	#endif

	MOAIPvrMipLevelInfo info = header.GetMipLevelInfo ( 1 );
	
	if ( this->Decompress ( header, info, image, stream )) {
		image.Transform ( transform );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormatPvr::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	
	return false;
}
