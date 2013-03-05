//
//  WaveFile.h
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef WAVEFILE_H_
#define WAVEFILE_H_

#include "RiffFile.h"


enum 
{
	R_WAVEFORMAT_UNKNOWN = 0x00,
	R_WAVEFORMAT_PCM = 0x01,
	R_WAVEFORMAT_IEEE = 0x03,
	R_WAVEFORMAT_ALAW = 0x06,
	R_WAVEFORMAT_MULAW = 0x07,
	R_WAVEFORMAT_EXTENSIBLE = 0xFE
};

typedef struct
{
	UInt16 formatTag;
	UInt16 numChannels;
	UInt32 samplesPerSecond;
	UInt32 averageBytesPerSecond;
	UInt16 bytesPerFrame;
	UInt16 bitsPerSample;
	// -- 16 bytes
	UInt16 cbSize;  // size of the extension (0 or 22)
	// -- 18 bytes
	UInt16 validBitsPerSample;
	UInt32 channelMask;
	UInt32 subFormat;
	// -- 40 bytes
} WaveHeader;

class WaveFile : public RiffFile
{
public:
   WaveFile();
   ~WaveFile();

   // RiffFile
   virtual int open(const char* path);

   // WaveFile
   WaveHeader getHeader() const;
   UInt32 getNumberOfFrames();
   void setPosition(UInt32 byteOffset);

protected:
   WaveHeader mHeader;
};

#endif // WAVEFILE_H_