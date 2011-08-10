#include "WaveFile.h"

int main(int argc, char *argv[])
{
   if(argc != 2)
   {
      fprintf(stderr, "usage: %s <path>\n", argv[0]);
      exit(1);
   }

   WaveFile *wf = WaveFile::create(argv[1]);
   if(!wf)
   {
      fprintf(stderr, "Failed to create WaveFile object for: %s\n", argv[1]);
      exit(1);
   }

   fprintf(stderr, "Compression code: %d\n", wf->compressionCode());
   fprintf(stderr, "Num Channels: %d\n", wf->numChannels());
   fprintf(stderr, "Sample Rate: %d\n", wf->sampleRate());
   fprintf(stderr, "Avg Bytes Per Sec: %d\n", wf->averageBytesPerSecond());
   fprintf(stderr, "Bytes per Frame: %d\n", wf->bytesPerFrame());
   fprintf(stderr, "Significant Bits Per Sample: %d\n", wf->significantBitsPerSample());
   fprintf(stderr, "Number of Frames: %lf\n", wf->numberOfFrames());

   char buf[10];
   while(wf->readSamples(buf, 10));
}
