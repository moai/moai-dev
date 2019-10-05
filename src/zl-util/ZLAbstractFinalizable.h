// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_H
#define	ZLABSTRACTFINALIZABLE_H

// OK, so what is this thing? sometimes we want to finalize an object without deallocating it. for
// example, if an object is a class member or lives on the stack, but has some resources we want
// to release.
//
// the typical approach is to implement a bunch of ad hoc cleanup methods (clear, release, destroy, etc.),
// but these get hard to maintain in deep and complicated class hierarchies (like moai uses everywhere
// all the time). in short, C++ destructors in class hierarchies are called in a determinstic order,
// going up the inheritance tree until every destructor has been called. this is *different* from the way
// virtual functions behave: in a virtual or overloaded function, the user must remember to *explicitely*
// call each superclass' cleanup functions, which requires a lot of boilerplate and care.
//
// so, in all cases, destructors are the best option for cleanup. the problem is how to call them
// safely on instances that we don't also control allocation for. we could use placement
// delete, but that would lead to destructors being called twice: first for the explicit cleanup,
// then again when the instance is deleted or goes out of scope. requiring destructors to be
// idempotent is slightly better than making explicit superclass calls in cleanup functions, but
// not much: theremight be less boilerplate (maybe), but we'd need to remember to add extra checks
// in the destructor, which is still error-prone.
//
// the solution offered here is probably the least of all evils. it works by calling placement delete,
// immediately followed by placement new. so when an instance is manually finalised, its destructor is
// called and then its constructor, producing a "clean" instance. since the constructor was called,
// the instance remains "valid" after explicit finalization and is this safe to destruct again normally.
//
// in some cases it won't be ideal to call the constructor again. for example, if the constructor
// allocates a resource or performs a costly allocation. (OK, this really should never happen, but
// hey - we should still allow for it). in these cases, an alternate "reconstructor" may be
// specified simply by specifying a unique paramater list to pass to the placement new operator
// when the instance is re-initialized.
//
// that's the basic part of this implementation. the rest of it deals with maintaining a
// directed acyclic graph (DAG) of finalizable classes, the intent being that when a finalizable
// instance depends on one or more other finalizables, it should automatically be finalized
// when any one of its dependencies are. originally, this was factored into two parts - a
// lightweight, stand-alone implementation of finalizable, and a separate DAG implementation based
// on it. in practice, the only additional (memory) overhead for the DAG is a single pointer in the base
// class, and users will almost always want the DAG functionality as well, so it made sense to support
// everything in a single abstract base class.

#include <zl-util/ZLAbstractFinalizable_HasInternal.h>
#include <zl-util/ZLAbstractFinalizable_Internal.h>

// this is ugly, but lordy do we hate boilerplate. even more than we hate macros.
// alas, templates introduce issues of their own, as does storing strongly typed
// pointers. so without dynamic casting, this our least hated option. not too onerous,
// and, for better or worse, it is moai-like.

// ZLAbstractFinalizable_Finalize () does the magic. it stores a typeless pointer
// to the class instance, calls placement delete and then placement new, passing
// along whatever constructor args the user specifies. there's also a static_cast
// that aims to produce a compilation error if the wrong TYPE is passed in. it's not
// completely foolproof, but can't hurt.

// the GetProvier() template method lies dormant unless called, in which case
// there needs to be a matching _HasDependencyOn template in the inheritance tree.
// SetProvider(), the same.

