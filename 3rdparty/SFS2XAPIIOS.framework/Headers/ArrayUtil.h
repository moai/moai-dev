//
//  ArrayUtil.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

@interface ArrayUtil : NSObject {

}

+(void)removeElement:(NSMutableArray *)array item:(id)item;
+(NSMutableArray *)copy:(NSArray *)array;
+(NSArray *)objToArray:(NSDictionary *)obj;


@end
