// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIASYNCACTION_H
#define MOAIASYNCACTION_H

#include <moaicore/MOAIAction.h>

class MOAIData;

//================================================================//
// MOAIDataIOAction
//================================================================//
/**	@brief Action for asynchronously loading data.
*/
class MOAIDataIOAction :
	public MOAIAction {
private:

	enum {
		IDLE,
		READY,
		LOADING,
		DONE,
	};

	STLString				mFilename;
	USRef < MOAIData >		mData;
	USLuaRef				mOnFinish;

	u32						mState;

	//----------------------------------------------------------------//
	static int	_setCallback		( lua_State* L );

	//----------------------------------------------------------------//
	void		Load				();
	void		LoadFinished		( USDataIOTask* task );
	void		SetBusy				( bool isBusy );

public:

	DECL_LUA_DATA ( MOAIDataIOAction )

	//----------------------------------------------------------------//
	void		Init				( cc8* filename, MOAIData* data );
	bool		IsBusy				();
				MOAIDataIOAction	();
				~MOAIDataIOAction	();
	void		OnUpdate			( float step );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
};

#endif
