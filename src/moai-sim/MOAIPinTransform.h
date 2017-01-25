// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPINTRANSFORM_H
#define	MOAIPINTRANSFORM_H

#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAINode.h>

class MOAILayerBase;

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

	MOAILuaSharedPtr < MOAILayerBase > mSourceLayer;
	MOAILuaSharedPtr < MOAILayerBase > mDestLayer;
		
	float			mFront;
	
	//----------------------------------------------------------------//
	static int		_init						( lua_State* L );

	//----------------------------------------------------------------//
	bool			MOAINode_ApplyAttrOp		( u32 attrID, MOAIAttribute& attr, u32 op );
	void			MOAINode_Update				();

public:
	
	DECL_LUA_FACTORY ( MOAIPinTransform )
	DECL_ATTR_HELPER ( MOAIPinTransform )
		
	enum {
		ATTR_FRONT,
	};
		
	//----------------------------------------------------------------//
					MOAIPinTransform			();
					~MOAIPinTransform			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
