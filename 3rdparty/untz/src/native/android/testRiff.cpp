#include "RiffFile.h"

int main(int argc, char *argv[])
{
   if(argc != 2)
   {
      fprintf(stderr, "usage: %s <path>\n", argv[0]);
      exit(1);
   }


   RiffFile *rf = RiffFile::create(argv[1]);
   if(!rf)
   {
      fprintf(stderr, "Failed to create RiffFile object for: %s\n", argv[1]);
      exit(1);
   }

   UInt32 fourcc = rf->formatCode();
   char str[5];

   fourcc = rf->chunkId();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "File Chunk ID: %s\n", str);
   fprintf(stderr, "Chunk Size: %d\n", rf->chunkSize());

   fourcc = rf->formatCode();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "File format code: %s\n", str);
   fprintf(stderr, "\n");

   rf->push(STR2FOURCC("fmt "));
   fprintf(stderr, "Pushed \"fmt \"\n");

   fourcc = rf->chunkId();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "Chunk ID: %s\n", str);
   fprintf(stderr, "Chunk Size: %d\n", rf->chunkSize());
   fprintf(stderr, "\n");

   rf->pop();
   fprintf(stderr, "Popped\n");

   fourcc = rf->chunkId();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "Chunk ID: %s\n", str);
   fprintf(stderr, "Chunk Size: %d\n", rf->chunkSize());
   fprintf(stderr, "\n");

   rf->push(STR2FOURCC("data"));
   fprintf(stderr, "Pushed \"data\"\n");

   fourcc = rf->chunkId();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "Chunk ID: %s\n", str);
   fprintf(stderr, "Chunk Size: %d\n", rf->chunkSize());
   fprintf(stderr, "\n");

   fprintf(stderr, "Rewind\n");
   rf->rewind();

   fourcc = rf->chunkId();
   FOURCC2STR(str, fourcc);
   fprintf(stderr, "Chunk Id: %s\n", str);
   fprintf(stderr, "Chunk Size: %d\n", rf->chunkSize());
   fprintf(stderr, "\n");


   fprintf(stderr, "Scanning top level chunks....\n");
   long total_bytes = rf->chunkSize();
   UInt8 buf[10];
   long bytes_read = 0;
   long c;
   while(bytes_read < total_bytes)
   {
      RiffChunkHeader header;
      rf->rawRead(&header, 2*sizeof(UInt32));
      bytes_read += 8;
      fourcc = header.chunkId;
      FOURCC2STR(str, fourcc);
      fprintf(stderr, "Chunk ID: %s\n", str);
      fprintf(stderr, "Chunk Size: %d\n", header.chunkSize);
      fprintf(stderr, "\n");

      // Eat till next header.
      c = header.chunkSize-10;
      int i;
      for(i=c; i > 0; i-=10)
      {
          rf->rawRead(buf, 10);
          bytes_read += 10;
      }
      i += 10;
      rf->rawRead(buf, i);
      bytes_read += i;
   }

   delete rf;
}
