#include "RiffFile.h"

RiffFile::RiffFile()
{
}

RiffFile *RiffFile::create(const char *path)
{
   // Get our new RiffFile
   RiffFile *rf = new RiffFile();
   if(!rf)
      return NULL;

   // Open the underlying file.
   rf->mpFile = fopen(path, "r");
   if(!rf->mpFile)
   {
      delete rf;
      return NULL;
   }

   // Allocate the root chunk struc
   RiffChunkHeader *root_chunk = (RiffChunkHeader *)malloc(sizeof(RiffChunkHeader));
   if(!root_chunk)
   {
      delete rf;
      return NULL;
   }
   rf->mChunkStack.push_back(root_chunk);


   // Read the header info for the root chunk.
   if (fread(root_chunk, sizeof(UInt32), 3, rf->mpFile) != 3)
   {
      delete rf;
      return NULL;
   }
   root_chunk->chunkSize -= sizeof(UInt32);  // root chunk has an extra format code in header.


   // Save the location of the root data segment.
   root_chunk->dataStart = ftell(rf->mpFile);
   if(root_chunk->dataStart < 0)
   {
      delete rf;
      return NULL;
   }

   return rf;
}

RiffFile::~RiffFile()
{
   if(mpFile)
      fclose(mpFile);
   for(int i=0; i<mChunkStack.size(); ++i)
      free(mChunkStack[i]);
   mChunkStack.clear();
}

void RiffFile::rewind()
{
    // Pop to root chunk.
    for(int i = mChunkStack.size()-1; i>=1; --i)
    {
        free(mChunkStack.back());
        mChunkStack.pop_back();
    }
    fseek(mpFile, (mChunkStack.back())->dataStart, SEEK_SET);
}

UInt32 RiffFile::formatCode()
{
   return (mChunkStack.back())->formatCode;
}

bool RiffFile::push(UInt32 chunkId)
{
    long start_pos = ftell(mpFile);
    RiffChunkHeader *parent_chunk = mChunkStack.back();

    // Seek to start of current chunk, and iterate over children looking for desired chunk id.
    fseek(mpFile, parent_chunk->dataStart, SEEK_SET);
    RiffChunkHeader *child_chunk = (RiffChunkHeader *)malloc(sizeof(RiffChunkHeader));
    child_chunk->formatCode = 0;
    long parent_end = parent_chunk->dataStart+parent_chunk->chunkSize;
    while(!feof(mpFile) && ftell(mpFile) < parent_end)
    {
        // Read the full header of this child.
        if(fread(child_chunk, sizeof(UInt32), 2, mpFile) != 2)
            break;  // something went wrong. bail.

        if(child_chunk->chunkId == STR2FOURCC("RIFF") || child_chunk->chunkId == STR2FOURCC("LIST"))
        {
            if(fread(&(child_chunk->formatCode), sizeof(UInt32), 1, mpFile) != 1)
               break;
            child_chunk->chunkSize -= sizeof(UInt32);
        }
        child_chunk->dataStart = ftell(mpFile);

        // If this child is the one we're looking for, push it and return.
        if(child_chunk->chunkId == chunkId)
        {
           mChunkStack.push_back(child_chunk);
           fseek(mpFile, (mChunkStack.back())->dataStart, SEEK_SET);
           return true;
        }

        // Find the start of our next sibling.
        long next_sibling_start = child_chunk->dataStart+child_chunk->chunkSize;
        next_sibling_start += next_sibling_start%2; // word align
        fseek(mpFile, next_sibling_start, SEEK_SET);
    }
    free(child_chunk);
    clearerr(mpFile);
    fseek(mpFile, start_pos, SEEK_SET);
    return false;
}

bool RiffFile::pop()
{
   free(mChunkStack.back());
   mChunkStack.pop_back();
   fseek(mpFile, (mChunkStack.back())->dataStart, SEEK_SET);
}

UInt32 RiffFile::chunkSize()
{
   return (mChunkStack.back())->chunkSize;
}

UInt32 RiffFile::chunkId()
{
   return (mChunkStack.back())->chunkId;
}

UInt32 RiffFile::readData(void *buf, UInt32 bytes)
{
   // Only read bytes up to the end of the current chunk.
   long cur_pos = ftell(mpFile);
   RiffChunkHeader *header = mChunkStack.back();
   UInt32 bytes_left = header->chunkSize - (cur_pos-header->dataStart);
   bytes = std::min(bytes, bytes_left);

   return fread(buf, sizeof(UInt8), bytes, mpFile);
}

UInt32 RiffFile::rawRead(void *buf, UInt32 bytes)
{
   return fread(buf, sizeof(UInt8), bytes, mpFile);
}
