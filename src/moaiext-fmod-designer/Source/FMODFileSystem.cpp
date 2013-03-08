#include "EventManager.h"
#include "FMODFileSystem.h"

#define FMOD_EOF_READ_WORKAROUND 1

// copied from NonDF/FMOD2011/Src/src/fmod_file.h
#define FMOD_FILE_STREAMING       0x00000800

union FmodFileUserdata {
    void* raw;
    struct {
        u32 isAsync  :  1;   // If true, the DF File* uses async api
        u32 isScheduled:1;   // If true, the DF File* is a ScheduledFile
        u32 origsize : 30;
    } s;
};

FMOD_RESULT F_CALLBACK DfFmodOpenCallback(
    const char * name,
    int unicode,
    u32 * filesize,
    void ** handle,
    void ** userdata
)
{
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK DfFmodCloseCallback(
    void * handle,
    void * /* unused */
)
{
    return FMOD_OK;
}


FMOD_RESULT F_CALLBACK DfFmodReadCallback(
    void * handle,
    void * buffer,
    u32 sizebytes,
    u32 * bytesread,
    void * userdata_
)
{
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK DfFmodSeekCallback(
    void * handle,
    u32 pos,
    void * /* unused */
)
{
    return FMOD_OK;
}