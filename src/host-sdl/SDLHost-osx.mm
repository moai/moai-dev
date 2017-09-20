//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <AppKit/AppKit.h>

#include "SDLHost.h"

//----------------------------------------------------------------//
void GetDeviceToPixelScale ( float& w, float& h ) {

	CGFloat scale = [[ NSScreen mainScreen ] backingScaleFactor ];

	w = ( float )scale;
	h = ( float )scale;
}
