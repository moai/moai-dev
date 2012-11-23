//
//  FindRoomsRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const FindRoomsRequest_KEY_EXPRESSION;
EXTERN NSString * const FindRoomsRequest_KEY_GROUP;
EXTERN NSString * const FindRoomsRequest_KEY_LIMIT;
EXTERN NSString * const FindRoomsRequest_KEY_FILTERED_ROOMS;

@class MatchExpression;

/** Search Rooms in the system by matching their variables and properties against a <MatchExpression>
 */
@interface FindRoomsRequest : BaseRequest {
	
	
	MatchExpression *_matchExpr;
	NSString *_groupId;
	NSInteger _limit;
	
}

-(id)initWithExpr:(MatchExpression *)expr groupId:(NSString *)groupId limit:(NSInteger)limit;
/**
 @param expr the match expression 
 @param groupId the id of the Room Group where to perform the search, if null the search will be Zone-wide (default)
 @param limit the number of results wanted. Zero means no limit, we will get all results (default)
 
 @see [ISFSEvent onRoomFindResult:]
 @see MatchExpression
 @see RoomProperties
 
 */
+(id)requestWithExpr:(MatchExpression *)expr groupId:(NSString *)groupId limit:(NSInteger)limit;
+(id)requestWithExpr:(MatchExpression *)expr;

@end
