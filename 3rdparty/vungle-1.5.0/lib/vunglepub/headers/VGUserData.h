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

// VGUserData.h -- vungle user data
// by allen brunson  january 3 2012

#ifndef VUNGLE_VGUSERDATA_H
#define VUNGLE_VGUSERDATA_H


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

typedef enum
{
    VGAdOrientationUnknown,
    VGAdOrientationPortrait,
    VGAdOrientationLandscape,
    
    VGAdOrientationFirst   = VGAdOrientationPortrait,
    VGAdOrientationLast    = VGAdOrientationLandscape,
    VGAdOrientationDefault = VGAdOrientationPortrait
    
}   VGAdOrientation;

typedef enum
{
    VGBaseUrlProd,
    VGBaseUrlTest,
    VGBaseUrlLocalhost,
    
    VGBaseUrlFirst   = VGBaseUrlProd,
    VGBaseUrlLast    = VGBaseUrlLocalhost,
    VGBaseUrlDefault = VGBaseUrlProd
    
}   VGConfigBaseUrl;

typedef enum
{
    VGGenderUnknown,
    VGGenderMale,
    VGGenderFemale
}   VGGender;

NS_INLINE BOOL VGAdOrientationValid(VGAdOrientation orient)
{
    switch (orient)
    {
        case VGAdOrientationPortrait:
        case VGAdOrientationLandscape:
        return TRUE;
        
        case VGAdOrientationUnknown:
        break;
    }
    
    return FALSE;
}

NS_INLINE VGAdOrientation VGAdOrientationForceValid(VGAdOrientation orient)
{
    if (VGAdOrientationValid(orient))
    {
        return orient;
    }
    else
    {
        return VGAdOrientationDefault;
    }
}


/******************************************************************************/
/*                                                                            */
/***  VGUserData class                                                      ***/
/*                                                                            */
/******************************************************************************/

@interface VGUserData: NSObject <NSCopying>
{
    @private
    
    NSInteger        age;
    VGGender         gender;
    VGAdOrientation  adOrientation;
    BOOL             locationEnabled;   // allow vungle to get user location
}

// properties

@property(nonatomic, assign) NSInteger        age;
@property(nonatomic, assign) VGGender         gender;
@property(nonatomic, assign) VGAdOrientation  adOrientation;
@property(nonatomic, assign) BOOL             locationEnabled;

// create a default VGUserData object

+(VGUserData*)defaultUserData;

// convert VGAdOrientation to displayable string

+(NSString*)adOrientationString:(VGAdOrientation)orient;

// convert VGConfigBaseURL to displayable string

+(NSString*)baseUrlString:(VGConfigBaseUrl)URL;

// construction and destruction

-(id)copyWithZone:(NSZone*)zone;
-(void)dealloc;
-(id)init;

// properties

-(NSString*)genderString;
-(NSDictionary*)JSONData;

@end


/******************************************************************************/
/*                                                                            */
/***  VGUserData class                                                      ***/
/*                                                                            */
/******************************************************************************

overview
--------

user and preferences data. supplied to VGVunglePub class at startup.

*/

#endif  // VUNGLE_VGUSERDATA_H
