#import <moai-iphone/NSString+MOAILib.h>

//================================================================//
// NSString ( MOAILib )
//================================================================//
@implementation NSString ( MOAILib )

	//----------------------------------------------------------------//
	+( NSString* ) affirm :( NSString* )str {
	
		if ( str ) return str;
		return @"";
	}

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushstring ( state, [ self UTF8String ]);
	}
	
@end
