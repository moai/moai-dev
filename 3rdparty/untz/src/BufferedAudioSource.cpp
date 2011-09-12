#include "BufferedAudioSource.h"
#include "BufferedAudioSourceThread.h"

BufferedAudioSource::BufferedAudioSource()
{
	mLoadedInMemory = false;
    mEOF = false;
}

BufferedAudioSource::~BufferedAudioSource()
{
}

bool BufferedAudioSource::init(float* interleavedData, Int64 numSamples)
{
    mCurrentFrame = 0;
	mLoadedInMemory = true;
    mEOF = false;
    
    mBuffer.resize(numSamples);
    memcpy(&mBuffer[0], interleavedData, sizeof(float) * numSamples);

    return true;
}

bool BufferedAudioSource::init(const RString& path, bool loadIntoMemory) 
{ 
	if(loadIntoMemory)
	{
		//RPRINT("loading sound into memory...\n");
        int channels = getNumChannels();
        double length = getLength();
		UInt32 totalNumSamples = (UInt32)(getNumChannels() * getSampleRate() * getLength());
		mBuffer.resize(totalNumSamples, 0);
		float *pWritePos = &mBuffer[0];
		UInt32 numFrames = (UInt32)(getSampleRate());
		UInt32 framesRead = 0;
		do
		{
			framesRead = decodeData(pWritePos, numFrames);
			pWritePos += framesRead * getNumChannels();
		}
		while(framesRead > 0);
		mLoadedInMemory = loadIntoMemory; 
	}
	else
	{
        RScopedLock l(&mLock);
		BufferedAudioSourceThread::getInstance()->addSource(this);
	}

	mCurrentFrame = 0;
    
	return true;
}

void BufferedAudioSource::close()
{
	if(!isLoadedInMemory())
	{
		BufferedAudioSourceThread::getInstance()->removeSource(this);        
    }
}

void BufferedAudioSource::setPosition(double seconds)
{
	seconds = seconds < 0 ? 0.0f : seconds;
	seconds = seconds > getLength() ? getLength() : seconds;

	RScopedLock l(&mLock);

	mCurrentFrame = (Int64)(seconds * getSampleRate());    
    if(!isLoadedInMemory())
    {
        mBuffer.clear();
        setDecoderPosition(mCurrentFrame);
        BufferedAudioSourceThread::getInstance()->readMore();
    }
}

double BufferedAudioSource::getPosition()
{
	double position = (double)mCurrentFrame / getSampleRate();
	return position;
}

Int64 BufferedAudioSource::readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames)
{
	mLock.lock();

	Int64 framesRead = numFrames;
	int framesAvailable = mBuffer.size() / getNumChannels() - mCurrentFrame;
    
	// For disk-streaming sources we calculate available frames using the whole buffer
    if(!isLoadedInMemory())
        framesAvailable = mBuffer.size() / getNumChannels();
    
	mLock.unlock();

	Int64 loopEndFrame = convertSecondsToSamples(mLoopEnd);
	Int64 totalFrames = convertSecondsToSamples(getLength());
	bool needToLoop = isLooping() && ((mCurrentFrame >= loopEndFrame && loopEndFrame > 0) || mCurrentFrame >= totalFrames);
	
	if(framesAvailable > 0 && !needToLoop)
	{
		RScopedLock l(&mLock);

		if(framesAvailable < numFrames)
			framesRead = framesAvailable;

		int sourceChannels = getNumChannels();
        int frameOffset = mCurrentFrame;
        
        // For disk-streaming sources we always start at the beginning of the buffer
        if(!isLoadedInMemory())
            frameOffset = 0;
        
		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = NULL;
			if(sourceChannels == 1)
				in = &mBuffer[frameOffset * sourceChannels];
			else
				in = &mBuffer[frameOffset * sourceChannels + j];

			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(buffer++) = *in;
				in += sourceChannels;
			}
		}

        mCurrentFrame += framesRead;
		
        if(!isLoadedInMemory())
		{
			mBuffer.erase(mBuffer.begin(), mBuffer.begin() + (framesRead * sourceChannels));
			framesAvailable = mBuffer.size() / getNumChannels();
			UInt32 minimumFrames = getSampleRate() * SECONDS_TO_BUFFER / 2;
			if(framesAvailable <= minimumFrames)
			{
				BufferedAudioSourceThread::getInstance()->readMore();
			}
		}
	}
    else
    {
        framesRead = ERR_BUFFERING;
        
		if(needToLoop)
			setPosition(mLoopStart);

		Int64 totalFrames = convertSecondsToSamples(getLength());
        if(mCurrentFrame >= totalFrames)
        {
            if(!isLoadedInMemory())
                BufferedAudioSourceThread::getInstance()->removeSource(this);
            
            return 0; // signal that we are done
        }
		else
            if(!isLoadedInMemory())
                BufferedAudioSourceThread::getInstance()->readMore();
/*
        if(isLooping() || !isEOF())
        {
            if(!isLoadedInMemory())
                BufferedAudioSourceThread::getInstance()->readMore();
        }
        else if(isEOF())
        {
            if(!isLoadedInMemory())
                BufferedAudioSourceThread::getInstance()->removeSource(this);
            
            return 0; // signal that we are done
        }
*/
    }
    

	return framesRead;
}