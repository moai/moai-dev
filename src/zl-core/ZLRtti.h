// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLRTTI_H
#define	ZLRTTI_H

#include <zl-core/STLArray.h>
#include <zl-core/STLMap.h>
#include <zl-core/STLString.h>
#include <zl-core/ZLType.h>

class RTTILinkBase;
class RTTIRecord;

#define RTTI_BEGIN(name) \
	if ( this->BeginRTTI ( this, #name )) {

#define RTTI_EXTEND(super) \
	this->ExtendRTTI < super >( this );

#define RTTI_VISITOR(interface, adapter) \
	this->RegisterVisitor < interface, adapter >();

#define RTTI_END \
	this->EndRTTI (); }

#define RTTI_SINGLE(name, super) \
	RTTI_BEGIN ( name ) \
	RTTI_EXTEND ( super ) \
	RTTI_END

//================================================================//
// RTTILinkBase
//================================================================//
class RTTILinkBase {
private:

	RTTIRecord* mTarget;

public:

	friend class RTTIRecord;

	//----------------------------------------------------------------//
	virtual ptrdiff_t		GetOffset			( void* ptr ) = 0;
							RTTILinkBase		() {}
	virtual					~RTTILinkBase		() {}
};

//================================================================//
// RTTILink
//================================================================//
template < typename TYPE, typename SUPER_TYPE >
class RTTILink :
	public RTTILinkBase {
public:

	//----------------------------------------------------------------//
	static RTTILink& Get () {
		static RTTILink single;
		return single;
	}

	//----------------------------------------------------------------//
	ptrdiff_t GetOffset ( void* ptr ) {
		return ( ptrdiff_t )(( SUPER_TYPE* )( TYPE* )ptr ) - ( ptrdiff_t )( ptr );
	}
};

//================================================================//
// RTTISuper
//================================================================//
class RTTISuper {
private:

	friend class RTTIRecord;

	const RTTIRecord* 		mRecord;
	ptrdiff_t 				mJump;
	ZLSize					mDepth;

public:

	//----------------------------------------------------------------//
	inline bool operator < ( const RTTISuper& compare ) const {
		return ( this->mDepth > compare.mDepth );
	}
};

//================================================================//
// RTTIRecord
//================================================================//
class RTTIRecord {
private:

	static const u32 MAX = 32;

	STLString 				mClassName;

	RTTILinkBase*			mLinks [ MAX ];
	u32						mLinkCount;
	bool					mIsConstructed;
	
	STLArray < RTTISuper >	mSupers; // set of all superclass records
	bool					mIsComplete;

	ZLTypeID				mTypeID;

	STLMap < ZLTypeID, const void* > mVisitors;
	STLMap < ZLTypeID, STLArray < const void* > > mVisitorArrays;

	//----------------------------------------------------------------//
	void									AffirmCasts				( void* ptr );
	void*									AsType					( ZLTypeID typeID, void* ptr );
	void									BuildVisitorArrays 		();
	void									Complete				();
	void									GatherSupers			( RTTIRecord& record, void* ptr, ptrdiff_t offset, ZLSize depth );
	const void*								GetVisitor				( ZLTypeID visitorTypeID ) const;
	const STLArray < const void* >&			GetVisitors				( ZLTypeID visitorTypeID  ) const;
	bool									IsType					( ZLTypeID typeID, void* ptr );

	//----------------------------------------------------------------//
	template < typename TYPE, typename SUPER_TYPE >
	void AddLink () {
		
		if ( this->mIsConstructed ) return;
		
		RTTILinkBase& link = RTTILink < TYPE, SUPER_TYPE >::Get ();
		link.mTarget = &RTTIRecord::Get < SUPER_TYPE >();
		assert ( !( this->mTypeID == link.mTarget->mTypeID ));
		this->mLinks [ this->mLinkCount++ ] = &link;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	static RTTIRecord& Get () {
		static RTTIRecord single ( ZLType::GetID < TYPE >());
		return single;
	}

	//----------------------------------------------------------------//
	template < typename ABSTRACT_VISITOR_TYPE, typename CONCRETE_VISITOR_TYPE >
	void RegisterVisitor () {
	
		static CONCRETE_VISITOR_TYPE singleVisitor;
		ZLTypeID visitorTypeID = ZLType::GetID < ABSTRACT_VISITOR_TYPE >();
		this->mVisitors [ visitorTypeID ] = ( void* )&singleVisitor;
	}

	//----------------------------------------------------------------//
		RTTIRecord		( ZLTypeID typeID );

public:

	friend class RTTIBase;

	//----------------------------------------------------------------//
		~RTTIRecord		();
};

//================================================================//
// RTTIVisitorIterator
//================================================================//
template < typename ABSTRACT_VISITOR_TYPE >
class RTTIVisitorIterator {
private:

	friend class RTTIRecord;
	friend class RTTIBase;

	const STLArray < const void* >& mVisitors;
	int 		mIndex;
	int 		mStep;
	ZLSize 		mCount;

public:

	//----------------------------------------------------------------//
	operator bool () const {
		return (( 0 <= this->mIndex ) && ( this->mIndex < ( int )this->mVisitors.size ()));
	}
	
	//----------------------------------------------------------------//
	RTTIVisitorIterator& operator ++ () {
		this->mIndex += this->mStep;
		this->mCount++;
		return *this;
	}
	
	//----------------------------------------------------------------//
	const ABSTRACT_VISITOR_TYPE& operator * () const {
		return *( const ABSTRACT_VISITOR_TYPE* )this->mVisitors [ this->mIndex ];
	}
	
	//----------------------------------------------------------------//
	ZLSize GetCount () const {
		return this->mCount;
	}
	
	//----------------------------------------------------------------//
	RTTIVisitorIterator ( const STLArray < const void* >& visitors, bool forward = true ) :
		mVisitors ( visitors ),
		mStep ( forward ? 1 : -1 ),
		mCount ( 0 ) {
		this->mIndex = forward ? 0 : ( int )visitors.size () - 1;
	}
};

//================================================================//
// RTTIBase
//================================================================//
class RTTIBase {
private:
	
	RTTIRecord* mRTTI;
	void* mThis;

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	bool BeginRTTI ( TYPE* ptr, cc8* name ) {
	
		this->mRTTI = &RTTIRecord::Get < TYPE >();
		this->mThis = ptr;
	
		if ( this->mRTTI->mClassName.size () == 0 ) {
			this->mRTTI->mClassName = name;
			return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	void EndRTTI () {
		this->mRTTI->Complete ();
	}
	
	//----------------------------------------------------------------//
	template < typename SUPER_TYPE, typename TYPE >
	void ExtendRTTI ( TYPE* ptr ) {
		UNUSED ( ptr ); // passed in for template inference - simplify use of ExtendRTTI
		this->mRTTI->AddLink < TYPE, SUPER_TYPE >();
	}
	
public:

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AsType () {
		
		return ( TYPE* )this->mRTTI->AsType ( ZLType::GetID < TYPE >(), this->mThis );
	}
	
	//----------------------------------------------------------------//
	template < typename ABSTRACT_VISITOR_TYPE >
	const ABSTRACT_VISITOR_TYPE* GetVisitor () const {
	
		this->mRTTI->AffirmCasts ( this->mThis );
		return ( const ABSTRACT_VISITOR_TYPE* )this->mRTTI->GetVisitor ( ZLType::GetID < ABSTRACT_VISITOR_TYPE >());
	}
	
	//----------------------------------------------------------------//
	template < typename ABSTRACT_VISITOR_TYPE >
	RTTIVisitorIterator < ABSTRACT_VISITOR_TYPE > GetVisitors () const {
	
		this->mRTTI->AffirmCasts ( this->mThis );
		return RTTIVisitorIterator < ABSTRACT_VISITOR_TYPE >(
			this->mRTTI->GetVisitors ( ZLType::GetID < ABSTRACT_VISITOR_TYPE >())
		);
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsType () {
		
		return this->mRTTI->IsType ( ZLType::GetID < TYPE >(), this->mThis );
	}
	
	//----------------------------------------------------------------//
	template < typename ABSTRACT_ADAPTER_TYPE, typename CONCRETE_ADAPTER_TYPE >
	void RegisterVisitor () {
	
		this->mRTTI->RegisterVisitor < ABSTRACT_ADAPTER_TYPE, CONCRETE_ADAPTER_TYPE >();
	}
	
	//----------------------------------------------------------------//
	void*			AsType			( ZLTypeID typeID );
	bool			IsType			( ZLTypeID typeID );
					RTTIBase		();
	virtual			~RTTIBase		();
	virtual cc8*	TypeName		() const;
};

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* MOAICast ( RTTIBase* object ) {

	if ( object ) {
		return object->AsType < TYPE >();
	}
	return 0;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* MOAICastAssert ( RTTIBase* object ) {

	if ( object ) {
		TYPE* cast = object->AsType < TYPE >();
		assert ( cast );
		return cast;
	}
	return 0;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* MOAICast ( RTTIBase& object ) {

	return object.AsType < TYPE >();
}

#endif
