// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRTTI_H
#define	MOAIRTTI_H

class RTTILinkBase;
class RTTIRecord;

#define RTTI_SINGLE(super) \
	this->SimpleRTTI < super >( this );

#define RTTI_BEGIN \
	this->BeginRTTI ( this );

#define RTTI_EXTEND(super) \
	this->ExtendRTTI < super >( this );

#define RTTI_END \
	this->EndRTTI ();

//================================================================//
// RTTILinkBase
//================================================================//
class RTTILinkBase {
private:

	RTTIRecord* mTarget;

public:

	friend class RTTIRecord;

	//----------------------------------------------------------------//
							RTTILinkBase		() {}
	virtual					~RTTILinkBase		() {}
	virtual sintptr			GetOffset			( void* ptr ) = 0;
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
	sintptr GetOffset ( void* ptr ) {
		return ( sintptr )(( SUPER_TYPE* )( TYPE* )ptr ) - ( sintptr )( ptr );
	}
};

//================================================================//
// RTTIRecord
//================================================================//
class RTTIRecord {
private:

	static const u32 MAX = 16;

	RTTILinkBase*	mLinks [ MAX ];
	u32				mLinkCount;
	bool			mIsConstructed;
	
	RTTIRecord*		mTypeSet [ MAX ];
	u32				mTypeCount;
	s32				mJumpTable [ MAX ];
	bool			mIsComplete;

	//----------------------------------------------------------------//
	void		AffirmCasts		( void* ptr );
	void*		AsType			( RTTIRecord& record, void* ptr );
	void		Complete		();
	void		Inherit			( RTTIRecord& record, void* ptr, s32 offset );
	bool		IsType			( RTTIRecord& record, void* ptr );

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
		static RTTIRecord single;
		return single;
	}

public:

	friend class RTTIBase;

	//----------------------------------------------------------------//
		RTTIRecord		();
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
	void BeginRTTI ( TYPE* ptr ) {
		this->mThis = ptr;
		this->mRTTI = &RTTIRecord::Get < TYPE >();
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
	
	//----------------------------------------------------------------//
	template < typename SUPER_TYPE, typename TYPE >
	void SimpleRTTI ( TYPE* ptr ) {
		this->BeginRTTI < TYPE >( ptr );
		this->ExtendRTTI < SUPER_TYPE >( ptr );
		this->EndRTTI ();
	}

public:

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AsType () {
		
		RTTIRecord& record = RTTIRecord::Get < TYPE >();
		return ( TYPE* )this->mRTTI->AsType ( record, this->mThis );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsType () {
		
		RTTIRecord& record = RTTIRecord::Get < TYPE >();
		return this->mRTTI->IsType ( record, this->mThis );
	}
	
	//----------------------------------------------------------------//
					RTTIBase		();
	virtual			~RTTIBase		();
	virtual cc8*	TypeName		() const;
};

//================================================================//
// RTTI
//================================================================//
// Convenience template class for automatically extending the RTTI hierarchy.
// To use, inherit it and pass in the current class (the class that you are defining) along
// with its super class.  A 'shim' class will be created in the hierarchy (between the class
// and its superclass) which will call RTTIBase::ExtendRTTI() automatically in its constructor.
// Note that the shim class itself will be excluded from the RTTI hierarchy.
template < typename TYPE, typename SUPER >
class RTTI :
	public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	RTTI () {
		this->template SimpleRTTI < SUPER >(( TYPE* )this );
	}
	
	//----------------------------------------------------------------//
	~RTTI () {
	}
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
TYPE* MOAICast ( RTTIBase& object ) {

	return object.AsType < TYPE >();
}

#endif
