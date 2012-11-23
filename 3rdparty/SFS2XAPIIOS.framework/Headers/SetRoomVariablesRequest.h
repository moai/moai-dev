//
//  SetRoomVariablesRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "User.h"

EXTERN NSString * const SetRoomVariablesRequest_KEY_VAR_ROOM;
EXTERN NSString * const SetRoomVariablesRequest_KEY_VAR_LIST;

/** Set custom Room Variables in a Room
 
 See <SFSRoomVariable>
 */
@interface SetRoomVariablesRequest : BaseRequest {
@private
	
	NSArray *_roomVariables;
	id <Room> _room;
}

@property (retain) NSArray *roomVariables;
@property (retain) id <Room> room;

-(id)initWithRoomVariables:(NSArray *)roomVariables room:(id <Room>)room;
/**
 
 @param roomVariables an Array of RoomVariables
 @param room the target Room

 */
+(id)requestWithRoomVariables:(NSArray *)roomVariables room:(id <Room>)room;
+(id)requestWithRoomVariables:(NSArray *)roomVariables;

@end
