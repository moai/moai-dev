//----------------------------------------------------------------//
// Copyright (c) 2014 Plumzi, Inc.
// All Rights Reserved.
//----------------------------------------------------------------//

#import "MOAIView.h"
#import "MOAIViewController.h"

#import <moai-core/host.h>
#import <moai-sim/host.h>

//================================================================//
// MOAIViewController ()
//================================================================//
@interface MOAIViewController ()

	//----------------------------------------------------------------//	
	-( void ) updateOrientation :( UIInterfaceOrientation )orientation;

@end

//================================================================//
// MOAIViewController
//================================================================//
@implementation MOAIViewController

    //----------------------------------------------------------------//
	-( BOOL ) shouldAutorotate {
		return true;
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldAutorotateToInterfaceOrientation :( UIInterfaceOrientation )interfaceOrientation {
		( void )interfaceOrientation;
        return true;
	}

	//----------------------------------------------------------------//
	-( void ) updateOrientation :( UIInterfaceOrientation )orientation {
		
		MOAIView* view = ( MOAIView* )self.view;
		
		if (( orientation == UIInterfaceOrientationPortrait ) || ( orientation == UIInterfaceOrientationPortraitUpsideDown )) {
            
            //if ([ view akuInitialized ] != 0 ) {
            //    AKUSetOrientation ( AKU_ORIENTATION_PORTRAIT );
            //    AKUSetViewSize (( int )view.width, ( int )view.height );
            //}
		}
		else if (( orientation == UIInterfaceOrientationLandscapeLeft ) || ( orientation == UIInterfaceOrientationLandscapeRight )) {
            //if ([ view akuInitialized ] != 0 ) {
            //    AKUSetOrientation ( AKU_ORIENTATION_LANDSCAPE );
            //    AKUSetViewSize (( int )view.height, ( int )view.width);
            //}
		}
	}

	//----------------------------------------------------------------//
	-( void ) willRotateToInterfaceOrientation :( UIInterfaceOrientation )toInterfaceOrientation duration:( NSTimeInterval )duration {
		
		[ self updateOrientation:toInterfaceOrientation ];
	}

@end
