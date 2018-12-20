//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

//----------------------------------------------------------------//
template < typename TYPE >
class HasInterface {
public:

	//----------------------------------------------------------------//
	virtual TYPE* AbstractGetInterface () = 0;
	TYPE* GetInterface () { return this->AbstractGetInterface (); }
	virtual ~HasInterface () {}
	
};

//----------------------------------------------------------------//
template < typename TYPE >
class ImplementsInterface :
	public virtual HasInterface < TYPE >,
	public virtual TYPE {
public:

	//----------------------------------------------------------------//
	TYPE* AbstractGetInterface () { return this; }
};

//----------------------------------------------------------------//
class FooInterface {
public:

	//----------------------------------------------------------------//
	virtual void Foo () = 0;
	virtual ~FooInterface () {}
};

//----------------------------------------------------------------//
class BarInterface {
public:

	//----------------------------------------------------------------//
	virtual void Bar () = 0;
	virtual ~BarInterface () {}
};

//----------------------------------------------------------------//
class MixinWithFooBar :
	public virtual HasInterface < FooInterface >,
	public virtual HasInterface < BarInterface > {
public:

	//----------------------------------------------------------------//
	void SayHello () {
		this->HasInterface < FooInterface >::GetInterface ()->Foo ();
		this->HasInterface < BarInterface >::GetInterface ()->Bar ();
	}
};

//----------------------------------------------------------------//
class ConcreteFooA :
	public virtual ImplementsInterface < FooInterface > {
public:

	//----------------------------------------------------------------//
	void Foo () { printf ( "Hello from ConcreteFooA!\n" ); };
};

//----------------------------------------------------------------//
class ConcreteFooB :
	public virtual ImplementsInterface < FooInterface > {
public:

	//----------------------------------------------------------------//
	void Foo () { printf ( "Hello from ConcreteFooB!\n" ); };
};

//----------------------------------------------------------------//
class ConcreteFooASubclass :
	public virtual ConcreteFooA {
public:

	//----------------------------------------------------------------//
	void Foo () { printf ( "Hello from ConcreteFooASubclass!\n" ); };
};

//----------------------------------------------------------------//
class ConcreteBarA :
	public virtual ImplementsInterface < BarInterface > {
public:

	//----------------------------------------------------------------//
	void Bar () { printf ( "Hello from ConcreteBarA!\n" ); };
};

//----------------------------------------------------------------//
class ConcreteBarB :
	public virtual ImplementsInterface < BarInterface > {
public:

	//----------------------------------------------------------------//
	void Bar () { printf ( "Hello from ConcreteBarB!\n" ); };
};

//----------------------------------------------------------------//
class FinalWithFooBarA :
	public virtual MixinWithFooBar,
	public virtual ConcreteFooA,
	public virtual ConcreteBarA {
};

//----------------------------------------------------------------//
class FinalWithFooBarB :
	public virtual MixinWithFooBar,
	public virtual ConcreteFooB,
	public virtual ConcreteBarB {
};

//----------------------------------------------------------------//
class FinalWithConcreteSubclass :
	public virtual MixinWithFooBar,
	public virtual ConcreteFooASubclass,
	public virtual ConcreteBarB {
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, composition ) {

	FinalWithFooBarA fa;
	fa.SayHello ();

	FinalWithFooBarB fb;
	fb.SayHello ();
	
	FinalWithConcreteSubclass fsc;
	fsc.SayHello ();
}
