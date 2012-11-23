//
//  IMatcher.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


@protocol IMatcher <NSObject>

/** Get the condition symbol of this matcher */
-(NSString *)symbol;

/** Get the type of this matcher */
-(NSInteger)type;


@end