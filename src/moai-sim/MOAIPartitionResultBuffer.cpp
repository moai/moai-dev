// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// IsoSortItem
//================================================================//
class IsoSortItem {
public:
	MOAIPartitionResult*	mResult;
	IsoSortItem*			mNext;
};

//================================================================//
// IsoSortList
//================================================================//
class IsoSortList {
public:

	IsoSortItem*	mHead;
	IsoSortItem*	mTail;
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mHead = 0;
		this->mTail = 0;
	}
	
	//----------------------------------------------------------------//
	IsoSortList () :
		mHead ( 0 ),
		mTail ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	inline IsoSortItem* PopFront () {
		IsoSortItem* item = this->mHead;
		if ( item ) {
			if ( item->mNext ) {
				this->mHead = item->mNext;
			}
			else {
				this->mHead = 0;
				this->mTail = 0;
			}
		}
		return item;
	}
	
	//----------------------------------------------------------------//
	inline void PushBack ( IsoSortItem& item, MOAIPartitionResult* result ) {
	
		item.mResult = result;
		item.mNext = 0;
	
		if ( this->mHead ) {
			this->mTail->mNext = &item;
			this->mTail = &item;
		}
		else {
			this->mHead = &item;
			this->mTail = &item;
		}
	}
	
	//----------------------------------------------------------------//
	inline void PushBack ( IsoSortList& list ) {
		
		if ( list.mHead ) {
			
			if ( this->mHead ) {
				this->mTail->mNext = list.mHead;
				this->mTail = list.mTail;
			}
			else {
				this->mHead = list.mHead;
				this->mTail = list.mTail;
			}
		}
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionResultBuffer::_findBest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionResultBuffer, "U" )

	if ( self->mTotalResults ) {

		u32 sortMode			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
		float xScale			= state.GetValue < float >( 3, 0.0f );
		float yScale			= state.GetValue < float >( 4, 0.0f );
		float zScale			= state.GetValue < float >( 5, 0.0f );
		float priorityScale		= state.GetValue < float >( 6, 1.0f );
	
		self->GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		MOAIPartitionHull* hull = self->FindBest ();
		if ( hull ) {
			hull->PushLuaUserdata ( state );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionResultBuffer::_getResults ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionResultBuffer, "U" )

	self->PushHulls ( state );
	return self->GetTotalResults ();
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionResultBuffer::_render ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionResultBuffer, "U" )

	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 2, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	self->Render ( renderPhase );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionResultBuffer::_sort ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionResultBuffer, "U" )

	if ( self->mTotalResults ) {

		u32 sortMode			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
		float xScale			= state.GetValue < float >( 3, 0.0f );
		float yScale			= state.GetValue < float >( 4, 0.0f );
		float zScale			= state.GetValue < float >( 5, 0.0f );
		float priorityScale		= state.GetValue < float >( 6, 1.0f );
		
		
		self->GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		self->Sort ( sortMode );
	}
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionResultBuffer::_view ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionResultBuffer, "U" )

	if ( self->mTotalResults ) {

		MOAICamera* camera = state.GetLuaObject < MOAICamera >( 2, false );
		if( camera ) {
			bool transformBounds = state.GetValue < bool >( 3, false );
			ZLMatrix4x4 viewMtx = ZLViewProj::GetViewMtx ( camera );
			self->Transform ( viewMtx, transformBounds );
		}
	}
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIPartitionResultBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIPartitionResult* MOAIPartitionResultBuffer::AffirmSwapBuffer () {

	this->mSwapBuffer->GrowChunked ( this->mTotalResults, BLOCK_SIZE );
	return this->mSwapBuffer->GetBuffer ();
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Clear () {

	this->mBufferA.Clear ();
	this->mBufferB.Clear ();
	
	this->mResults = 0;
	
	this->mMainBuffer = 0;
	this->mSwapBuffer = 0;
	
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
MOAIPartitionHull* MOAIPartitionResultBuffer::FindBest () {

	if ( !this->mTotalResults ) return 0;
	
	MOAIPartitionResult* best = &this->mResults [ 0 ];
	
	u32 bestKey = best->mKey;
	
	for ( u32 i = 1; i < this->mTotalResults; ++i ) {
		MOAIPartitionResult* compare = &this->mResults [ i ];
		
		u32 compKey = compare->mKey;
		if ( bestKey < compKey ) {
			best = compare;
			bestKey = compKey;
		}
	}
	return best->mHull;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::GenerateKeys ( u32 mode, float xScale, float yScale, float zScale, float priority ) {

	float floatSign = ( mode & SORT_FLAG_DESCENDING ) ? -1.0f : 1.0f;
	s32 intSign = ( int )floatSign;

	switch ( mode & SORT_MODE_MASK ) {
		
		case SORT_DIST_SQUARED_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mResults [ i ];
				
				float dist =
					(( result.mLoc.mX * result.mLoc.mX ) - ( xScale * xScale )) +
					(( result.mLoc.mY * result.mLoc.mY ) - ( yScale * yScale )) +
					(( result.mLoc.mZ * result.mLoc.mZ ) - ( zScale * zScale ));
				
				result.mKey = ZLFloat::FloatToIntKey ( dist * floatSign );
			}
			break;
		
		case SORT_KEY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				this->mResults [ i ].mKey = this->mResults [ i ].mKey * intSign;
			}
			break;
		
		case SORT_PRIORITY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 p = this->mResults [ i ].mPriority * intSign;
				this->mResults [ i ].mKey = ( u32 )(( p ^ 0x80000000 ) | ( p & 0x7fffffff ));
			}
			break;

		case SORT_X_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float x = this->mResults [ i ].mLoc.mX;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( x * floatSign );
			}
			break;
		
		case SORT_Y_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float y = this->mResults [ i ].mLoc.mY;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( y * floatSign );
			}
			break;
		
		case SORT_Z_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float z = this->mResults [ i ].mLoc.mZ;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( z * floatSign );
			}
			break;
		
		case SORT_VECTOR_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mResults [ i ];
				float axis = ( result.mLoc.mX * xScale ) + ( result.mLoc.mY * yScale ) + ( result.mLoc.mZ * zScale ) + (( float )result.mPriority * priority );
				result.mKey = ZLFloat::FloatToIntKey ( axis * floatSign );
			}
			break;
		
		case SORT_NONE:
		default:
			return;
	}
}

