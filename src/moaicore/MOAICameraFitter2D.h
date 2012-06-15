// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICAMERAFITTER2D_H
#define MOAICAMERAFITTER2D_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>

class MOAICameraAnchor2D;
class MOAITransform;
class MOAIViewport;

//================================================================//
// MOAICameraFitter2D
//================================================================//
/**	@name	MOAICameraFitter2D
	@text	Action to dynamically fit a camera transform to a set of targets
			given a viewport and world space constraints.
	
	@const	FITTING_MODE_SEEK_LOC
	@const	FITTING_MODE_SEEK_SCALE
	@const	FITTING_MODE_APPLY_ANCHORS
	@const	FITTING_MODE_APPLY_BOUNDS
	@const	FITTING_MODE_DEFAULT
	@const	FITTING_MODE_MASK
*/
class MOAICameraFitter2D :
	public MOAIAction,
	public MOAINode {
private:

	MOAILuaSharedPtr < MOAITransform >	mCamera;
	MOAILuaSharedPtr < MOAIViewport >		mViewport;

	typedef STLSet < MOAICameraAnchor2D* >::iterator AnchorIt;
	STLSet < MOAICameraAnchor2D* > mAnchors;

	USVec3D		mFitLoc;
	float		mFitScale;

	USVec3D		mTargetLoc;
	float		mTargetScale;

	USRect		mBounds;
	float		mMin;
	float		mDamper;

	u32			mFittingMode;

	//----------------------------------------------------------------//
	static int		_clearAnchors			( lua_State* L );
	static int		_clearFitMode			( lua_State* L );
	static int		_getFitDistance			( lua_State* L );
	static int		_getFitLoc				( lua_State* L );
	static int		_getFitMode				( lua_State* L );
	static int		_getFitScale			( lua_State* L );
	static int		_getTargetLoc			( lua_State* L );
	static int		_getTargetScale			( lua_State* L );
	static int		_insertAnchor			( lua_State* L );
	static int		_removeAnchor			( lua_State* L );
	static int		_setBounds				( lua_State* L );
	static int		_setCamera				( lua_State* L );
	static int		_setDamper				( lua_State* L );
	static int		_setFitLoc				( lua_State* L );
	static int		_setFitMode				( lua_State* L );
	static int		_setFitScale			( lua_State* L );
	static int		_setMin					( lua_State* L );
	static int		_setViewport			( lua_State* L );
	static int		_snapToTarget			( lua_State* L );

	//----------------------------------------------------------------//
	USRect			GetAnchorRect			();
	void			GetCamera				( USAffine3D& camera );
	void			OnDepNodeUpdate			();
	void			OnUpdate				( float step );
	void			SetTarget				( const USAffine3D& camera, const USRect& screenRect );
	void			SnapToTargetLoc			( MOAITransform& camera );
	void			SnapToTargetScale		( MOAITransform& camera );
	void			UpdateFit				();
	void			UpdateTarget			();

public:

	DECL_LUA_FACTORY ( MOAICameraFitter2D )

	enum {
		FITTING_MODE_SEEK_LOC		= 0x00000001,
		FITTING_MODE_SEEK_SCALE		= 0x00000002,
		FITTING_MODE_APPLY_ANCHORS	= 0x00000004,
		FITTING_MODE_APPLY_BOUNDS	= 0x00000008,
		
		FITTING_MODE_MASK			= 0x0000000f,
	};

	static const u32 FITTING_MODE_DEFAULT = FITTING_MODE_SEEK_LOC | FITTING_MODE_SEEK_SCALE | FITTING_MODE_APPLY_ANCHORS;

	//----------------------------------------------------------------//
	void			AddAnchor				( MOAICameraAnchor2D& anchor );
	void			Clear					();
	float			GetFitDistance			();
	bool			IsDone					();
					MOAICameraFitter2D		();
					~MOAICameraFitter2D		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			RemoveAnchor			( MOAICameraAnchor2D& anchor );
};

#endif
