// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSOUND_H
#define	MOAIUNTZSOUND_H

#include <moaicore/moaicore.h>
#include <moaiext-untz/MOAIUntzSound.h>
#include <UntzSound.h>

class MOAIDataBuffer;

//================================================================//
// MOAIUntzSound
//================================================================//
/**	@name	MOAIUntzSound
	@text	Untz sound object.
	
	@attr	ATTR_VOLUME
*/
class MOAIUntzSound :
	public virtual MOAINode {
private:

	UNTZ::Sound* mSound;
	STLString mFilename;
	bool mInMemory;

	//----------------------------------------------------------------//
	static int		_getLength			( lua_State* L );
	static int		_getPosition		( lua_State* L );
	static int		_getVolume			( lua_State* L );
	static int		_getFilename		( lua_State* L );
	static int		_isLooping			( lua_State* L );
	static int		_isPaused			( lua_State* L );
	static int		_isPlaying			( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_moveVolume			( lua_State* L );
	static int		_pause				( lua_State* L );
	static int		_play				( lua_State* L );
	static int		_seekVolume			( lua_State* L );
	static int		_setLooping			( lua_State* L );
	static int		_setLoopPoints		( lua_State* L );
	static int		_setPosition		( lua_State* L );
	static int		_setVolume			( lua_State* L );
	static int		_stop				( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIUntzSound )
	DECL_ATTR_HELPER ( MOAIUntzSound )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
					MOAIUntzSound		();
					~MOAIUntzSound		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );		
};

#endif
