//
//  Message.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMessage.h"


@interface Message : NSObject <IMessage>{
@private
	NSInteger _id;
	id <ISFSObject> _content;
	NSInteger _targetController;
	BOOL _isEncrypted;
    BOOL _isUDP;
}

@property (nonatomic, readwrite) NSInteger id;
@property (nonatomic, retain) id <ISFSObject> content;
@property (nonatomic, readwrite) NSInteger targetController;
@property (nonatomic, readwrite) BOOL isEncrypted;
@property (nonatomic, readwrite) BOOL isUDP;


@end
