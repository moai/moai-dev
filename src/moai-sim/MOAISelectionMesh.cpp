// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAISelectionMesh.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_addSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UNN*" )
	
	ZLIndex set		= 0;
	ZLIndex base	= 0;
	ZLIndex top		= 0;
	
	if ( state.IsType ( 4, LUA_TNUMBER )) {

		set			= state.GetValue < MOAILuaIndex >( 2, 0 );
		base		= state.GetValue < MOAILuaIndex >( 3, 0 );
		top			= state.GetValue < MOAILuaIndex >( 4, 0 );
	}
	else {
	
		ZLResult < ZLIndex > result = self->AffirmSpanSet ();
		if ( result.mCode != ZL_OK ) return 0;
		
		set			= result;
		base		= state.GetValue < MOAILuaIndex >( 2, 0 );
		top			= state.GetValue < MOAILuaIndex >( 3, 0 );
	}
	
	self->AddSelection ( set, base, top );
	state.Push ( MOAILuaIndex ( set ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_clearSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UN" )

	ZLIndex set		= state.GetValue < MOAILuaIndex >( 2, 0 );
	
	if ( state.CheckParams ( 3, "NN", false )) {
	
		ZLIndex base	= state.GetValue < MOAILuaIndex >( 3, 0 );
		ZLIndex top		= state.GetValue < MOAILuaIndex >( 4, 0 );
	
		self->ClearSelection ( set, base, top );
	}
	else {
		self->ClearSelection ( set );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_mergeSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "UNN" )

	ZLIndex set		= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex merge	= state.GetValue < MOAILuaIndex >( 3, 0 );

	self->MergeSelection ( set, merge );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_printSelection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->PrintSelection ( state.GetValue < MOAILuaIndex >( 2, 0 ));
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

	ZLSize total = state.GetValue < MOAILuaSize >( 2, 0 );
	self->ReserveSelections ( total );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISelectionMesh::_setMesh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISelectionMesh, "U" )

	MOAIMesh* mesh = state.GetLuaObject < MOAIMesh >( 2, true );

	self->mDeck.Set ( *self, mesh );
	self->mMesh = mesh;

	return 0;
}

//================================================================//
// MOAISelectionMesh
//================================================================//

//----------------------------------------------------------------//
void MOAISelectionMesh::AddSelection ( ZLIndex set, ZLIndex base, ZLIndex top ) {

	// the approach here is to insert the new span after the last span that has a
	// base entirely below the new span, then clean up any overlaps.

	// there are a few edge cases dealing with inserting the span after an existing span:
	//   the new span may match the previous span
	//   the new span may clip the previous span
	//   the new span may be entirely inside the previous span
	//   the previous span is entirely below the new span (top and bottom)

	// once the insertion edge cases have been dealt with, the new span will be inserted
	// cleanly (i.e. no overlap) after the previous span. one this is done, any subsequent
	// spans the new span overlaps will be removed, and any span left straddling the top
	// of the new span will be clipped.

	// make sure the array of sets is big enough
	this->mSets.Grow (( ZLSize )set + 1, 0 );
	
	// identify the previous span in both the master and set lists.
	// a span is only 'previous' if its base is entirely below the new span's base.
	// if the base is the same, then it the new span is added *before* it and it is cured by FixOverlaps ().
	
	MOAISelectionSpan* prevInSet		= 0;
	MOAISelectionSpan* prevInMaster		= 0;

	MOAISelectionSpan* cursor = this->mSpanListHead;
	for ( ; cursor && ( cursor->mBase < base ); cursor = cursor->mNextInMaster ) {
	
		prevInMaster = cursor;
	
		if ( cursor->mSetID == set ) {
			prevInSet = cursor;
		}
	}
	
	if ( prevInMaster ) {
	
		// we have a span with a base entirely below the new span's base.
		// wacky edge cases ensue.
	
		if ( prevInMaster->mTop >= base ) {
		
			// previous span touches or overlaps the new span
			
			if ( prevInMaster->mTop <= top ) {
			
				// previous span only overlaps the base of the new span.
				// clip it to the base and insert the new span after it
				// or, if it matches the new span, extend it.
			
				if ( prevInMaster->mSetID == set ) {
				
					// make the previous span longer and clean up any overlaps.
					prevInMaster->mTop = top;
					this->FixOverlaps ( prevInMaster );
				}
				else {
				
					// clip the previous span, add the new span and fix overlaps.
					prevInMaster->mTop = base;
					this->FixOverlaps ( this->InsertSpan ( this->AllocSpan ( set, base, top ), prevInMaster, prevInSet ));
				}
			}
			else {
				
				// new span lies fully inside of previous span.
				// if new span is in a different set, split previous span in two.
				if ( prevInMaster->mSetID != set ) {
				
				
					MOAISelectionSpan* span = this->AllocSpan ( set, base, top );
					MOAISelectionSpan* tail = this->AllocSpan ( prevInMaster->mSetID, top, prevInMaster->mTop );
					
					this->InsertSpan ( span, prevInMaster, prevInSet );
					this->InsertSpan ( tail, span, prevInMaster );
					
					prevInMaster->mTop = base;
				}
			}
		}
		else {
		
			// previous span is entirely below the new span.
			// create a new span and clean up its overlaps.
			this->FixOverlaps ( this->InsertSpan ( this->AllocSpan ( set, base, top ), prevInMaster, prevInSet ));
		}
	}
	else {
	
		// insert the new span at the head of the list and fix all the overlaps.
		this->FixOverlaps ( this->InsertSpan ( this->AllocSpan ( set, base, top ), 0, 0 ));
	}
}

