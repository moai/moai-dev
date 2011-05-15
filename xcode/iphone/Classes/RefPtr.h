//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>

#define PROPERTY(type,name) @property(nonatomic,assign) type name;

#define PROPERTY_READONLY(type,name) @property(readonly,nonatomic,assign) type name;

#define SYNTHESIZE_PTR_READONLY(type,lower,upper) \
	-( type ) lower { return [[ m##upper.Get () retain ] autorelease ]; } \
	-( void ) set##upper :( type )assign { NSAssert ( false, @"value is read only" ); }

#define SYNTHESIZE_PTR_RETAIN(type,lower,upper) \
	-( type ) lower { return [[ m##upper.Get () retain ] autorelease ]; } \
	-( void ) set##upper :( type )assign { m##upper.Set ( assign ); }

#define SYNTHESIZE_PTR_COPY(type,lower,upper) \
	-( type ) lower { return [[ m##upper.Get () retain ] autorelease ]; } \
	-( void ) set##upper :( type )assign { m##upper.SetCopy ( assign ); }

#define SYNTHESIZE(type,lower,upper) \
	-( type ) lower { return m##upper; } \
	-( void ) set##upper :( type )assign { m##upper = assign; }

#define SYNTHESIZE_READONLY(type,lower,upper) \
	-( type ) lower { return m##upper; } \
	-( void ) set##upper :( type )assign { NSAssert ( false, @"value is read only" ); }

#define SYNTHESIZE_BITFLAG(lower,upper,member,mask) \
	-( bool ) lower { return (( member & mask ) == mask ); } \
	-( void ) set##upper :( bool )assign { \
		if ( assign ) { member |= mask; } \
		else { member &= ~mask; } \
	}

#define SYNTHESIZE_REF(type,lower,upper) \
	-( type ) lower { return &m##upper; } \
	-( void ) set##upper :( type )assign { m##upper = *assign; }

#define SYNTHESIZE_DYNAMIC(lower) \
	@dynamic lower;

//================================================================//
// RefPtr
//================================================================//
template < typename TYPE >
class RefPtr {
protected:

	id			mRef;

public:

	//----------------------------------------------------------------//
	inline operator bool () {
		return mRef != nil;
	};

	//----------------------------------------------------------------//
	inline TYPE* operator * () const {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline TYPE* operator -> () const {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline void operator = ( TYPE* assign ) {
		this->Set (( id )assign );
	};
	
	//----------------------------------------------------------------//
	inline void operator = ( const RefPtr < TYPE >& assign ) {
		this->Set ( assign.mRef );
	};
	
	//----------------------------------------------------------------//
	inline BOOL operator == ( TYPE* compare ) {
		return ( this->mRef == compare );
	};
	
	//----------------------------------------------------------------//
	inline BOOL operator == ( const RefPtr < TYPE >& compare ) {
		return ( this->mRef == compare.mRef );
	};

	//----------------------------------------------------------------//
	inline TYPE* Get () const {
		return this->mRef;
	}

	//----------------------------------------------------------------//
	inline TYPE* Id () const {
		return this->Get ();
	}

	//----------------------------------------------------------------//
	inline TYPE* New () {
		
		TYPE* assign = [ TYPE alloc ];
		[ this->mRef release ];
		this->mRef = assign;
		
		return assign;
	}

	//----------------------------------------------------------------//
	RefPtr () :
		mRef ( nil ) {
	}

	//----------------------------------------------------------------//
	RefPtr ( TYPE* assign ) {
		this->mRef = nil;
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	RefPtr ( const RefPtr < TYPE >& assign ) {
		this->mRef = nil;
		this->Set ( assign.mRef );
	};
	
	//----------------------------------------------------------------//
	~RefPtr () {
		[ mRef release ];
		mRef = nil;
	}
	
	//----------------------------------------------------------------//
	inline void Set ( id assign ) {
		if ( this->mRef != assign ) {
			[ assign retain ];
			[ this->mRef release ];
			this->mRef = assign;
		}
	}
	
	//----------------------------------------------------------------//
	inline void SetCopy ( id assign ) {
		if ( this->mRef != assign ) {
			id copy = [ assign copy ];
			[ this->mRef release ];
			this->mRef = copy;
		}
	}
};

//================================================================//
// RefPtr < objc_object* >
//================================================================//
template <>
class RefPtr < id > :
	public RefPtr < objc_object > {
public:

	//----------------------------------------------------------------//
	inline void operator = ( id assign ) {
		this->Set ( assign );
	};
	
	//----------------------------------------------------------------//
	inline void operator = ( const RefPtr < id >& assign ) {
		this->Set ( assign.mRef );
	};
	
	//----------------------------------------------------------------//
	RefPtr () {
	}

	//----------------------------------------------------------------//
	RefPtr ( id assign ) {
		this->mRef = nil;
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	RefPtr ( const RefPtr < id >& assign ) {
		this->mRef = nil;
		this->Set ( assign.mRef );
	};
	
	//----------------------------------------------------------------//
	~RefPtr () {
		[ mRef release ];
		mRef = nil;
	}
};

