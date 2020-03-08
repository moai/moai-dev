// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLRTTI_H
#define	ZLRTTI_H

#include <zl-common/zl_types.h>
#include <zl-util/STLMap.h>
#include <zl-util/ZLType.h>

class RTTILinkBase;
class RTTIRecord;

#define RTTI_BEGIN(name) \
	this->BeginRTTI ( this, #name );

#define RTTI_EXTEND(super) \
	this->ExtendRTTI < super >( this );

#define RTTI_VISITOR(interface, adapter) \
	this->RegisterVisitor < interface, adapter >();

#define RTTI_END \
	this->EndRTTI ();

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
// RTTIRecord
//================================================================//
class RTTIRecord {
private:

	static const u32 MAX = 32;

	STLString 		mClassName;

	RTTILinkBase*	mLinks [ MAX ];
	u32				mLinkCount;
	bool			mIsConstructed;
	
	RTTIRecord*		mTypeSet [ MAX ]; // set of all superclass records
	u32				mTypeCount;
	ptrdiff_t		mJumpTable [ MAX ]; // offset to superclass records
	bool			mIsComplete;

	ZLTypeID		mTypeID;

	STLMap < ZLTypeID, void* > mVisitors;
	STLMap < ZLTypeID, ZLLeanArray < void* > > mVisitorArrays;

	//----------------------------------------------------------------//
	void						AffirmCasts				( void* ptr );
	void*						AsType					( ZLTypeID typeID, void* ptr );
	void						BuildVisitorArrays 		();
	void						Complete				();
	void*						GetVisitor				( ZLTypeID visitorTypeID );
	ZLLeanArray < void* >&		GetVisitors				( ZLTypeID visitorTypeID  );
	void						Inherit					( RTTIRecord& record, void* ptr, ptrdiff_t offset );
	bool						IsType					( ZLTypeID typeID, void* ptr );

	//----------------------------------------------------------------//
	template < typename TYPE, typename SUPER_TYPE >
	void AddLink () {
		
		if ( this->mIsConstructed ) return;
		
		RTTILinkBase& link = RTTILink < TYPE, SUPER_TYPE >::Get ();
		link.mTarget = &RTTIRecord::Get < SUPER_TYPE >();
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
// RTTIBase
//================================================================//
class RTTIBase {
private:
	
	RTTIRecord* mRTTI;
	void* mThis;

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	void BeginRTTI ( TYPE* ptr, cc8* name ) {
		this->mThis = ptr;
		this->mRTTI = &RTTIRecord::Get < TYPE >();
		this->mRTTI->mClassName = name;
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
	ZLLeanArray < ABSTRACT_VISITOR_TYPE* >& GetVisitors () {
	
		this->mRTTI->AffirmCasts ( this->mThis );
		return ( ZLLeanArray < ABSTRACT_VISITOR_TYPE* >& )this->mRTTI->GetVisitors ( ZLType::GetID < ABSTRACT_VISITOR_TYPE >());
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
