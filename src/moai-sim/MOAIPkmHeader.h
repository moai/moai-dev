// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPKMHEADER_H
#define MOAIPKMHEADER_H

#include <moai-sim/MOAITextureHeader.h>

//================================================================//
// MOAIPkmHeader
//================================================================//
class MOAIPkmHeader :
	public MOAITextureHeader {
public:

	enum {
		ETC1_RGB_NO_MIPMAPS = 0x0000,
	};

private:

	static const u32 HEADER_SIZE		= 16;
	static const u32 PKM_FILE_MAGIC		= 0x204d4b50;
	static const u16 PKM_VERSION_1		= 0x3031;
	
	u32 mPKM;				// should be 'P' 'K' 'M' ' '
	u16 mVersion;			// should be '1' '0'
	u8 mType [ 2 ];			// should be ETC1_RGB_NO_MIPMAPS, 2-byte big endian
	u8 mDataWidth [ 2 ];	// pixel width of image data (multiple of 4), 2-byte big endian
	u8 mDataHeight [ 2 ];	// pixel height of image data (multiple of 4), 2-byte big endian
	u8 mWidth [ 2 ];		// image pixel width (can be less due to padding), 2-byte big endian
	u8 mHeight [ 2 ];		// image pixel height (can be less due to padding), 2-byte big endian

public:

	//----------------------------------------------------------------//
	virtual u32				GetDataSize				();
	virtual u32				GetHeaderSize			();
	virtual void*			GetHeaderStart			();
	virtual u32				GetHeight				();
	u16						GetType					();
	virtual u32				GetWidth				();
	virtual bool			IsValid					();
	virtual void			Invalidate				();
							MOAIPkmHeader			();
	virtual					~MOAIPkmHeader			();
};

#endif
