//
//  BanMode.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"


/** This class contains constants to specify the banishment modality for a <BanUserRequest>
 
 */
@interface BanMode : NSObject {
	
}


/** User is banned by IP Address */
EXTERN NSInteger const BanMode_BY_ADDRESS;
/** User is banned by name */
EXTERN NSInteger const BanMode_BY_NAME;

@end
