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

// VGStatusData.h -- vungle status data
// by allen brunson  january 25 2012

#ifndef VUNGLE_VGSTATUSDATA_H
#define VUNGLE_VGSTATUSDATA_H


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

typedef enum
{
    VGStatusOkay,
    VGStatusNetworkError,
    VGStatusDiskError
}   VGStatus;


/******************************************************************************/
/*                                                                            */
/***  VGStatusData class                                                    ***/
/*                                                                            */
/******************************************************************************/

@interface VGStatusData: NSObject <NSCopying>
{
    @private
    
    NSInteger  videoAdsCached;
    NSInteger  videoAdsUnviewed;
    VGStatus   status;
}

// properties

@property(nonatomic, assign) NSInteger  videoAdsCached;
@property(nonatomic, assign) NSInteger  videoAdsUnviewed;
@property(nonatomic, assign) VGStatus   status;

// class methods

+(VGStatusData*)statusData;

// construction and destruction

-(id)copyWithZone:(NSZone*)zone;
-(void)dealloc;
-(id)init;

// properties

-(NSString*)description;  // all members as a string
-(NSString*)statusString; // status enum converted to text

@end


/******************************************************************************/
/*                                                                            */
/***  VGStatusData class                                                    ***/
/*                                                                            */
/******************************************************************************

overview
--------

internal statistics that we make available to the calling program

*/

#endif  // VUNGLE_VGSTATUSDATA_H
