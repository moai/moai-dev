//
//  BoolMatch.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
///

#import "IMatcher.h"


/** This matcher is used in MatchExpressions to check boolean conditions
 
 See <MatchExpression>
 */
@interface BoolMatch : NSObject <IMatcher>{
@private	
	NSString *_symbol;
}

@property (readonly) NSString *symbol;

-(id)initWithSymbol:(NSString *)symbol;

/** Checks that Bool1 == Bool2 */
+(BoolMatch *)boolMatchEquals;

/** Checks that Bool1 != Bool2 */
+(BoolMatch *)boolMatchNotEquals;

@end
