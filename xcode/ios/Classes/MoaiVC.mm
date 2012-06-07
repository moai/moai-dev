//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/AKU.h>
#import "MoaiVC.h"

//================================================================//
// MoaiVC ()
//================================================================//
@interface MoaiVC ()

	//----------------------------------------------------------------//	
	-( void ) updateOrientation :( UIInterfaceOrientation )orientation;

@end

//================================================================//
// MoaiVC
//================================================================//
@implementation MoaiVC

	//----------------------------------------------------------------//
	-( void ) willRotateToInterfaceOrientation :( UIInterfaceOrientation )toInterfaceOrientation duration:( NSTimeInterval )duration {
		
		[ self updateOrientation:toInterfaceOrientation ];
	}

	//----------------------------------------------------------------//
	- ( id ) init {
	
		self = [ super init ];
		if ( self ) {
		
		}
		return self;
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldAutorotateToInterfaceOrientation :( UIInterfaceOrientation )interfaceOrientation {
		
		return true;
	}
	
	//----------------------------------------------------------------//
	-( void ) updateOrientation :( UIInterfaceOrientation )orientation {
		
		UIView* view = self.view;
		
		CGFloat width = view.frame.size.width;
		CGFloat height = view.frame.size.height;
		
		if (( orientation == UIInterfaceOrientationPortrait ) || ( orientation == UIInterfaceOrientationPortraitUpsideDown )) {
			AKUSetOrientation ( AKU_ORIENTATION_PORTRAIT );
			AKUSetViewSize (( int )width, ( int )height );
		}
		else if (( orientation == UIInterfaceOrientationLandscapeLeft ) || ( orientation == UIInterfaceOrientationLandscapeRight )) {
			AKUSetOrientation ( AKU_ORIENTATION_LANDSCAPE );
			AKUSetViewSize (( int )height, ( int )width );
		}
	}
	
@end