// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSOUND_H
#define	MOAIUNTZSOUND_H

#include <moai-sim/headers.h>
#include <moai-untz/MOAIUntzSound.h>
#include <UntzSound.h>

class MOAIDataBuffer;

//================================================================//
// MOAIUntzSound
//================================================================//
/**	@lua	MOAIUntzSound
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

	//----------------------------------------------------------------//
	bool			MOAINode_ApplyAttrOp	( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIUntzSound )
	DECL_ATTR_HELPER ( MOAIUntzSound )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
					MOAIUntzSound		();
					~MOAIUntzSound		();
	void			MOAILuaObject_RegisterLuaClass	( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs	( MOAIComposer& composer, MOAILuaState& state );		
};

#endif
