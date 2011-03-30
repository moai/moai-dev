// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moaicore/MOAIDeck.h>

class MOAITexture;
class MOAIVertexBuffer;

//================================================================//
// MOAIMesh
//================================================================//
/**	@brief Texture class.
*/
class MOAIMesh :
	public MOAIDeck {
private:

	USRef < MOAITexture	> mTexture;
	USRef < MOAIVertexBuffer > mVertexBuffer;

	//----------------------------------------------------------------//
	static int		_setTexture			( lua_State* L );
	static int		_setVertexBuffer	( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIMesh )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	void			Draw					( const USAffine2D& transform, u32 idx );
	USRect			GetBounds				( u32 idx );
					MOAIMesh				();
					~MOAIMesh				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
