// VGAdType.h -- ad type info
// by allen brunson  march 28 2012

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

#ifndef VUNGLE_VGADTYPE_H
#define VUNGLE_VGADTYPE_H


/******************************************************************************/
/*                                                                            */
/***  VGAdType defines                                                      ***/
/*                                                                            */
/******************************************************************************/

typedef enum
{
    VGAdTypeUnknown,
    
    VGAdTypeReal,
    
    VGAdTypeFakePreBundleHTML,
    VGAdTypeFakePreBundleZip,
    VGAdTypeFakePostBundleHTML,
    VGAdTypeFakePostBundleZip,
    VGAdTypeFakeMovie,
    
    VGAdTypeFakeFirst = VGAdTypeFakePreBundleHTML,
    VGAdTypeFakeLast  = VGAdTypeFakeMovie
    
}   VGAdType;

NS_INLINE BOOL VGAdTypeIsValid(VGAdType type)
{
    switch (type)
    {
        case VGAdTypeReal:
        case VGAdTypeFakePreBundleHTML:
        case VGAdTypeFakePreBundleZip:
        case VGAdTypeFakePostBundleHTML:
        case VGAdTypeFakePostBundleZip:
        case VGAdTypeFakeMovie:
        return TRUE;
        
        case VGAdTypeUnknown:
        break;
    }
    
    return FALSE;
}

NS_INLINE BOOL VGAdTypeForceValid(VGAdType type, BOOL fake)
{
    if (!fake) return VGAdTypeReal;
    
    if (VGAdTypeIsValid(type) && (type != VGAdTypeReal))
    {
        return type;
    }
    else
    {
        return VGAdTypeFakeFirst;
    }
}


/******************************************************************************/
/*                                                                            */
/***  VGAdType defines                                                      ***/
/*                                                                            */
/******************************************************************************

overview
--------

internal ad types. you can safely ignore this information. it's used only for
debugging.

*/

#endif  // VUNGLE_VGADTYPE_H
