// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTYPE_H
#define	ZLTYPE_H

//================================================================//
// ZLTypeID
//================================================================//
class ZLTypeID {
private:
	
	friend class ZLType;
	
	u32		mTypeID;

	//----------------------------------------------------------------//
	ZLTypeID ( u32 typeID ) :
		mTypeID ( typeID ) {
	}

public:

	//----------------------------------------------------------------//
	bool operator < ( const ZLTypeID& v ) const {
		return ( this->mTypeID < v.mTypeID );
	}

	//----------------------------------------------------------------//
	bool operator == ( const ZLTypeID& v ) const {
		return ( this->mTypeID == v.mTypeID );
	}
	
	//----------------------------------------------------------------//
	bool operator != ( const ZLTypeID& v ) const {
		return ( this->mTypeID == v.mTypeID );
	}

	//----------------------------------------------------------------//
	ZLTypeID () :
	 	mTypeID ( -1 ) {
	}

	//----------------------------------------------------------------//
	ZLTypeID ( const ZLTypeID& typeID ) :
		mTypeID ( typeID.mTypeID ) {
	}
};

//================================================================//
// ZLType
//================================================================//
class ZLType {
public:

	typedef u32 RAW_ID;
	
	static const ZLTypeID NONE;

private:

	//----------------------------------------------------------------//
	static RAW_ID GetUniqueID () {
		static RAW_ID counter = 1;
		return counter++;
	};

public:
	
	//----------------------------------------------------------------//
	// TODO: why is this here?
	template < typename TYPE >
	static const TYPE& Dummy () {
		assert ( false );
		return ZLType::Dummy < TYPE >();
	}
	
	//----------------------------------------------------------------//
	// TODO: why is this here?
	template < typename TYPE >
	static TYPE& DummyRef () {
		assert ( false );
		static TYPE dummy = TYPE ();
		return dummy;
	}
	
	//----------------------------------------------------------------//
	static ZLTypeID FromRaw ( u32 typeID ) {
		return ZLTypeID ( typeID );
	};
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static ZLTypeID GetID () {
		return ZLTypeID ( ZLType::RawID < TYPE >());
	};
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static RAW_ID RawID () {
		static RAW_ID rawID = ZLType::GetUniqueID ();
		return rawID;
	};
	
	//----------------------------------------------------------------//
	static RAW_ID ToRaw ( const ZLTypeID& typeID ) {
		return typeID.mTypeID;
	};
};

#endif
