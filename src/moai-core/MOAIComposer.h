// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOMPOSER_H
#define	MOAICOMPOSER_H

#define MOAI_CALL_SUPER_ONCE(composer, type, func)				\
	if ( composer.Compose < type >()) this->type::func

//================================================================//
// MOAIComposer
//================================================================//
class MOAIComposer {
private:

	STLSet < ZLType::RAW_ID > mTypeIDs;

	//----------------------------------------------------------------//
	bool Compose ( ZLType::RAW_ID typeID ) {
	
		if ( this->mTypeIDs.contains ( typeID )) return false;
		this->mTypeIDs.insert ( typeID );
		return true;
	}

public:

	//----------------------------------------------------------------//
	template < typename TYPE >
	bool Compose () {
		return this->Compose ( ZLType::RawID < TYPE >());
	}
};

#endif
