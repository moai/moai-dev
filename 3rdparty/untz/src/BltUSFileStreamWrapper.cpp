#include <stdio.h>
#include <stdlib.h>
#include "pch.h"
#include <moaicore/moaicore.h>
#include "BlueTune.h"
#include <uslscore/USFileSys.h>
#include "BltUSFileStreamWrapper.h"

static ATX_Result
BltUSFileStream_Read(void*     _self,
                   void*     buffer,
                   ATX_Size  bytes_to_read,
                   ATX_Size* bytes_read)
{
    SeekableUSFileStream* self = (SeekableUSFileStream*)_self;

    size_t read = 0;
    read = self->ReadBytes(buffer, bytes_to_read);
    if (bytes_read)
        *bytes_read = read;
    //MOAIPrint("[BLT] Bytes read : %d", read);
    if (read < 0) {
      //MOAIPrint("[BLT] End of stream");

        return BLT_ERROR_EOS;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   BltUSFileStream_Seek
+---------------------------------------------------------------------*/
static ATX_Result
BltUSFileStream_Seek(void* _self, ATX_Position position)
{
    SeekableUSFileStream* self = (SeekableUSFileStream*)_self;
    //MOAIPrint("[BLT] seek at : %lu", (unsigned long)position);
    self->SetCursor(position);
    return ATX_SUCCESS;

}

/*----------------------------------------------------------------------
|   BltUSFileStream_Tell
+---------------------------------------------------------------------*/
static ATX_Result
BltUSFileStream_Tell(void* _self, ATX_Position* where)
{
    SeekableUSFileStream* self = (SeekableUSFileStream*)_self;

    *where = (ATX_Position)self->GetCursor();
    //MOAIPrint("[BLT] position at : %lu", (unsigned long)*where);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   BltUSFileStream_GetSize
+---------------------------------------------------------------------*/
static ATX_Result
BltUSFileStream_GetSize(void* _self, ATX_LargeSize* size)
{
    SeekableUSFileStream* self = (SeekableUSFileStream*)_self;

    (*size) = self->GetLength();
    //MOAIPrint("[BLT] size  : %lu", (unsigned long)*size);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   BltUSFileStream_GetAvailable
+---------------------------------------------------------------------*/
static ATX_Result
BltUSFileStream_GetAvailable(void* _self, ATX_LargeSize* available)
{
    SeekableUSFileStream* self = (SeekableUSFileStream*)_self;

    *available = (ATX_LargeSize)self->GetLength() - self->GetCursor();
    //MOAIPrint("[BLT] available  : %lu", (unsigned long)*available);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   BltUSFileStream_Interface
+---------------------------------------------------------------------*/
static BLT_InputStream_CallbackInterface
BltSeekableUSFileStreamInterface = {
    BltUSFileStream_Read,
    BltUSFileStream_Seek,
    BltUSFileStream_Tell,
    BltUSFileStream_GetSize,
    BltUSFileStream_GetAvailable
};

/*----------------------------------------------------------------------
|   BltUSFileStream_Create : opens a file for reading. Returns false on failure
+---------------------------------------------------------------------*/
bool
BltUSFileStream_Create(const char* filename, ATX_InputStream** stream)
{
    /* create a new SeekableUSFileStream */
    SeekableUSFileStream* fstream = new SeekableUSFileStream();

    /* load the file in a buffer */
    if (!fstream->OpenRead(filename))
        return false;

    /* create a wrapper for our instance */
    *stream = BLT_InputStreamWrapper_Create(fstream, &BltSeekableUSFileStreamInterface);

    return true;
}



