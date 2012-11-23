//
//  NumberMatch.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMatcher.h"

/** This matcher is used in MatchExpressions to check numeric conditions
 
 See MatchExpression
 */
@interface NumberMatch : NSObject <IMatcher> {
@private
	
	NSString *_symbol;
	
}

@property (readonly) NSString *symbol;

-(id)initWithSymbol:(NSString *)symbol;

/** Checks that Num1 == Num2 */
+(NumberMatch *)numberMatchEquals;
/** Checks that Num1 != Num2 */
+(NumberMatch *)numberMatchNotEquals;
/** Checks that Num1 &gt; Num2 */
+(NumberMatch *)numberMatchGreaterThan;
/** Checks that Num1 &gt;= Num2 */
+(NumberMatch *)numberMatchGreaterThanOrEqualTo;
/** Checks that Num1 &lt; Num2 */
+(NumberMatch *)numberMatchLessThan;
/** Checks that Num1 &lt;= Num2 */
+(NumberMatch *)numberMatchLessThanOrEqualTo;

@end
