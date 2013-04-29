#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSError ( MOAILib )
//================================================================//
@interface NSError ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSError ();
