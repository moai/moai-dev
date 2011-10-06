// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYERBRIDGE2D_H
#define	MOAILAYERBRIDGE2D_H

#include <moaicore/MOAITransform.h>

class MOAILayer2D;

//================================================================//
// MOAILayerBridge2D
//================================================================//
/**	@name	MOAILayerBridge2D
	@text	2D transform for connecting transforms across scenes. Useful for
			HUD overlay items and map pins.
*/
class MOAILayerBridge2D :
	public MOAITransform {
private:

	USWeak < MOAILayer2D > mSourceScene;
	USWeak < MOAILayer2D > mTargetScene;

	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );

	//----------------------------------------------------------------//
	void				OnDepNodeUpdate			();

public:
	
	DECL_LUA_FACTORY ( MOAILayerBridge2D )
	
	//----------------------------------------------------------------//
					MOAILayerBridge2D			();
					~MOAILayerBridge2D			();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );
};

#endif
