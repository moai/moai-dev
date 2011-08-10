#ifndef __RIFFFILE_H__
#define __RIFFFILE_H__

#include "UntzSystem.h"

typedef struct {                    // Chunk structure
    UInt32 chunkId;                 // Chunk type identifier
    UInt32 chunkSize;               // Chunk size field (size of chunkData)
    UInt32 formatCode;              // Only used for RIFF and LIST chunks
   long dataStart;
} RiffChunkHeader;

class RiffFile
{
public:
    RiffFile();
    ~RiffFile();
    static RiffFile *create(const char *path);
    void    rewind();
    UInt32  formatCode();
    bool    push(UInt32 chunkId);
    bool    pop();
    UInt32  chunkSize();
    UInt32  chunkId();
    UInt32  readData(void *buf, UInt32 bytes);
    UInt32  rawRead(void *buf, UInt32 bytes);

private:
    FILE                            *mpFile;
    std::vector<RiffChunkHeader *>  mChunkStack;
};

#define FOURCC2STR(str, fourcc) ( memcpy(str, (char *)(&(fourcc)), 4), (str)[4]=0 )
#define STR2FOURCC(str) ( *((UInt32 *)(str)) )
#endif
