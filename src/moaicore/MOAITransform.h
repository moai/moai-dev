// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORM_H
#define	MOAITRANSFORM_H

#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAITransform
//================================================================//
/**	@name	MOAITransform
	@text	2D transformation hierarchy node.

	@attr	ATTR_X_LOC
	@attr	ATTR_Y_LOC
	@attr	ATTR_Z_ROT
	@attr	ATTR_X_SCL
	@attr	ATTR_Y_SCL
*/
class MOAITransform :
	public MOAITransformBase {
protected:

	USVec2D			mLoc;
	USVec2D			mScale;
	float			mDegrees;

	//----------------------------------------------------------------//
	static int	_addLoc			( lua_State* L );
	static int	_addRot			( lua_State* L );
	static int	_addScl			( lua_State* L );
	static int	_getLoc			( lua_State* L );
	static int	_getRot			( lua_State* L );
	static int	_getScl			( lua_State* L );
	static int	_modelToWorld	( lua_State* L );
	static int	_move			( lua_State* L );
	static int	_moveLoc		( lua_State* L );
	static int	_moveRot		( lua_State* L );
	static int	_moveScl		( lua_State* L );
	static int	_seek			( lua_State* L );
	static int	_seekLoc		( lua_State* L );
	static int	_seekRot		( lua_State* L );
	static int	_seekScl		( lua_State* L );
	static int	_setLoc			( lua_State* L );
	static int	_setParent		( lua_State* L );
	static int	_setRot			( lua_State* L );
	static int	_setScl			( lua_State* L );
	static int	_worldToModel	( lua_State* L );

	//----------------------------------------------------------------//
	void	BuildTransforms			( float xOff, float yOff, float xStretch, float yStretch );
	void	OnDepNodeUpdate			();

public:

	DECL_LUA_FACTORY ( MOAITransform )
	DECL_ATTR_HELPER ( MOAITransform )

	enum {
		ATTR_X_LOC,
		ATTR_Y_LOC,
		ATTR_Z_ROT,
		ATTR_X_SCL,
		ATTR_Y_SCL,
		TOTAL_ATTR,
	};
	
	GET_SET ( USVec2D, Loc, mLoc )
	GET_SET ( USVec2D, Scl, mScale )
	GET_SET ( float, Rot, mDegrees )
	
	//----------------------------------------------------------------//
	bool				ApplyAttrOp						( u32 attrID, USAttrOp& attrOp );
	const USAffine2D&	GetLocalToWorldMtx				();
	const USAffine2D&	GetWorldToLocalMtx				();
						MOAITransform					();
						~MOAITransform					();
	void				RegisterLuaClass				( USLuaState& state );
	void				RegisterLuaFuncs				( USLuaState& state );
	void				SetLoc							( float x, float y );
	virtual void		SetParent						( MOAITransformBase* parent );
	void				SetScl							( float x, float y );
	STLString			ToString						();
};

#endif
