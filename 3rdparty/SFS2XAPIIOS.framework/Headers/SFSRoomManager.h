//
//  SFSRoomManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IRoomManager.h"


@interface SFSRoomManager : NSObject <IRoomManager> {
	
	NSString *_ownerZone;
	NSMutableArray *_groups;
	NSMutableDictionary *_roomsById;
	NSMutableDictionary *_roomsByName;
	SmartFox2XClient *_smartFox; 
}

-(id)initWithSfs:(SmartFox2XClient *)sfs;


@end
