#import <Foundation/Foundation.h>
#import <moai-core/headers.h>

//================================================================//
// NSArray ( MOAILib )
//================================================================//
@interface NSArray ( MOAILib )

	//----------------------------------------------------------------//
	-( void	)			toLua				:( lua_State* )luaState;

@end

//----------------------------------------------------------------//
extern void loadMoaiLib_NSArray ();
