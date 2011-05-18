// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICAMERAFITTER2D_H
#define MOAICAMERAFITTER2D_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAITransformBase.h>

class MOAICameraAnchor2D;
class MOAIViewport;

//================================================================//
// MOAICameraFitter2D
//================================================================//
class MOAICameraFitter2D :
	public MOAIAction,
	public MOAITransformBase {
private:

	USRect mViewRect;

	USRef < MOAIViewport > mViewport;

	typedef STLSet < MOAICameraAnchor2D* >::iterator AnchorIt;
	STLSet < MOAICameraAnchor2D* > mAnchors;

	USVec2D		mTargetLoc;
	float		mTargetScale;

	USRect		mBounds;
	float		mMin;
	float		mMax;

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_insertAnchor			( lua_State* L );
	static int		_removeAnchor			( lua_State* L );
	static int		_setBounds				( lua_State* L );
	static int		_setMin					( lua_State* L );
	static int		_setViewport			( lua_State* L );

	//----------------------------------------------------------------//
	void			Center					();
	void			Fit						();
	void			GetCamera				( USAffine2D& camera );
	USRect			GetScreenRect			( const USAffine2D& worldToWnd );
	USRect			GetWorldRect			();
	void			OnDepNodeUpdate			();
	void			OnUpdate				( float step );
	void			SetTarget				( const USAffine2D& camera, const USRect& screenRect );

public:

	DECL_LUA_FACTORY ( MOAICameraFitter2D )

	//----------------------------------------------------------------//
	void			Clear					();
	void			AddAnchor				( MOAICameraAnchor2D& anchor );
	bool			IsDone					();
					MOAICameraFitter2D		();
					~MOAICameraFitter2D		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			RemoveAnchor			( MOAICameraAnchor2D& anchor );
	STLString		ToString				();
};

#endif
