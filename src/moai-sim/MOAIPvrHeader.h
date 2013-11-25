// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPVRHEADER_H
#define MOAIPVRHEADER_H

#include <moai-sim/MOAITextureHeader.h>

//================================================================//
// MOAIPvrHeader
//================================================================//
class MOAIPvrHeader :
	public MOAITextureHeader {
private:

	friend class MOAITextureBase;
	
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

public:

	//----------------------------------------------------------------//
	virtual u32				GetDataSize				();
	virtual u32				GetHeaderSize			();
	virtual void*			GetHeaderStart			();
	virtual u32				GetHeight				();
	virtual u32				GetWidth				();
	virtual bool			IsValid					();
	virtual void			Invalidate				();
							MOAIPvrHeader			();
	virtual					~MOAIPvrHeader			();
};

#endif
