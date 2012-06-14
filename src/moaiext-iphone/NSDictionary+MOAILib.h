#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSDictionary ( MOAILib )
//================================================================//
@interface NSDictionary ( MOAILib )

	//----------------------------------------------------------------//
	-( bool )			boolForKey			:( id )key withFallback:( bool )value;
	-( double )			doubleForKey		:( id )key withFallback:( double )value;
	-( float )			floatForKey			:( id )key withFallback:( float )value;
	-( int )			intForKey			:( id )key withFallback:( int )value;
	-( s64 )			int64ForKey			:( id )key withFallback:( s64 )value;
	-( void	)			toLua				:( lua_State* )luaState;

@end

//================================================================//
// NSMutableDictionary ( MOAILib )
//================================================================//
@interface NSMutableDictionary ( MOAILib )

	//----------------------------------------------------------------//
	-( id )				initWithLua         :( lua_State* )luaState stackIndex:( int )idx;
@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSDictionary ();
