//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
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
	//-( void ) updateOrientation :( UIInterfaceOrientation )orientation;

@end

//================================================================//
// MOAIViewController
//================================================================//
@implementation MOAIViewController

    //----------------------------------------------------------------//
	-( BOOL ) shouldAutorotate {
		return YES;
	}

//    //----------------------------------------------------------------//
//    -( NSUInteger ) supportedInterfaceOrientations {
//    
//        return UIInterfaceOrientationMaskAll;
//        //return UIInterfaceOrientationMaskLandscape;
//    }

	//----------------------------------------------------------------//
//	-( void ) updateOrientation :( UIInterfaceOrientation )orientation {
//		
//		MOAIView* view = ( MOAIView* )self.view;
//		
//		if (( orientation == UIInterfaceOrientationPortrait ) || ( orientation == UIInterfaceOrientationPortraitUpsideDown )) {
//            
//            //if ([ view akuInitialized ] != 0 ) {
//            //    AKUSetOrientation ( AKU_ORIENTATION_PORTRAIT );
//            //    AKUSetViewSize (( int )view.width, ( int )view.height );
//            //}
//		}
//		else if (( orientation == UIInterfaceOrientationLandscapeLeft ) || ( orientation == UIInterfaceOrientationLandscapeRight )) {
//            //if ([ view akuInitialized ] != 0 ) {
//            //    AKUSetOrientation ( AKU_ORIENTATION_LANDSCAPE );
//            //    AKUSetViewSize (( int )view.height, ( int )view.width);
//            //}
//		}
//	}

	//----------------------------------------------------------------//
//	-( void ) willRotateToInterfaceOrientation :( UIInterfaceOrientation )toInterfaceOrientation duration:( NSTimeInterval )duration {
//		
//		[ self updateOrientation:toInterfaceOrientation ];
//	}

@end
