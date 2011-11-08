// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASK_H
#define MOAIHTTPTASK_H

#include <moaicore/MOAILua.h>

#define DEFAULT_MOAI_HTTP_USERAGENT "Moai SDK beta; support@getmoai.com"

class MOAIHttpTaskInfo;

//================================================================//
// MOAIHttpTask
//================================================================//
class MOAIHttpTask :
	public virtual MOAIObject {
private:

	MOAIHttpTaskInfo*	mInfo;

	void*				mData;
	u32					mSize;

	bool				mVerbose;
	u32					mResponseCode;

	MOAILuaLocal		mOnFinish;

	//----------------------------------------------------------------//
	static int		_getSize			( lua_State* L );
	static int		_getString			( lua_State* L );
	static int		_httpGet			( lua_State* L );
	static int		_httpPost			( lua_State* L );
	static int		_parseXml			( lua_State* L );
	static int		_setCallback		( lua_State* L );

	//----------------------------------------------------------------//
	void			Finish				();
					MOAIHttpTask		( const MOAIHttpTask& task );

public:

	friend class MOAIUrlMgr;
	
	GET ( u32, ResponseCode, mResponseCode )
	
	GET ( void*, Data, mData )
	GET ( u32, Size, mSize )
	
	DECL_LUA_FACTORY ( MOAIHttpTask )
	
	//----------------------------------------------------------------//
	void			Clear					();	
	void			GetData					( void* buffer, u32 size );
	void			HttpGet					( cc8* url, cc8* useragent, bool verbose );
	void			HttpPost				( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose );
					MOAIHttpTask			();
					~MOAIHttpTask			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
