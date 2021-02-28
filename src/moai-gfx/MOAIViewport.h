// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWPORT_H
#define	MOAIVIEWPORT_H

#include <moai-gfx/MOAITransform.h>

//================================================================//
// MOAIViewport
//================================================================//
/**	@lua	MOAIViewport
	@text	Viewport object.
*/
class MOAIViewport :
	public virtual ZLViewport,
	public virtual MOAITransform {
private:

	//----------------------------------------------------------------//
	static int		_getViewFrame			( lua_State* L );
	static int		_getViewSize			( lua_State* L );
	static int		_setViewOffset			( lua_State* L );
	static int		_setViewRotation		( lua_State* L );
	static int		_setViewScale			( lua_State* L );
	static int		_setViewSize			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIViewport )
	
	friend class MOAICamera;
	
	//----------------------------------------------------------------//
	ZLViewport		GetWorldViewport		() const;
					MOAIViewport			( ZLContext& context );
					~MOAIViewport			();
};

#endif
