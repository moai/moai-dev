#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSString ( MOAILib )
//================================================================//
@interface NSString ( MOAILib )

	//----------------------------------------------------------------//
	+( NSString* )		affirm			:( NSString* )str;
	-( void	)			toLua			:( lua_State* )state;
	-( id )				initWithLua		:( lua_State* )state stackIndex:( int )idx;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSString ();
