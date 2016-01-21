//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIContextMgr.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIContextMgr ()
//================================================================//
@interface MOAIContextMgr () {
}

	//----------------------------------------------------------------//
    +( void )           affirmLock;

@end

//================================================================//
// MOAIContextMgr
//================================================================//
@implementation MOAIContextMgr

    static NSLock*      sDisplayListLock;

    //----------------------------------------------------------------//
    +( void ) affirmLock {
    
        if ( !sDisplayListLock ) {
            sDisplayListLock = [[ NSLock alloc ] init ];
        }
    }

    //----------------------------------------------------------------//
    +( void ) displayListBeginPhase :( int )list {
	
        [ MOAIContextMgr affirmLock ];
    
		[ sDisplayListLock lock ];
		AKUDisplayListBeginPhase ( list );
		[ sDisplayListLock unlock ];
	}
	
	//----------------------------------------------------------------//
	+( void ) displayListEndPhase :( int )list {
	
		[ sDisplayListLock lock ];
		AKUDisplayListEndPhase ( list );
		[ sDisplayListLock unlock ];
	}

@end
