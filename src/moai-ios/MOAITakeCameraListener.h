//
//  MOAITakeCameraListener.h
//  libmoai
//
//  Created by Antonio Pascual Alonso on 13/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>


@interface MOAITakeCameraListener : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIPopoverControllerDelegate> {
@private
	UIPopoverController *popover;
}

@property ( readwrite, assign ) UIPopoverController*	popover;

@end
