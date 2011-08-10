#include "RiffFile.h"

typedef struct
{
   UInt16 compressionCode;
   UInt16 numChannels;
   UInt32 sampleRate;
   UInt32 averageBytesPerSecond;
   UInt16 bytesPerFrame;
   UInt16 significantBitsPerSample;
} WaveHeader;

class WaveFile
{
public:
   WaveFile();
   ~WaveFile();
   static WaveFile *create(const char *path);
   UInt16 compressionCode();
   UInt16 numChannels();
   UInt32 sampleRate();
   UInt32 averageBytesPerSecond();
   UInt16 bytesPerFrame();
   UInt16 significantBitsPerSample();
   double numberOfFrames();

   void rewind();
   UInt32 readSamples(void *buf, UInt32 numBytes);

private:
   RiffFile *mpRiff;
   WaveHeader mHeader;
};
