//
//  MatchExpression.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMatcher.h"
#import "ISFSArray.h"

@class LogicOperator;

/** Match Expressions are built like "if" conditions in any common programming language. 
 
 They work like queries in a database and can be used to search for Rooms or Users using custom criteria. 
 These expressions are extremely easy to create and concatenate and they can be used for many different filtering operations within the SFS2X framework.

 This is a quick example:

     MatchExpression *exp = [MatchExpression expressionWithVarName:@"rank" condition:[NumberMatch numberMatchGreaterThan] value:[NSNumber numberWithInt:5]];
     exp = [exp and:@"country" condition:[StringMatch stringMatchEquals] value:@"Italy"];
 
 Expressions are made of three elements:
 
 - Variable name
 - Match operator
 - Value

 Additionally any number of expressions can be linked together with a logical <b>AND</b> / <b>OR</b> operator, just like in regular code. 
 In the above example we have created an expression that will check for a rank value &gt; 5 and a country value == "Italy".
  
 The search options are not just limited to User/Room Variables name. In fact the Matching engine provides two extra classes, RoomProperties and UserProperties, 
 where you can access many specific attributes of the Room and User class.
 
 See <RoomProperties>, <UserProperties>
 
 */
@interface MatchExpression : NSObject {
@private
	NSString *_varName;
	id <IMatcher> _condition;
	id _value;
	
	LogicOperator *_logicOp;
	MatchExpression *_parent;
	MatchExpression *_next;
}

/** Get the name of the variable or property that is being matched.
 
 This can be the name of a User/Room variable or a property from the classes listed below.
 
 @see RoomProperties
 @see UserProperties
 */
@property (nonatomic, retain) NSString *varName;

/** Get the condition used for matching
 
 @see BoolMatch
 @see NumberMatch
 @see StringMatch
 */
@property (nonatomic, retain) id <IMatcher> condition;

/** The value used to test the condition in the expression
 */
@property (nonatomic, retain) id value;

/** Get the current logic operator, could be null if the expression has no other concatenated expressions
 @see LogicOperator
 */
@property (nonatomic, retain) LogicOperator *logicOp;
@property (nonatomic, retain) MatchExpression *parent;

/** Get the next expression chained to the current one.
 */
@property (nonatomic, retain) MatchExpression *next;

-(id)initWithVarName:(NSString *)varName condition:(id <IMatcher>)condition value:(id)value;

/**
 @param varName name of the variable/property to match
 @param condition the match condition
 @param value the value to match against
 */
+(id)expressionWithVarName:(NSString *)varName condition:(id <IMatcher>)condition value:(id)value;

/** Concatenate the current expression with a new one using the logical AND operator
 
 @param varName name of the variable/property to match
 @param condition the match condition
 @param value the value to match against
 
 return a new MatchExpression  
 */
-(MatchExpression *)and:(NSString *)varName condition:(id <IMatcher>)condition value:(id)value;

/** Concatenate the current expression with a new one using the logical OR operator
 
 @param varName name of the variable/property to match
 @param condition the match condition
 @param value the value to match against
 
 return a new MatchExpression  
 */
-(MatchExpression *)or:(NSString *)varName condition:(id <IMatcher>)condition value:(id)value;

/** Check if the current expression is concatenated to another one via a logic operator
 
 @see LogicOperator
 */
-(BOOL)hasNext;

/** Rewinds the cursor to the first expression in the chain and return the MatchExpression at the top of the chain of expressions
 */
-(MatchExpression *)rewind;

-(NSString *)asString;


-(id <ISFSArray>)toSFSArray;

-(id <ISFSArray>)expressionAsSFSArray;

@end
