// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartitionViewLayer.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getPropViewList
	@text	Return a list of props gathered and sorted by layer.
	
	@in		MOAIPartitionViewLayer self
	@opt	number sortMode					Default is layer's current value.
	@opt	boolean sortInViewSpace			Default is layer's current value.
	@opt	number xSortScale				Default is layer's current value.
	@opt	number ySortScale				Default is layer's current value.
	@opt	number zSortScale				Default is layer's current value.
	@opt	number pSortScale				Priority sort scale. Default is layer's current value.
	@out	...								Gathered props.
*/
int	MOAIPartitionViewLayer::_getPropViewList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )
	
	MOAIPartition* partition = self->MOAIPartitionHolder::mPartition;
	
	if ( partition && self->mViewport ) {
		
		ZLTypeID typeID = ZLType::GetID < MOAIAbstractRenderable >();
		
		float sortScale [ 4 ];
		
		u32 sortMode			= state.GetValue < u32 >( 2, self->mSortMode );
		bool sortInViewSpace	= state.GetValue < bool >( 3, self->mSortInViewSpace );
		
		sortScale [ 0 ]			= state.GetValue < float >( 4, self->mSortScale [ 0 ]);
		sortScale [ 1 ]			= state.GetValue < float >( 5, self->mSortScale [ 1 ]);
		sortScale [ 2 ]			= state.GetValue < float >( 6, self->mSortScale [ 2 ]);
		sortScale [ 3 ]			= state.GetValue < float >( 7, self->mSortScale [ 3 ]);
		
		ZLMatrix4x4 viewMtx = ZLViewProj::GetViewMtx ( self->mCamera, self->mParallax );
		ZLMatrix4x4 invViewProjMtx = viewMtx;
		invViewProjMtx.Append ( ZLViewProj::GetProjectionMtx ( self->mViewport, self->mCamera ));
		invViewProjMtx.Inverse ();
	
		ZLFrustum viewVolume;
		viewVolume.Init ( invViewProjMtx );
		
		ZLStrongPtr < MOAIPartitionResultBuffer > buffer;
		MOAIPool::Get ().Provision < MOAIPartitionResultBuffer >( buffer );
		
		u32 totalResults = 0;
		
		if ( self->mPartitionCull2D ) {
			totalResults = partition->GatherHulls ( *buffer, 0, viewVolume.mAABB, typeID );
		}
		else {
			totalResults = partition->GatherHulls ( *buffer, 0, viewVolume, typeID );
		}
		
		if ( !totalResults ) return 0;
		
		if ( sortInViewSpace ) {
			buffer->Transform ( viewMtx, false );
		}
		
		buffer->GenerateKeys (
			sortMode,
			sortScale [ 0 ],
			sortScale [ 1 ],
			sortScale [ 2 ],
			sortScale [ 3 ]
		);
		
		buffer->Sort ( self->mSortMode );

		buffer->PushHulls ( L );
		return totalResults;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getSortMode
	@text	Get the sort mode for rendering.
	
	@in		MOAIPartitionViewLayer self
	@out	number sortMode
*/
int MOAIPartitionViewLayer::_getSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )
	
	lua_pushnumber ( state, self->mSortMode );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getSortScale
	@text	Return the scalar applied to axis sorts.
	
	@in		MOAIPartitionViewLayer self
	@out	number x
	@out	number y
	@out	number priority
*/
int	MOAIPartitionViewLayer::_getSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )

	lua_pushnumber ( state, self->mSortScale [ 0 ]);
	lua_pushnumber ( state, self->mSortScale [ 1 ]);
	lua_pushnumber ( state, self->mSortScale [ 3 ]);

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	setPartitionCull2D
	@text	Enables 2D partition cull (projection of frustum AABB will
			be used instead of AABB or frustum).
	
	@in		MOAIPartitionViewLayer self
	@in		boolean partitionCull2D		Default value is false.
	@out	nil
*/
int	MOAIPartitionViewLayer::_setPartitionCull2D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )

	self->mPartitionCull2D = state.GetValue < bool >( 2, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSortMode
	@text	Set the sort mode for rendering.
	
	@in		MOAIPartitionViewLayer self
	@in		number sortMode				One of MOAIPartitionViewLayer.SORT_NONE, MOAIPartitionViewLayer.SORT_PRIORITY_ASCENDING,
										MOAIPartitionViewLayer.SORT_PRIORITY_DESCENDING, MOAIPartitionViewLayer.SORT_X_ASCENDING,
										MOAIPartitionViewLayer.SORT_X_DESCENDING, MOAIPartitionViewLayer.SORT_Y_ASCENDING,
										MOAIPartitionViewLayer.SORT_Y_DESCENDING, MOAIPartitionViewLayer.SORT_Z_ASCENDING,
										MOAIPartitionViewLayer.SORT_Z_DESCENDING
	@in		boolean sortInViewSpace		Default value is 'false'.
	@out	nil
*/
int MOAIPartitionViewLayer::_setSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )
	
	self->mSortMode			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	self->mSortInViewSpace	= state.GetValue < bool >( 3, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSortScale
	@text	Set the scalar applied to axis sorts.
	
	@in		MOAIPartitionViewLayer self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@opt	number priority		Default value is 1.
	@out	nil
*/
int	MOAIPartitionViewLayer::_setSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )

	self->mSortScale [ 0 ] = state.GetValue < float >( 2, 0.0f );
	self->mSortScale [ 1 ] = state.GetValue < float >( 3, 0.0f );
	self->mSortScale [ 2 ] = state.GetValue < float >( 4, 0.0f );
	self->mSortScale [ 3 ] = state.GetValue < float >( 5, 1.0f );

	return 0;
}

