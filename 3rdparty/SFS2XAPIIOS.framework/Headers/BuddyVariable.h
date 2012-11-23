//
//  BuddyVariable.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "UserVariable.h"

/**
 See <SFSBuddyVariable>
 */
@protocol BuddyVariable <UserVariable>

/** Returns true if this is an offline Buddy Variable
 
 By convention, any Buddy Variable whose name starts with the dollar sign ($)
 will be regarded as persistent and stored locally by the server.
 
 These variables are also referred to as "offline variables" because they are available
 to other Users even when the Buddy is offline.
 */
-(BOOL)isOffline;

@end