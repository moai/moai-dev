// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIM_H
#define	MOAIANIM_H

#include <moaicore/MOAITimer.h>

class MOAIAnimCurve;

//================================================================//
// MOAIAnimLink
//================================================================//
class MOAIAnimLink {
public:

	USRef < MOAIAnimCurve >		mCurve;
	USRef < MOAINode >			mTarget;
	u32							mAttrID;
	bool						mRelative;
};

//================================================================//
// MOAIAnim
//================================================================//
/**	@brief	Bind anim curves to nodes and provides timer controls for anim playback.
*/
class MOAIAnim :
	public virtual MOAITimer {
private:

	float mLength;

	USLeanArray < MOAIAnimLink > mLinks;

	//----------------------------------------------------------------//
	static int	_apply				( lua_State* L );
	static int	_getLength			( lua_State* L );
	static int	_reserveLinks		( lua_State* L );
	static int	_setLink			( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnim )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void			Apply				( float t );
	void			Apply				( float t0, float t1 );
	void			Clear				();
					MOAIAnim			();
					~MOAIAnim			();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			ReserveLinks		( u32 totalLinks );
	void			SetLink				( u32 linkID, MOAIAnimCurve* curve, MOAINode* target, u32 attrID, bool relative );
	STLString		ToString			();
};

#endif
