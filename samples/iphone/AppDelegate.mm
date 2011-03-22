#import <aku/aku.h>

#import "AppDelegate.h"
#import "MoaiView.h"

//================================================================//
// AppDelegate
//================================================================//
@implementation AppDelegate

	//----------------------------------------------------------------//
	-( void ) dealloc {

		[ mMoaiView release ];
		[ mWindow release ];
		[ super dealloc ];
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIApplicationDelegate
	//================================================================//	

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		
		[ application setStatusBarHidden:YES ];
		
		[ mWindow makeKeyAndVisible ];
		
		NSString* projectPath = [ NSString stringWithFormat:@"%@/%@", [[ NSBundle mainBundle ] resourcePath ], @"lua" ];
		[[ NSFileManager defaultManager ] changeCurrentDirectoryPath:projectPath ];
		
		[ mMoaiView run:@"main.lua" ];

		return YES;
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {

		AKUFinalize ();
	}

@end

