//
//  ISFSDataSerializer.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "ISFSArray.h"
#import "ISFSObject.h"

@protocol ISFSDataSerializer <NSObject>

-(NSData *)object2binary:(id <ISFSObject>)object;
-(NSData *)array2binary:(id <ISFSArray>)array;
-(id <ISFSObject>)binary2object:(NSData *)data;
-(id <ISFSArray>)binary2array:(NSData *)data;

@end