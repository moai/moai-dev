// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIM_H
#define	MOAIANIM_H

#include <moai-core/MOAITimer.h>

class MOAIAnimCurve;

//================================================================//
// MOAIAnimLink
//================================================================//
class MOAIAnimLink {
public:

	MOAILuaSharedPtr < MOAIAnimCurve >	mCurve;
	MOAILuaSharedPtr < MOAINode > 		mTarget;
	ZLAttrID							mAttrID;
	bool								mRelative;
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
	static int		_apply					( lua_State* L );
	static int		_getLength				( lua_State* L );
	static int		_reserveLinks			( lua_State* L );
	static int		_setLink				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAction_Update		( double step );
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnim )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void			Apply					( float t );
	void			Apply					( float t0, float t1 );
	void			Clear					();
	void			ClearLinks				();
					MOAIAnim				( ZLContext& context );
					~MOAIAnim				();
	void			ReserveLinks			( u32 totalLinks );
	void			SetLink					( ZLIndex linkID, MOAIAnimCurve* curve, MOAINode* target, ZLAttrID attrID, bool relative );
};

#endif
