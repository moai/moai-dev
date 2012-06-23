//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/AKU.h>
#import "MoaiVC.h"
#import "MoaiView.h"

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
		
        /*
            The following block of code is used to lock the sample into a Portrait orientation, skipping the landscape views as you rotate your device.
            To complete this feature, you must specify the correct Portraits as the only supported orientations in your plist under the setting,
                "Supported Device Orientations"
         */
        
        if (( interfaceOrientation == UIInterfaceOrientationPortrait ) || ( interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown )) {
            return true;
        }
        
        return false;
        
        /*
            The following is used to support all view orientations.
         */
        
        //return true;
	}
	
	//----------------------------------------------------------------//
	-( void ) updateOrientation :( UIInterfaceOrientation )orientation {
		
		MoaiView* view = ( MoaiView* )self.view;        
		
		if (( orientation == UIInterfaceOrientationPortrait ) || ( orientation == UIInterfaceOrientationPortraitUpsideDown )) {
            
            if ([ view akuInitialized ] != 0 ) {
                AKUSetOrientation ( AKU_ORIENTATION_PORTRAIT );
                AKUSetViewSize (( int )view.width, ( int )view.height );
            }
		}
		else if (( orientation == UIInterfaceOrientationLandscapeLeft ) || ( orientation == UIInterfaceOrientationLandscapeRight )) {
            if ([ view akuInitialized ] != 0 ) {
                AKUSetOrientation ( AKU_ORIENTATION_LANDSCAPE );
                AKUSetViewSize (( int )view.height, ( int )view.width);
            }
		}
	}
	
@end