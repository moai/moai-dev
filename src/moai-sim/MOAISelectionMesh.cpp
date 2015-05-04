// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAISelectionMesh.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_addSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UNNN" )

	u32 set			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 base		= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 top			= state.GetValue < u32 >( 4, 0 ) + base;

	self->AddSelection ( set, base, top );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_clearSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UN" )

	u32 set			= state.GetValue < u32 >( 2, 1 ) - 1;
	
	self->ClearSelection ( set );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_mergeSelections ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UNN" )

	u32 set			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 merge		= state.GetValue < u32 >( 3, 1 ) - 1;

	self->MergeSelections ( set, merge );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_printSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->PrintSelection ( state.GetValue < u32 >( 2, 1 ) - 1 );
	}
	else {
		self->PrintSelections ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_reserveSelections ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "U" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveSelections ( total );

	return 0;
}

//================================================================//
// MOAISelectionMesh
//================================================================//

//----------------------------------------------------------------//
void MOAISelectionMesh::AddSelection ( u32 set, size_t base, size_t top ) {

	if ( set >= this->mSets.Size ()) return;

	MOAISelectionSpan* cursor = this->mSpanListHead;
	
	MOAISelectionSpan* prevInSet = 0;
	MOAISelectionSpan* prevAdjacentInSet = 0;
	MOAISelectionSpan* spanListTail = 0;
	
	for ( ; cursor && ( cursor->mTop < base ); cursor = cursor->mNext ) {
	
		spanListTail = cursor;
	
		if ( cursor->mSetID == set ) {
		
			prevInSet = cursor;
		
			if ( cursor->mTop == base ) {
				prevAdjacentInSet = cursor;
			}
		}
	}

	MOAISelectionSpan* span = 0;

	if ( cursor ) {
	
		if ( prevAdjacentInSet ) {
			cursor = prevAdjacentInSet;
			prevAdjacentInSet = 0;
		}
		else if ( cursor->mBase == base ) {
			this->ChangeSpanSet ( cursor, set );
		}
		
		if ( cursor->mSetID == set ) {
			
			if ( cursor->mTop <= top ) {
			
				MOAISelectionSpan* next = cursor->mNext;
				
				while ( next && ( next->mTop <= top )) {
				
					if ( cursor->mNextInSet == next ) {
						cursor->mNextInSet = next->mNextInSet;
					}
					next = this->FreeSpanAndGetNext ( next );
				}
				
				if ( next && ( next->mSetID == set ) && ( next->mBase <= top )) {
					top = next->mTop;
					cursor->mNextInSet = next->mNextInSet;
					next = this->FreeSpanAndGetNext ( next );
				}
				
				cursor->mNext = next;
				cursor->mTop = top;
				
				if ( next ) {
					next->mBase = top;
				}
			}
		}
		else {
		
			if ( cursor->mTop <= top ) {
			
				cursor->mTop = base;
				MOAISelectionSpan* next = cursor->mNext;
				
				if ( next && ( next->mSetID == set )) {
					
					// next span is already in the set, so just move the boundary
					next->mBase = base;
				}
				else {
				
					// create a new span and add it
					span = this->AllocSpan ( set, base, top );
				
					span->mNext = cursor->mNext;
					cursor->mNext = span;
				}
			}
			else {
			
				// we need to split the span
				span = this->AllocSpan ( set, base, top );
				MOAISelectionSpan* cap = this->AllocSpan ( cursor->mSetID, top, cursor->mTop );
				
				cap->mNext = cursor->mNext;
				span->mNext = cap;
				cursor->mNext = span;
				
				cap->mNextInSet = cursor->mNextInSet;
				cursor->mNextInSet = cap;
				
				cursor->mTop = base;
			}
		}
	}
	else {
	
		if ( prevAdjacentInSet ) {
			prevAdjacentInSet->mTop = top;
		}
		else {
	
			span = this->AllocSpan ( set, base, top );
			
			if ( spanListTail ) {
				spanListTail->mNext = span;
			}
			else {
				this->mSpanListHead = span;
			}
		}
	}
	
	// if there's a new span
	if ( span ) {
		if ( prevInSet ) {
			span->mNextInSet = prevInSet->mNextInSet;
			prevInSet->mNextInSet = span;
		}
	}
}

