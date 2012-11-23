//
//  IUserManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "User.h"

@class SmartFox2XClient;

/** 
 
 See <SFSUserManager>
 */
@protocol IUserManager <NSObject>

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

-(void)addUser:(id <User>)user;

-(void)removeUser:(id <User>)user;

-(void)removeUserById:(NSInteger)id_;

/**
 Get the total number of Users 
 */
-(NSInteger)userCount;

/**
 Get the full list of Users
 */
-(NSArray *)getUserList;

-(SmartFox2XClient *)smartFox;

@end