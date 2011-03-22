// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPRITE2D_H
#define	MOAISPRITE2D_H

#include <moaicore/MOAIGfxPrim2D.h>

class MOAILayer2D;

//================================================================//
// MOAISprite2D
//================================================================//
/**	@brief Graphics primitive for rendering 2D images.
*/
class MOAISprite2D :
	public MOAIGfxPrim2D {
private:

	USRef < MOAITransform2D >		mUVTransform;
	
	u32								mContentAddr;
	
	//----------------------------------------------------------------//
	static int	_getContentAddr		( lua_State* L );
	static int	_inside				( lua_State* L );
	static int	_setContentAddr		( lua_State* L );
	static int	_setUVTransform		( lua_State* L );

public:

	friend class MOAILayer2D;
	
	enum {
		ATTR_CONTENT_ADDR = MOAIGfxPrim2D::TOTAL_ATTR,
		TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAISprite2D )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes			();
	void			Draw					();
	void			DrawDebug				();
	void			GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	u32				GetLocalFrame			( USRect& frame );
	bool			Inside					( USVec2D vec );
	bool			Inside					( USVec2D vec, MOAILayer2D& scene );
					MOAISprite2D			();
					~MOAISprite2D			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
