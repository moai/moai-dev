#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@interface NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( id )			initWithLua		:( lua_State* )state stackIndex:( int )idx;
	-( void	)		toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSNumber ();
