//
//  MOAITakeCameraListener.m
//  libmoai
//
//  Created by Antonio Pascual Alonso on 13/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <moaiext-iphone/MOAIAppIOS.h>
#import <moaiext-iphone/MOAITakeCameraListener.h>

@implementation MOAITakeCameraListener

@synthesize popover;

- (void)imagePickerController:(UIImagePickerController *)picker 
	didFinishPickingMediaWithInfo:(NSDictionary *)info
{
	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) { 
		// If on the phone, the image picker is presented modally. Dismiss it.
		[picker dismissModalViewControllerAnimated:YES]; 
	} else {
		// If on the pad, the image picker is in the popover. Dismiss the popover. 
		[popover dismissPopoverAnimated:YES];
	}
	
	UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];

	// Create a CFUUID object - it knows how to create unique identifier strings 
    CFUUIDRef newUniqueID = CFUUIDCreate(kCFAllocatorDefault);
    CFStringRef newUniqueIDString =
	CFUUIDCreateString (kCFAllocatorDefault, newUniqueID);
    
	NSString *path;
	path = [NSString stringWithFormat:
			@"%@/Documents/%@.PNG", 
			NSHomeDirectory(), 
			(__bridge NSString *)newUniqueIDString];
	
	NSData *data1 = [NSData dataWithData:UIImagePNGRepresentation(image)];
	[data1 writeToFile:path atomically:YES];	
	
	// call the lua callback
	MOAIAppIOS::callTakeCameraLuaCallback(path);

	CFRelease(newUniqueIDString); 
    CFRelease(newUniqueID);
	[picker release];	
}

@end
