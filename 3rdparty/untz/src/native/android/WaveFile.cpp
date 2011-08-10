#include <assert.h>
#include "WaveFile.h"

WaveFile *WaveFile::create(const char *path)
{
   // Get our WaveFile.
   WaveFile *w = new WaveFile();
   if(!w)
      return NULL;

   // Open the underlying RiffFile
   w->mpRiff = RiffFile::create(path);
   if(!w->mpRiff)
   {
      delete w;
      return NULL;
   }

   // Grab the format info.
   w->mpRiff->push(STR2FOURCC("fmt "));
   int header_size = sizeof(WaveHeader);
   if(w->mpRiff->readData(&(w->mHeader), header_size) != header_size)
   {
      delete w;
      return NULL;
   }

   assert(w->mHeader.compressionCode == 1); // only uncompressed files are supported 

   // Some writers put a bad block align and/or avgBytesPerSecond value.
   // Let's recalculate it.
   w->mHeader.bytesPerFrame = w->mHeader.significantBitsPerSample / 8 * w->mHeader.numChannels;
   w->mHeader.averageBytesPerSecond = w->mHeader.bytesPerFrame * w->mHeader.sampleRate;

   w->mpRiff->pop();
   w->mpRiff->push(STR2FOURCC("data"));

   return w;
}

WaveFile::WaveFile()
{
   mpRiff = NULL;
}

WaveFile::~WaveFile()
{
   if(mpRiff)
      delete mpRiff;
}

UInt16 WaveFile::compressionCode()
{
   return mHeader.compressionCode;
}

UInt16 WaveFile::numChannels()
{
   return mHeader.numChannels;
}

UInt32 WaveFile::sampleRate()
{
   return mHeader.sampleRate;
}

UInt32 WaveFile::averageBytesPerSecond()
{
   return mHeader.averageBytesPerSecond;
}

UInt16 WaveFile::bytesPerFrame()
{
   return mHeader.bytesPerFrame;
}

UInt16 WaveFile::significantBitsPerSample()
{
   return mHeader.significantBitsPerSample;
}

void WaveFile::rewind()
{
   mpRiff->rewind();
}

double WaveFile::numberOfFrames()
{
   return mpRiff->chunkSize()/mHeader.bytesPerFrame;
}

UInt32 WaveFile::readSamples(void *buf, UInt32 numBytes)
{
    return mpRiff->readData(buf, numBytes);
}
