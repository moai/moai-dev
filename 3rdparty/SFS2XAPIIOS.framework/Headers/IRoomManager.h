//
//  IRoomManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@class SmartFox2XClient;

@protocol Room;
@protocol User;

@protocol IRoomManager <NSObject>

-(NSString *)ownerZone;

-(void)addRoom:(id <Room>)room addGroupIfMissing:(BOOL)addGroupIfMissing;

-(void)addGroup:(NSString *)groupId;

-(void)replaceRoom:(id <Room>)room addToGroupIfMissing:(BOOL)addToGroupIfMissing;

-(void)removeGroup:(NSString *)groupId;

-(BOOL)containsGroup:(NSString *)groupId;

-(BOOL)containsRoom:(id)idOrName;
-(BOOL)containsRoomInGroup:(id)idOrName groupId:(NSString *)groupId;

-(void)changeRoomName:(id <Room>)room newName:(NSString *)newName;


-(void)changeRoomPasswordState:(id <Room>)room isPassProtected:(BOOL)isPassProtected;

-(void)changeRoomCapacity:(id <Room>)room maxUsers:(NSInteger)maxUsers maxSpect:(NSInteger)maxSpect;

-(id <Room>)getRoomById:(NSInteger)id_;
-(id <Room>)getRoomByName:(NSString *)name;
-(NSArray *)getRoomList;
-(NSInteger)getRoomCount;
-(NSArray *)getRoomGroups;
-(NSArray *)getRoomListFromGroup:(NSString *)groupId;
-(NSMutableArray *)getJoinedRooms;
-(NSArray *)getUserRooms:(id <User>)user;

-(void)removeRoom:(id <Room>)room;

-(void)removeRoomById:(NSInteger)id_;

-(void)removeRoomByName:(NSString *)name;

-(void)removeUser:(id <User>)user;

-(SmartFox2XClient *)smartFox;

@end