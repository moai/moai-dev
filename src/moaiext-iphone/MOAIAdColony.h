//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONY_H
#define MOAIADCOLONY_H

#include <moaicore/moaicore.h>
#import <AdColonyPublic.h>

//================================================================//
// MOAIAdColony
//================================================================//
class MOAIAdColony :
public MOAIGlobalClass < MOAIAdColony, MOAILuaObject > {
private:
    
	id < AdColonyDelegate > mAdColonyDelegate;
	
	//----------------------------------------------------------------//
	static int		_initAdColony                   ( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIAdColony );
	
                    MOAIAdColony					();
					~MOAIAdColony                   ();
	static void		RegisterAdColony				();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			SetAdColonyDelegate				( id < AdColonyDelegate > delegate );
};

#endif
