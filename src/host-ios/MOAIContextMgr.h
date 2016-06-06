//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAIContextMgr
//================================================================//
@interface MOAIContextMgr : NSObject {
@protected
}

    //----------------------------------------------------------------//
    +( void )       displayListBeginPhase       :( int )list;
    +( void )       displayListEndPhase         :( int )list;

@end
