// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISPINESLOT_H
#define MOAISPINESLOT_H

#include <spine/spine.h>

//================================================================//
// MOAISpineSlot
//================================================================//
/**	@name	MOAISpineSlot
	@text	MOAIColor that is bound to spine bone

*/
class MOAISpineSlot :
	public MOAIColor {
private:
	
	friend class MOAISpineSkeleton;
		
	spSlot* mSlot;
	bool mLocked;
	
	//----------------------------------------------------------------//
	static int		_lockColor		( lua_State* L );
	static int		_setColor		( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAISpineSlot )
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			ClearLock				();
	void			LockColor				( bool flag );
					MOAISpineSlot			();
					~MOAISpineSlot			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetSlot					( spSlot* slot );
};

#endif
