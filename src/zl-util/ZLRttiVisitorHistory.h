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

	//----------------------------------------------------------------//
	bool DidVisit ( ZLType::RAW_ID typeID ) {
	
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
	template < typename TYPE >
	bool DidVisit ( TYPE& self ) {
		UNUSED ( self );
		this->mVisits++;
		return this->DidVisit ( ZLType::RawID < TYPE >());
	}
	
	//----------------------------------------------------------------//
	RTTIVisitorHistory () :
		mVisits ( 0 )  {
	}
};

#endif
