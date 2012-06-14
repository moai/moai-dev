// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYERBRIDGE_H
#define	MOAILAYERBRIDGE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

class MOAILayer;

//================================================================//
// MOAILayerBridge
//================================================================//
/**	@name	MOAILayerBridge
	@text	2D transform for connecting transforms across scenes. Useful for
			HUD overlay items and map pins.
*/
class MOAILayerBridge :
	public MOAITransform {
private:

	MOAIWeakPtr < MOAITransformBase > mSourceTransform;
	MOAIWeakPtr < MOAILayer > mSourceLayer;
	MOAIWeakPtr < MOAILayer > mDestLayer;

	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );

	//----------------------------------------------------------------//
	void				OnDepNodeUpdate			();

public:
	
	DECL_LUA_FACTORY ( MOAILayerBridge )
	
	//----------------------------------------------------------------//
					MOAILayerBridge			();
					~MOAILayerBridge			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
