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

	ZLIntRect		mRegion;

	//----------------------------------------------------------------//
	static int		_updateRegion			( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAIImage_OnImageStatusChanged		( bool isOK );

public:

	//----------------------------------------------------------------//
					MOAIImageTexture			();
					~MOAIImageTexture			();
	void			MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	void			UpdateRegion				();
	void			UpdateRegion				( ZLIntRect rect );
};

#endif
