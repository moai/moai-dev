#import <moai-iphone/NSArray+MOAILib.h>
#import <moai-iphone/NSObject+MOAILib.h>

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
			lua_settable ( state, -2 );
		}
	}

@end
