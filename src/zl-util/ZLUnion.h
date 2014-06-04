// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLUNION_H
#define	ZLUNION_H

#include <zl-util/ZLFactory.h>

//================================================================//
// ZLUnion
//================================================================//
template < typename BASE_TYPE >
class ZLUnion {
private:

	size_t			mBufferSize;
	void*			mBuffer;
	BASE_TYPE*		mType;

	//----------------------------------------------------------------//
	void AffirmBuffer ( size_t bufferSize ) {

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
		static ZLConcreteCreator < TYPE, BASE_TYPE > creator;

		this->AffirmBuffer ( creator.SizeOf ());
		creator.Place ( this->mBuffer );

		return ( *( TYPE* )this->mBuffer );
	}

	//----------------------------------------------------------------//
	ZLUnion () :
		mBufferSize ( 0 ),
		mBuffer ( 0 ),
		mType ( 0 ) {
	}

	//----------------------------------------------------------------//
	~ZLUnion () {

		this->Clear ();

		if ( this->mBuffer ) {
			free ( this->mBuffer );
			this->mBuffer = 0;
		}
	}
};

#endif
