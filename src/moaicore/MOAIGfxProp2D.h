// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXPROP2D_H
#define	MOAIGFXPROP2D_H

#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAITransform2D.h>

//================================================================//
// MOAIGfxProp2D
//================================================================//
/**	@brief Partial implementation for renderable primitive.
*/
class MOAIGfxProp2D :
	public virtual MOAIProp2D {
protected:

	USRef < MOAIShader >		mShader;
	USRef < MOAITransform2D >	mUVTransform;
	
	//----------------------------------------------------------------//
	static int		_setShader				( lua_State* L );
	static int		_setUVTransform			( lua_State* L );
	
	//----------------------------------------------------------------//
	bool			BindDeck				();
	void			LoadShader				();
	
public:
	
	DECL_LUA_DATA ( MOAIGfxProp2D )
	
	//----------------------------------------------------------------//
	
	void			Draw					();
	void			DrawDebug				();
	void			GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	u32				GetLocalFrame			( USRect& frame );
					MOAIGfxProp2D			();
					~MOAIGfxProp2D			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
