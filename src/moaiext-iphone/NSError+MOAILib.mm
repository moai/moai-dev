#import <moaiext-iphone/NSError+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSError () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSError ( MOAILib )
//================================================================//
@implementation NSError ( MOAILib )

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushstring ( state, [[ self localizedDescription ] UTF8String ]);
	}
	
@end
