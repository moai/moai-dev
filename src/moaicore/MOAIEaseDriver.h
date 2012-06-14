// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIEASEDRIVER_H
#define MOAIEASEDRIVER_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITimer.h>
#include <moaicore/MOAIWeakPtr.h>

class MOAINode;

//================================================================//
// MOAIEaseDriverLink
//================================================================//
class MOAIEaseDriverLink {
public:
	
	MOAIWeakPtr < MOAINode >	mSource;
	u32							mSourceAttrID;
	
	MOAIWeakPtr < MOAINode >	mDest;
	u32							mDestAttrID;
	
	float						mV0;
	float						mV1;
	u32							mMode;
};

//================================================================//
// MOAIEaseDriver
//================================================================//
/**	@name MOAIEaseDriver
	@text Action that applies simple ease curves to node attributes.
*/
class MOAIEaseDriver :
	public MOAITimer {
private:

	USLeanArray < MOAIEaseDriverLink > mLinks;

	//----------------------------------------------------------------//
	static int		_reserveLinks		( lua_State* L );
	static int		_setLink			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIEaseDriver )

	//----------------------------------------------------------------//
					MOAIEaseDriver		();
					~MOAIEaseDriver		();
	void			OnUpdate			( float step );
	u32				ParseForMove		( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... );
	u32				ParseForSeek		( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveLinks		( u32 total );
	void			SetLink				( u32 idx, MOAINode* dest, u32 destAttrID, float v1, u32 mode );
	void			SetLink				( u32 idx, MOAINode* dest, u32 destAttrID, MOAINode* source, u32 sourceAttrID, u32 mode );
};

#endif
