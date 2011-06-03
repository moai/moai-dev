#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSData ( MOAILib )
//================================================================//
@interface NSData ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSData ();
