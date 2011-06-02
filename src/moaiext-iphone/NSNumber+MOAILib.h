#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

//================================================================//
// NSNumber ( MOAILib )
//================================================================//
@interface NSNumber ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSNumber ();
