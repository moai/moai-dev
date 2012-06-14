// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITEXTBUNDLE_H
#define MOAITEXTBUNDLE_H

#include <moaicore/MOAILua.h>

class MOAIDataBuffer;


//================================================================//
// MOAITextBundle
//================================================================//
/**	@name	MOAITextBundle
 @text	A read-only lookup table of strings suitable for internationalization purposes. This
 currently wraps a loaded gettext() style MO file (see http://www.gnu.org/software/gettext/manual/gettext.html).
 So you are going to want to generate the .mo file from one of the existing tools such as poedit or msgfmt,
 and then load that file using this class. Then you can lookup strings using MOAITextBundle->Lookup().
 */

class MOAITextBundle :
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_load			( lua_State* L );
	static int		_lookup			( lua_State* L );
		
	void * mData;
	bool mReversed;
	int mNumStrings;
	int mKTableOffset;
	int mVTableOffset;
	int mNumHashEntries;
	int mHashOffset;
	
	inline int readInt4(int offset) {
		unsigned long *ptr = (unsigned long *)(((char *)this->mData) + offset);
		
		if( this->mReversed ) {
			unsigned long v = *ptr;
			unsigned long c0 = (v >> 0) & 0xff;
			unsigned long c1 = (v >> 8) & 0xff;
			unsigned long c2 = (v >> 16) & 0xff;
			unsigned long c3 = (v >> 24) & 0xff;
			return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
		}
		else {
			return *ptr;
		}
	}
		
	const char *			GetKeyString(int idx);
	const char *			GetValueString(int idx);
	int						GetIndex(const char *key);
		
public:
	
	DECL_LUA_FACTORY ( MOAITextBundle )
	
	//----------------------------------------------------------------//
	MOAITextBundle			();
	~MOAITextBundle			();
	void					Clear();
	bool					Load(const char *filename);
	bool					Load(MOAIDataBuffer *buffer);
	const char *			Lookup(const char *key);
		
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};


#endif