// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFileCache.h>

//================================================================//
// MOAIFileCache
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>func ( self, priority, size )</tt>\n
\n
	Creates a partition in the file cache.
	@param self (in)
	@param priority (in) Priority of data in this partition.
	@param size (in) Maximum size (in bytes) for this partition.
*/
int MOAIFileCache::_affirmPartition ( lua_State* L ) {
	LUA_SETUP ( MOAIFileCache, "UNN" )

	int priority	= state.GetValue < int >( 2, 0 );
	int maxSize		= state.GetValue < u32 >( 3, 0 );

	self->AffirmPartition ( priority, maxSize );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>close ( self )</tt>\n
\n
	Closes the file cache and stops caching.
	@param self (in)
*/
int MOAIFileCache::_close ( lua_State* L ) {
	LUA_SETUP ( MOAIFileCache, "U" )

	self->Close ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>open ( self, filename )</tt>\n
\n
	Opens or creates a file cache for configuration and use.
	@param self (in)
	@param filename (in)
*/
int MOAIFileCache::_open ( lua_State* L ) {
	LUA_SETUP ( MOAIFileCache, "US" )

	cc8* filename = state.GetValue < cc8* >( 2, 0 );
	
	self->SetCacheFilename ( filename );
	self->Open ();

	return 0;
}

//================================================================//
// MOAIFileCache
//================================================================//

//----------------------------------------------------------------//
MOAIFileCache::MOAIFileCache () {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIFileCache::~MOAIFileCache () {
}

//----------------------------------------------------------------//
void MOAIFileCache::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "KBYTE", ( u32 )KBYTE );
	state.SetField ( -1, "MBYTE", ( u32 )MBYTE );
}

//----------------------------------------------------------------//
void MOAIFileCache::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "affirmPartition",	_affirmPartition },
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIFileCache::ToString () {

	STLString repr;

	// TODO: pretty print

	return repr;
}
