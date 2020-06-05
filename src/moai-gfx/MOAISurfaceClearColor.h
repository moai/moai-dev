// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACECLEARCOLOR_H
#define	MOAISURFACECLEARCOLOR_H

class MOAIColor;

//================================================================//
// MOAISurfaceClearColor
//================================================================//
class MOAISurfaceClearColor :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32								mClearFlags;
	u32								mClearColor;
	MOAILuaSharedPtr < MOAIColor >	mClearColorNode;

	//----------------------------------------------------------------//
	static int		_setClearColor				( lua_State* L );
	static int		_setClearDepth				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	void			ClearSurface				() const;
					MOAISurfaceClearColor		();
					~MOAISurfaceClearColor		();
	void			SetClearColor				( MOAIColor* color );
};

#endif
