// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBLOCKER_H
#define	MOAIBLOCKER_H

#include <moaicore/MOAILua.h>

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
	virtual void		OnUnblock				();
	void				RemoveBlocked			( MOAIBlocker* blocked );

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
