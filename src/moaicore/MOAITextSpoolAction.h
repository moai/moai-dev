// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITEXTSPOOLACTION_H
#define MOAITEXTSPOOLACTION_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAITextBox.h>

//================================================================//
// MOAITextSpoolAction
//================================================================//
/**	@brief Text spooling action.
*/
class MOAITextSpoolAction :
	public MOAIAction {
private:

	static const float DEFAULT_SPOOL_SPEED;

	float			mSpoolSpeed;
	float			mThrottle;

	USRef < MOAITextBox > mTextBox;

	//----------------------------------------------------------------//
	static int		_more					( lua_State* L );
	static int		_nextPage				( lua_State* L );
	static int		_setSpeed				( lua_State* L );
	static int		_start					( lua_State* L );
	static int		_throttle				( lua_State* L );

public:

	DECL_LUA_DATA ( MOAITextSpoolAction )

	SET ( MOAITextBox*, TextBox, mTextBox )

	//----------------------------------------------------------------//
	bool			IsBusy					();
					MOAITextSpoolAction		();
					~MOAITextSpoolAction	();
	bool			More					();
	void			OnUpdate				( float step );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
