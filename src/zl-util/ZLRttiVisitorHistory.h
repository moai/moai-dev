// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLRTTIVISITORHISTORY_H
#define	ZLRTTIVISITORHISTORY_H

//================================================================//
// RTTIVisitorHistory
//================================================================//
class RTTIVisitorHistory {
private:

	STLSet < ZLType::RAW_ID > mTypeIDs;
	size_t mVisits;
	size_t mRevisits;

	//----------------------------------------------------------------//
	bool Visit ( ZLType::RAW_ID typeID ) {
	
		if ( this->mTypeIDs.contains ( typeID )) return true;
		this->mTypeIDs.insert ( typeID );
		return false;
	}

public:

	//----------------------------------------------------------------//
	size_t CountVisits () {
		return this->mVisits;
	}
	
	//----------------------------------------------------------------//
	size_t CountRevisits () {
		return this->mRevisits;
	}
	
	//----------------------------------------------------------------//
	RTTIVisitorHistory () :
		mVisits ( 0 ),
		mRevisits ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool Visit ( TYPE& self ) {
		UNUSED ( self );
		this->mVisits++;
		if ( this->Visit ( ZLType::RawID < TYPE >())) {
			this->mRevisits++;
			return true;
		}
		return false;
	}
};

#endif
