//
//  StringMatch.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMatcher.h"

/** This matcher is used in MatchExpressions to check String conditions
 
 See <MatchExpression>
 */
@interface StringMatch : NSObject <IMatcher> {
@private
	NSString *_symbol;
}

@property (readonly) NSString *symbol;

-(id)initWithSymbol:(NSString *)symbol;

/** Checks that Str1 == Str2 */
+(StringMatch *)stringMatchEquals;
/** Checks that Str1 != Str2 */
+(StringMatch *)stringMatchNotEquals;
/** Checks that Str1.indexOf(Str2) != -1 */
+(StringMatch *)stringMatchContains;
/** Checks that Str1 starts with characters contained in Str2 */
+(StringMatch *)stringMatchStartsWith;
/** Checks that Str1 end with characters contained in Str2 */
+(StringMatch *)stringMatchEndsWith;

@end
