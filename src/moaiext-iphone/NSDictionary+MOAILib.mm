#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSObject+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSDictionary () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSDictionary ( MOAILib )
//================================================================//
@implementation NSDictionary ( MOAILib )

	//----------------------------------------------------------------//
	-( bool ) boolForKey:( id )key withFallback:( bool )value {
	
		NSNumber* number = [ self objectForKey:key ];
		return number ? [ number boolValue ] : value;
	}
	
	//----------------------------------------------------------------//
	-( double ) doubleForKey:( id )key withFallback:( double )value {
	
		NSNumber* number = [ self objectForKey:key ];
		return number ? [ number doubleValue ] : value;
	}
	
	//----------------------------------------------------------------//
	-( float ) floatForKey:( id )key withFallback:( float )value {
	
		NSNumber* number = [ self objectForKey:key ];
		return number ? [ number floatValue ] : value;
	}
	
	//----------------------------------------------------------------//
	-( int ) intForKey:( id )key withFallback:( int )value {
	
		NSNumber* number = [ self objectForKey:key ];
		return number ? [ number intValue ] : value;
	}
	
	//----------------------------------------------------------------//
	-( s64 ) int64ForKey:( id )key withFallback:( s64 )value {
	
		NSNumber* number = [ self objectForKey:key ];
		return number ? [ number longLongValue ] : value;
	}
	
	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {
		
		lua_newtable ( state );

		for ( id key in [ self allKeys ]) {
			
			if ([ key isOf:[ NSString class ]] || [ key isOf:[ NSNumber class ]]) {
				
				id object = [ self objectForKey:key ];
				
				[ key toLua:state ];
				[ object toLua:state ];
				lua_settable ( state, -3 );
			}
		}
	}

@end


//================================================================//
// NSMutableDictionary ( MOAILib )
//================================================================//
@implementation NSMutableDictionary ( MOAILib )

	//----------------------------------------------------------------//
	-( id ) initWithLua:( lua_State* )state stackIndex:( int )idx {

		// table is in the stack at index 'idx'
		lua_pushnil ( state );  // first key
		while ( lua_next ( state, idx ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			id key = [ NSObject objectFromLua:state stackIndex:-2 ];
			if( key != nil ) {
				
				id value = [ NSObject objectFromLua:state stackIndex:-1 ];
				if ( value != nil ) {
					[ self setObject:value forKey:key ];
				}
			}			
			
			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( state, 1 );
		}
		
		return self;
	}

@end
