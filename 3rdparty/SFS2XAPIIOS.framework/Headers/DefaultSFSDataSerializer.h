//
//  DefaultSFSDataSerializer.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ISFSDataSerializer.h"

@class Data;

@interface DefaultSFSDataSerializer : NSObject <ISFSDataSerializer> {

}

+(id)getInstance;
-(SFSDataWrapper *)decodeObject:(Data *)buffer;
-(id)unwrapObjcField:(SFSDataWrapper *)wrapper;
-(id <ISFSObject>)objc2sfs:(id)objcObj;
-(SFSDataWrapper *)wrapObjcField:(id)value;
@end
