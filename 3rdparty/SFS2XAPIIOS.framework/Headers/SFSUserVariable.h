//
//  SFSUserVariable.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//
#import "UserVariable.h"

/** The UserVariable class is used to represent variables maintained on the Server side and automatically updated to the clients. 
 
 See <User>, <SetUserVariablesRequest>
 
 */
@interface SFSUserVariable : NSObject <UserVariable>{
@protected	
	NSString *_name;
	NSString *_type;
	id _value;
}
/** The Variable name */
@property (readonly) NSString *name;
/** The Variable Type
 
 @see VariableType
 */
@property (readonly) NSString *type;

-(id)initWithName:(NSString *)name value:(id)value type:(NSInteger)type;

/**
 @param name the name of the variable
 @param value the variable value ( can be Boolean, int, Number, String, SFSObject, SFSArray )
 @param type (optional -1) it's usually not necessary to pass this parameter as the variable value is auto-detected
 */
+(id)variableWithName:(NSString *)name value:(id)value type:(NSInteger)type;
+(id)variableWithName:(NSString *)name value:(id)value;

+(id <UserVariable>)fromSFSArray:(id <ISFSArray>)sfsa;


/** The Variable value */
-(id)getValue;

/** The Variable value as BOOL */
-(BOOL)getBoolValue;

/** The Variable value as NSInteger */
-(NSInteger)getIntValue;

/** The Variable value as Number */
-(NSNumber *)getDoubleValue;

/** The Variable value as NSString */
-(NSString *)getStringValue;

/**  The Variable value as SFSObject
 
 @see SFSObject 
 */
-(id <ISFSObject>)getSFSObjectValue;

/** 
 The Variable value as SFSArray
 */
-(id <ISFSArray>)getSFSArrayValue;

/** Checks if the variable value is null */
-(BOOL)isNull;

-(id<ISFSArray>)toSFSArray;


@end
