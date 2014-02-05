// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREDATA_H
#define	MOAITEXTUREDATA_H

#include <moai-sim/MOAIInvalidHeader.h>
#include <moai-sim/MOAIPkmHeader.h>
#include <moai-sim/MOAIPvrHeader.h>

//================================================================//
// MOAITextureData
//================================================================//
/**	@name	MOAITextureData
	@text	A container for raw (e.g. PVR-compressed) texture data.
*/
class MOAITextureData {
public:

	enum {
		FORMAT_INVALID,
		FORMAT_PVR,
		FORMAT_PKM,
		TOTAL_FORMATS,
	};

protected:

	ZLLeanArray < u8 >	mData;
	u32					mFormat;
	MOAITextureHeader*	mHeaders [ TOTAL_FORMATS ];
	MOAIInvalidHeader	mInvalidHeader;
	MOAIPkmHeader		mPkmHeader;
	MOAIPvrHeader		mPvrHeader;
	
public:

	GET ( u32, Format, mFormat );
	
	//----------------------------------------------------------------//
	void			Clear					();
	u8*				GetData					();
	u32				GetHeight				();
	MOAIPkmHeader*	GetPkmHeader			();
	MOAIPvrHeader*	GetPvrHeader			();
	u32				GetWidth				();
	bool			IsValid					();
					MOAITextureData			();
					~MOAITextureData		();
	void			Init					( ZLStream& stream );
};

#endif
