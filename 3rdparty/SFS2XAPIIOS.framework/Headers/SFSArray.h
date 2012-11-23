//
//  SFSArray.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "ISFSArray.h"
#import "ISFSDataSerializer.h"


/** SFSArray is used from both server and client sides to exchange data. It can be thought of a specialized Array/List object that can contain any type of data.

 The advantage of using SFSArray is that you can fine tune the way your data will be transmitted over the network.
 For instance, a number like 100 can be transmitted as a normal <b>integer</b> (which takes 32 bits) but also a <b>short</b> (16 bit) or even a <b>byte</b> (8 bit)
 
 SFSArray supports many primitive data types and related arrays of primitives. It also allows to serialize class instances and rebuild them on the Java side. This is explained in greater detail in a separate document.
 
 See <ISFSArray>, <SFSObject>
 */
@interface SFSArray : NSObject <ISFSArray> {
@private
	
	id <ISFSDataSerializer> _serializer;
	NSMutableArray *_dataHolder;
	 
}

/** Alternative static constructor that builds an SFSArray populated with the data found in the passed Array */
+(SFSArray *)newFromArray:(NSArray *)arr;
/** Alternative static constructor that builds an SFSArray from a valid SFSArray binary representation */
+(SFSArray *)newFromBinaryData:(NSData *)data;
/** Alternative static constructor */
+(SFSArray *)newInstance;


/** Returns true if the passed object is contained in the Array */
-(BOOL)contains:(id)obj;

/** Returns the element at the specified index */
-(id)getElementAt:(NSInteger)index;

/**  */
-(SFSDataWrapper *)getWrappedElementAt:(NSInteger)index;/** @endcond */

/** Remove the element at the specified index */
-(id)removeElementAt:(NSInteger)index;

/** Return the number of elements in the Array */
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

/** Add a null element */
-(void)addNull;

/** Add a Boolean value */
-(void)addBool:(BOOL)value;

/** Add a byte value (8 bit) */
-(void)addByte:(NSInteger)value;

/** Add a short int value (16 bit) */
-(void)addShort:(NSInteger)value;

/** Add an int value (32 bit) */
-(void)addInt:(NSInteger)value;

/** Add a long int value (64 bit) */
-(void)addLong:(NSNumber *)value;

/** Add a float value (32 bit) */
-(void)addFloat:(NSNumber *)value;

/** Add a dobule value (64 bit) */
-(void)addDouble:(NSNumber *)value;

/** Add a UTF-8 String */
-(void)addUtfString:(NSString *)value;

/** Add an array of Booleans */
-(void)addBoolArray:(NSArray *)value;

/** Add an array of bytes */
-(void)addByteArray:(NSData *)value;

/** Add an array of short ints */
-(void)addShortArray:(NSArray *)value;

/** Add an array of ints */
-(void)addIntArray:(NSArray *)value;

/** Add an array of long ints */
-(void)addLongArray:(NSArray *)value;

/** Add an array of floats */
-(void)addFloatArray:(NSArray *)value;

/** Add an array of doubles */
-(void)addDoubleArray:(NSArray *)value;

/** Add an array of UTF-8 String */
-(void)addUtfStringArray:(NSArray *)value;

/** Add an SFSArray  */
-(void)addSFSArray:(id<ISFSArray>)value;

/** 
 Add an SFSObject
 @see SFSObject 
 */
-(void)addSFSObject:(id<ISFSObject>)value;

-(void)addClass:(id)value;

/**  */
-(void)add:(SFSDataWrapper *)wrappedObject;/** @endcond */

/** Checks if a certain element in the Array is null */
-(BOOL)isNull:(NSInteger)index;

/** Get a Boolean element at the provided index */
-(BOOL)getBool:(NSInteger)index;

/** Get a byte element at the provided index */
-(NSInteger)getByte:(NSInteger)index;

/** Get an unsigned byte element at the provided index */
-(NSInteger)getUnsignedByte:(NSInteger)index;

/** Get a short int element at the provided index */
-(NSInteger)getShort:(NSInteger)index;

/** Get an int element at the provided index */
-(NSInteger)getInt:(NSInteger)index;

/** Get a long int element at the provided index */
-(NSNumber *)getLong:(NSInteger)index;

/** Get a float element at the provided index */
-(NSNumber *)getFloat:(NSInteger)index;

/** Get a double element at the provided index */
-(NSNumber *)getDouble:(NSInteger)index;

/** Get a String element at the provided index */
-(NSString *)getUtfString:(NSInteger)index;

/** Get a Boolean Array element at the provided index */
-(NSArray *)getBoolArray:(NSInteger)index;

/** Get a byte Array element at the provided index */
-(NSData *)getByteArray:(NSInteger)index;

/** Get an Array of unsigned integers at the provided index */
-(NSArray *)getUnsignedByteArray:(NSInteger)index;

/** Get a short Array element at the provided index */
-(NSArray *)getShortArray:(NSInteger)index;

/** Get a int Array element at the provided index */
-(NSArray *)getIntArray:(NSInteger)index;

/** Get a lomg Array element at the provided index */
-(NSArray *)getLongArray:(NSInteger)index;

/** Get a float Array element at the provided index */
-(NSArray *)getFloatArray:(NSInteger)index;

/** Get a double Array element at the provided index */
-(NSArray *)getDoubleArray:(NSInteger)index;

/** Get a String Array element at the provided index */
-(NSArray *)getUtfStringArray:(NSInteger)index;

/** Get an SFSArray element at the provided index */
-(id <ISFSArray>)getSFSArray:(NSInteger)index;

/** Get an SFSObject element at the provided index */
-(id <ISFSObject>)getSFSObject:(NSInteger)index;

-(id)getClass:(NSInteger)index;

@end
