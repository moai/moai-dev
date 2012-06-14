// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITextBundle.h>
#include <moaicore/MOAIDataBuffer.h>


#define HASHWORDBITS 32

inline unsigned long hashpjw(const char *str_param) {
	
	unsigned long hval = 0;
	unsigned long g;
	const char *s = str_param;
	
	while (*s) {
		hval <<= 4;
		hval += (unsigned char) *s++;
		g = hval & ((unsigned long int) 0xf << (HASHWORDBITS - 4));
		if (g != 0) {
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	
	return hval;
}


//----------------------------------------------------------------//
/**	@name	load
 @text	Load a text bundle from a .mo file.
 
 @in MOAITextBundle self
 @in mixed source A MOAIDataBuffer containing the text bundle, or a string containing the filename to load.
 @out	number size				The number of bytes in this data buffer object.
 */
int MOAITextBundle::_load( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBundle, "U" );
	
	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, false );
	if( data )
	{
		lua_pushboolean(state, self->Load(data));
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {
		lua_pushboolean(state, self->Load(state.GetValue<cc8*>(2, 0)));
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	lookup
 @text	Look up a string in the bundle (defaulting to the lookup string itself). In the case
	of defaulting, a false value is returned as the second value (useful for falling back to
	less-specific bundles if desireable).
 
 @in MOAITextBundle self
 @in string key A text string to use as a "key"
 @out string value The value if found, otherwise it returns the original string if not found.
 @out boolean found True if the string was found in the table (regardless of returned value), or false if it couldn't be found.
 */
int MOAITextBundle::_lookup( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBundle, "US" );
	
	cc8 *key = state.GetValue<cc8*>(2, 0);
	if( !key ) {
		lua_pushnil(L);
		return 1;
	}
	
	cc8 *val = self->Lookup(key);
	lua_pushstring(L, val);
	lua_pushboolean(L, val != key);
	
	return 2;
}

//================================================================//
// MOAITextBundle
//================================================================//

//----------------------------------------------------------------//
MOAITextBundle::MOAITextBundle ()
: mData(0)
{
	RTTI_SINGLE ( MOAILuaObject )

	Clear();
}

//----------------------------------------------------------------//
MOAITextBundle::~MOAITextBundle () {
	Clear();
}

//----------------------------------------------------------------//
void MOAITextBundle::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED(state);
}

//----------------------------------------------------------------//
void MOAITextBundle::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "load",			_load },
		{ "lookup",			_lookup },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextBundle::Clear ( ) {
	if( this->mData )
	{
		delete [] (char *)this->mData;
		this->mData = 0;
	}
	this->mReversed = false;
	this->mNumStrings = 0;
	this->mKTableOffset = 0;
	this->mVTableOffset = 0;
	this->mNumHashEntries = 0;
	this->mHashOffset = 0;
}

//----------------------------------------------------------------//
bool MOAITextBundle::Load(const char *filename) {
	Clear();

	if( filename == 0 ) {
		return false;
	}
	
	MOAIDataBuffer buf;
	if( !buf.Load ( filename ) ) {
		// TODO: log error message?
		return false;
	}
	
	return this->Load(&buf);
}

//----------------------------------------------------------------//
bool MOAITextBundle::Load(MOAIDataBuffer *buffer) {
	
	Clear();
	if( buffer == 0 ) {
		return false;
	}
	
	void *bytes;
	size_t nbytes;
	
	buffer->Lock(&bytes, &nbytes);
	
	if( nbytes > 0 ) {
		mData = (void *)( new char[nbytes] );
		memcpy(mData, bytes, nbytes);
	}
	
	buffer->Unlock();
	
	if( mData == 0 )
		return false;

	const unsigned long MAGIC = 0x950412DE;
	const unsigned long MAGIC_REVERSED = 0xDE120495;
	
	unsigned long magic = ((unsigned long *)mData)[0];
	if( magic == MAGIC ) {
		mReversed = false;
	}
	else if( magic == MAGIC_REVERSED ) {
		mReversed = true;
	}
	else {
		// Bad file?
		Clear();
		return false;
	}
	
	mNumStrings = readInt4(8);
	mKTableOffset = readInt4(12);
	mVTableOffset = readInt4(16);
	mNumHashEntries = readInt4(20);
	mHashOffset = readInt4(24);
	
	if( mNumHashEntries == 0 ) {
		// We require an embedded hash table
		Clear();
		return false;
	}
	
	return true;
}

//----------------------------------------------------------------//
int MOAITextBundle::GetIndex(const char *key) {
	unsigned long V = hashpjw(key);
	int S = this->mNumHashEntries;
	
	int cursor = V % S;
	int cursor0 = cursor;
	int inc = 1 + (V % (S - 2));
	
	bool more = true;
	while (more) {
		int idx = readInt4(this->mHashOffset + 4 * cursor);
		if( idx == 0 ) {
			break;
		}
		
		idx--; // Indexes are stored as +1, so 0 indicates empty
		
		const char *s = this->GetKeyString(idx);
		if( strcmp(key, s) == 0 ) {
			return idx;
		}
		
		cursor += inc;
		cursor %= S;
		
		if( cursor == cursor0 ) {
			break;
		}
		
	}
	
	return -1;
}

//----------------------------------------------------------------//
const char *MOAITextBundle::GetKeyString(int idx) {
	int offset = mKTableOffset + 8 * idx + 4;
	offset = readInt4(offset);
	return ((const char *)mData) + offset;
}

//----------------------------------------------------------------//
const char *MOAITextBundle::GetValueString(int idx) {
	int offset = mVTableOffset + 8 * idx + 4;
	offset = readInt4(offset);
	return ((const char *)mData) + offset;
}

//----------------------------------------------------------------//
const char *MOAITextBundle::Lookup(const char *key) {
	
	if( this->mData == 0 ) {
		return key;
	}
	
	int idx = this->GetIndex(key);
	if( idx == -1 )
	{
		// TODO: log error/warning here?
		return key;
	}
	
	return this->GetValueString(idx);
}

