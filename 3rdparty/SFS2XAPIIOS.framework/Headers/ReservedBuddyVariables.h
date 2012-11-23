//
//  ReservedBuddyVariables.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

/** This variable keeps track of the Online Status of the Buddy */
EXTERN NSString * const ReservedBuddyVariables_BV_ONLINE;
/** This variable stores the custom Buddy state (e.g. "Available", "Busy" etc...) */
EXTERN NSString * const ReservedBuddyVariables_BV_STATE;
/** This variable stores an optional nickname for the Buddy */
EXTERN NSString * const ReservedBuddyVariables_BV_NICKNAME;


@interface ReservedBuddyVariables : NSObject {
}

@end
