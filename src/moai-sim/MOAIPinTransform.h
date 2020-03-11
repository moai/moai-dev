// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPINTRANSFORM_H
#define	MOAIPINTRANSFORM_H

#include <moai-sim/MOAITransform.h>

class MOAIAbstractViewLayer;

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

	MOAILuaSharedPtr < MOAIAbstractViewLayer > mSourceLayer;
	MOAILuaSharedPtr < MOAIAbstractViewLayer > mDestLayer;
		
	ZLReal			mFront;
	ZLBounds		mWorldBounds;
	
	//----------------------------------------------------------------//
	static int		_getWorldBounds				( lua_State* L );
	static int		_getWorldBoundsCenter		( lua_State* L );
	static int		_init						( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	bool			MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update				();

public:
	
	DECL_LUA_FACTORY ( MOAIPinTransform )
	DECL_ATTR_HELPER ( MOAIPinTransform )
		
	enum {
		ATTR_FRONT,
		ATTR_INHERIT_WORLD_BOUNDS,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
					MOAIPinTransform			();
					~MOAIPinTransform			();
};

#endif
