#import <moaiext-iphone/NSNumber+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSNumber () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@implementation NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( id ) initWithLua:(lua_State* )state stackIndex:( int )idx {
		
		#ifdef LUA_NUMBER_DOUBLE
				return [ self initWithDouble:lua_tonumber ( state, idx )];
		#else
				return [ self initWithFloat:lua_tonumber ( state, idx )];
		#endif
	}

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushnumber ( state, [ self doubleValue ]);
	}

@end
