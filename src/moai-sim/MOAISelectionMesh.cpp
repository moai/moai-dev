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
	u32 base		= state.GetValue < u32 >( 3, 0 );
	u32 size		= state.GetValue < u32 >( 4, 0 );

	self->AddSelection ( set, base, size );

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
int MOAISelectionMesh::_reserveSets ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "U" )

	return 0;
}

//================================================================//
// MOAISelectionMesh
//================================================================//

//----------------------------------------------------------------//
void MOAISelectionMesh::AddSelection ( u32 set, size_t base, size_t size ) {

	size_t top = base + size;

	MOAISelectionSpan* cursor = this->mSpanListHead;
	
	MOAISelectionSpan* prevInSet = 0;
	MOAISelectionSpan* prevAdjacentInSet = 0;
	
	for ( ; cursor && ( cursor->mTop < base ); cursor = cursor->mNext ) {
	
		if ( cursor->mSetID == set ) {
		
			prevInSet = cursor;
		
			if (( cursor->mTop + 1 ) == base ) {
				prevAdjacentInSet = cursor;
			}
		}
	}

	MOAISelectionSpan* span = 0;

	if ( cursor ) {
	
		if ( prevAdjacentInSet ) {
			cursor = prevAdjacentInSet;
		}
		
		if ( cursor->mSetID == set ) {
		
			if ( cursor->mTop < top ) {
			
				cursor->mTop = top;
				
				MOAISelectionSpan* next = cursor->mNext;
				
				while ( next && ( next->mTop < top )) {
				
					MOAISelectionSpan* kill = next;
					next = next->mNext;
					
					this->mSpanPool.Free ( kill );
				}
				
				cursor->mNext = next;
				
				if ( next ) {
					next->mBase = top + 1;
				}
				else {
					this->mSpanListTail = cursor;
				}
			}
		}
		else {
		
		
			if ( cursor->mTop < top ) {
			
				cursor->mTop = base - 1;
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
				cursor->mTop = base - 1;
				span = this->AllocSpan ( set, base, top );
				MOAISelectionSpan* cap = this->AllocSpan ( cursor->mSetID, top + 1, cursor->mTop );
				
				cap->mNext = cursor->mNext;
				span->mNext = cap;
				cursor->mNext = span;
				
				if ( this->mSpanListTail == cursor ) {
					this->mSpanListTail = cap;
				}
			}
		}
	}
	else {
	
		if ( prevAdjacentInSet ) {
			prevAdjacentInSet->mTop = top;
		}
		else {
	
			MOAISelectionSpan* span = this->AllocSpan ( set, base, top );
			
			if ( this->mSpanListTail ) {
				this->mSpanListTail->mNext = span;
			}
			else {
				this->mSpanListHead = span;
			}
			this->mSpanListTail = span;
		}
	}
	
	// if there's a new span
	if ( span ) {
	
		if ( prevInSet ) {
		
			span->mNextInSet = prevInSet->mNextInSet;
			prevInSet->mNextInSet = span;
		}
		else {
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
	
	return span;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ClearSelection ( u32 set ) {
}

//----------------------------------------------------------------//
//void MOAISelectionMesh::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
//	UNUSED ( idx );
//	UNUSED ( xOff );
//	UNUSED ( yOff );
//	UNUSED ( zOff );
//	UNUSED ( xScl );
//	UNUSED ( yScl );
//	UNUSED ( zScl );
//
//	// TODO: make use of offset and scale
//	
//	//if ( !this->mVertexBuffer ) return;
//	
//	//const MOAIVertexFormat* format = this->mVertexBuffer->GetVertexFormat ();
//	//if ( !format ) return;
//
//	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
//	gfxDevice.Flush (); // TODO: should remove this call
//	MOAIGfxDevice::Get ().SetVertexFormat ();
//
//	this->FinishInit ();
//
//	if ( this->Bind ()) {
//
//		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
//		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
//		gfxDevice.SetGfxState ( this->mTexture );
//		
//		gfxDevice.SetPenWidth ( this->mPenWidth );
//		gfxDevice.SetPointSize ( this->mPointSize );
//		
//		gfxDevice.UpdateShaderGlobals ();
//		
//		// TODO: use gfxDevice to cache buffers
//		if ( this->mIndexBuffer ) {
//			if ( this->mIndexBuffer->Bind ()) {
//				zglDrawElements ( this->mPrimType, this->mTotalElements, this->mIndexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT, 0 );
//			}
//		}
//		else {
//			zglDrawArrays ( this->mPrimType, 0, this->mTotalElements );
//		}
//		this->Unbind ();
//	}
//}

//----------------------------------------------------------------//
void MOAISelectionMesh::MergeSelections ( u32 set, u32 merge ) {
}

//----------------------------------------------------------------//
MOAISelectionMesh::MOAISelectionMesh () :
	mSpanListHead ( 0 ),
	mSpanListTail ( 0 ) {

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
}

//----------------------------------------------------------------//
void MOAISelectionMesh::PrintSelections () {

	MOAISelectionSpan* span = this->mSpanListHead;
	for ( ; span; span = span->mNext ) {
	
		printf ( "%d:[%d-%d]", span->mSetID, span->mBase, span->mTop );
		
		if ( span->mNext ) {
			printf ( ", " );
		}
		else {
			printf ( "\n" );
		}
	}
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
		{ "reserveSets",			_reserveSets },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ReserveSets () {
}

//----------------------------------------------------------------//
void MOAISelectionMesh::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIMesh::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIMesh::SerializeOut ( state, serializer );
}
