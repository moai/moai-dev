//
//  ConfigData.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

/**
 Stoes the client configuration data loaded from an external XML file
 */
@interface ConfigData : NSObject {

	
	NSString *_host;
	NSInteger _port;
	NSString *_udpHost;
	NSInteger _udpPort;
	NSString *_zone;
	BOOL _debug;
	
	NSInteger _httpPort;
	BOOL _useBlueBox;
	NSInteger _blueBoxPollingRate;
	
}

@property (retain) NSString *host;
@property (assign) NSInteger port;
@property (retain) NSString *udpHost;
@property (assign) NSInteger udpPort;
@property (retain) NSString *zone;
@property (assign) BOOL debug;
@property (assign) NSInteger httpPort;
@property (assign) BOOL useBlueBox;
@property (assign) NSInteger blueBoxPollingRate;

@end