//----------------------------------------------------------------//
ZLResult < ZLIndex > MOAISelectionMesh::AffirmSpanSet () {

	ZLIndex top = this->mSets.Size ();

	for ( ZLIndex i = 0; i < top; ++i ) {
		if ( !this->mSets [ i ]) {
			ZL_RETURN_RESULT ( ZLIndex, i, ZL_OK );
		}
	}
	
	if ( this->mSets.Grow ( top ) == ZL_OK ) {
		ZL_RETURN_RESULT ( ZLIndex, top, ZL_OK );
	}
	ZL_RETURN_RESULT ( ZLIndex, ZLIndexOp::INVALID, ZL_ERROR );
}

//----------------------------------------------------------------//
MOAISelectionSpan* MOAISelectionMesh::AllocSpan ( ZLIndex set, ZLIndex base, ZLIndex top ) {

	MOAISelectionSpan* span = this->mSpanPool.Alloc ();
	assert ( span );
	
	span->mSetID = set;
			
	span->mBase = base;
	span->mTop = top;
	
	span->mPrev = 0;
	span->mPrevInMaster = 0;
	span->mNextInMaster = 0;
	
	span->mNext = 0;
	
	return span;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ChangeSpanSet ( MOAISelectionSpan* span, ZLIndex set ) {

	if ( span && ( span->mSetID != set )) {

		if ( this->mSets [ span->mSetID ] == span ) {
			this->mSets [ span->mSetID ] = span->mNext;
		}
		
		MOAIMeshSpan* firstInSet = this->mSets [ set ];
		
		if (( firstInSet && ( span->mBase <= firstInSet->mBase )) || !firstInSet ) {
			span->mNext = firstInSet;
			this->mSets [ set ] = span;
		}
		
		span->mSetID = set;
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::Clear () {
	
	this->mSpanPool.Clear ();
	this->mSets.Clear ();
	
	this->mSpanListHead = 0;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ClearSelection ( ZLIndex set ) {

	MOAIMeshSpan* cursor = this->mSets [ set ];
	while ( cursor ) {
	
		MOAIMeshSpan* span = cursor;
		cursor = cursor->mNext;
			
		this->FreeSpan (( MOAISelectionSpan* )span );
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ClearSelection ( ZLIndex set, ZLIndex base, ZLIndex top ) {

	MOAISelectionSpan* cursor = this->mSpanListHead;
	for ( ; cursor; cursor = cursor->mNextInMaster ) {
	
		if (( cursor->mSetID == set ) && ((( cursor->mBase <= base ) && ( cursor->mTop >= base )) || (( cursor->mBase <= top ) && ( cursor->mTop >= top )))) break;
	}
	
	if ( cursor ) {
	
		if (( cursor->mBase < base ) && ( cursor->mTop > top )) {
			
			this->InsertSpan ( this->AllocSpan ( set, top, cursor->mTop ), cursor, cursor );
			cursor->mTop = base;
		}
		else {
		
			while ( cursor && ( cursor->mBase < top )) {
			
				MOAISelectionSpan* span = cursor;
				cursor = cursor->mNextInMaster;
			
				if ( span->mSetID == set ) {
				
				
					if ( span->mBase < base ) {
						span->mTop = base;
						continue;
					}
					
					if ( span->mTop > top ) {
						span->mBase = top;
						break;
					}
					
					this->FreeSpan ( span );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::FixOverlaps ( MOAISelectionSpan* span ) {

	// we expect the new span to be *entirely* after the base of the previous span and
	// for the previous span to have already been clipped.
	
	// in other words, we only fix overlaps for *subsequent* spans in the list.
	
	// note that the base of the next span may coincide with the base of the new span.

	MOAISelectionSpan* cursor = span->mNextInMaster;
	while ( cursor && ( cursor->mBase <= span->mTop )) {
	
		MOAISelectionSpan* overlap = cursor;
		cursor = cursor->mNextInMaster;
		
		if ( overlap->mTop <= span->mTop ) {
			// the span we're evalutating is entirely covered by the new span.
			this->FreeSpan ( overlap );
		}
		else {
			// just the base overlaps the new span, so clip it.
			overlap->mBase = span->mTop;
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::FreeSpan ( MOAISelectionSpan* span ) {

	if ( span ) {
	
		if ( span->mPrevInMaster ) {
			span->mPrevInMaster->mNextInMaster = span->mNextInMaster;
		}
		else {
			this->mSpanListHead = span->mNextInMaster;
		}
		
		if ( span->mPrev ) {
			span->mPrev->mNext = span->mNext;
		}
		else {
			this->mSets [ span->mSetID ] = span->mNext;
		}
		
		if ( span->mNextInMaster ) {
			span->mNextInMaster->mPrevInMaster = span->mPrevInMaster;
		}
		
		if ( span->mNext ) {
			span->mNext->mPrev = span->mPrev;
		}
		
		this->mSpanPool.Free ( span );
	}
}

//----------------------------------------------------------------//
MOAISelectionSpan* MOAISelectionMesh::InsertSpan ( MOAISelectionSpan* span, MOAISelectionSpan* prevInMaster, MOAISelectionSpan* prevInSet ) {

	if ( prevInMaster ) {
	
		span->mPrevInMaster = prevInMaster;
		span->mNextInMaster = prevInMaster->mNextInMaster;
		prevInMaster->mNextInMaster = span;
	}
	else {
	
		span->mNextInMaster = this->mSpanListHead;
		this->mSpanListHead = span;
	}
	
	if ( prevInSet ) {
	
		span->mPrev = prevInSet;
		span->mNext = prevInSet->mNext;
		prevInSet->mNext = span;
	}
	else {
	
		span->mNext = this->mSets [ span->mSetID ];
		this->mSets [ span->mSetID ] = span;
	}
	
	if ( span->mNextInMaster ) {
		span->mNextInMaster->mPrevInMaster = span;
	}
	
	if ( span->mNext ) {
		span->mNext->mPrev = span;
	}
	
	return span;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::MergeSelection ( ZLIndex set, ZLIndex merge ) {

	MOAISelectionSpan* cursor = this->mSpanListHead;
	
	if ( cursor ) {
	
		MOAISelectionSpan* prevInSet = 0;
	
		if ( cursor->mSetID == merge ) {
		
			ZLIndex base = cursor->mBase;
			ZLIndex top = cursor->mTop;
		
			this->FreeSpan ( cursor );
			cursor = this->InsertSpan ( this->AllocSpan ( set, base, top ), 0, 0 );
			
			prevInSet = cursor;
		}
		
		cursor = cursor->mNextInMaster;
		
		while ( cursor ) {
		
			MOAISelectionSpan* span = cursor;
			cursor = cursor->mNextInMaster;
			
			MOAISelectionSpan* prevInMaster = span->mPrevInMaster;
			
			if ( span->mSetID == merge ) {
				
				ZLIndex base = span->mBase;
				ZLIndex top = span->mTop;
			
				this->FreeSpan ( span );
				span = this->InsertSpan ( this->AllocSpan ( set, base, top ), prevInMaster, prevInSet );
				prevInSet = span;
			}
			
			if ( span->mSetID == set ) {
			
				if ( prevInMaster && ( prevInMaster->mSetID == set ) && ( prevInMaster->mTop == span->mBase )) {
				
					span->mPrevInMaster->mTop = span->mTop;
					this->FreeSpan ( span );
				}
				else {
					prevInSet = span;
				}
			}
		}
	}
	
	this->mSets [ merge ] = 0;
}

//----------------------------------------------------------------//
MOAISelectionMesh::MOAISelectionMesh () :
	mSpanListHead ( 0 ),
	mMesh ( 0 ) {

	RTTI_BEGIN ( MOAISelectionMesh )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAISelectionMesh >)
		RTTI_EXTEND ( MOAIDeckProxy )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISelectionMesh::~MOAISelectionMesh () {
}

//----------------------------------------------------------------//
void MOAISelectionMesh::PrintSelection ( ZLIndex set ) {

	if ( set < this->mSets.Size ()) {
	
		printf ( "set %d - ", ( int )(( ZLSize )set ));
	
		MOAISelectionSpan* span = ( MOAISelectionSpan* )this->mSets [ set ];
		for ( ; span; span = ( MOAISelectionSpan* )span->mNext ) {
		
			printf ( "%d:[%d-%d]", ( int )(( ZLSize )span->mSetID ) + 1, ( int )(( ZLSize )span->mBase ), ( int )(( ZLSize )span->mTop ));
			
			if ( span->mNext ) {
				printf ( ", " );
			}
		}
		printf ( "\n" );
	}
}

//----------------------------------------------------------------//
void MOAISelectionMesh::PrintSelections () {

	MOAISelectionSpan* span = this->mSpanListHead;
	for ( ; span; span = span->mNextInMaster ) {
	
		printf ( "%d:[%d-%d]", ( int )(( ZLSize )span->mSetID ) + 1, ( int )(( ZLSize )span->mBase ), ( int )(( ZLSize )span->mTop ));
		
		if ( span->mNextInMaster ) {
			printf ( ", " );
		}
	}
	printf ( "\n" );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::ReserveSelections ( ZLSize total ) {

	this->Clear ();
	
	this->mSets.Resize ( total, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISelectionMesh::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAISelectionMesh::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "addSelection",			_addSelection },
		{ "clearSelection",			_clearSelection },
		{ "mergeSelection",			_mergeSelection },
		{ "printSelection",			_printSelection },
		{ "reserveSelections",		_reserveSelections },
		{ "setDeck",				_setMesh }, // override
		{ "setMesh",				_setMesh },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISelectionMesh::MOAIDeck_Draw ( ZLIndex idx ) {
	
	if ( !this->mMesh ) return;
	
	size_t size = this->mSets.Size ();
	if ( !size ) return;

	ZLIndex itemIdx =  ZLIndexOp::Wrap ( idx, size );
	
	MOAIMeshSpan* span = this->mSets [ itemIdx ];
	if ( !span ) return;

	this->mMesh->DrawIndex ( idx, span );
}
