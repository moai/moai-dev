
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

ExtAudioFileAudioSource::ExtAudioFileAudioSource()
{
}

ExtAudioFileAudioSource::~ExtAudioFileAudioSource()
{
    ExtAudioFileDispose(mAudioFile);
    free (mpBufferList);
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

bool ExtAudioFileAudioSource::open(const RString& path, bool loadIntoMemory)
{
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

    return BufferedAudioSource::open(path, loadIntoMemory);
}
 
void ExtAudioFileAudioSource::setDecoderPosition(Int64 startFrame)
{
    ExtAudioFileSeek(mAudioFile, startFrame);  
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
}

Int64 ExtAudioFileAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
    UInt32 numChannels = getNumChannels();
    OSStatus err = noErr;
    
    mReadBuffer.resize(numChannels * numFrames);
    
    // Set up the buffers
    setUpBuffers(&mReadBuffer[0], numChannels, numFrames);
    
    // Read the data out of our file, filling our output buffer
    UInt32 framesRead = numFrames;
    err = ExtAudioFileRead (mAudioFile, &framesRead, mpBufferList);
    if(err || framesRead == 0)
    {
        mEOF = true;
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
