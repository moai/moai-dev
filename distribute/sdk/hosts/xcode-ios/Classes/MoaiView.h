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
	
	RefPtr < LocationObserver > mLocationObserver;
	
	NSTimer*			mAnimTimer;
	NSTimeInterval		mAnimInterval;
	
	AKUContextID		mAku;
}

	//----------------------------------------------------------------//
	-( void )			run		:( NSString* )filename;
	
@end
