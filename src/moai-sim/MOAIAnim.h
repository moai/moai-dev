// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIM_H
#define	MOAIANIM_H

#include <moai-sim/MOAITimer.h>

class MOAIAnimCurveBase;

//================================================================//
// MOAIAnimLink
//================================================================//
class MOAIAnimLink {
public:

	MOAILuaSharedPtr < MOAIAnimCurveBase > mCurve;
	MOAILuaSharedPtr < MOAINode > mTarget;
	u32 mAttrID;
	bool mRelative;
};

//================================================================//
// MOAIAnim
//================================================================//
/**	@lua MOAIAnim
	@text Bind animation curves to nodes and provides timer controls for animation playback.
*/
class MOAIAnim :
	public virtual MOAITimer {
private:

	float mLength;

	ZLLeanArray < MOAIAnimLink > mLinks;

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
	void			ClearLinks			();
					MOAIAnim			();
					~MOAIAnim			();
	void			OnUpdate			( double step );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveLinks		( u32 totalLinks );
	void			SetLink				( u32 linkID, MOAIAnimCurveBase* curve, MOAINode* target, u32 attrID, bool relative );
};

#endif
