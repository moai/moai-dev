//
//  ISFSObject.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@protocol ISFSArray;

@class SFSDataWrapper;

/**

 See <SFSObject> 
 */
@protocol ISFSObject <NSObject>

/** Checks if a specific element in the SFSObject is null */
-(BOOL)isNull:(NSString *)key;

/** Returns true if an element exists with the provided key */
-(BOOL)containsKey:(NSString *)key;

/** Removes an element */
-(void)removeElement:(NSString *)key;

/** Return an array with all the keys in the SFSObject */
-(NSArray *)getKeys;

/** Return the number of elements in the SFSObject */
-(NSInteger)size;

/** Return the binary form of the object */
-(NSData *)toBinary;

/** 
 Return a formatted dump of the object that can logged or traced in the console
 for debugging purposes.
 
 @param format turns the "pretty print" on/off
 */
-(NSString *)getDump:(BOOL)format;

/**
 Returns a detailed hex-dump of the object that can logged or traced in the console
 for debugging purposes.
 */
-(NSString *)getHexDump;

-(SFSDataWrapper *)getData:(NSString *)key;

/** Get the element for the provided key as a Boolean */
-(BOOL)getBool:(NSString *)key;

/** Get the element for the provided key as a signed byte (8 bit) (int in AS3) */
-(NSInteger)getByte:(NSString *)key;

/** Get the element for the provided key as a unsigned byte (8 bit) (int in AS3) */
-(NSInteger)getUnsignedByte:(NSString *)key;

/** Get the element for the provided key as a short int (16 bit) (int in AS3) */
-(NSInteger)getShort:(NSString *)key;

/** Get the element for the provided key as an int (32 bit) */
-(NSInteger)getInt:(NSString *)key;

/** Get the element for the provided key as a long int (64 bit) (Number in AS3) */
-(NSNumber *)getLong:(NSString *)key;

/** Get the element for the provided key as a float (Number in AS3) */
-(NSNumber *)getFloat:(NSString *)key;

/** Get the element for the provided key as a double (Number in AS3) */
-(NSNumber *)getDouble:(NSString *)key;

/** Get the element for the provided key as a UTF-8 String */
-(NSString *)getUtfString:(NSString *)key;

/** Get the element for the provided key as an array of Booleans */
-(NSArray *)getBoolArray:(NSString *)key;

/** Get the element for the provided key as a an array of byte */
-(NSData *)getByteArray:(NSString *)key;

/** Get the element for the provided key as a an array of integers representing each byte as unsigned values  */
-(NSArray *)getUnsignedByteArray:(NSString *)key;

/** Get the element for the provided key as an array of short int */
-(NSArray *)getShortArray:(NSString *)key;

/** Get the element for the provided key as an array of int */
-(NSArray *)getIntArray:(NSString *)key;

/** Get the element for the provided key as an array of long int */
-(NSArray *)getLongArray:(NSString *)key;

/** Get the element for the provided key as an array of floats */
-(NSArray *)getFloatArray:(NSString *)key;

/** Get the element for the provided key as an array of doubles */
-(NSArray *)getDoubleArray:(NSString *)key;

/** Get the element for the provided key as an array of UTF-8 Strings */
-(NSArray *)getUtfStringArray:(NSString *)key;

/** 
 Get the element for the provided key as an SFSArray 
 */
-(id <ISFSArray>)getSFSArray:(NSString *)key;

/** Get the element for the provided key as an SFSObject */
-(id <ISFSObject>)getSFSObject:(NSString *)key;



-(id)getClass:(NSString *)key;

-(void)putNull:(NSString *)key;

/** puts a Boolean value with the provided key */
-(void)putBool:(NSString *)key value:(BOOL)value;

/** puts a byte value (8 bit) with the provided key */
-(void)putByte:(NSString *)key value:(NSInteger)value;

/** puts a short int (16 bit) with the provided key */
-(void)putShort:(NSString *)key value:(NSInteger)value;

/** puts an int (32 bit) with the provided key */
-(void)putInt:(NSString *)key value:(NSInteger)value;

/** puts a long int (64 bit) with the provided key */
-(void)putLong:(NSString *)key value:(NSNumber *)value;

/** puts a float (32 bit) with the provided key */
-(void)putFloat:(NSString *)key value:(NSNumber *)value;

/** puts a double (64 bit) with the provided key */
-(void)putDouble:(NSString *)key value:(NSNumber *)value;

/** puts a UTF-8 String with the provided key */
-(void)putUtfString:(NSString *)key value:(NSString *)value;

/** puts an array of Booleans with the provided key */
-(void)putBoolArray:(NSString *)key value:(NSArray *)value;

/** puts an array of bytes with the provided key */
-(void)putByteArray:(NSString *)key value:(NSData *)value;

/** puts an array of short ints with the provided key */
-(void)putShortArray:(NSString *)key value:(NSArray *)value;

/** puts an array of ints with the provided key */
-(void)putIntArray:(NSString *)key value:(NSArray *)value;

/** puts an array of long ints with the provided key */
-(void)putLongArray:(NSString *)key value:(NSArray *)value;

/** puts an array of floats with the provided key */
-(void)putFloatArray:(NSString *)key value:(NSArray *)value;

/** puts an array of doubles with the provided key */
-(void)putDoubleArray:(NSString *)key value:(NSArray *)value;

/** puts an array of Strings with the provided key */
-(void)putUtfStringArray:(NSString *)key value:(NSArray *)value;

/** 
 puts an SFSArray with the provided key
 
 @see SFSArray 
 */
-(void)putSFSArray:(NSString *)key value:(id<ISFSArray>)value;

/** puts an SFSObject with the provided key */
-(void)putSFSObject:(NSString *)key value:(id<ISFSObject>)value;

-(void)putClass:(NSString *)key value:(id)value;

-(void)put:(NSString *)key value:(SFSDataWrapper *)value;



@end