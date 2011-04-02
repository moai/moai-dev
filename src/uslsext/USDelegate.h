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
