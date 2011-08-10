#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@interface NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua			:( lua_State* )state;
	-( id )				initWithLua		:(lua_State* )state stackIndex:(int)idx;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSNumber ();
