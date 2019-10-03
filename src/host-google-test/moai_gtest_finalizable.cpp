//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <zl-util/headers.h>
#include "moai_gtest.h"

using namespace std;

namespace moai_gtest_finalizable {

#define CLASS_E "E"
#define CLASS_D "D"

static GTESTLifecycleTracker sLifecycle;

//----------------------------------------------------------------//
class E :
	public ZLAbstractFinalizable {
public:

	IMPLEMENT_FINALIZABLE ( E )

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

	IMPLEMENT_FINALIZABLE ( D, ReconstructionFlag::RECONSTRUCTING )

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
TEST_F ( GTESTMoaiContext, finalizable ) {
	
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 0, 0, 0 ));
	ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 0, 0, 0 ));
	
	{
		D d;
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_E, 1, 0, 1 ));
		ASSERT_TRUE ( sLifecycle.Check ( CLASS_D, 1, 0, 1 ));
		
		d.Finalize ();
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
