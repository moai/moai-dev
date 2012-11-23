//
//  ExtensionRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const ExtensionRequest_KEY_CMD;
EXTERN NSString * const ExtensionRequest_KEY_PARAMS;
EXTERN NSString * const ExtensionRequest_KEY_ROOM;
EXTERN NSString * const ExtensionRequest_KEY_UDP;

/**
 Sends a request to a Zone or Room Extension.
 
 Each request to an extension is characterized by two parameters:

 - <b>command name:</b> any string that identifies an action that should be executed on the server side. We recommend to use the same command name for both the client request and the server response.
 - <b>request data:</b> you can send any simple or complex data structures to your extension(s) via the <SFSObject> class

 */ 
@interface ExtensionRequest : BaseRequest {
	
	NSString *_extCmd;
	id<ISFSObject> _params;
	id<Room> _room;
}



-(id)initWithExtCmd:(NSString *)extCmd params:(id<ISFSObject>)params room:(id<Room>)room isUDP:(BOOL) udp;

/**
 @param extCmd the command name
 @param params the custom extension request data
 @param room when specified it will send the request to the Room Extension. The user must be joined in that Room.
 @param udp whether the extension should be sent via UDP (YES) or TCP (NO)
 
 The following example shows usage.

     SFSObject *obj = [[SFSObject newInstance] autorelease];
     [obj putUtfString:@"m" value:@"Hello World!"];
     [smartFox send:[ExtensionRequest requestWithExtCmd:@"myCmd" params:obj udp:NO]];
 
 @see [ISFSEvent onExtensionResponse:]
 
 */
+(id)requestWithExtCmd:(NSString *)extCmd params:(id<ISFSObject>)params room:(id<Room>)room udp:(BOOL) udp;
+(id)requestWithExtCmd:(NSString *)extCmd params:(id<ISFSObject>)params room:(id<Room>)room;
+(id)requestWithExtCmd:(NSString *)extCmd params:(id<ISFSObject>)params udp:(BOOL) udp;
+(id)requestWithExtCmd:(NSString *)extCmd params:(id<ISFSObject>)params;

@end
