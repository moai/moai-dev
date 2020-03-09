//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

class IVisitAdapter;

//================================================================//
// IVisit
//================================================================//
class IVisit :
	virtual public RTTIBase {
public:

	//----------------------------------------------------------------//
	template < typename TYPE >
	IVisitAdapter* GetVisitables () {
		return NULL;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLSize CountVisitables () {
		return 0;
	}

	//----------------------------------------------------------------//
	IVisit () {
	}

	//----------------------------------------------------------------//
	virtual ~IVisit () {
	}

	//----------------------------------------------------------------//
	virtual void IVisit_Visit ( int x, int y ) = 0;
	
	//----------------------------------------------------------------//
	void VisitAll ( int x, int y );
};

//================================================================//
// IVisitAdapter
//================================================================//
class IVisitAdapter {
public:

	//----------------------------------------------------------------//
	IVisitAdapter () {
	}

	//----------------------------------------------------------------//
	virtual ~IVisitAdapter () {
	}

	//----------------------------------------------------------------//
	virtual void Visit ( IVisit& self, int x, int y ) const = 0;
};

//----------------------------------------------------------------//
void IVisit::VisitAll ( int x, int y ) {

	RTTIVisitor < IVisitAdapter > visitor = this->GetVisitor < IVisitAdapter >();
	for ( ; visitor; ++visitor ) {
		( *visitor ).Visit ( *this, x, y );
	}
}

//================================================================//
// VisitAdapter
//================================================================//
template < typename TYPE >
class VisitAdapter :
	public IVisitAdapter {
public:

	//----------------------------------------------------------------//
	virtual void Visit ( IVisit& self, int x, int y ) const {
		TYPE* cast = self.AsType < TYPE >();
		assert ( cast );
		cast->TYPE::IVisit_Visit ( x, y );
	}
};

//================================================================//
// A
//================================================================//
class A :
	public IVisit {
public:

	//----------------------------------------------------------------//
	A () {
		RTTI_BEGIN ( A )
			RTTI_EXTEND ( IVisit )
			RTTI_VISITOR ( IVisitAdapter, VisitAdapter < A >)
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~A () {
	}
	
	//----------------------------------------------------------------//
	virtual void Hello () {
		printf ( "HELLO FROM A!\n" );
	}
	
	//----------------------------------------------------------------//
	void IVisit_Visit ( int x, int y ) {
		printf ( "VISIT FROM A: %d %d\n", x, y );
	}
};


//================================================================//
// B
//================================================================//
class B :
	virtual public A {
public:

	//----------------------------------------------------------------//
	B () {
		RTTI_BEGIN ( B )
			RTTI_EXTEND ( A )
			RTTI_VISITOR ( IVisitAdapter, VisitAdapter < B >)
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~B () {
	}
	
	//----------------------------------------------------------------//
	void Hello () {
		printf ( "HELLO FROM B!\n" );
	}
	
	//----------------------------------------------------------------//
	void IVisit_Visit ( int x, int y ) {
		printf ( "VISIT FROM B: %d %d\n", x, y );
	}
};

//================================================================//
// C
//================================================================//
class C :
	virtual public A {
public:

	//----------------------------------------------------------------//
	C () {
		RTTI_BEGIN ( C )
			RTTI_EXTEND ( A )
			RTTI_VISITOR ( IVisitAdapter, VisitAdapter < C >)
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~C () {
	}
	
	//----------------------------------------------------------------//
	void Hello () {
		printf ( "HELLO FROM C!\n" );
	}
	
	//----------------------------------------------------------------//
	void IVisit_Visit ( int x, int y ) {
		printf ( "VISIT FROM C: %d %d\n", x, y );
	}
};

//================================================================//
// D
//================================================================//
class D :
	public B,
	public C {
public:

	//----------------------------------------------------------------//
	D () {
		RTTI_BEGIN ( D )
			RTTI_EXTEND ( B )
			RTTI_EXTEND ( C )
			RTTI_VISITOR ( IVisitAdapter, VisitAdapter < D >)
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~D () {
	}
	
	//----------------------------------------------------------------//
	void Hello () {
		printf ( "HELLO FROM D!\n" );
	}
	
	//----------------------------------------------------------------//
	void IVisit_Visit ( int x, int y ) {
		printf ( "VISIT FROM D: %d %d\n", x, y );
	}
};


//----------------------------------------------------------------//
TEST ( ZLRtti, rtti ) {

	D instance;
	A* a = instance.AsType < A >();
	B* b = a->AsType < B >();
	C* c = a->AsType < C >();
	printf ( "A: %p\n", a );
	printf ( "B: %p\n", b );
	printf ( "C: %p\n", c );

	a->Hello ();
	b->Hello ();
	c->Hello ();
	
	a->VisitAll ( 1, 2 );
	b->VisitAll ( 3, 4 );
	c->VisitAll ( 5, 6 );
}
