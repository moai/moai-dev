//
//  SFSUserManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "IUserManager.h"

@class SmartFox2XClient;
@class Logger;

/** Manages the local Users.
 
 The client side API only see the Users that are joined in the same Rooms of the current User.
 */
@interface SFSUserManager : NSObject <IUserManager>{
@private
	NSMutableDictionary *_usersByName;
	NSMutableDictionary *_usersById;
@protected
	Logger *_log;
	SmartFox2XClient *_smartFox;
}
/**
 Get the total number of Users 
 */
@property (readonly) NSInteger userCount;

-(id)initWithSfs:(SmartFox2XClient *)sfs;
-(void)_addUser:(id <User>)user;

/**
 Checks if a specific User name exists in the local User list
 */
-(BOOL)containsUserName:(NSString *)userName;

/**
 Checks if a specific User id exists in the local User list
 */
-(BOOL)containsUserId:(NSInteger)userId;

/**
 Checks if a specific User object exists in the local User list
 */
-(BOOL)containsUser:(id <User>)user;

/**
 Get a User object from its name
 */
-(id <User>)getUserByName:(NSString *)userName;

/**
 Get a User object from its id
 */
-(id <User>)getUserById:(NSInteger)userId;

/**
 Get the full list of Users
 */
-(NSArray *)getUserList;


@end