//----------------------------------------------------------------//
MOAIPartitionResultBuffer::MOAIPartitionResultBuffer () :
	mResults ( 0 ),
	mMainBuffer ( 0 ),
	mSwapBuffer ( 0 ),
	mTotalResults ( 0 ) {
	
	RTTI_BEGIN ( MOAIPartitionResultBuffer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPartitionResultBuffer >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartitionResultBuffer::~MOAIPartitionResultBuffer () {
}

//----------------------------------------------------------------//
MOAIPartitionResult* MOAIPartitionResultBuffer::PopResult () {

	if ( this->mTotalResults ) {
		return &this->mResults [ --this->mTotalResults ];
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Project ( const ZLMatrix4x4& mtx ) {

	for ( u32 i = 0; i < this->mTotalResults; ++i ) {
		mtx.Project ( this->mResults [ i ].mLoc );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushHulls ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 total = this->mTotalResults;

	// make sure there is enough stack space to push all props
	// the +1 is needed because pushing a hull requires an
	// additional value to be pushed onto the stack temporarily
	lua_checkstack ( L, total + 1 );

	for ( u32 i = 0; i < total; ++i ) {
		this->mResults [ i ].mHull->PushLuaUserdata ( state );
   }
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResult ( MOAIPartitionHull& hull, u32 key, int subPrimID, s32 priority, const ZLVec3D& loc, const ZLBox& aabb, const ZLVec3D& piv ) {

	u32 idx = this->mTotalResults++;
	
	if ( idx >= this->mMainBuffer->Size ()) {
		this->mMainBuffer->GrowChunked ( idx + 1, BLOCK_SIZE );
		this->mResults = this->mMainBuffer->GetBuffer ();
	}
	
	MOAIPartitionResult& result = this->mResults [ idx ] ;
	
	result.mKey = key;
	
	result.mHull = &hull;
	result.mSubPrimID = subPrimID;
	result.mPriority = priority;
	
	result.mLoc = loc;
	result.mAABB = aabb;
	
	// TODO: do we need this?
	result.mLoc.Add ( piv );
	result.mAABB.Offset ( piv );
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::SetResultsBuffer ( MOAIPartitionResult* buffer ) {

	assert (( buffer == this->mBufferA.GetBuffer ()) || ( buffer == this->mBufferB.GetBuffer ()));
	
	this->mResults = buffer;
	
	if ( buffer == this->mBufferA.GetBuffer ()) {
		this->mMainBuffer = &this->mBufferA;
		this->mSwapBuffer = &this->mBufferB;
	}
	else {
		this->mMainBuffer = &this->mBufferB;
		this->mSwapBuffer = &this->mBufferA;
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mBufferA.GrowChunked ( 1, BLOCK_SIZE );
	this->SetResultsBuffer ( this->mBufferA.GetBuffer ());
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::Sort ( u32 mode ) {

	if ( mode == SORT_NONE ) {
		return this->mTotalResults;
	}
	else if ( mode == SORT_ISO ) {
		return this->SortResultsIso ();
	}
	return this->SortResultsLinear ();
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::SortResultsIso () {

	MOAIPartitionResult* mainBuffer = this->mResults;
	IsoSortItem* sortBuffer = ( IsoSortItem* )alloca ( this->mTotalResults * sizeof ( IsoSortItem ));
	
	IsoSortList frontList;
	IsoSortList backList;
	IsoSortList dontCareList;
	IsoSortList list;
	
	// sort by priority
	for ( u32 i = 0; i < this->mTotalResults; ++i ) {
		
		frontList.Clear ();
		backList.Clear ();
		dontCareList.Clear ();
		
		// get the next hull to add
		MOAIPartitionResult* result0 = &mainBuffer [ i ];
		const ZLBox& aabb0 = result0->mAABB;
		
		// check incoming hull against all others
		IsoSortItem* cursor = list.PopFront ();
		while ( cursor ) {
			IsoSortItem* item = cursor;
			cursor = list.PopFront ();
			
			MOAIPartitionResult* result1 = item->mResult;
			const ZLBox& aabb1 = result1->mAABB;
			
			// front flags
			bool f0 =(( aabb1.mMax.mX < aabb0.mMin.mX ) || ( aabb1.mMax.mY < aabb0.mMin.mY ) || ( aabb1.mMax.mZ < aabb0.mMin.mZ ));
			bool f1 =(( aabb0.mMax.mX < aabb1.mMin.mX ) || ( aabb0.mMax.mY < aabb1.mMin.mY ) || ( aabb0.mMax.mZ < aabb1.mMin.mZ ));
			
			if ( f1 == f0 ) {
				// if ambiguous, add to the don't care list
				dontCareList.PushBack ( *item, result1 );
			}
			else if ( f0 ) {
				// if prop0 is *clearly* in front of prop1, add prop1 to back list
				backList.PushBack ( dontCareList );
				backList.PushBack ( *item, result1 );
				dontCareList.Clear ();
			}
			else {
				// if prop0 is *clearly* behind prop1, add prop1 to front list
				frontList.PushBack ( dontCareList );
				frontList.PushBack ( *item, result1 );
				dontCareList.Clear ();
			}
		}
		
		list.Clear ();
		list.PushBack ( backList );
		list.PushBack ( sortBuffer [ i ], result0 );
		list.PushBack ( frontList );
		list.PushBack ( dontCareList );
	}
	
	// affirm the swap buffer
	MOAIPartitionResult* swapBuffer = this->AffirmSwapBuffer ();
	
	IsoSortItem* cursor = list.mHead;
	for ( u32 i = 0; cursor; cursor = cursor->mNext, ++i ) {
		MOAIPartitionResult* result = cursor->mResult;
		swapBuffer [ i ] = *result;
		swapBuffer [ i ].mKey = i;
	}
	
	this->SetResultsBuffer ( swapBuffer );
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Render ( MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	u32 totalResults = this->GetTotalResults ();

	for ( u32 i = 0; i < totalResults; ++i ) {
		
		MOAIPartitionResult* result = this->GetResultUnsafe ( i );
		MOAIAbstractRenderable* prop = result->AsType < MOAIAbstractRenderable >();
		if ( prop ) {
			gfxMgr.SetIndex ( result->mSubPrimID );
			prop->Render ( renderPhase );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::SortResultsLinear () {

	MOAIPartitionResult* swapBuffer = this->AffirmSwapBuffer ();
	MOAIPartitionResult* results = RadixSort32 < MOAIPartitionResult >( this->mResults, swapBuffer, this->mTotalResults );
	this->SetResultsBuffer ( results );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Transform ( const ZLMatrix4x4& mtx, bool transformBounds ) {

	if ( transformBounds ) {
		for ( u32 i = 0; i < this->mTotalResults; ++i ) {
			MOAIPartitionResult& result = this->mResults [ i ];
			mtx.Transform ( result.mLoc );
			result.mAABB.Transform ( mtx );
		}
	}
	else {
		for ( u32 i = 0; i < this->mTotalResults; ++i ) {
			mtx.Transform ( this->mResults [ i ].mLoc );
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "findBest",				_findBest },
		{ "getResults",				_getResults },
		{ "render",					_render },
		{ "sort",					_sort },
		{ "view",					_view },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
