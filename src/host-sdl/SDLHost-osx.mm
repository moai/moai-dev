//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <AppKit/AppKit.h>

//----------------------------------------------------------------//
void AppleGetDeviceToPixelScale ( float& w, float& h );
void AppleGetDeviceToPixelScale ( float& w, float& h ) {

	CGFloat scale = [[ NSScreen mainScreen ] backingScaleFactor ];

	w = ( float )scale;
	h = ( float )scale;
}
