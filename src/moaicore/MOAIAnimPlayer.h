// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMPLAYER_H
#define	MOAIANIMPLAYER_H

#include <moaicore/MOAITimer.h>

class MOAIAnim;
class MOAINode;

//================================================================//
// MOAIAnimLink
//================================================================//
class MOAIAnimLink {
public:

	u32					mCurveID;
	USRef < MOAINode >	mTarget;
	u32					mAttrID;
	bool				mRelative;
};

//================================================================//
// MOAIAnimPlayer
//================================================================//
/**	@brief Animation playback class.
*/
class MOAIAnimPlayer :
	public MOAITimer {
private:

	USRef < MOAIAnim > mAnim;
	USLeanArray < MOAIAnimLink > mLinks;

	//----------------------------------------------------------------//
	static int	_apply				( lua_State* L );
	static int	_reserveLinks		( lua_State* L );
	static int	_setAnim			( lua_State* L );
	static int	_setLink			( lua_State* L );
	
	//----------------------------------------------------------------//
	
public:
	
	DECL_LUA_DATA ( MOAIAnimPlayer )
	
	//----------------------------------------------------------------//
	void		Apply				( float t );
				MOAIAnimPlayer		();
				~MOAIAnimPlayer		();
	void		OnUpdate			( float step );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
	void		ReserveLinks		( u32 totalLinks );
	void		SetLink				( u32 linkID, u32 curveID, MOAINode* target, u32 attrID, bool relative );
	STLString	ToString			();
};

#endif
