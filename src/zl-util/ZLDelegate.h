// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDELEGATE_H
#define ZLDELEGATE_H

#define UNARY_SELECTOR_TYPE(type,param) void ( type::* )( param )
#define UNARY_SELECTOR_DECL(type,param,name) void ( type::*name )( param )

//================================================================//
// ZLFunctor
//================================================================//
template < typename PARAM >
class ZLFunctor {
public:

	//----------------------------------------------------------------//
	virtual void Call ( PARAM param ) = 0;

	//----------------------------------------------------------------//
	virtual ~ZLFunctor () {
	}
};

//================================================================//
// ZLDelegate
//================================================================//
template < typename TYPE, typename PARAM >
class ZLDelegate :
	public ZLFunctor < PARAM > {
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
	ZLDelegate () :
		mTarget ( 0 ),
		mSelector ( 0 ) {
	}
};

//================================================================//
// ZLCallback
//================================================================//
template < typename PARAM >
class ZLCallback {
private:

	static const u32 FUNCTOR_SIZE = 32;

	ZLFunctor < PARAM >* mFunc;
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
			this->mFunc->ZLFunctor < PARAM >::~ZLFunctor ();
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
		
		this->Functor < ZLDelegate < TYPE, PARAM > >().Set ( target, selector );
	}
	
	//----------------------------------------------------------------//
	ZLCallback () :
		mFunc ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLCallback () {
		this->Clear ();
	}
};

#endif
