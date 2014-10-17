// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYER_H
#define	MOAILAYER_H

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIViewport.h>

class MOAIBox2DWorld;
class MOAICamera;
class MOAICpSpace;

//================================================================//
// MOAILayer
//================================================================//
/**	@lua	MOAILayer
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
	public virtual MOAIGraphicsProp,
	public MOAIClearableView {
private:

	MOAILuaSharedPtr < MOAICamera >			mCamera;
	MOAILuaSharedPtr < MOAIViewport >		mViewport;
	MOAILuaSharedPtr < MOAIPartition >		mPartition;

	MOAILuaStrongRef						mOverlayTable;
	MOAILuaStrongRef						mUnderlayTable;

	ZLVec3D		mParallax;
	bool		mShowDebugLines;
	u32			mSortMode;
	bool		mSortInViewSpace;
	
	u32			mLODMode;
	float		mLODFactor;

	float		mSortScale [ 4 ];

	bool		mPartitionCull2D;

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_getFitting				( lua_State* L );
	static int		_getPartition			( lua_State* L );
	static int		_getPropViewList		( lua_State* L );
	static int		_getSortMode			( lua_State* L );
	static int		_getSortScale			( lua_State* L );
	static int		_insertProp				( lua_State* L );
	static int		_removeProp				( lua_State* L );
	static int		_setCamera				( lua_State* L );
	static int		_setLODFactor			( lua_State* L );
	static int		_setLODMode				( lua_State* L );
	static int		_setOverlayTable		( lua_State* L );
	static int		_setParallax			( lua_State* L );
	static int		_setPartition			( lua_State* L );
	static int		_setPartitionCull2D		( lua_State* L );
	static int		_setSortMode			( lua_State* L );
	static int		_setSortScale			( lua_State* L );
	static int		_setUnderlayTable		( lua_State* L );
	static int		_setViewport			( lua_State* L );
	static int		_showDebugLines			( lua_State* L );
	static int		_wndToWorld				( lua_State* L );
	static int		_wndToWorldRay			( lua_State* L );
	static int		_worldToWnd				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AffirmPartition			();
	void			DrawProps				( MOAIPartitionResultBuffer& buffer, float lod );
	void			DrawPropsDebug			( MOAIPartitionResultBuffer& buffer, float lod );
	ZLMatrix4x4		GetProjectionMtx		() const;
	ZLMatrix4x4		GetViewMtx				() const;
	void			RenderTable				( MOAILuaRef& ref );
	void			RenderTable				( MOAILuaState& state, int idx );

public:
	
	
	DECL_ATTR_HELPER ( MOAILayer )

	enum {
		ATTR_LOD,
		TOTAL_ATTR,
	};
	
	enum {
		LOD_CONSTANT,				// use the LOD factor
		LOD_FROM_PROP_SORT_Z,		// use the prop's Z coordinate multiplied by the LOD factor in the sort results list (enable view space sort for depth)
	};
	
	DECL_LUA_FACTORY ( MOAILayer )
	
	//----------------------------------------------------------------//
	void				Draw					( int subPrimID, float lod  );
	float				GetFitting				( ZLRect& worldRect, float hPad, float vPad );
	MOAIPartition*		GetPartition			();
	ZLMatrix4x4			GetWndToWorldMtx		() const;
	ZLMatrix4x4			GetWorldToWndMtx		() const;
						MOAILayer				();
						~MOAILayer				();
	u32					OnGetModelBounds		( ZLBox& bounds );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Render					();
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
