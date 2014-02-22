//  VGConstants.h
//  vunglepub

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


#define kVGInternalBuildNumber @"1.3.3"
#define kStatusBarOffset 20
#define kAdViewUpwardAnimDuration   0.5
#define kAdViewForMovieDuration     0.8
#define kAdViewDownwardAnimDuration 0.4
#define kScreenLengthFactor         1.5


//Base Url configuration
#define kBaseUrlKey            @"vgBaseUrl"

#define kBaseUrlProdName       @"prod"
#define kBaseUrlTestName       @"test"
#define kBaseUrlLocalhostName  @"localhost"

#define kBaseUrlProd           @"http://api.vungle.com"
#define kBaseUrlTest           @"http://acceptance.vungle.com"    
#define kBaseUrlLocalhost      @"http://localhost:3000"

//if kIsRemoveMuteButtonFromAlliOS == YES then remove mute button for all iOS versions 
//if kIsRemoveMuteButtonFromAlliOS == NO then show mute button in iOS 2.x to iOS 4.x and do not show in iOS 5.x and later
#define kIsRemoveMuteButtonFromAlliOS      NO

#define kInAppStoreEnabled                 NO

//Convertions 
#define degreesToRadian(x) (M_PI * x / 180.0)

/** Float: Portrait Screen Height **/
#define SCREEN_HEIGHT_PORTRAIT ( [[UIScreen mainScreen ] bounds ].size.height )

/** Float: Portrait Screen Width **/
#define SCREEN_WIDTH_PORTRAIT ( [[UIScreen mainScreen ] bounds ].size.width )

/** Float: Landscape Screen Height **/
#define SCREEN_HEIGHT_LANDSCAPE ( [[UIScreen mainScreen ] bounds ].size.width )  

/** Float: Landscape Screen Width **/
#define SCREEN_WIDTH_LANDSCAPE ( [[UIScreen mainScreen ] bounds ].size.height )

/** CGRect: Portrait Screen Frame **/
#define SCREEN_FRAME_PORTRAIT_WITH_STATUSBAR ( CGRectMake( 0, 20, SCREEN_WIDTH_PORTRAIT , SCREEN_HEIGHT_PORTRAIT ) )

/** CGRect: Landscape Screen Frame **/
#define SCREEN_FRAME_LANDSCAPE_WITH_STATUSBAR ( CGRectMake( 0, 20, SCREEN_WIDTH_LANDSCAPE , SCREEN_HEIGHT_LANDSCAPE ) )

/** Float: Screen Scale **/
#define SCREEN_SCALE ([[UIScreen mainScreen] scale ] )

/** CGSize: Screen Size Portrait **/
#define SCREEN_SIZE_PORTRIAT ( CGSizeMake( SCREEN_WIDTH_PORTRAIT * SCREEN_SCALE , SCREEN_HEIGHT_PORTRAIT * SCREEN_SCALE ) )

/** CGSize: Screen Size Landscape **/
#define SCREEN_SIZE_LANDSCAPE ( CGSizeMake( SCREEN_WIDTH_LANDSCAPE * SCREEN_SCALE , SCREEN_HEIGHT_LANDSCAPE * SCREEN_SCALE ) )

#define STATUS_BAR_SIZE 20.0

#define DONT_BLOCK_UI() \
NSAssert(![NSThread isMainThread], @"Don't block the UI thread please!")

#define BLOCK_UI() \
NSAssert([NSThread isMainThread], @"You aren't running in the UI thread!")
