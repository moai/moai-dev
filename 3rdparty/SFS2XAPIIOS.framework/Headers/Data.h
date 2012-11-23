//
//  Data.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Data : NSObject {
	NSMutableData *_data;
	NSInteger _dataPosition;
}


+(id)dataWithData:(NSData *)anotherData;

-(void)decreasePosition;

-(BOOL)readBool;
-(uint8_t)readByte;
-(int16_t)readShort;
-(int)readInt;
-(long long)readLong;
-(float)readFloat;
-(double)readDouble;
-(NSString *)readUTF;
-(NSData *)readDataWithLength:(int)len;

@end
