#import <moai-iphone/NSError+MOAILib.h>

//================================================================//
// NSError ( MOAILib )
//================================================================//
@implementation NSError ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushstring ( state, [[ self localizedDescription ] UTF8String ]);
	}
	
@end
