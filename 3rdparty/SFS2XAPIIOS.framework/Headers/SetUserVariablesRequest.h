//
//  SetUserVariablesRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"

EXTERN NSString * const SetUserVariablesRequest_KEY_USER;
EXTERN NSString * const SetUserVariablesRequest_KEY_VAR_LIST;

/** Set custom User Variables for the current User
 
 See <SFSUserVariable>
 */
@interface SetUserVariablesRequest : BaseRequest {
@private
	
	NSArray *_userVariables;
}

@property (retain) NSArray *userVariables;

-(id)initWithUserVariables:(NSArray *)userVariables;
/**
 
 @param userVariables an Array of UserVariables
  
 @see SFSUserVariable
 */
+(id)requestWithUserVariables:(NSArray *)userVariables;

@end
