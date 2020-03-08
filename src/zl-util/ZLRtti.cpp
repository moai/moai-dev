// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLRtti.h>

//================================================================//
// RTTIRecord
//================================================================//

//----------------------------------------------------------------//
void RTTIRecord::AffirmCasts ( void* ptr ) {

	if ( !this->mIsComplete ) {
		this->Inherit ( *this, ptr, 0 );
		this->BuildVisitorArrays ();
	}
	this->mIsComplete = true;
}

//----------------------------------------------------------------//
void* RTTIRecord::AsType ( ZLTypeID typeID, void* ptr ) {

	if ( this->mTypeID == typeID ) return ptr;
	
	this->AffirmCasts ( ptr );

	// TODO: binary search?
	for ( u32 i = 0; i < this->mTypeCount; ++i ) {
		if ( this->mTypeSet [ i ]->mTypeID == typeID ) {
			return ( void* )(( ptrdiff_t )ptr + this->mJumpTable [ i ]);
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void RTTIRecord::BuildVisitorArrays () {

	void* adapters [ MAX ];
	ZLSize total = 0;

	STLMap < ZLTypeID, void* >::iterator visitorIt = this->mVisitors.begin ();
	for ( ; visitorIt != this->mVisitors.end (); ++visitorIt ) {
	
		ZLTypeID adapterTypeID = visitorIt->first;
		ZLLeanArray < void* >& adapterArray = this->mVisitorArrays [ adapterTypeID ];
		
		for ( ZLIndex i = 0; i < this->mTypeCount; ++i ) {
			void* adapter = this->mTypeSet [ i ]->GetVisitor ( adapterTypeID );
			if ( adapter ) {
				adapters [ total++ ] = adapter;
			}
		}
		
		adapterArray.Init ( total + 1 );
		adapterArray [ 0 ] = visitorIt->second;
		for ( ZLIndex i = 0; i < total; ++i ) {
			adapterArray [ i + 1 ] = adapters [ i ];
		}
	}
}

//----------------------------------------------------------------//
void RTTIRecord::Complete () {

	this->mIsConstructed = true;
}

//----------------------------------------------------------------//
void* RTTIRecord::GetVisitor ( ZLTypeID visitorTypeID ) {

	if ( this->mVisitors.contains ( visitorTypeID )) {
		return this->mVisitors [ visitorTypeID ];
	}
	return NULL;
}

//----------------------------------------------------------------//
ZLLeanArray < void* >& RTTIRecord::GetVisitors ( ZLTypeID visitorTypeID  ) {

	assert ( this->mIsComplete );

	static ZLLeanArray < void* > staticDummy;
	
	if ( this->mVisitors.contains ( visitorTypeID )) {
		return this->mVisitorArrays [ visitorTypeID ];
	}
	return staticDummy;
}

//----------------------------------------------------------------//
void RTTIRecord::Inherit ( RTTIRecord& record, void* ptr, ptrdiff_t offset ) {
	
	// TODO: maybe sort type set for binary search? or by frequency of use?
	
	// if not own record, add to type set
	if ( this != &record ) {
	
		// bail if already inherited
		for ( u32 i = 0; i < this->mTypeCount; ++i ) {
			if ( this->mTypeSet [ i ] == &record ) {
				return; // TODO: warn about this
			}
		}
		
		// can only handle MAX super classes
		assert ( this->mTypeCount < MAX );
		
		
		this->mTypeSet [ this->mTypeCount ] = &record; // type info for superclass
		this->mJumpTable [ this->mTypeCount ] = offset; // offset to convert pointer to superclass
		this->mTypeCount++;
	}
	
	// follow the links and inherit them
	for ( u32 i = 0; i < record.mLinkCount; ++i ) {
		
		RTTILinkBase& link = *record.mLinks [ i ];
		
		ptrdiff_t jump = link.GetOffset ( ptr );
		RTTIRecord& nextRecord = *link.mTarget;
		
		// walk up the class hierarchy
		void* nextPtr = ( void* )(( ptrdiff_t )ptr + jump );
		
		this->Inherit ( nextRecord, nextPtr, offset + jump );
	}
}

//----------------------------------------------------------------//
bool RTTIRecord::IsType ( ZLTypeID typeID, void* ptr ) {

	if ( this->mTypeID == typeID ) return true;
	
	this->AffirmCasts ( ptr );

	// TODO: binary search?
	for ( u32 i = 0; i < this->mTypeCount; ++i ) {
		if ( this->mTypeSet [ i ]->mTypeID == typeID ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
RTTIRecord::RTTIRecord ( ZLTypeID typeID ) :
	mLinkCount ( 0 ),
	mIsConstructed ( false ),
	mTypeCount ( 0 ),
	mIsComplete ( false ),
	mTypeID ( typeID ) {
}

//----------------------------------------------------------------//
RTTIRecord::~RTTIRecord () {
}

//================================================================//
// RTTIBase
//================================================================//

//----------------------------------------------------------------//
void* RTTIBase::AsType ( ZLTypeID typeID ) {

	return this->mRTTI->AsType ( typeID, this->mThis );
}

//----------------------------------------------------------------//
bool RTTIBase::IsType ( ZLTypeID typeID ) {

	return this->mRTTI->IsType ( typeID, this->mThis );
}

//----------------------------------------------------------------//
RTTIBase::RTTIBase () {
	this->BeginRTTI < RTTIBase >( this, "RTTIBase" );
	this->EndRTTI ();
}

//----------------------------------------------------------------//
RTTIBase::~RTTIBase () {
}

//----------------------------------------------------------------//
cc8* RTTIBase::TypeName () const {
	return this->mRTTI->mClassName.c_str ();
}
