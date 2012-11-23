//
//  LogicOperator.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The LogicOperator is used to concatenate two MatchExpression together using a logical AND / OR operator
 
 See <MatchExpression>
 */
@interface LogicOperator : NSObject {
	
	NSString *_id;
}

-(id)initWithId:(NSString *)id_;

/** @return an LogicOperator instance */
+(LogicOperator *)logicOperatorAnd;

/** @return an LogicOperator instance */
+(LogicOperator *)logicOperatorOr;

@property (readonly) NSString *id; 

@end
