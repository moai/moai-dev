#import <Foundation/Foundation.h>
#import <moai-core/headers.h>

//================================================================//
// NSData ( MOAILib )
//================================================================//
@interface NSData ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua			:( lua_State* )state;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSData ();
