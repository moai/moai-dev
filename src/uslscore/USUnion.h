// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	UNION_H
#define	UNION_H

#include <uslscore/USFactory.h>

//================================================================//
// USUnion
//================================================================//
template < typename BASE_TYPE >
class USUnion {
private:

	u32				mBufferSize;
	void*			mBuffer;
	BASE_TYPE*		mType;

	//----------------------------------------------------------------//
	void AffirmBuffer ( u32 bufferSize ) {

		if ( bufferSize > this->mBufferSize ) {

			if ( this->mBuffer ) {
				free ( this->mBuffer );
			}
			this->mBufferSize = bufferSize;
			this->mBuffer = malloc ( bufferSize );
			this->mType = ( BASE_TYPE* )this->mBuffer;
		}
	}

	//----------------------------------------------------------------//
	void Clear () {
	
		if ( this->mType ) {
			this->mType->BASE_TYPE::~BASE_TYPE ();
			this->mType = 0;
		}
	}

public:

	//----------------------------------------------------------------//
	inline operator bool () {
		return this->mType != 0;
	};

	//----------------------------------------------------------------//
	inline BASE_TYPE& operator * () {
		return *this->Get ();
	};

	//----------------------------------------------------------------//
	inline BASE_TYPE* operator -> () {
		return this->mType;
	};

	//----------------------------------------------------------------//
	inline operator BASE_TYPE* () {
		return this->mType;
	};

	//----------------------------------------------------------------//
	BASE_TYPE& Get () {
	
		return *this->mType;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& Set () {
		
		this->Clear ();
		
		// static creator
		static USConcreteCreator < TYPE, BASE_TYPE > creator;

		this->AffirmBuffer ( creator.SizeOf ());
		creator.Place ( this->mBuffer );

		return ( *( TYPE* )this->mBuffer );
	}

	//----------------------------------------------------------------//
	USUnion () :
		mBufferSize ( 0 ),
		mBuffer ( 0 ),
		mType ( 0 ) {
	}

	//----------------------------------------------------------------//
	~USUnion () {

		this->Clear ();

		if ( this->mBuffer ) {
			free ( this->mBuffer );
			this->mBuffer = 0;
		}
	}
};

#endif
