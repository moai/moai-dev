#import <moaiext-iphone/NSArray+MOAILib.h>
#import <moaiext-iphone/NSObject+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSArray () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSArray ( MOAILib )
//================================================================//
@implementation NSArray ( MOAILib )
	
	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_newtable ( state );
		
		int idx = 1;
		for ( id object in self ) {
		
			lua_pushnumber ( state, idx++ );
			[ object toLua:state ];
			lua_settable ( state, -3 );
		}
	}

@end
