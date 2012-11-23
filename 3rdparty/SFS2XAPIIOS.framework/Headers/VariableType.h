//
//  VariableType.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "Common.h"

EXTERN NSInteger const VariableType_NULL;
EXTERN NSInteger const VariableType_BOOL;
EXTERN NSInteger const VariableType_INT;
EXTERN NSInteger const VariableType_DOUBLE;
EXTERN NSInteger const VariableType_STRING;
EXTERN NSInteger const VariableType_OBJECT;
EXTERN NSInteger const VariableType_ARRAY;

@interface VariableType : NSObject {
	
}

+(NSString *)getTypeName:(NSInteger)id_;

+(NSInteger)getTypeFromName:(NSString *)name;


@end
