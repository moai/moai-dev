//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

// build an inheritance tree from hell

// 	  E
//  / | \
//	K D |
//	| | B
//	J C |
//	\ | /
//	  A

//----------------------------------------------------------------//
class E {
public:

	static int sCounter;

	E () {
		sCounter++;
		printf ( "HELLO FROM E!\n" );
	}
	virtual ~E () {}
};

int E::sCounter = 0;

//----------------------------------------------------------------//
class D : virtual public E {
public:
	D () { printf ( "HELLO FROM D!\n" ); }
	virtual ~D () {}
};

//----------------------------------------------------------------//
class C : virtual public D {
public:
	C () { printf ( "HELLO FROM C!\n" ); }
	virtual ~C () {}
};

//----------------------------------------------------------------//
class B : virtual public E {
public:
	B () { printf ( "HELLO FROM B!\n" ); }
	virtual ~B () {}
};

//----------------------------------------------------------------//
class K : virtual public E {
public:
	K () { printf ( "HELLO FROM K!\n" ); }
	virtual ~K () {}
};

//----------------------------------------------------------------//
class J : virtual public K {
public:
	J () { printf ( "HELLO FROM J!\n" ); }
	virtual ~J () {}
};

//----------------------------------------------------------------//
class A : virtual public B, virtual public C, virtual public J {
public:
	A () { printf ( "HELLO FROM A!\n" ); }
	virtual ~A () {}
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, virtual_inheritance ) {
	
	E::sCounter = 0;
	
	A a;
	A* ap = &a;
	printf ( "%p\n", ap );
	
	printf ( "E: %p\n", ( E* )ap );
	printf ( "E: %p\n", ( E* )(( B* )ap ));
	printf ( "E: %p\n", ( E* )(( C* )ap ));
	printf ( "E: %p\n", ( E* )(( J* )ap ));
	
	ASSERT_TRUE ( E::sCounter == 1 );
	
	ASSERT_TRUE (( E* )ap  == ( E* )(( B* )ap ));
	ASSERT_TRUE (( E* )ap  == ( E* )(( C* )ap ));
	ASSERT_TRUE (( E* )ap  == ( E* )(( J* )ap ));
	
	MOAIGraphicsProp* graphicsProp = new MOAIGraphicsProp ();
	
	MOAINode* n0 = MOAICast < MOAINode >( graphicsProp );
	ASSERT_TRUE ( n0 != NULL );
	
	MOAINode* n1 = MOAICast < MOAINode >( MOAICast < MOAIDeckHolderWithIndex >( graphicsProp ));
	MOAINode* n2 = MOAICast < MOAINode >( MOAICast < MOAIPartitionHull >( MOAICast < MOAIGraphicsPropBase >( graphicsProp )));
	MOAINode* n3 = MOAICast < MOAINode >( MOAICast < MOAIColor >( MOAICast < MOAIGraphicsPropBase >( graphicsProp )));
	
	printf ( "n0: %p\n", n0 );
	printf ( "n1: %p\n", n1 );
	printf ( "n2: %p\n", n2 );
	printf ( "n3: %p\n", n3 );
	
	ASSERT_TRUE ( n0 == n1 );
	ASSERT_TRUE ( n0 == n2 );
	ASSERT_TRUE ( n0 == n3 );
	
	delete graphicsProp;
}
