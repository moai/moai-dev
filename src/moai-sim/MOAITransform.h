// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORM_H
#define	MOAITRANSFORM_H

#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAITransform
//================================================================//
/**	@lua	MOAITransform
	@text	Transformation hierarchy node.

	@attr	ATTR_X_PIV
	@attr	ATTR_Y_PIV
	@attr	ATTR_Z_PIV
	
	@attr	ATTR_X_LOC
	@attr	ATTR_Y_LOC
	@attr	ATTR_Z_LOC
	
	@attr	ATTR_X_ROT
	@attr	ATTR_Y_ROT
	@attr	ATTR_Z_ROT
	
	@attr	ATTR_X_SCL
	@attr	ATTR_Y_SCL
	@attr	ATTR_Z_SCL
	
	@attr	ATTR_ROTATE_QUAT
	@attr	ATTR_TRANSLATE
	
	@attr	INHERIT_LOC
	@attr	INHERIT_TRANSFORM
*/
class MOAITransform :
	public MOAITransformBase {
protected:

	float			mShearYX;
	float			mShearZX;

	float			mShearXY;
	float			mShearZY;
	
	float			mShearXZ;
	float			mShearYZ;

	ZLVec3D			mPiv;
	ZLVec3D			mLoc;
	ZLVec3D			mScale;
	ZLVec3D			mRot;		// Euler angles, in degrees

	u32				mEulerOrder;

	//----------------------------------------------------------------//
	static int	_addLoc			( lua_State* L );
	static int	_addPiv			( lua_State* L );
	static int	_addRot			( lua_State* L );
	static int	_addScl			( lua_State* L );
	static int	_getLoc			( lua_State* L );
	static int	_getPiv			( lua_State* L );
	static int	_getRot			( lua_State* L );
	static int	_getScl			( lua_State* L );
	static int	_move			( lua_State* L );
	static int	_moveLoc		( lua_State* L );
	static int	_movePiv		( lua_State* L );
	static int	_moveRot		( lua_State* L );
	static int	_moveScl		( lua_State* L );
	static int	_seek			( lua_State* L );
	static int	_seekLoc		( lua_State* L );
	static int	_seekPiv		( lua_State* L );
	static int	_seekRot		( lua_State* L );
	static int	_seekScl		( lua_State* L );
	static int	_setLoc			( lua_State* L );
	static int	_setPiv			( lua_State* L );
	static int	_setRot			( lua_State* L );
	static int	_setScl			( lua_State* L );
	static int	_setShearByX	( lua_State* L );
	static int	_setShearByY	( lua_State* L );
	static int	_setShearByZ	( lua_State* L );

	//----------------------------------------------------------------//
	virtual void	BuildLocalToWorldMtx	( ZLAffine3D& localToWorldMtx );
	static float	ClampEuler				( float r );

public:

	DECL_LUA_FACTORY ( MOAITransform )
	DECL_ATTR_HELPER ( MOAITransform )

	enum {
		EULER_XYZ		= ( 2 << 0x04 ) + ( 1 << 0x02 ) + ( 0 << 0x00 ),
		EULER_XZY		= ( 1 << 0x04 ) + ( 2 << 0x02 ) + ( 0 << 0x00 ),
		EULER_YXZ		= ( 2 << 0x04 ) + ( 0 << 0x02 ) + ( 1 << 0x00 ),
		EULER_YZX		= ( 0 << 0x04 ) + ( 2 << 0x02 ) + ( 1 << 0x00 ),
		EULER_ZXY		= ( 1 << 0x04 ) + ( 0 << 0x02 ) + ( 2 << 0x00 ),
		EULER_ZYX		= ( 0 << 0x04 ) + ( 1 << 0x02 ) + ( 2 << 0x00 ),
	};

	enum {
		ATTR_X_PIV,
		ATTR_Y_PIV,
		ATTR_Z_PIV,
		
		ATTR_X_LOC,
		ATTR_Y_LOC,
		ATTR_Z_LOC,
		
		ATTR_X_ROT,
		ATTR_Y_ROT,
		ATTR_Z_ROT,
		
		ATTR_X_SCL,
		ATTR_Y_SCL,
		ATTR_Z_SCL,
		
		ATTR_ROTATE_QUAT,
		ATTR_TRANSLATE,
		
		TOTAL_ATTR,
	};
	
	GET_SET ( ZLVec3D, Piv, mPiv )
	GET_SET ( ZLVec3D, Loc, mLoc )
	GET_SET ( ZLVec3D, Scl, mScale )
	GET_SET ( ZLVec3D, Rot, mRot )
	
	GET_SET ( float, XLoc, mLoc.mX )
	GET_SET ( float, YLoc, mLoc.mY )
	GET_SET ( float, ZLoc, mLoc.mZ )
	
	GET_SET ( u32, EulerOrder, mEulerOrder )
	
	//----------------------------------------------------------------//
	bool					ApplyAttrOp					( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	ZLAffine3D				GetBillboardMtx				( const ZLAffine3D& faceCameraMtx ) const;
							MOAITransform				();
							~MOAITransform				();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void					SetLoc						( float x, float y, float z );
	void					SetPiv						( float x, float y, float z );
	void					SetRot						( float x, float y, float z );
	void					SetScl						( float x, float y, float z );
};

#endif
