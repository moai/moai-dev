//
//  FindUsersRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const FindUsersRequest_KEY_EXPRESSION;
EXTERN NSString * const FindUsersRequest_KEY_GROUP;
EXTERN NSString * const FindUsersRequest_KEY_ROOM;
EXTERN NSString * const FindUsersRequest_KEY_LIMIT;
EXTERN NSString * const FindUsersRequest_KEY_FILTERED_USERS;

@class MatchExpression;
/**
 Search Users in the system by matching their variables and properties against a <  MatchExpression>
 */
@interface FindUsersRequest : BaseRequest {
	
	MatchExpression *_matchExpr;
	id _target;
	NSInteger _limit;
}

-(id)initWithExpr:(MatchExpression *)expr target:(id)target limit:(NSInteger)limit;


/**
 @param expr the match expression 
 @param target it can be a Room or String (the Group Id), if null the search will done in the whole Zone (default)
 @param limit the number of results wanted. Zero means no limit, we will get all results (default)
 
 @see [ISFSEvent onUserFindResult:]
 @see UserProperties
 @see MatchExpression
 
 */
+(id)requestWithExpr:(MatchExpression *)expr target:(id)target limit:(NSInteger)limit;
+(id)requestWithExpr:(MatchExpression *)expr;

@end
