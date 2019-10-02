//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

using namespace std;

namespace moai_gtest_destructors {

#define CLASS_E "E"
#define CLASS_D "D"

//----------------------------------------------------------------//
class LifecycleCount {
public:

	int mConstructed;
	int mDestructed;
	
	int Alive () {
		return this->mConstructed - this->mDestructed;
	}
	
	void Construct () {
		this->mConstructed++;
	}
	
	void Destruct () {
		this->mDestructed++;
	}
	
	LifecycleCount () :
		mConstructed ( 0 ),
		mDestructed ( 0 ) {
	}
};

//----------------------------------------------------------------//
class Lifecycle {
public:

	STLMap < string, LifecycleCount > mLifecycle;

	int Alive ( string classname ) {
		int alive = this->mLifecycle [ classname ].Alive ();
		printf ( "ALIVE %s: %d!\n", classname.c_str (), alive );
		return alive;
	}

	bool Check ( string classname, int constructed, int destructed, int alive ) {
		LifecycleCount& count = this->mLifecycle [ classname ];
		return (
			( count.mConstructed == constructed ) &&
			( count.mDestructed == destructed ) &&
			( count.Alive () == alive )
		);
	}

	void Construct ( string classname ) {
		printf ( "CONSTRUCTED %s!\n", classname.c_str ());
		this->mLifecycle [ classname ].Construct ();
	}

	int Constructed ( string classname ) {
		return this->mLifecycle [ classname ].mConstructed;
	}

	void Destruct ( string classname ) {
		printf ( "DESTRUCTED %s!\n", classname.c_str ());
		this->mLifecycle [ classname ].Destruct ();
	}
	
	int Destructed ( string classname ) {
		return this->mLifecycle [ classname ].mDestructed;
	}
}

static sLifecycle;

//----------------------------------------------------------------//
class AbstractDestructifiable {
public:

	enum ReconstructionFlag {
		RECONSTRUCTING,
	};

	virtual void	AbstractDestructifiable_Destructify ( void* self ) = 0;
	virtual void*	AbstractDestructifiable_GetSelf () = 0;
	
	virtual ~AbstractDestructifiable () {
	}
	
	void Destructify () {
		void* self = AbstractDestructifiable_GetSelf ();
		this->AbstractDestructifiable_Destructify ( self );
	}
};

#define IMPLEMENT_DESTRUCTIFIABLE(TYPE,...)						\
void AbstractDestructifiable_Destructify ( void* self ) {		\
	(( TYPE* )self )->~TYPE ();									\
	new ( this ) TYPE ( __VA_ARGS__ );							\
}																\
																\
void* AbstractDestructifiable_GetSelf () {						\
	return this;												\
}

#define IMPLEMENT_DESTRUCTIFIABLE_WITH_RECONSTRUCTOR(TYPE)		\
	void AbstractDestructifiable_Destructify ( void* self ) {	\
		(( TYPE* )self )->~TYPE ();								\
		new ( this ) TYPE ( RECONSTRUCTING );					\
	}															\
																\
	void* AbstractDestructifiable_GetSelf () {					\
		return this;											\
	}

//----------------------------------------------------------------//
class E :
	public AbstractDestructifiable {
public:

	IMPLEMENT_DESTRUCTIFIABLE ( E )

	E () {
		sLifecycle.Construct ( CLASS_E );
	}

	virtual ~E () {
		sLifecycle.Destruct ( CLASS_E );
	}
};

//----------------------------------------------------------------//
class D :
	public E {
public:

	IMPLEMENT_DESTRUCTIFIABLE ( D, ReconstructionFlag::RECONSTRUCTING )

	D () {
		sLifecycle.Construct ( CLASS_D );
	}

	D ( ReconstructionFlag ) :
		D () {
		printf ( "RECONSTRUCTED D!\n" );
	}

	~D () {
		sLifecycle.Destruct ( CLASS_D );
	}
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, desctructors ) {
	
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 0, 0, 0 ));
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 0, 0, 0 ));
	
	{
		D d;
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 1, 0, 1 ));
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 1, 0, 1 ));
		
		d.Destructify ();
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 2, 1, 1 ));
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 2, 1, 1 ));
	}
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 2, 2, 0 ));
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 2, 2, 0 ));
	
	{
		D d;
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 3, 2, 1 ));
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 3, 2, 1 ));
	}
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 3, 3, 0 ));
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 3, 3, 0 ));
}

}; // namespace moai_gtest_destructors
