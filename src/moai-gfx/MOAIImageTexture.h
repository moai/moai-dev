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
	void			MOAIImage_OnImageStatusChanged		( bool isOK );
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:

	//----------------------------------------------------------------//
					MOAIImageTexture			();
					~MOAIImageTexture			();
	void			UpdateRegion				();
	void			UpdateRegion				( ZLIntRect rect );
};

#endif
