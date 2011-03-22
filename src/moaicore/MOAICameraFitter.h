// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERAFITTER_H
#define	MOAICAMERAFITTER_H

#include <moaicore/MOAITransform2D.h>

//================================================================//
// MOAICameraFitter
//================================================================//
/**	@brief 2D camera fitter.
*/
class MOAICameraFitter :
	public virtual USLuaData {
private:

	USCamera2D*		mCamera;
	USViewport*		mViewport;

	USCameraFitter	mFitter;

	//----------------------------------------------------------------//
	static int	_addPin			( lua_State* L );
	static int	_center			( lua_State* L );
	static int	_fit			( lua_State* L );
	static int	_reset			( lua_State* L );
	static int	_setCamera		( lua_State* L );
	static int	_setViewport	( lua_State* L );

public:
	
	DECL_LUA_DATA ( MOAICameraFitter )
	
	GET_SET ( USCamera2D*, Camera, mCamera )
	GET_SET ( USViewport*, Viewport, mViewport )
	
	//----------------------------------------------------------------//
	void		AddPin					( float x, float y, float radius );
	void		Center					();
	void		Fit						();
				MOAICameraFitter		();
				~MOAICameraFitter		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		Reset					();
	STLString	ToString				();
};

#endif
