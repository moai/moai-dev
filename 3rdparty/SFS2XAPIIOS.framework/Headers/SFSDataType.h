//
//  SFSDataType.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

EXTERN NSInteger const SFSDataType_NULL;
EXTERN NSInteger const SFSDataType_BOOL;
EXTERN NSInteger const SFSDataType_BYTE;
EXTERN NSInteger const SFSDataType_SHORT;
EXTERN NSInteger const SFSDataType_INT;

EXTERN NSInteger const SFSDataType_LONG;
EXTERN NSInteger const SFSDataType_FLOAT;
EXTERN NSInteger const SFSDataType_DOUBLE;
EXTERN NSInteger const SFSDataType_UTF_STRING;
EXTERN NSInteger const SFSDataType_BOOL_ARRAY;

EXTERN NSInteger const SFSDataType_BYTE_ARRAY;
EXTERN NSInteger const SFSDataType_SHORT_ARRAY;
EXTERN NSInteger const SFSDataType_INT_ARRAY;
EXTERN NSInteger const SFSDataType_LONG_ARRAY;
EXTERN NSInteger const SFSDataType_FLOAT_ARRAY;

EXTERN NSInteger const SFSDataType_DOUBLE_ARRAY;
EXTERN NSInteger const SFSDataType_UTF_STRING_ARRAY;
EXTERN NSInteger const SFSDataType_SFS_ARRAY;
EXTERN NSInteger const SFSDataType_SFS_OBJECT;
EXTERN NSInteger const SFSDataType_CLASS;

@interface SFSDataType : NSObject {
	
}

+(NSString *)fromId:(NSInteger)id_;

@end
