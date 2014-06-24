//
//  BltAudioSource.h
//  BlueTune codec
//
//  Created by Sylvain Rebaud (srebaud@plumzi.com) on 01/09/2014.
//  Copyright (c) 2012-2014 Plumzi, Inc. All Rights Reserved.
//

#ifndef BLTAUDIOSOURCE_H_
#define BLTAUDIOSOURCE_H_

#include "BufferedAudioSource.h"
#include "BlueTune.h"
#include <moaicore/moaicore.h>
#include <threading/Threading.h>
#include <vector>
#include <cstdio>
#include <iostream>

class BltAudioSource : public BufferedAudioSource
{
public:
	BltAudioSource();
	~BltAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
	UInt32 getBitsPerSample();

	// BufferedAudioSource
	virtual bool  init(const RString& path, bool loadIntoMemory);
    virtual void  close();
	virtual Int64 decodeData(float* buffer, UInt32 size, int &version);
	virtual void  setDecoderPosition(Int64 startFrame);

private:
	void setDecoderPosition(double position);

	RCriticalSection mDecodeLock;
	RString mPath;
    BLT_Decoder* mDecoder;
    BLT_MediaPacket* mLastPacket;

	BLT_UInt32    mSampleRate;
    BLT_UInt16    mChannelCount;
    BLT_UInt8     mBitsPerSample;
	double		  mLength;
};

#endif