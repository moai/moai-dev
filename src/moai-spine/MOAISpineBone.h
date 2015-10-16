// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISPINEBONE_H
#define MOAISPINEBONE_H

#include <spine/spine.h>

//================================================================//
// MOAISpineBone
//================================================================//
/**	@name	MOAISpineBone
	@text	MOAITransform that is bound to spine bone

*/
class MOAISpineBone :
	public MOAITransform {
private:
	
	friend class MOAISpineSkeleton;
		
	spBone* mBone;
	MOAITransform* mRootTransform;
	u32 mLockFlags;
	
	//----------------------------------------------------------------//
	static int		_clearLock		( lua_State* L );
	static int		_lockTransform	( lua_State* L );
	
	enum {
		LOCK_LOC = 0x01,
		LOCK_SCL = 0x02,
		LOCK_ROT = 0x04,
		LOCK_ALL = 0x07,
	};
		
public:
	
	DECL_LUA_FACTORY ( MOAISpineBone )

	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			ClearLock				();
	void			LockTransform			( u32 flags );
					MOAISpineBone			();
					~MOAISpineBone			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetAsRootBone			( MOAITransform* rootTransform );
	void			SetBone					( spBone* bone );
};

#endif
