//
//  HandshakeRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"


EXTERN NSString * const HandshakeRequest_KEY_SESSION_TOKEN;
EXTERN NSString * const HandshakeRequest_KEY_API;
EXTERN NSString * const HandshakeRequest_KEY_COMPRESSION_THRESHOLD;
EXTERN NSString * const HandshakeRequest_KEY_RECONNECTION_TOKEN;
EXTERN NSString * const HandshakeRequest_KEY_CLIENT_TYPE;

@interface HandshakeRequest : BaseRequest {
	
}


-(id)initWithApiVersion:(NSString *)apiVersion reconnectionToken:(NSString *)reconnectionToken;

@end
