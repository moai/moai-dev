//
//  Categories.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>


#if TARGET_OS_EMBEDDED || TARGET_IPHONE_SIMULATOR
    #import <CFNetwork/CFNetwork.h>
#else
    #import <CoreServices/CoreServices.h>
#endif


@interface NSString (MD5)

- (NSString *)md5;

@end



@interface NSStream (MyAdditions)

+ (void)getStreamsToHostNamed:(NSString *)hostName port:(NSInteger)port inputStream:(NSInputStream **)inputStream outputStream:(NSOutputStream **)outputStream;

@end


@interface NSMutableData (NSMutableDataExtension)

-(void)writeBool:(BOOL)value;
-(void)writeByte:(uint8_t)value;
-(void)writeShort:(short)value;
-(void)writeInt:(int)value;
-(void)writeLong:(long long)value;
-(void)writeFloat:(float)value;
-(void)writeDouble:(double)value;
-(void)writeUTF:(NSString *)value;

@end

@interface NSData (NSDataExtension)


-(NSData *)gzipInflate;
-(NSData *)gzipDeflate;
-(NSData *)zlibDeflate;
-(NSData *)zlibInflate;

@end


