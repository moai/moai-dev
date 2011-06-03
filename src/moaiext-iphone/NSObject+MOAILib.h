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

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSObject ();
