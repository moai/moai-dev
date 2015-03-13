#import <moai-apple/NSDictionary+MOAILib.h>
#import <moai-apple/NSNumber+MOAILib.h>
#import <moai-apple/NSObject+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>

#import <objc/runtime.h>

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
		if ( object_getClass( base ) == object_getClass( dummyProtocol )) {
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
	+( id ) objectFromLua:( lua_State* )state stackIndex:( int )idx {
	
		int type = lua_type ( state, idx );
		switch ( type ) {

			case LUA_TBOOLEAN:
				return lua_toboolean ( state, idx ) > 0 ? @YES : @NO;
				break;
				
			case LUA_TNUMBER:
				return [[[ NSNumber alloc ] initWithLua:state stackIndex:idx ] autorelease ];
				break;
				
			case LUA_TSTRING:
				return [[[ NSString alloc ] initWithLua:state stackIndex:idx ] autorelease ];
				break;
				
			case LUA_TTABLE:
				return [[[ NSMutableDictionary dictionary ] initWithLua:state stackIndex:idx ] autorelease ];
				break;
				
			default:
				// No other compatible types atm.
				return nil;
		}
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
