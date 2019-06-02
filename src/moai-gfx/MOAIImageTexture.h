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
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void			UpdateRegion				();
	void			UpdateRegion				( ZLIntRect rect );
};

#endif
