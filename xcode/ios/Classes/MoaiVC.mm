//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MoaiVC.h"

//================================================================//
// MoaiVC
//================================================================//
@implementation MoaiVC

	//----------------------------------------------------------------//
	- ( id ) init {
	
		self = [ super init ];
		if ( self ) {
		
		}
		return self;
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldAutorotateToInterfaceOrientation :( UIInterfaceOrientation )interfaceOrientation {
		
		//if ( interfaceOrientation == UIInterfaceOrientationLandscapeLeft ) {
		//	return true;
		//}

		return false;
	}
	
@end