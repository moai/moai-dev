#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>
#import <contrib/MOAIISO8601DateFormatter.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSDate () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSDate ( MOAILib )
//================================================================//
@implementation NSDate ( MOAILib )

	//----------------------------------------------------------------//
	+( NSDate* ) dateFromISO8601String :( NSString* )iso8601String {
	
		if ( iso8601String ) {
			
			MOAIISO8601DateFormatter* formatter = [[[ MOAIISO8601DateFormatter alloc ] init ] autorelease ];
			return [ formatter dateFromString:iso8601String ];
		}
		
		return nil;
	}
	
	//----------------------------------------------------------------//
	+( NSString* ) iso8601StringFromDate :( NSDate* )date {
	
		if ( date ) {

			MOAIISO8601DateFormatter* formatter = [[[ MOAIISO8601DateFormatter alloc ] init ] autorelease ];
			formatter.includeTime = true;
			return [ formatter stringFromDate:date ];
		}
		return @"";
	}
	
	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		[[ NSDate iso8601StringFromDate:self ] toLua:state ];
	}

@end
