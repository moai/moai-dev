//
//  PendingPacket.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@class PacketHeader;

@interface PendingPacket : NSObject {
@private
	PacketHeader *_header;
	NSMutableData *_buffer;
}

@property (nonatomic, retain) PacketHeader *header;
@property (nonatomic, retain) NSMutableData *buffer;

-(id)initWithPacketHeader:(PacketHeader *)header;

@end
