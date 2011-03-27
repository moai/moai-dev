// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMDRIVER_H
#define	MOAIANIMDRIVER_H

#include <moaicore/MOAITimer.h>

class MOAIAnim;

//================================================================//
// MOAIAnimDriver
//================================================================//
/**	@brief Animation playback class.
*/
class MOAIAnimDriver :
	public MOAITimer {
private:

	USRef < MOAIAnim > mAnim;
	
	//----------------------------------------------------------------//
	static int		_setAnim			( lua_State* L );
	
public:
	
	DECL_LUA_DATA ( MOAIAnimDriver )
	
	//----------------------------------------------------------------//
					MOAIAnimDriver		();
					~MOAIAnimDriver		();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
