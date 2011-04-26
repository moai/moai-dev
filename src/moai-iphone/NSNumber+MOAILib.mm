#import <moai-iphone/NSNumber+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSNumber () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@implementation NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushnumber ( state, [ self doubleValue ]);
	}
	
@end
