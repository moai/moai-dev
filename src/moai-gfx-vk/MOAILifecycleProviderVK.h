// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILIFECYCLEPROVIDERVK_H
#define MOAILIFECYCLEPROVIDERVK_H

template < typename PROVIDER_TYPE > class MOAILifecycleProviderVK;

//================================================================//
// MOAIAbstractLifecycleClientVK
//================================================================//
template < typename PROVIDER_TYPE >
class MOAIAbstractLifecycleClientVK {
private:

	friend class MOAILifecycleProviderVK < PROVIDER_TYPE >;

	ZLLeanLink < MOAIAbstractLifecycleClientVK* > mClientListLink;
	PROVIDER_TYPE* mProvider;

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractLifecycleClientVK_Finalize		() = 0;

	//----------------------------------------------------------------//
	void SetInitializer ( PROVIDER_TYPE& provider ) {
		MOAILifecycleProviderVK < PROVIDER_TYPE >* check = &provider; // trigger an error if wrong type
		
		// not strictly needed; just here to do something with 'check'
		if ( check ) {
			this->mProvider = provider;
		}
	}

protected:

	//----------------------------------------------------------------//
	MOAIAbstractLifecycleClientVK () :
		mClientListLink ( this ),
		mProvider ( NULL ) {
	}

public:

	GET ( PROVIDER_TYPE*, LifecycleProvider, mProvider )

	//----------------------------------------------------------------//
	MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& operator = ( const MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& other ) {
		assert ( other.mProvider == NULL );
		return *this;
	}

	//----------------------------------------------------------------//
	MOAIAbstractLifecycleClientVK ( const MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& other ) :
		mClientListLink ( this ),
		mProvider ( NULL ) {
		assert ( other.mProvider == NULL );
	}

	//----------------------------------------------------------------//
	~MOAIAbstractLifecycleClientVK () {
		assert ( this->mProvider == NULL );
	}
	
	//----------------------------------------------------------------//
	void Finalize () {
		if ( this->mProvider ) {
			this->mProvider->FinalizeClient ( *this );
		}
	}
};

//================================================================//
// MOAILifecycleProviderVK
//================================================================//
template < typename PROVIDER_TYPE >
class MOAILifecycleProviderVK {
protected:

	ZLLeanList < MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >* >	mClients;
	
public:

	//----------------------------------------------------------------//
	void AddClient ( PROVIDER_TYPE& self, MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& client ) {
	
		if ( client.mProvider == &self ) return;
		assert ( client.mProvider == NULL );
		this->mClients.PushBack ( client.mClientListLink );
		client.mProvider = &self;
	}

	//----------------------------------------------------------------//
	void FinalizeClient ( MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& client ) {
	
		assert ( client.mProvider != NULL );
		assert ( client.mClientListLink.InList ( this->mClients ));
	
		ZLSize count = this->mClients.Count ();
		assert ( count > 0 );
		client.MOAIAbstractLifecycleClientVK_Finalize ();
		assert ( this->mClients.Count () < count ); // make sure client was actually removed from list
	}
	
	//----------------------------------------------------------------//
	void FinalizeClients () {
		while ( this->mClients.Count ()) {
			this->FinalizeClient ( *this->mClients.Front ());
		}
	}
	
	//----------------------------------------------------------------//
	MOAILifecycleProviderVK () {
	}
	
	//----------------------------------------------------------------//
	~MOAILifecycleProviderVK () {
		assert ( this->mClients.Count () == 0 );
	}
	
	//----------------------------------------------------------------//
	void RemoveClient ( MOAIAbstractLifecycleClientVK < PROVIDER_TYPE >& client ) {
		
		assert ( client.mProvider != NULL );
		assert ( client.mClientListLink.InList ( this->mClients ));
		
		this->mClients.Remove ( client.mClientListLink );
		client.mProvider = NULL;
	}
};

#endif
