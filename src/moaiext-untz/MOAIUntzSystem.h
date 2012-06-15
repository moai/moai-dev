// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSYSTEM_H
#define	MOAIUNTZSYSTEM_H

#include <moaicore/moaicore.h>
#include <UntzSound.h>

//================================================================//
// MOAIUntzSystem
//================================================================//
/**	@name	MOAIUntzSystem
	@text	Untz system singleton.
*/
class MOAIUntzSystem :
	public MOAIGlobalClass < MOAIUntzSystem, MOAILuaObject > {
private:

	static const u32 DEFAULT_SAMPLE_RATE		= 44100;
	static const u32 DEFAULT_FRAMES_PER_BUFFER	= 8192;
	static const u32 DEFAULT_OPTIONS			= 0;

	//----------------------------------------------------------------//
	static int		_getDeviceCount			( lua_State* L );
	static int		_getDeviceInfo			( lua_State* L );
	static int		_getOptions				( lua_State* L );
	static int		_getSampleRate			( lua_State* L );
	static int		_getSupportedFormats	( lua_State* L );
	static int		_initialize				( lua_State* L );
	static int		_setInputDevice			( lua_State* L );
	static int		_setOptions				( lua_State* L );
	static int		_setOutputDevice		( lua_State* L );
	static int		_setSampleRate			( lua_State* L );
	static int		_setVolume				( lua_State* L );
	static int		_getVolume				( lua_State* L );
public:

	DECL_LUA_SINGLETON ( MOAIUntzSystem )

	//----------------------------------------------------------------//
					MOAIUntzSystem		();
					~MOAIUntzSystem		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Resume				();
	void			Suspend				();
};

#endif