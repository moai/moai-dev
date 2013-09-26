// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPINTRANSFORM_H
#define	MOAIPINTRANSFORM_H

#include <moai-sim/MOAITransform.h>

class MOAILayer;

//================================================================//
// MOAIPinTransform
//================================================================//
/**	@name	MOAIPinTransform
	@text	2D transform for connecting transforms across layers. Useful for
			HUD overlay items and map pins.
*/
class MOAIPinTransform :
	public MOAITransform {
private:

	MOAILuaSharedPtr < MOAITransformBase > mSourceTransform;
	MOAILuaSharedPtr < MOAILayer > mSourceLayer;
	MOAILuaSharedPtr < MOAILayer > mDestLayer;

	//----------------------------------------------------------------//
	static int		_init						( lua_State* L );

	//----------------------------------------------------------------//
	void			OnDepNodeUpdate				();

public:
	
	DECL_LUA_FACTORY ( MOAIPinTransform )
	
	//----------------------------------------------------------------//
					MOAIPinTransform				();
					~MOAIPinTransform			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
