//
//  IMessage.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "ISFSObject.h"

@protocol IMessage <NSObject>

@required

-(NSInteger)id;
-(void)setId:(NSInteger)value;

-(id<ISFSObject>)content;
-(void)setContent:(id<ISFSObject>)value;

-(NSInteger)targetController;
-(void)setTargetController:(NSInteger)value;

-(BOOL)isEncrypted;
-(void)setIsEncrypted:(BOOL)value;

-(BOOL)isUDP;
-(void)setIsUDP:(BOOL)value;

@end