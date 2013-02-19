// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYER_H
#define	MOAILAYER_H

#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIViewport.h>

class MOAIBox2DWorld;
class MOAICamera;
class MOAICpSpace;

//================================================================//
// MOAILayer
//================================================================//
/**	@name	MOAILayer
	@text	Scene controls class.
	
	@const	SORT_NONE
	@const	SORT_ISO
	@const	SORT_PRIORITY_ASCENDING
	@const	SORT_PRIORITY_DESCENDING
	@const	SORT_X_ASCENDING
	@const	SORT_X_DESCENDING
	@const	SORT_Y_ASCENDING
	@const	SORT_Y_DESCENDING
	@const	SORT_Z_ASCENDING
	@const	SORT_Z_DESCENDING
	@const	SORT_VECTOR_ASCENDING
	@const	SORT_VECTOR_DESCENDING
*/
class MOAILayer :
	public virtual MOAIProp,
	public MOAIClearableView {
private:

	MOAILuaSharedPtr < MOAICamera >			mCamera;
	MOAILuaSharedPtr < MOAIViewport >		mViewport;
	MOAILuaSharedPtr < MOAIPartition >		mPartition;

	#if USE_CHIPMUNK
		MOAILuaSharedPtr < MOAICpSpace >	mCpSpace;
	#endif
	
	#if USE_BOX2D
		MOAILuaSharedPtr < MOAIBox2DWorld >	mBox2DWorld;
	#endif

	USVec3D		mParallax;
	bool		mShowDebugLines;
	u32			mSortMode;

	float		mSortScale [ 4 ];

	bool		mPartitionCull2D;

	//----------------------------------------------------------------//
	static int	_clear				( lua_State* L );
	static int	_getFitting			( lua_State* L );
	static int	_getPartition		( lua_State* L );
	static int	_getSortMode		( lua_State* L );
	static int	_getSortScale		( lua_State* L );
	static int	_insertProp			( lua_State* L );
	static int	_removeProp			( lua_State* L );
	static int	_setBox2DWorld		( lua_State* L );
	static int	_setCamera			( lua_State* L );
	static int	_setCpSpace			( lua_State* L );
	static int	_setParallax		( lua_State* L );
	static int	_setPartition		( lua_State* L );
	static int	_setPartitionCull2D	( lua_State* L );
	static int	_setSortMode		( lua_State* L );
	static int	_setSortScale		( lua_State* L );
	static int	_setViewport		( lua_State* L );
	static int	_showDebugLines		( lua_State* L );
	static int	_wndToWorld			( lua_State* L );
	static int	_worldToWnd			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AffirmPartition			();
	void			GetBillboardMtx			( USMatrix4x4& billboard );
	void			GetProjectionMtx		( USMatrix4x4& proj );
	void			GetViewMtx				( USMatrix4x4& view );

public:
	
	DECL_LUA_FACTORY ( MOAILayer )
	
	//----------------------------------------------------------------//
	void			Draw					( int subPrimID );
	float			GetFitting				( USRect& worldRect, float hPad, float vPad );
	u32				GetPropBounds			( USBox& bounds );
	void			GetWndToWorldMtx		( USMatrix4x4& wndToWorld );
	void			GetWorldToWndMtx		( USMatrix4x4& worldToWnd );
					MOAILayer				();
					~MOAILayer				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Render					();
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
