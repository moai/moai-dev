// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXPRIM2D_H
#define	MOAIGFXPRIM2D_H

#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIPrim.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAITransform2D.h>

//================================================================//
// MOAIGfxPrim2D
//================================================================//
/**	@brief Partial implementation for renderable primitive.
*/
class MOAIGfxPrim2D :
	public virtual MOAIPrim {
protected:

	USRef < MOAIContentLibrary2D >	mGfxSource;
	USRef < MOAIShader >			mShader;
	
	//----------------------------------------------------------------//
	static int			_setGfxSource			( lua_State* L );
	static int			_setShader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				LoadShader				();
	
public:
	
	GET_SET ( MOAIContentLibrary2D*, ContentLibrary, mGfxSource )
	
	enum {
		ATTR_SHADER = MOAIGfxPrim2D::TOTAL_ATTR,
		TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIGfxPrim2D )
	
	//----------------------------------------------------------------//
	bool				BindGfxSource			();
	virtual void		Draw					();
						MOAIGfxPrim2D			();
						~MOAIGfxPrim2D			();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
