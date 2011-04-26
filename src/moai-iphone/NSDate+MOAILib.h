#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSDate ( MOAILib )
//================================================================//
@interface NSDate ( MOAILib )

	//----------------------------------------------------------------//
	+( NSDate* )		dateFromISO8601String		:( NSString* )iso8601String;
	+( NSString* )		iso8601StringFromDate		:( NSDate* )date;
	-( void	)			toLua						:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSDate ();
