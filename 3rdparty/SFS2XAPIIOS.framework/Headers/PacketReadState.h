//
//  PacketReadState.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

EXTERN NSInteger const PacketReadState_WAIT_NEW_PACKET;
EXTERN NSInteger const PacketReadState_WAIT_DATA_SIZE;
EXTERN NSInteger const PacketReadState_WAIT_DATA_SIZE_FRAGMENT;
EXTERN NSInteger const PacketReadState_WAIT_DATA;

@interface PacketReadState : NSObject {
	
}

@end
