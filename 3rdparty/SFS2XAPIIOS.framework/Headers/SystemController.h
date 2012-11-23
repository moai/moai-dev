//
//  SystemController.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseController.h"

@class SmartFox2XClient;
@class BitSwarmClient;

@interface SystemController : BaseController {
@private
	SmartFox2XClient *_sfs;
	BitSwarmClient *_bitSwarm;
	NSMutableDictionary *_requestHandlers;
}

-(id)initWithBitSwarm:(BitSwarmClient *)bitSwarm;

@property (retain) SmartFox2XClient *sfs;
@property (retain) BitSwarmClient *bitSwarm;
@property (retain) NSMutableDictionary *requestHandlers;

@end
