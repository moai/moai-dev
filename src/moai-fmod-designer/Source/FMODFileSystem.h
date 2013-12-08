#ifndef _FMODFILESYSTEM_H_
#define _FMODFILESYSTEM_H_

#include <fmod.h>

// For FMODEventManager -- this is called before DfFmodOpenCallback(), for some files.
void DfFmod_MaybePreload(const char* name);

FMOD_RESULT F_CALLBACK DfFmodOpenCallback(
    const char * name,
    int unicode,
    u32 * filesize,
    void ** handle,
    void ** userdata
);

FMOD_RESULT F_CALLBACK DfFmodCloseCallback(
    void * handle,
    void * userdata
);

FMOD_RESULT F_CALLBACK DfFmodReadCallback(
    void * handle,
    void * buffer,
    u32 sizebytes,
    u32 * bytesread,
    void * userdata
);

FMOD_RESULT F_CALLBACK DfFmodSeekCallback(
    void * handle,
    u32 pos,
    void * userdata
);

#endif // _FMODFILESYSTEM_H_
