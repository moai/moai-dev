#import <moaiext-iphone/NSData+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSData () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSData ( MOAILib )
//================================================================//
@implementation NSData ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushlstring ( state, ( cc8* )[ self bytes ], [ self length ]);
	}
	
@end
