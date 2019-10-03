//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"
#include "ZLAbstractLinkedFinalizable.h"
#include "ZLLinkedFinalizableWithDependency.h"

using namespace std;

namespace moai_gtest_linked_finalizable {

#define CLASS_PROVIDER_0 "P0"
#define CLASS_PROVIDER_1 "P1"
#define CLASS_PROVIDER_2 "P2"

#define CLASS_CLIENT_0 "C0"
#define CLASS_CLIENT_1 "C1"
#define CLASS_CLIENT_2 "C2"

static GTESTLifecycleTracker sLifecycle;

//----------------------------------------------------------------//
class Provider0 :
	public ZLAbstractLinkedFinalizable {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Provider0 )
	
	//----------------------------------------------------------------//
	~Provider0 () {
		sLifecycle.Destruct ( CLASS_PROVIDER_0 );
	}
};

//----------------------------------------------------------------//
class Provider1 :
	public ZLAbstractLinkedFinalizable {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Provider1 )
	
	//----------------------------------------------------------------//
	~Provider1 () {
		sLifecycle.Destruct ( CLASS_PROVIDER_1 );
	}
};

//----------------------------------------------------------------//
class Provider2 :
	public ZLAbstractLinkedFinalizable {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Provider2 )
	
	//----------------------------------------------------------------//
	~Provider2 () {
		sLifecycle.Destruct ( CLASS_PROVIDER_2 );
	}
};

//----------------------------------------------------------------//
class Client0 :
	public ZLAbstractLinkedFinalizable,
	public ZLLinkedFinalizableWithDependency < Provider0 >,
	public ZLLinkedFinalizableWithDependency < Provider1 > {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Client0 )
	
	//----------------------------------------------------------------//
	~Client0 () {
		sLifecycle.Destruct ( CLASS_CLIENT_0 );
	}
};

//----------------------------------------------------------------//
class Client1 :
	public ZLAbstractLinkedFinalizable,
	 public ZLLinkedFinalizableWithDependency < Provider1 >,
	 public ZLLinkedFinalizableWithDependency < Provider2 > {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Client1 )
	
	//----------------------------------------------------------------//
	~Client1 () {
		sLifecycle.Destruct ( CLASS_CLIENT_1 );
	}
};

//----------------------------------------------------------------//
class Client2 :
	public ZLAbstractLinkedFinalizable {
public:
	IMPLEMENT_LINKED_FINALIZEABLE ( Client2 )
	
	//----------------------------------------------------------------//
	~Client2 () {
		sLifecycle.Destruct ( CLASS_CLIENT_2 );
	}
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, linked_finalizable ) {

	{
		Provider0	provider0;
		Provider1	provider1;
		Provider2	provider2;
		
		Client0 client0;
		Client1 client1;
		
		client0.ZLLinkedFinalizableWithDependency < Provider0 >::SetProvider ( provider0 );
		client0.ZLLinkedFinalizableWithDependency < Provider1 >::SetProvider ( provider1 );
		
		client1.ZLLinkedFinalizableWithDependency < Provider1 >::SetProvider ( provider1 );
		client1.ZLLinkedFinalizableWithDependency < Provider2 >::SetProvider ( provider2 );
		
		ASSERT_TRUE ( &client0.GetProvider < Provider0 >() == &provider0 );
		ASSERT_TRUE ( &client0.GetProvider < Provider1 >() == &provider1 );
		
		ASSERT_TRUE ( &client1.GetProvider < Provider1 >() == &provider1 );
		ASSERT_TRUE ( &client1.GetProvider < Provider2 >() == &provider2 );
		
		provider1.Finalize ();
		
		ASSERT_TRUE ( sLifecycle.Destructed ( CLASS_PROVIDER_1 ) == 1 );
		ASSERT_TRUE ( sLifecycle.Destructed ( CLASS_CLIENT_0 ) == 1 );
		ASSERT_TRUE ( sLifecycle.Destructed ( CLASS_CLIENT_1 ) == 1 );
		
		provider0.Finalize ();
		provider2.Finalize ();
		
		ASSERT_TRUE ( sLifecycle.Destructed ( CLASS_PROVIDER_0 ) == 1 );
		ASSERT_TRUE ( sLifecycle.Destructed ( CLASS_PROVIDER_2 ) == 1 );
	}
}

}; // namespace moai_gtest_destructors
