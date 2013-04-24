// VGMemoryMacros.h -- free/release and retain objects
// by allen brunson  may 19 2010


/******************************************************************************/
/*                                                                            */
/***  retain and release macros                                             ***/
/*                                                                            */
/******************************************************************************/

// free this autorelease pool, then set it to nil

#define VG_POOL_RELEASE(pool)              \
{                                          \
    [pool drain];                          \
    pool = nil;                            \
}

// free this foundation object, then set it to NULL

#define VG_SAFE_CFRELEASE(object)          \
{                                          \
    if (object) CFRelease(object);         \
    object = NULL;                         \
}

// free this malloc'ed pointer, then set it to nil

#define VG_SAFE_FREE(object)               \
{                                          \
    if (object) free(object);              \
    object = NULL;                         \
}  

// free this iokit object, then set it to 0
  
#define VG_SAFE_IORELEASE(object)          \
{                                          \
    if (object) IOObjectRelease(object);   \
    object = 0;                            \
}

// release this NSObject, then set it to nil

#define VG_SAFE_RELEASE(object)            \
{                                          \
    [object release];                      \
    object = nil;                          \
}
  
// save newObject to localObject
  
#define VG_SAFE_SETTER(localObject, newObject)  \
{                                               \
    [newObject retain];                         \
    [localObject release];                      \
    localObject = newObject;                    \
}


/******************************************************************************/
/*                                                                            */
/***  retain and release macros                                             ***/
/*                                                                            */
/******************************************************************************

overview
--------

macros for freeing/releasing various kinds of objects, then setting their
pointers to nil or NULL as appropriate

*/
