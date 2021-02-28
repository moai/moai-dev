// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIEASEDRIVER_H
#define MOAIEASEDRIVER_H

#include <moai-core/MOAITimer.h>

class MOAINode;

//================================================================//
// MOAIEaseDriverLink
//================================================================//
class MOAIEaseDriverLink {
public:
	
	MOAILuaSharedPtr < MOAINode >	mSource;
	ZLAttrID						mSourceAttrID;
	
	MOAILuaSharedPtr < MOAINode >	mDest;
	ZLAttrID						mDestAttrID;
	
	float							mV0;
	float							mV1;
	u32								mMode;
};

//================================================================//
// MOAIEaseDriver
//================================================================//
/**	@lua MOAIEaseDriver
	@text Action that applies simple ease curves to node attributes.
*/
class MOAIEaseDriver :
	public MOAITimer {
private:

	ZLLeanArray < MOAIEaseDriverLink > mLinks;

	//----------------------------------------------------------------//
	static int		_reserveLinks			( lua_State* L );
	static int		_setLink				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAction_Update		( double step );

public:

	DECL_LUA_FACTORY ( MOAIEaseDriver )

	//----------------------------------------------------------------//
					MOAIEaseDriver			( ZLContext& context );
					~MOAIEaseDriver			();
	u32				ParseForMove			( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... );
	u32				ParseForSeek			( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... );
	void			ReserveLinks			( u32 total );
	void			SetLink					( ZLIndex idx, MOAINode* dest, ZLAttrID destAttrID, float v1, u32 mode );
	void			SetLink					( ZLIndex idx, MOAINode* dest, ZLAttrID destAttrID, MOAINode* source, ZLAttrID sourceAttrID, u32 mode );
};

#endif
