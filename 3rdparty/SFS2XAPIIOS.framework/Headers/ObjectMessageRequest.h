//
//  ObjectMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "GenericMessageRequest.h"

/** Send a custom data Object to all users in a Room or a sub-selection of them
 
 (If you are familiar with SmartFoxServer 1.x this request the equivalent of sendObject)
 The request allow to send a custom data object to all users in the same Room. 
 A typical use of this request is for sending game moves to other players 
 
 */
@interface ObjectMessageRequest : GenericMessageRequest {

}

-(id)initWithObject:(id <ISFSObject>)obj targetRoom:(id <Room>)targetRoom recipients:(NSArray *)recipients;


/**

 @param obj the custom object
 @param targetRoom an optional target room. This room must be joined and by default it is the last joined Room  
 @param recipients an optional selection of Users in the target Room (by default the message is sent to all Users)
  
 @see [ISFSEvents onObjectMessage:]

*/

+(id)requestWithObject:(id <ISFSObject>)obj targetRoom:(id <Room>)targetRoom recipients:(NSArray *)recipients;
+(id)requestWithObject:(id <ISFSObject>)obj;

@end
