//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <aku/AKU.h>

#import "OpenGLView.h"
#import "RefPtr.h"

@class LocationObserver;

//================================================================//
// MoaiView
//================================================================//
@interface MoaiView : OpenGLView < UIAccelerometerDelegate > {
@private
	
	AKUContextID					mAku;
	NSTimeInterval					mAnimInterval;
    RefPtr < CADisplayLink >		mDisplayLink;
	RefPtr < LocationObserver >		mLocationObserver;
}

	//----------------------------------------------------------------//
	-( void )	moaiInit	:( UIApplication* )application;
	-( void )	pause		:( BOOL )paused;
	-( void )	run			:( NSString* )filename;
	
@end
