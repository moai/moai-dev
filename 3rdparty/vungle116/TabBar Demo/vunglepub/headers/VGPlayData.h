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

// VGPlayData.h -- data for one movie play event
// by allen brunson  january 30 2012

#ifndef VUNGLE_VGPLAYDATA_H
#define VUNGLE_VGPLAYDATA_H


/******************************************************************************/
/*                                                                            */
/***  VGPlayData class                                                      ***/
/*                                                                            */
/******************************************************************************/

@interface VGPlayData: NSObject <NSCoding, NSCopying>
{
    @private
    
    NSTimeInterval  start;        // time when we started playing
    NSTimeInterval  movieTotal;   // total length of the movie
    NSTimeInterval  movieViewed;  // total length viewed by the user
}

// properties

@property(nonatomic, assign) NSTimeInterval  start;
@property(nonatomic, assign) NSTimeInterval  movieTotal;
@property(nonatomic, assign) NSTimeInterval  movieViewed;

// class methods

+(VGPlayData*)playData;

// construction and destruction

-(id)copyWithZone:(NSZone*)zone;
-(void)dealloc;
-(void)encodeWithCoder:(NSCoder*)coder;
-(id)init;
-(id)initWithCoder:(NSCoder*)coder;

// instance methods

-(NSDictionary*)JSONData;
-(BOOL)playedFull;
-(BOOL)valid;

@end


/******************************************************************************/
/*                                                                            */
/***  VGPlayData class                                                      ***/
/*                                                                            */
/******************************************************************************

overview
--------

data for one movie play event

*/

#endif  // VUNGLE_VGPLAYDATA_H
