//
//  UserPrivileges.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"


/** Guest user, usually the lowest level in the permission scale */
EXTERN NSInteger const UserPrivileges_GUEST;
/** The standard user is usally registered in system and uses a unique name and password to login */
EXTERN NSInteger const UserPrivileges_STANDARD;
/** The moderator has powers to send ModeratorMessages, kicking users and banning them */
EXTERN NSInteger const UserPrivileges_MODERATOR;
/** The administrator has powers to send AdminMessages, kicking users and banning them */
EXTERN NSInteger const UserPrivileges_ADMINISTRATOR;

@interface UserPrivileges : NSObject {
}

@end

