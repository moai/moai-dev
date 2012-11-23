//
//  PacketHeader.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@interface PacketHeader : NSObject {

	NSInteger _expectedLen;
	BOOL _binary;
	BOOL _compressed;
	BOOL _encrypted;
	BOOL _blueBoxed;
	BOOL _bigSized;
}

@property (readwrite) NSInteger expectedLen;
@property (readwrite) BOOL binary;
@property (readwrite) BOOL compressed;
@property (readwrite) BOOL encrypted;
@property (readwrite) BOOL blueBoxed;
@property (readwrite) BOOL bigSized;


-(id)initWithParams:(BOOL)encrypted compressed:(BOOL)compressed blueBoxed:(BOOL)blueBoxed bigSized:(BOOL)bigSized;
+(PacketHeader *)fromBinary:(NSInteger)headerByte;
-(NSInteger)encode;

@end
