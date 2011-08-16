#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSObject ( MOAILib )
//================================================================//
@interface NSObject ( MOAILib )

	//----------------------------------------------------------------//
	-( id )		isOf				:( id )base;
	-( void )	performSelector		:( SEL )selector afterDelay:( float )delay;
	-( void	)	toLua				:( lua_State* )state;
	+( id )		objectFromLua		:( lua_State* )state stackIndex:( int )idx;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSObject ();
