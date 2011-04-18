#import <moai-iphone/NSData+MOAILib.h>

//================================================================//
// NSData ( MOAILib )
//================================================================//
@implementation NSData ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushlstring ( state, ( cc8* )[ self bytes ], [ self length ]);
	}
	
@end
