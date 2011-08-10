#include "WaveFileAudioSource.h"
#include <math.h>
#include <android/log.h>

WaveFileAudioSource::WaveFileAudioSource()
{
    mpWaveFile = NULL;
}

WaveFileAudioSource::~WaveFileAudioSource()
{
    if(mpWaveFile)
        delete mpWaveFile;
}

double WaveFileAudioSource::getSampleRate()
{
    return mpWaveFile->sampleRate();
}

double WaveFileAudioSource::getLength()
{
    // seconds
    return mpWaveFile->numberOfFrames()/(double)mpWaveFile->sampleRate();
}

UInt32 WaveFileAudioSource::getNumChannels()
{
    return mpWaveFile->numChannels();
}

bool WaveFileAudioSource::init(const RString& path, bool loadIntoMemory)
{
    mpWaveFile = WaveFile::create(path.c_str());
    if(!mpWaveFile)
        return false;

    return BufferedAudioSource::init(path, loadIntoMemory);
}

void WaveFileAudioSource::close()
{
   delete mpWaveFile;
   mpWaveFile = NULL;
}



Int64 WaveFileAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
#if 1
    char str[500];

    UInt8 buf[numFrames*mpWaveFile->bytesPerFrame()];
    double num_bytes = mpWaveFile->readSamples(&buf, numFrames*mpWaveFile->bytesPerFrame());
    Int64 num_frames = num_bytes/mpWaveFile->bytesPerFrame();

    sprintf(str, "read numFrames: %ld", num_frames);
    __android_log_write(ANDROID_LOG_ERROR,"UntzJNI", str);

   Int16 *shortSamples = (Int16 *)buf;
   for(int i=0; i<num_frames; ++i)
   {
      *(buffer++) = *(shortSamples++)/32767.0f;
   }

   if(num_frames == 0)
      mEOF = true;

   return num_frames;

#else
    static double time = 0.0;
    // Fake it with a sine wave for now.
    for(int i=0; i<numFrames*2; i+=2)
    {
        buffer[i] = sin(200.0*time*(2.0*3.14));
        buffer[i+1] = buffer[i];
        time += 1.0/44100.0;
    }
    return numFrames;
#endif
}

void WaveFileAudioSource::setDecoderPosition(Int64 startFrame)
{
}
