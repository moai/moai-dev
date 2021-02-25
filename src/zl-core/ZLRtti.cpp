// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLRtti.h>

//================================================================//
// RTTIRecord
//================================================================//

//----------------------------------------------------------------//
void RTTIRecord::AffirmCasts ( void* ptr ) {

	if ( !this->mIsComplete ) {
		this->GatherSupers ( *this, ptr, 0, 0 );
		sort ( this->mSupers.begin (), this->mSupers.end ());
		this->BuildVisitorArrays ();
	}
	this->mIsComplete = true;
}

//----------------------------------------------------------------//
void* RTTIRecord::AsType ( ZLTypeID typeID, void* ptr ) {

	if ( this->mTypeID == typeID ) return ptr;
	
	this->AffirmCasts ( ptr );

	// TODO: binary search?
	ZLSize superCount = this->mSupers.size ();
	for ( ZLIndex i = superCount; i > 0; --i ) {
	
		const RTTISuper& super = this->mSupers [ i - 1 ];
	
		if ( super.mRecord->mTypeID == typeID ) {
			return ( void* )(( ptrdiff_t )ptr + super.mJump );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void RTTIRecord::BuildVisitorArrays () {

	// for each super
	for ( ZLIndex i = 0; i < this->mSupers.size (); ++i ) {
		const RTTIRecord* superRecord = this->mSupers [ i ].mRecord;
		
		// for each visitor in each super
		STLMap < ZLTypeID, const void* >::const_iterator visitorIt = superRecord->mVisitors.cbegin ();
		for ( ; visitorIt != superRecord->mVisitors.end (); ++visitorIt ) {
			this->mVisitorArrays [ visitorIt->first ].push_back ( visitorIt->second );
		}
	}

	// for each visitor in self
	STLMap < ZLTypeID, const void* >::iterator visitorIt = this->mVisitors.begin ();
	for ( ; visitorIt != this->mVisitors.end (); ++visitorIt ) {
		this->mVisitorArrays [ visitorIt->first ].push_back ( visitorIt->second );
	}
}

//----------------------------------------------------------------//
void RTTIRecord::Complete () {

	this->mIsConstructed = true;
}

//----------------------------------------------------------------//
void RTTIRecord::GatherSupers ( RTTIRecord& record, void* ptr, ptrdiff_t offset, ZLSize depth ) {
	
	// TODO: maybe sort type set for binary search? or by frequency of use?
	
	// if not own record, add to type set
	if ( this != &record ) {
	
		// bail if already inherited
		for ( ZLIndex i = 0; i < this->mSupers.size (); ++i ) {
			if ( this->mSupers [ i ].mRecord->mTypeID == record.mTypeID ) {
				return; // TODO: warn about this
			}
		}
		
		RTTISuper super;
		
		super.mRecord = &record; // type info for superclass
		super.mJump = offset; // offset to convert pointer to superclass
		super.mDepth = depth;
		
		this->mSupers.push_back ( super );
	}
	
	// follow the links and inherit them
	for ( u32 i = 0; i < record.mLinkCount; ++i ) {
		
		RTTILinkBase& link = *record.mLinks [ i ];
		
		ptrdiff_t jump = link.GetOffset ( ptr );
		RTTIRecord& nextRecord = *link.mTarget;
		
		// walk up the class hierarchy
		void* nextPtr = ( void* )(( ptrdiff_t )ptr + jump );
		
		this->GatherSupers ( nextRecord, nextPtr, offset + jump, depth + 1 );
	}
}

//----------------------------------------------------------------//
const void* RTTIRecord::GetVisitor ( ZLTypeID visitorTypeID ) const {

	STLMap < ZLTypeID, const void* >::const_iterator visitorIt = this->mVisitors.find ( visitorTypeID );
	return ( visitorIt != this->mVisitors.end ()) ? visitorIt->second : NULL;
}

//----------------------------------------------------------------//
const STLArray < const void* >& RTTIRecord::GetVisitors ( ZLTypeID visitorTypeID  ) const {

	assert ( this->mIsComplete );

	static STLArray < const void* > staticDummy;
	STLMap < ZLTypeID, STLArray < const void* > >::const_iterator visitorArrayIt = this->mVisitorArrays.find ( visitorTypeID );
	return ( visitorArrayIt != this->mVisitorArrays.end ()) ? visitorArrayIt->second : staticDummy;
}

//----------------------------------------------------------------//
bool RTTIRecord::IsType ( ZLTypeID typeID, void* ptr ) {

	if ( this->mTypeID == typeID ) return true;
	
	this->AffirmCasts ( ptr );

	// TODO: binary search?
	ZLSize superCount = this->mSupers.size ();
	for ( ZLIndex i = superCount; i > 0; --i ) {
		if ( this->mSupers [ i - 1 ].mRecord->mTypeID == typeID ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
RTTIRecord::RTTIRecord ( ZLTypeID typeID ) :
	mLinkCount ( 0 ),
	mIsConstructed ( false ),
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
RTTIBase::RTTIBase () :
	mRTTI ( NULL ),
	mThis ( NULL ) {
	if ( this->BeginRTTI < RTTIBase >( this, "RTTIBase" )) {
		this->EndRTTI ();
	}
}

//----------------------------------------------------------------//
RTTIBase::~RTTIBase () {
}

//----------------------------------------------------------------//
cc8* RTTIBase::TypeName () const {
	return this->mRTTI->mClassName.c_str ();
}
