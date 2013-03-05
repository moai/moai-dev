#pragma once

#include "BufferedAudioSource.h"
#include "WaveFile.h"

class WaveFileAudioSource : public BufferedAudioSource
{
public:
    WaveFileAudioSource();
    ~WaveFileAudioSource();

    // AudioSource
    double getSampleRate();
    double getLength();
    UInt32 getNumChannels();

    // BufferedAudioSource
    virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
    virtual Int64 decodeData(float* buffer, UInt32 size);
    virtual void setDecoderPosition(Int64 startFrame);

protected:
	RString mPath;
    WaveFile mWaveFile;
	std::vector<UInt8> mRawBuffer;
};
