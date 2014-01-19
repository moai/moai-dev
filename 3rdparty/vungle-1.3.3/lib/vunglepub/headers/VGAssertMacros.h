// VGAssertMacros.h -- assert macros
// by allen brunson  july 11 2010

/*
 Copyright (C) 2012, Vungle, Inc. All Rights Reserved
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of the author nor the names of its contributors may be used
 to endorse or promote products derived from this software without specific
 prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VUNGLE_VGASSERTMACROS_H
#define VUNGLE_VGASSERTMACROS_H

#include <assert.h>


/******************************************************************************/
/*                                                                            */
/***  defines                                                               ***/
/*                                                                            */
/******************************************************************************/

// don't call this function, it is an implementation detail

void VGAssertProc(const char* test, const char* func,
 const char* file, int32_t line);


/******************************************************************************/
/*                                                                            */
/***  VG_DEBUG setup                                                        ***/
/*                                                                            */
/******************************************************************************/

// VG_DEBUG should be set to 0 for release, 1 for debugging. if not set, we
// default to release.

#ifndef VG_DEBUG
#define VG_DEBUG 0
#endif

#if VG_DEBUG != 0
#define VG_DEBUGGING
#endif

NS_INLINE BOOL VGIsDebugBuild(void)
{
    return (VG_DEBUG != 0);
}

NS_INLINE NSString* VGIsDebugBuildString(void)
{
    if (VGIsDebugBuild())
    {
        return @"on";
    }
    else
    {
        return @"off";
    }
}


/******************************************************************************/
/*                                                                            */
/***  VG_DEBUGONLY() and VG_RELEASE_ONLY() macros                           ***/
/*                                                                            */
/******************************************************************************/

// code inside VG_DEBUG_ONLY() is executed only in debug builds
// code inside VG_RELEASE_ONLY() is executed only in release builds

#ifdef   VG_DEBUGGING
#define  VG_DEBUG_ONLY(code)    code
#define  VG_RELEASE_ONLY(code)  (void)0
#else
#define  VG_DEBUG_ONLY(code)    (void)0
#define  VG_RELEASE_ONLY(code)  code
#endif


/******************************************************************************/
/*                                                                            */
/***  VG_ASSERT() macro                                                     ***/
/*                                                                            */
/******************************************************************************/

// similar to the standard assert() macro. if the VG_ASSERT() condition fails,
// data about the failure is saved to prefs and displayed in an alert the next
// time the app starts. for vungle internal use only.

#if VG_DEBUG == 0

#define  VG_ASSERT(cond)  (void)0

#else

#define  VG_ASSERT(cond)                                     \
{                                                            \
    if (!(cond))                                             \
    {                                                        \
        VGAssertProc(#cond, __func__, __FILE__, __LINE__);   \
        assert(false);                                       \
    }                                                        \
}

#endif

#endif  // VUNGLE_VGASSERTMACROS_H
