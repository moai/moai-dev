// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPRITE2D_H
#define	MOAISPRITE2D_H

#include <moaicore/MOAIGfxProp2D.h>

class MOAIGfxLayer2D;

//================================================================//
// MOAISprite2D
//================================================================//
/**	@brief Graphics primitive for rendering 2D images.
*/
class MOAISprite2D :
	public MOAIGfxProp2D {
private:
	
	//----------------------------------------------------------------//

public:

	friend class MOAIGfxLayer2D;
	
	enum {
		ATTR_CONTENT_ADDR = MOAIGfxProp2D::TOTAL_ATTR,
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
	bool			Inside					( USVec2D vec, MOAIGfxLayer2D& scene );
					MOAISprite2D			();
					~MOAISprite2D			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
