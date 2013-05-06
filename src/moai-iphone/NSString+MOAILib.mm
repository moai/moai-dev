#import <moaiext-iphone/NSString+MOAILib.h>
#import <moaiext-iphone/NSObject+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSString () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

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
	-( id ) initWithLua:( lua_State* )state stackIndex:( int )idx {
		return [ self initWithUTF8String:lua_tostring ( state, idx)];
	}

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushstring ( state, [ self UTF8String ]);
	}
	
@end
