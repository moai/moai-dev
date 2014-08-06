//
//  ExtAudioFileAudioSource.h
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "ExtAudioFileAudioSource.h"
#include <CoreFoundation/CFURL.h>

void printCode(const char *tag, OSStatus s)
{
    char str[7];
    *(UInt32 *)(str + 1) = CFSwapInt32HostToBig(s);
    if (isprint(str[1]) && isprint(str[2]) && isprint(str[3]) && isprint(str[4])) {
        str[0] = str[5] = '\'';
        str[6] = '\0';
    } else
        // no, format it as an integer
        sprintf(str, "%d", (int)s);
    fprintf(stderr, "%s%s\n", tag, str);
	//RPRINT("%s%s\n", tag, str);
}

ExtAudioFileAudioSource::ExtAudioFileAudioSource()
{
	mpBufferList = 0;
    mAudioFile = 0;
}

ExtAudioFileAudioSource::~ExtAudioFileAudioSource()
{
	close();
}

void ExtAudioFileAudioSource::doneDecoding()
{
	RPRINT("freeing decoder memory.\n");
	
	mReadBuffer.clear();
	std::vector<float>().swap(mReadBuffer);
}

double ExtAudioFileAudioSource::getLength() 
{ 
    return (double)mTotalFrames / mClientFormat.mSampleRate;
}

double ExtAudioFileAudioSource::getSampleRate() 
{
    return UNTZ::System::get()->getSampleRate();
}

UInt32 ExtAudioFileAudioSource::getNumChannels()
{
    return mClientFormat.mChannelsPerFrame;
}

bool ExtAudioFileAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;
	
    // FIXME: query the file to find out how many channels instead of hardcoding.
    
    CFURLRef path_url = CFURLCreateFromFileSystemRepresentation (kCFAllocatorDefault,
                                                                 (const UInt8*)path.data(),
                                                                 path.length(),
                                                                 false
                                                                 );
    
    OSStatus err = ExtAudioFileOpenURL(path_url, &mAudioFile);
    if(err)
    {
        printCode("ExtAudioFileOpenURL: ", err);        
        return false;
    }
    
    UInt32 propSize = sizeof(mClientFormat);
    err = ExtAudioFileGetProperty(mAudioFile, kExtAudioFileProperty_FileDataFormat, &propSize, &mClientFormat);    

    propSize = sizeof(mTotalFrames);
    err = ExtAudioFileGetProperty(mAudioFile, kExtAudioFileProperty_FileLengthFrames, &propSize, &mTotalFrames);    
    
    // Setup output format
    FillOutASBDForLPCM(mFormat, getSampleRate(), getNumChannels(), 32, 32, true, false,true);
    
    err = ExtAudioFileSetProperty(mAudioFile, kExtAudioFileProperty_ClientDataFormat, sizeof(mFormat), &mFormat);
    if(err)
    {
        printCode("ExtAudioFileSetProperty: ", err);
        return false;
    }
    
    // Allocate our buffer list with NUM_CHANNELS buffers in it.
    mpBufferList = (AudioBufferList *)malloc(sizeof(AudioBufferList) + (getNumChannels() - 1) * sizeof(AudioBuffer));
    mpBufferList->mNumberBuffers = getNumChannels();    

    return BufferedAudioSource::init(path, loadIntoMemory);
}

void ExtAudioFileAudioSource::close()
{
	RPRINT("closing...\n");
    BufferedAudioSource::close();
    
    if(mAudioFile)
    {
        ExtAudioFileDispose(mAudioFile);
        mAudioFile = 0;
    }

	if(mpBufferList)
    {
	    free (mpBufferList);
        mpBufferList = 0;
    }
    
	RPRINT("done closing.\n");
}
 
void ExtAudioFileAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	RPRINT("setting decoder position\n");
    ExtAudioFileSeek(mAudioFile, startFrame);  
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
}

Int64 ExtAudioFileAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	RPRINT("decoding data\n");
    UInt32 numChannels = getNumChannels();
    OSStatus err = noErr;
    
    mReadBuffer.resize(numChannels * numFrames);
    
    // Set up the buffers
    setUpBuffers(&mReadBuffer[0], numChannels, numFrames);
    
    // Read the data out of our file, filling our output buffer
    UInt32 framesRead = numFrames;
	if(!isLoadedInMemory())
	{
		RScopedLock l(&mDecodeLock);
		err = ExtAudioFileRead (mAudioFile, &framesRead, mpBufferList);
	}
	else
	{
		err = ExtAudioFileRead (mAudioFile, &framesRead, mpBufferList);
	}
		
    if(err || framesRead == 0)
    {
        mEOF = true;
		RPRINT("done decoding data\n");
        return 0;
    }
    
    // The data is expected to be interlaced
    for(UInt32 j = 0; j < numChannels; ++j)
    {
        float *pTemp = &mReadBuffer[j * numFrames];
        float *pOut = &buffer[j];
        for(UInt32 i = j; i < framesRead; i++)
        {
            *pOut = *pTemp++;
            pOut += numChannels;
        }
    }
    
	RPRINT("done decoding data\n");
    return framesRead;    
}

void ExtAudioFileAudioSource::setUpBuffers(float *buffer, UInt32 numChannels, UInt32 numFrames)
{
    for(int i=0; i<numChannels; i++)
    {
        mpBufferList->mBuffers[i].mNumberChannels = 1;
        mpBufferList->mBuffers[i].mDataByteSize = numFrames*sizeof(float);
        mpBufferList->mBuffers[i].mData = buffer + i*numFrames;
    }
}
