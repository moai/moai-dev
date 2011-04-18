#import <moai-iphone/NSString+MOAILib.h>

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@implementation NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushnumber ( state, [ self doubleValue ]);
	}
	
@end
