//
//  IRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//



#import "IMessage.h"

@class SmartFox2XClient;

@protocol IRequest <NSObject>

@required

-(void)validate:(SmartFox2XClient *)sfs;
-(void)execute:(SmartFox2XClient *)sfs;

-(NSInteger)getTargetController;
-(void)setTargetController:(NSInteger)target;
-(BOOL)getIsEncrypted;
-(void)setIsEncrypted:(BOOL)flag;

-(id <IMessage>)getMessage;

@end

