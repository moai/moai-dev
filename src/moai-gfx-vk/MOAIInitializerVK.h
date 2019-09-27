// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINITIALIZERVK_H
#define MOAIINITIALIZERVK_H

template < typename INITIALIZER_TYPE > class MOAIInitializerVK;

//================================================================//
// MOAIAbstractInitializerClientVK
//================================================================//
template < typename INITIALIZER_TYPE >
class MOAIAbstractInitializerClientVK {
private:

	friend class MOAIInitializerVK < INITIALIZER_TYPE >;

	ZLLeanLink < MOAIAbstractInitializerClientVK* > mClientListLink;
	INITIALIZER_TYPE* mInitializer;

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractInitializerClientVK_Finalize		() = 0;

	//----------------------------------------------------------------//
	void SetInitializer ( INITIALIZER_TYPE& initializer ) {
		MOAIInitializerVK < INITIALIZER_TYPE >* check = &initializer; // trigger an error if wrong type
		
		// not strictly needed; just here to do something with 'check'
		if ( check ) {
			this->mInitializer = initializer;
		}
	}

protected:

	//----------------------------------------------------------------//
	MOAIAbstractInitializerClientVK () :
		mClientListLink ( this ),
		mInitializer ( NULL ) {
	}

public:

	GET ( INITIALIZER_TYPE*, Initializer, mInitializer )

	//----------------------------------------------------------------//
	MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& operator = ( const MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& other ) {
		assert ( other.mInitializer == NULL );
		return *this;
	}

	//----------------------------------------------------------------//
	MOAIAbstractInitializerClientVK ( const MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& other ) :
		mClientListLink ( this ),
		mInitializer ( NULL ) {
		assert ( other.mInitializer == NULL );
	}

	//----------------------------------------------------------------//
	~MOAIAbstractInitializerClientVK () {
		assert ( this->mInitializer == NULL );
	}
	
	//----------------------------------------------------------------//
	void Finalize () {
		if ( this->mInitializer ) {
			this->mInitializer->FinalizeClient ( *this );
		}
	}
};

//================================================================//
// MOAIInitializerVK
//================================================================//
template < typename INITIALIZER_TYPE >
class MOAIInitializerVK {
protected:

	ZLLeanList < MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >* >	mClients;
	
public:

	//----------------------------------------------------------------//
	void AddClient ( INITIALIZER_TYPE& self, MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& client ) {
	
		assert ( client.mInitializer == NULL );
		this->mClients.PushBack ( client.mClientListLink );
		client.mInitializer = &self;
	}

	//----------------------------------------------------------------//
	void FinalizeClient ( MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& client ) {
	
		assert ( client.mInitializer != NULL );
		assert ( client.mClientListLink.InList ( this->mClients ));
	
		ZLSize count = this->mClients.Count ();
		assert ( count > 0 );
		client.MOAIAbstractInitializerClientVK_Finalize ();
		assert ( this->mClients.Count () < count ); // make sure client was actually removed from list
	}
	
	//----------------------------------------------------------------//
	void FinalizeClients () {
		while ( this->mClients.Count ()) {
			this->FinalizeClient ( *this->mClients.Front ());
		}
	}
	
	//----------------------------------------------------------------//
	MOAIInitializerVK () {
	}
	
	//----------------------------------------------------------------//
	~MOAIInitializerVK () {
		assert ( this->mClients.Count () == 0 );
	}
	
	//----------------------------------------------------------------//
	void RemoveClient ( MOAIAbstractInitializerClientVK < INITIALIZER_TYPE >& client ) {
		
		assert ( client.mInitializer != NULL );
		assert ( client.mClientListLink.InList ( this->mClients ));
		
		this->mClients.Remove ( client.mClientListLink );
		client.mInitializer = NULL;
	}
};

#endif
