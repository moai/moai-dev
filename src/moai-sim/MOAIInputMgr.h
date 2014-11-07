// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTMGR_H
#define MOAIINPUTMGR_H

#include <moai-sim/MOAIInputQueue.h>

//================================================================//
// MOAIInputMgr
//================================================================//
/**	@lua	MOAIInputMgr
	@text	Input device class.
*/
class MOAIInputMgr :
	public MOAIGlobalClass < MOAIInputMgr, MOAIInputQueue > {
public:

	DECL_LUA_SINGLETON ( MOAIInputMgr )

	//----------------------------------------------------------------//
	
	
	
	
	
	
	
	
	
						MOAIInputMgr				();
						~MOAIInputMgr				();
	void				RegisterLuaClass			( MOAILuaState& state );
};

#endif
