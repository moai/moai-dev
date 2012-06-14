#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSString ( MOAILib )
//================================================================//
@interface NSString ( MOAILib )

	//----------------------------------------------------------------//
	+( NSString* )		affirm			:( NSString* )str;
	-( id )				initWithLua		:( lua_State* )state stackIndex:( int )idx;
	-( void	)			toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSString ();
