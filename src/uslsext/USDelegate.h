// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DELEGATE_H
#define DELEGATE_H

#define UNARY_SELECTOR_TYPE(type,param) void ( type::* )( param )
#define UNARY_SELECTOR_DECL(type,param,name) void ( type::*name )( param )

//================================================================//
// USFunctor
//================================================================//
template < typename PARAM >
class USFunctor {
public:

	//----------------------------------------------------------------//
	virtual void Call ( PARAM param ) = 0;

	//----------------------------------------------------------------//
	virtual ~USFunctor () {
	}
};

//================================================================//
// USDelegate
//================================================================//
template < typename TYPE, typename PARAM >
class USDelegate :
	public USFunctor < PARAM > {
protected:

	typedef UNARY_SELECTOR_DECL ( TYPE, PARAM, SelectorType );

	TYPE* mTarget;
	SelectorType mSelector;

public:

	//----------------------------------------------------------------//
	virtual void Call ( PARAM param ) {
		( this->mTarget->*this->mSelector )( param );
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* target, SelectorType selector ) {
		this->mTarget = target;
		this->mSelector = selector;
	}

	//----------------------------------------------------------------//
	USDelegate () :
		mTarget ( 0 ),
		mSelector ( 0 ) {
	}
};

//================================================================//
// USCallback
//================================================================//
/**
 * @brief	A handle to an implementation of USFunctor.
 *
 * To implement a callback mechanism between two classes, the 'caller'
 * class would have an initially un-set instance of USCallback as a 
 * member, and would provide a way for the 'callee' to access it.  
 * The callee class would be responsible for calling the templated
 * factory method Functor() on that USCallback (using the type USDelegate
 * if the callee is a C++ class, or GSDelegate if the callee is a Obj-C
 * class).  The callee can then initialize the created USFunctor subclass
 * to point to the desired member function of the callee.  Since the
 * USCallback allocated that USFunctor subclass instance, it maintains
 * ownership.  The caller is then free to invoke the Call() function
 * on its USCallback without having to know what implementation of
 * USFunctor is used under the hood.
 */
template < typename PARAM >
class USCallback {
private:

	static const u32 FUNCTOR_SIZE = 32;

	USFunctor < PARAM >* mFunc;
	u8 mBuffer [ FUNCTOR_SIZE ];

public:

	//----------------------------------------------------------------//
	inline operator bool () {
		return this->mFunc != 0;
	};

	//----------------------------------------------------------------//
	void Call ( PARAM param ) {
		if ( this->mFunc ) {
			this->mFunc->Call ( param );
		}
	}

	//----------------------------------------------------------------//
	void Clear () {
	
		if ( this->mFunc ) {
			this->mFunc->USFunctor < PARAM >::~USFunctor ();
			this->mFunc = 0;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename FUNCTOR_TYPE >
	FUNCTOR_TYPE& Functor () {
		
		this->Clear ();
		
		assert ( sizeof ( FUNCTOR_TYPE ) <= FUNCTOR_SIZE );
		
		new ( this->mBuffer ) FUNCTOR_TYPE (); 
		this->mFunc = ( FUNCTOR_TYPE* )this->mBuffer;
		
		return *( FUNCTOR_TYPE* )this->mFunc;
	}
	
	//----------------------------------------------------------------//
	/** 
	 * @brief	A convenience method for C++ classes, as an alternative
	 *			to the boiler-plate code of calling Functor() and then Set().
	 */
	template < typename TYPE >
	void Set ( TYPE* target, UNARY_SELECTOR_DECL ( TYPE, PARAM, selector )) {
		
		this->Functor < USDelegate < TYPE, PARAM > >().Set ( target, selector );
	}
	
	//----------------------------------------------------------------//
	USCallback () :
		mFunc ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~USCallback () {
		this->Clear ();
	}
};

#endif
