// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBLOCKER_H
#define	MOAIBLOCKER_H

#include <moai-core/MOAILua.h>

//================================================================//
// MOAIBlocker
//================================================================//
class MOAIBlocker :
	public virtual MOAILuaObject {
private:

	MOAIBlocker*		mBlocker;
	MOAIBlocker*		mBlockedList;
	MOAIBlocker*		mNextBlocked;

	//----------------------------------------------------------------//
	void				AddBlocked				( MOAIBlocker* blocked );
	void				RemoveBlocked			( MOAIBlocker* blocked );

	//----------------------------------------------------------------//
	virtual void		MOAIBlocker_Unblock		();

public:

	friend class MOAIAction;

	//----------------------------------------------------------------//
	bool				IsBlocked				();
						MOAIBlocker				();
	virtual				~MOAIBlocker			();
	void				SetBlocker				( MOAIBlocker* blocker );
	void				UnblockAll				();
	void				UnblockSelf				();
};

#endif
