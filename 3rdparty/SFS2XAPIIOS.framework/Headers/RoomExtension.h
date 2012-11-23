//
//  RoomExtension.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

/** This object is part of the RoomSettings parameters used for creating a new Room.
 
 It describes the properties of an Extension attached to the Room
 
 See <RoomSettings>, <CreateRoomRequest>
 
 */
@interface RoomExtension : NSObject {
	NSString *_id;
	NSString *_className;
	NSString *_propertiesFile;
}

/**
 The name of the extension
 */
@property (retain) NSString *id_;		
/**
 The fully qualified name of the main class in the extension
 */
@property (retain) NSString *className;			
/**
 An optional properties file that will be loaded on the server side when the extension is initialized
 */
@property (retain) NSString *propertiesFile;


-(id)initWithId:(NSString *)id_ className:(NSString *)className;

/**
 @param id_ the name of the extension
 @param className the fully qualified name of the main class in the extension
 
 @see RoomSettings
 @see CreateRoomRequest
 
 */
+(id)extensionWithId:(NSString *)id_ className:(NSString *)className;

@end
