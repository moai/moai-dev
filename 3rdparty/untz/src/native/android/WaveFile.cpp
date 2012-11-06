//
//  WaveFile.cpp
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "WaveFile.h"
#include <assert.h>

WaveFile::WaveFile()
{
}

WaveFile::~WaveFile()
{
}

int WaveFile::open(const char *path)
{
	int status = RiffFile::open(path);
	if(status != 0)
		return status;
	
	char s[5];
	FOURCC2STR(formatCode(), s);
	printf("type = %s\n", s);

	if(formatCode() != STR2FOURCC("WAVE"))
	{
		return 1; // error
	}

	// Jump into the "fmt " chunk
	if(push(STR2FOURCC("fmt ")))
	{
		memset(&mHeader, 0, sizeof(mHeader));

		UInt32 fmtSize = chunkSize();
		if(RiffFile::readData(&mHeader, fmtSize) != fmtSize)
		{
			return 1; // error
		}

		assert(mHeader.formatTag == 1); // only uncompressed files are supported 

		// Some writers put a bad block align and/or avgBytesPerSecond value.
		// Let's recalculate it.
		mHeader.bytesPerFrame = mHeader.bitsPerSample / 8 * mHeader.numChannels;
		mHeader.averageBytesPerSecond = mHeader.bytesPerFrame * mHeader.samplesPerSecond;
	}

	// Jump out of the "fmt " chunk
	pop();

	// Jump into the "data" chunk
	if(!push(STR2FOURCC("data")))
	{
		return 1; // error, couldn't find the data chunk
	}

	return 0; // success
}

WaveHeader WaveFile::getHeader() const
{
	return mHeader;
}

UInt32 WaveFile::getNumberOfFrames()
{
   return chunkSize() / mHeader.bytesPerFrame;
}

void WaveFile::setPosition(UInt32 byteOffset)
{
	fseek(mpFile, dataStart() + byteOffset, SEEK_SET);
}
