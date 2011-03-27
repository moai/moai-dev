// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYER2D_H
#define	MOAILAYER2D_H

#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAIViewport.h>

class MOAICpSpace;
class MOAIBox2DWorld;

//================================================================//
// MOAILayer2D
//================================================================//
/**	@brief 2D scene controls class.
*/
class MOAILayer2D :
	public virtual MOAIProp2D {
private:

	USRef < MOAITransform2D >	mCamera;
	USRef < MOAIViewport >		mViewport;
	USRef < MOAIPartition >		mPartition;

	#if USE_CHIPMUNK
		USRef < MOAICpSpace >		mCpSpace;
	#endif
	
	#if USE_BOX2D
		USRef < MOAIBox2DWorld >	mBox2DWorld;
	#endif

	USVec2D						mParallax;

	bool						mShowDebugLines;

	//----------------------------------------------------------------//
	static int	_getFitting			( lua_State* L );
	static int	_getPartition		( lua_State* L );
	static int	_insertPrim			( lua_State* L );
	static int	_removePrim			( lua_State* L );
	static int	_setBox2DWorld		( lua_State* L );
	static int	_setCamera			( lua_State* L );
	static int	_setCpSpace			( lua_State* L );
	static int	_setParallax		( lua_State* L );
	static int	_setPartition		( lua_State* L );
	static int	_setViewport		( lua_State* L );
	static int	_showDebugLines		( lua_State* L );
	static int	_wndToWorld			( lua_State* L );
	static int	_worldToWnd			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AffirmPartition			();
	USAffine2D		GetViewProjMtx			();

public:
	
	DECL_LUA_DATA ( MOAILayer2D )
	
	GET_SET ( MOAITransform2D*, Camera, mCamera )
	GET_SET ( MOAIViewport*, Viewport, mViewport )
	GET_SET ( MOAIPartition*, Partition, mPartition )
	
	//----------------------------------------------------------------//
	void			Draw					();
	float			GetFitting				( USRect& worldRect, float hPad, float vPad );
	u32				GetLocalFrame			( USRect& frame );
	void			GetWndToWorldMtx		( USAffine2D& wndToWorld );
	void			GetWorldToWndMtx		( USAffine2D& worldToWnd );
					MOAILayer2D			();
					~MOAILayer2D			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
