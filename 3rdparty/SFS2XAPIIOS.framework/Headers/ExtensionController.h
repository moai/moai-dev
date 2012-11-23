//
//  ExtensionController.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseController.h"
#import "Common.h"


EXTERN NSString * const ExtensionController_KEY_CMD;
EXTERN NSString * const ExtensionController_KEY_PARAMS;

@class SmartFox2XClient;
@class BitSwarmClient;

@interface ExtensionController : BaseController {
@private
	
	SmartFox2XClient *_sfs;
	BitSwarmClient *_bitSwarm;
}

@property (nonatomic, retain) SmartFox2XClient *sfs;
@property (nonatomic, retain) BitSwarmClient *bitSwarm;

-(id)initWithBitSwarmClient:(BitSwarmClient *)bitSwarm;

@end
