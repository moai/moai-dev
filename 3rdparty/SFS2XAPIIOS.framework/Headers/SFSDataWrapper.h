//
//  SFSDataWrapper.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

/** A wrapper object used by <SFSObject> and <SFSArray> to encapsulate data and relative types
 */
@interface SFSDataWrapper : NSObject {
@private	
	NSInteger _type;
	id _data;
}

@property (nonatomic, retain) id data;
@property (nonatomic, assign) NSInteger type;

-(id)initWithType:(NSInteger)type data:(id)data;

@end
