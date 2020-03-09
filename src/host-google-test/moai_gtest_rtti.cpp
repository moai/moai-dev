//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

class IVisitAdapter;

//================================================================//
// AbstractVisitable
//================================================================//
class AbstractVisitable :
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
	AbstractVisitable () {
	}

	//----------------------------------------------------------------//
	virtual ~AbstractVisitable () {
	}

	//----------------------------------------------------------------//
	virtual void AbstractVisitable_Visit ( int x, int y ) = 0;
	
	//----------------------------------------------------------------//
	void VisitAll ( int x, int y );
};

//================================================================//
// AbstractVisitor
//================================================================//
class AbstractVisitor {
public:

	//----------------------------------------------------------------//
	AbstractVisitor () {
	}

	//----------------------------------------------------------------//
	virtual ~AbstractVisitor () {
	}

	//----------------------------------------------------------------//
	virtual void Visit ( AbstractVisitable& self, int x, int y ) const = 0;
};

//================================================================//
// Visitor
//================================================================//
template < typename TYPE >
class Visitor :
	public AbstractVisitor {
public:

	//----------------------------------------------------------------//
	virtual void Visit ( AbstractVisitable& self, int x, int y ) const {
	
		void ( TYPE::*funcPtr )( int, int ) = &TYPE::AbstractVisitable_Visit;
		( self.AsType < TYPE >()->*funcPtr )( x, y );
	}
};

//----------------------------------------------------------------//
void AbstractVisitable::VisitAll ( int x, int y ) {

	RTTIVisitor < AbstractVisitor > visitor = this->GetVisitor < AbstractVisitor >();
	for ( ; visitor; ++visitor ) {
		( *visitor ).Visit ( *this, x, y );
	}
}


//================================================================//
// A
//================================================================//
class A :
	public AbstractVisitable {
public:

	//----------------------------------------------------------------//
	A () {
		RTTI_BEGIN ( A )
			RTTI_VISITOR ( AbstractVisitor, Visitor < A >)
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
	void AbstractVisitable_Visit ( int x, int y ) {
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
			RTTI_VISITOR ( AbstractVisitor, Visitor < B >)
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
	void AbstractVisitable_Visit ( int x, int y ) {
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
			RTTI_VISITOR ( AbstractVisitor, Visitor < C >)
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
	void AbstractVisitable_Visit ( int x, int y ) {
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
			RTTI_VISITOR ( AbstractVisitor, Visitor < D >)
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
	void AbstractVisitable_Visit ( int x, int y ) {
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
