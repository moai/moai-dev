// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPINTRANSFORM_H
#define	MOAIPINTRANSFORM_H

#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAINode.h>

class MOAILayer;

//================================================================//
// MOAIPinTransform
//================================================================//
/**	@lua	MOAIPinTransform
	@text	2D transform for connecting transforms across layers. Useful for
			HUD overlay items and map pins.
*/
class MOAIPinTransform :
	public MOAITransform {
private:

	MOAILuaSharedPtr < MOAILayer > mSourceLayer;
	MOAILuaSharedPtr < MOAILayer > mDestLayer;
		
	float			mFront;
	
	//----------------------------------------------------------------//
	static int		_init						( lua_State* L );

	//----------------------------------------------------------------//
	void			OnDepNodeUpdate				();

public:
	
	DECL_LUA_FACTORY ( MOAIPinTransform )
	DECL_ATTR_HELPER ( MOAIPinTransform )
		
	enum {
		ATTR_FRONT
	};
		
	//----------------------------------------------------------------//
		
	virtual bool	ApplyAttrOp					( u32 attrID, MOAIAttrOp& attrOp, u32 op );
					MOAIPinTransform			();
					~MOAIPinTransform			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
