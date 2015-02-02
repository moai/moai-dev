#import <moai-apple/NSString+MOAILib.h>
#import <moai-apple/NSObject+MOAILib.h>

//----------------------------------------------------------------//
void loadMoaiLib_NSString () {
	// do nothing; force linker to load obj-c categories w/o needing linker flags
}

//================================================================//
// NSString ( MOAILib )
//================================================================//
@implementation NSString ( MOAILib )

	//----------------------------------------------------------------//
	+( NSString* ) affirm :( NSString* )str {
	
		if ( str ) return str;
		return @"";
	}

	//----------------------------------------------------------------//
	-( id ) initWithLua:( lua_State* )state stackIndex:( int )idx {
		return [ self initWithUTF8String:lua_tostring ( state, idx)];
	}

	//----------------------------------------------------------------//
	-( void	) toLua:( lua_State* )state {

		lua_pushstring ( state, [ self UTF8String ]);
	}

	//----------------------------------------------------------------//
	+( NSString* ) stringWithGuessedEncoding :( const char* )bytes {
	
		if ( !bytes ) return nil;
	
		NSString* string = [ NSString stringWithUTF8String:bytes ];
		if ( string ) return string;
		
		string = [ NSString stringWithCString:bytes encoding:NSWindowsCP1252StringEncoding ];
		if ( string ) return string;
		
		// TODO: more fallbacks?
		// TODO: escape string for utf8?
		
		return [ NSString stringWithCString:bytes encoding:[ NSString defaultCStringEncoding ]];
	}

@end