//----------------------------------------------------------------//
MOAISelectionSpan* MOAISelectionMesh::AllocSpan ( u32 set, size_t base, size_t top ) {

	MOAISelectionSpan* span = this->mSpanPool.Alloc ();
	assert ( span );
	
	span->mSetID = set;
			
	span->mBase = base;
	span->mTop = top;
	
	span->mNext = 0;
	span->mNextInSet = 0;
	
	if ( !this->mSets [ set ]) {
		this->mSets [ set ] = span;
	}
	return span;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ChangeSpanSet ( MOAISelectionSpan* span, u32 set ) {

	if ( span && span->mSetID != set ) {

		if ( this->mSets [ span->mSetID ] == span ) {
			this->mSets [ span->mSetID ] = span->mNextInSet;
		}
		
		MOAISelectionSpan* firstInSet = this->mSets [ set ];
		
		if (( firstInSet && ( span->mBase <= firstInSet->mBase )) || !firstInSet ) {
			span->mNextInSet = firstInSet;
			this->mSets [ set ] = span;
		}
		
		span->mSetID = set;
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::Clear () {

	MOAISelectionSpan* next = this->mSpanListHead;
	while ( next ) {
		next = FreeSpanAndGetNext ( next );
	}
	
	this->mSets.Clear ();
	
	this->mSpanListHead = 0;
	//this->mSpanListTail = 0;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ClearSelection ( u32 set ) {
}

//----------------------------------------------------------------//
void MOAISelectionMesh::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( offset );
	UNUSED ( scale );
	
	size_t size = this->mSets.Size ();
	if ( !size ) return;

	idx = idx - 1;
	u32 itemIdx = idx % size;
	
	MOAISelectionSpan* span = this->mSets [ itemIdx ];
	if ( !span ) return;

	materials.LoadGfxState ( this, idx, MOAIShaderMgr::MESH_SHADER );

	// TODO: make use of offset and scale

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.Flush (); // TODO: should remove this call
	MOAIGfxDevice::Get ().SetVertexFormat ();

	this->FinishInit ();

	if ( this->Bind ()) {

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		gfxDevice.SetPointSize ( this->mPointSize );
		
		gfxDevice.UpdateShaderGlobals ();
		
		// TODO: use gfxDevice to cache buffers
		if ( this->mIndexBuffer ) {
			if ( this->mIndexBuffer->Bind ()) {
			
				for ( ; span; span = span->mNextInSet ) {
					
					zglDrawElements (
						this->mPrimType,
						span->mTop - span->mBase,
						this->mIndexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT,
						( const void* )span->mBase
					);
				}
			}
		}
		else {
		
			for ( ; span; span = span->mNextInSet ) {
				zglDrawArrays ( this->mPrimType, span->mBase, span->mTop - span->mBase );
			}
		}
		this->Unbind ();
	}
}

//----------------------------------------------------------------//
MOAISelectionSpan* MOAISelectionMesh::FreeSpanAndGetNext ( MOAISelectionSpan* span ) {

	MOAISelectionSpan* next = 0;

	if ( span ) {
	
		if ( this->mSets [ span->mSetID ] == span ) {
			this->mSets [ span->mSetID ] = span->mNextInSet;
		}
		
		next = span->mNext;
		this->mSpanPool.Free ( span );
	}
	return next;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::MergeSelections ( u32 set, u32 merge ) {
}

//----------------------------------------------------------------//
MOAISelectionMesh::MOAISelectionMesh () :
	mSpanListHead ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMesh )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISelectionMesh::~MOAISelectionMesh () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
	this->SetIndexBuffer ( 0 );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::PrintSelection ( u32 idx ) {

	if ( idx < this->mSets.Size ()) {
	
		printf ( "set %d - ", idx );
	
		MOAISelectionSpan* span = this->mSets [ idx ];
		for ( ; span; span = span->mNextInSet ) {
		
			printf ( "%d:[%d-%d]", span->mSetID + 1, span->mBase, span->mTop );
			
			if ( span->mNextInSet ) {
				printf ( ", " );
			}
		}
		printf ( "\n" );
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::PrintSelections () {

	MOAISelectionSpan* span = this->mSpanListHead;
	for ( ; span; span = span->mNext ) {
	
		printf ( "%d:[%d-%d]", span->mSetID + 1, span->mBase, span->mTop );
		
		if ( span->mNext ) {
			printf ( ", " );
		}
	}
	printf ( "\n" );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIMesh::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIMesh::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addSelection",			_addSelection },
		{ "clearSelection",			_clearSelection },
		{ "mergeSelections",		_mergeSelections },
		{ "printSelection",			_printSelection },
		{ "reserveSelections",		_reserveSelections },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ReserveSelections ( u32 total ) {

	this->Clear ();

	this->mSets.Init ( total );
	this->mSets.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIMesh::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIMesh::SerializeOut ( state, serializer );
}
