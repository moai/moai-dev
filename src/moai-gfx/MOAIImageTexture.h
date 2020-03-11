// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTURE_H
#define	MOAIIMAGETEXTURE_H

#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIImageTexture
//================================================================//
class MOAIImageTexture :
	public virtual MOAIImage,
	public virtual MOAITexture {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLIntRect		mRegion;

	//----------------------------------------------------------------//
	static int		_updateRegion			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIImage_OnImageStatusChanged		( bool isOK );

public:

	//----------------------------------------------------------------//
					MOAIImageTexture			();
					~MOAIImageTexture			();
	void			UpdateRegion				();
	void			UpdateRegion				( ZLIntRect rect );
};

#endif
