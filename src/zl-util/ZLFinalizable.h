// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFINALIZABLE_H
#define	ZLFINALIZABLE_H

#include <zl-util/ZLAbstractFinalizable.h>

#define IMPLEMENT_DEPENDS_ON(TYPE)							\
															\
template < typename DEPENDENCY_TYPE >						\
DEPENDENCY_TYPE& GetDependency () {							\
	return this->ZLFinalizable_DependsOn < DEPENDENCY_TYPE >::GetDependency (); \
}															\
															\
template < typename DEPENDENCY_TYPE >						\
bool HasDependency () {										\
	return this->ZLFinalizable_DependsOn < DEPENDENCY_TYPE >::HasDependency (); \
}															\
															\
template < typename DEPENDENCY_TYPE >						\
void SetDependency ( DEPENDENCY_TYPE& dependency ) {		\
	this->ZLFinalizable_DependsOn < DEPENDENCY_TYPE >::SetDependency ( dependency ); \
}

//================================================================//
// ZLFinalizable
//================================================================//
class ZLFinalizable :
	public virtual ZLAbstractFinalizable {
public:

	//----------------------------------------------------------------//
					ZLFinalizable							();
					~ZLFinalizable							();
};

// inherit a _DependsOn for each DAG dependency you want to track. each _HasDependencyOn shares
// a common _HasInternal (through virtual inheritance). if there are multiple dependencies, accessing
// the dependency or its getter will require disambiguation, as each inherited _HasDependencyOn shares
// the same class member names. for this reason, IMPLEMENT_DEPENDS_ON provides a helper getter, implemented
// as a template function, parameterized with the type of the dependency.

// it's worth noting that _DependsOn could have been implemented as a "smart-pointer" like class, but
// the downside would be that an extra pointer to either the containing class or its _HasInternal would be
// needed (and would have to be passed in on construction or when setting the provider). the pointer would
// also be the same for every provider dependency specified. composition through inheritance is moai-like
// and spares us that redundant pointer.

//================================================================//
// ZLFinalizable_DependsOn
//================================================================//
template < typename DEPENDENCY_TYPE >
class ZLFinalizable_DependsOn :
	public virtual ZLAbstractFinalizable {
private:

//	friend class ZLAbstractFinalizable;

	DEPENDENCY_TYPE*		mDependency;

protected:

	//----------------------------------------------------------------//
	DEPENDENCY_TYPE& GetDependency () {
		assert ( this->mDependency );
		return *this->mDependency;
	}
	
	//----------------------------------------------------------------//
	bool HasDependency () {
		return ( this->mDependency != NULL );
	}

	//----------------------------------------------------------------//
	void SetDependency ( DEPENDENCY_TYPE& dependency ) {
		
		// we only allow the dependency to be set once, because the *meaning* of
		// setting a dependency is that if the dependency goes away for any reason,
		// the client should be automatically finalized.
		
		// it's certainly possibly to support swapping out dependencies, but the
		// use case for tha isn't clear right now. the typical use will be to
		// instantiate/construct, initialize, finalize, and then
		// deallocate/destruct.
		
		assert ( !this->mDependency );
		
		ZLAbstractFinalizableImpl& clientMembership = this->AffirmImpl ();
		ZLAbstractFinalizableImpl& dependencyMembership = dependency.AffirmImpl ();
		
		clientMembership.AddUpstream ( dependency );
		dependencyMembership.AddDownstream ( clientMembership.GetOwner ());
		
		this->mDependency = &dependency;
	}

public:

	//----------------------------------------------------------------//
	ZLFinalizable_DependsOn () :
		mDependency ( NULL ) {
		
		// make sure the provider is also a sublass of ZLAbstractFinalizable
		( void )static_cast < ZLAbstractFinalizable* >(( DEPENDENCY_TYPE* )NULL );
	}

	//----------------------------------------------------------------//
	~ZLFinalizable_DependsOn () {
		
		this->mDependency = NULL;
	}
};

#endif