#define IMPLEMENT_FINALIZABLE(TYPE,...)						\
															\
	void ZLAbstractFinalizable_Finalize () {				\
		( void )static_cast < TYPE* >( this );				\
		void* self = this;									\
		(( TYPE* )self )->~TYPE ();							\
		new ( this ) TYPE ( __VA_ARGS__ );					\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	PROVIDER_TYPE& GetProvider () {							\
		return this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::GetProvider ();		\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	bool HasProvider () {									\
		return this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::HasProvider ();		\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	void SetProvider ( PROVIDER_TYPE& provider ) {			\
		this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::SetProvider ( provider );	\
	}

#define IMPLEMENT_ABSTRACT_FINALIZABLE(TYPE)				\
															\
	void ZLAbstractFinalizable_Finalize () {				\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	PROVIDER_TYPE& GetProvider () {							\
		return this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::GetProvider ();		\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	bool HasProvider () {									\
		return this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::HasProvider ();		\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	void SetProvider ( PROVIDER_TYPE& provider ) {			\
		this->ZLAbstractFinalizable_HasDependencyOn < PROVIDER_TYPE >::SetProvider ( provider );	\
	}

//================================================================//
// ZLAbstractFinalizable
//================================================================//
class ZLAbstractFinalizable :

	// has intenral is a separate class just to enforce the use of virtual inheritance.
	// otherwise, user would have to remember to always use virtual inheritance or
	// face conflicts with _HasDependencyOn (see below).
	public virtual ZLAbstractFinalizable_HasInternal {
private:
	
	// use a pimpl/internal in case DAG functionality isn't needed
	ZLAbstractFinalizable_Internal*		mInternal;
	
	//----------------------------------------------------------------//
	virtual void						ZLAbstractFinalizable_Finalize							() = 0;
	ZLAbstractFinalizable_Internal&		ZLAbstractFinalizable_HasInternal_AffirmInternal		();
	ZLAbstractFinalizable_Internal*		ZLAbstractFinalizable_HasInternal_GetInternal			();

protected:

	// handy flag for passing as a dummy parameter to a "reconstructor." pass it to the
	// IMPLEMENT_FINALIZABLE() macro for a quick-and-dirty reconstructor signatrue.
	enum ReconstructionFlag {
		RECONSTRUCTING,
	};
	
	//----------------------------------------------------------------//
	void				FinalizeDependencies			();

public:

	//----------------------------------------------------------------//
						ZLAbstractFinalizable			();
	virtual				~ZLAbstractFinalizable			();
	void				Finalize						(); // this is it! manual, explicit finalization through C++ destructors.
};

// inherit a _HasDependencyOn for each DAG dependency provider you want to track. each _HasDependencyOn shares
// a common _HasInternal (through virtual inheritance), which will match that of the ZLAbstractFinalizable's
// own _HasInternal. if there are multiple providers, accessing the provider or its getter will require
// disambiguation, as each inherited _HasDependencyOn shares the same class member names. for this reason,
// IMPLEMENT_FINALIZABLE provides a helper getter, implemented as a template function, parameterized
// with the type of the provider.

// it's worth noting that _HasDependencyOn could have been implemented as a "smart-pointer" like class, but
// the downside would be that an extra pointer to either the containing class or its _HasInternal would be
// needed (and would have to be passed in on construction or when setting the provider). the pointer would
// also be the same for every provider dependency specified. composition through inheritance is moai-like
// and spares us that redundant pointer.

//================================================================//
// ZLAbstractFinalizable_HasDependencyOn
//================================================================//
template < typename PROVIDER_TYPE >
class ZLAbstractFinalizable_HasDependencyOn :
	public virtual ZLAbstractFinalizable_HasInternal {
private:

	friend class ZLAbstractFinalizable;

	PROVIDER_TYPE*		mProvider;

protected:

	//----------------------------------------------------------------//
	PROVIDER_TYPE& GetProvider () {
		assert ( this->mProvider );
		return *this->mProvider;
	}
	
	//----------------------------------------------------------------//
	bool HasProvider () {
		return ( this->mProvider != NULL );
	}

	//----------------------------------------------------------------//
	void SetProvider ( PROVIDER_TYPE& provider ) {
		
		// we only allow the provider to be set once, because the *meaning* of
		// setting a provider is that if the provider goes away for any reason,
		// the client should be automatically finalized.
		
		// it's certainly possibly to support swapping out providers, but the
		// use case for tha isn't clear right now. the typical use will be to
		// instantiate/construct, initialize, finalize, and then
		// deallocate/destruct.
		
		assert ( !this->mProvider );
		
		ZLAbstractFinalizable_Internal& clientMembership	= this->AffirmInternal ();
		ZLAbstractFinalizable_Internal& providerMembership	= provider.AffirmInternal ();
		
		clientMembership.AddProvider ( provider );
		providerMembership.AddClient ( clientMembership.GetOwner ());
		
		this->mProvider = &provider;
	}

public:

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_HasDependencyOn () :
		mProvider ( NULL ) {
		
		// make sure the provider is also a sublass of ZLAbstractFinalizable
		( void )static_cast < ZLAbstractFinalizable* >(( PROVIDER_TYPE* )NULL );
	}

	//----------------------------------------------------------------//
	~ZLAbstractFinalizable_HasDependencyOn () {
		
		this->mProvider = NULL;
	}
};

#endif