//================================================================//
// MOAIPartitionViewLayer
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionViewLayer::DrawPartition ( MOAIPartition& partition, MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	ZLTypeID typeID = ZLType::GetID < MOAIAbstractRenderable >();
	
	ZLStrongPtr < MOAIPartitionResultBuffer > buffer;
	MOAIPool::Get ().Provision < MOAIPartitionResultBuffer >( buffer );
	
	const ZLFrustum& viewVolume = gfxMgr.GetViewVolume ();
	
	u32 totalResults = 0;
		
	if ( this->mPartitionCull2D ) {
		totalResults = partition.GatherHulls ( *buffer, 0, viewVolume.mAABB, typeID );
	}
	else {
		totalResults = partition.GatherHulls ( *buffer, 0, viewVolume, typeID );
	}
	
	if ( !totalResults ) return;
	
	if ( this->mSortInViewSpace ) {
		buffer->Transform ( gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_VIEW_MTX ), false );
	}
	
	buffer->GenerateKeys (
		this->mSortMode,
		this->mSortScale [ 0 ],
		this->mSortScale [ 1 ],
		this->mSortScale [ 2 ],
		this->mSortScale [ 3 ]
	);
	
	buffer->Sort ( this->mSortMode );
	buffer->Render ( renderPhase );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		partition.DrawDebugBack ();
		buffer->Render ( MOAIRenderPhaseEnum::RENDER_PHASE_DRAW_DEBUG );
		partition.DrawDebugFront ();
	}
}

//----------------------------------------------------------------//
MOAIPartitionViewLayer::MOAIPartitionViewLayer () :
	mSortMode ( MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING ),
	mSortInViewSpace ( false ),
	mPartitionCull2D ( true ) {
	
	RTTI_BEGIN ( MOAIPartitionViewLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPartitionViewLayer >)
		RTTI_EXTEND ( MOAIAbstractViewLayer )
		RTTI_EXTEND ( MOAIPartitionHolder )
	RTTI_END
	
	this->mSortScale [ 0 ] = 0.0f;
	this->mSortScale [ 1 ] = 0.0f;
	this->mSortScale [ 2 ] = 0.0f;
	this->mSortScale [ 3 ] = 1.0f;
}

//----------------------------------------------------------------//
MOAIPartitionViewLayer::~MOAIPartitionViewLayer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionViewLayer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField ( -1, "SORT_NONE",						( u32 )MOAIPartitionResultBuffer::SORT_NONE );
	state.SetField ( -1, "SORT_ISO",						( u32 )MOAIPartitionResultBuffer::SORT_ISO );
	state.SetField ( -1, "SORT_PRIORITY_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	state.SetField ( -1, "SORT_PRIORITY_DESCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_DESCENDING );
	state.SetField ( -1, "SORT_X_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_X_ASCENDING );
	state.SetField ( -1, "SORT_X_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_X_DESCENDING );
	state.SetField ( -1, "SORT_Y_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Y_ASCENDING );
	state.SetField ( -1, "SORT_Y_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Y_DESCENDING );
	state.SetField ( -1, "SORT_Z_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Z_ASCENDING );
	state.SetField ( -1, "SORT_Z_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Z_DESCENDING );
	state.SetField ( -1, "SORT_VECTOR_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_ASCENDING );
	state.SetField ( -1, "SORT_VECTOR_DESCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_DESCENDING );
	state.SetField ( -1, "SORT_DIST_SQUARED_ASCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_DIST_SQUARED_ASCENDING );
	state.SetField ( -1, "SORT_DIST_SQUARED_DESCENDING",	( u32 )MOAIPartitionResultBuffer::SORT_DIST_SQUARED_DESCENDING );
}

//----------------------------------------------------------------//
void MOAIPartitionViewLayer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "getPropViewList",		_getPropViewList },
		{ "getSortMode",			_getSortMode },
		{ "getSortScale",			_getSortScale },
		{ "setPartitionCull2D",		_setPartitionCull2D },
		{ "setSortMode",			_setSortMode },
		{ "setSortScale",			_setSortScale },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPartitionViewLayer::MOAIAbstractRenderNode_Render ( MOAIRenderPhaseEnum::_ renderPhase ) {
	
	this->ClearSurface ();
	
	if ( this->MOAIPartitionHolder::mPartition ) {
		this->DrawPartition ( *this->MOAIPartitionHolder::mPartition, renderPhase  );
	}
}
