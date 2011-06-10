//
//  ExtAudioFileAudioSrc.cpp
//  MOAIAudio
//
//  Created by Zach Saul on 5/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
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
    
}

ExtAudioFileAudioSource::ExtAudioFileAudioSource(const RString& path)
{
    // FIXME: query the file to find out how many channels instead of hardcoding.
    
    CFURLRef path_url = CFURLCreateFromFileSystemRepresentation (kCFAllocatorDefault,
                                                                 (const UInt8*)path.data(),
                                                                 path.length(),
                                                                 false
                                                                 );
    OSStatus s = ExtAudioFileOpenURL(path_url, &mAudioFile);
    
    printCode("ExtAudioFileOpenURL: ", s);

    // 44.1, NUM_CHANNELS, 32 bit float, little endian, non-interleaved 
    FillOutASBDForLPCM(mFormat, 44100.0, NUM_CHANNELS, 32, 32, true, false,true);
    
    s= ExtAudioFileSetProperty(mAudioFile, kExtAudioFileProperty_ClientDataFormat, sizeof(mFormat), &mFormat);
    printCode("ExtAudioFileSetProperty: ", s);

    
    // Allocate our buffer list with NUM_CHANNELS buffers in it.
    mpBufferList = (AudioBufferList *)malloc(sizeof(AudioBufferList)+(NUM_CHANNELS-1)*sizeof(AudioBuffer));
    mpBufferList->mNumberBuffers = NUM_CHANNELS;
}


void ExtAudioFileAudioSource::setUpBuffers(float *buffer, UInt32 numChannels, UInt32 numFrames)
{
    for(int i=0; i<numChannels; i++)
    {
        mpBufferList->mBuffers[i].mNumberChannels = 1;
        mpBufferList->mBuffers[i].mDataByteSize = numFrames*sizeof(float);
        mpBufferList->mBuffers[i].mData = buffer+i*512;
    }
}
UInt32 ExtAudioFileAudioSource::readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames)
{
    // Set up the buffers
    setUpBuffers(buffer, numChannels, numFrames);
    
    // Read the data out of our file, filling our output buffer
    UInt32 framesRead = numFrames;
    OSStatus s = ExtAudioFileRead (mAudioFile, &framesRead, mpBufferList);

    // If there was an error, stop playing and return 0 frames read.
    if(s != noErr)
    {
        mpSound->stop();
        return 0;        
    }
    
    // If we didn't read any frames, rewind the file and either read the next buffer or stop.
    if(framesRead == 0)
    {
        ExtAudioFileSeek(mAudioFile, 0);
        if(mLooping)
        {
            framesRead = numFrames;
            setUpBuffers(buffer, numChannels, numFrames);
            s = ExtAudioFileRead (mAudioFile, &framesRead, mpBufferList);            
            // If there was an error, stop playing and return 0 frames read.
            if(s != noErr)
            {    printCode("ExtAudioFileRead on loop: ", s);
                mpSound->stop();
                return 0;        
            }
        }
        else
        {
            mpSound->stop();
        }
    }
    
    return framesRead;
}

ExtAudioFileAudioSource::~ExtAudioFileAudioSource()
{
    ExtAudioFileDispose(mAudioFile);
    free (mpBufferList);
}