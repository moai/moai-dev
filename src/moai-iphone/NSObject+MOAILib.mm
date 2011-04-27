#import <moai-iphone/NSObject+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSObject () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// MOAILibDummyProtocol
//================================================================//
@protocol MOAILibDummyProtocol
@end

//================================================================//
// NSObject ( MOAILib )
//================================================================//
@implementation NSObject ( MOAILib )

	//----------------------------------------------------------------//
	-( id ) isOf:( id )base {
	
		id obj = self;
		id result = obj;

		const id dummyProtocol = @protocol ( MOAILibDummyProtocol );

		// check to see if base is a Protocol
		if ( base->isa == dummyProtocol->isa ) {
			if ([ obj conformsToProtocol:base ]) {
				return result;
			}
			return nil;
		}
		else {

			obj = [ obj class ];

			while ( obj ) {
				if ( obj == base ) return result;
				obj = [ obj superclass ];
			}
		}
		return nil;
	}

	//----------------------------------------------------------------//
	-( void ) performSelector :( SEL )selector afterDelay:( float )delay {
	
		[ self performSelector:selector withObject:nil afterDelay:delay ];
	}

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushnil ( state );
	}

@end
