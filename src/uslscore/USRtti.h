// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	RTTI_H
#define	RTTI_H

class USLuaSerializer;
class USLuaState;
class USStream;
class USSerializationContext;

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
	virtual s32				GetOffset			( void* ptr ) = 0;
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
	s32 GetOffset ( void* ptr ) {
		return ( s32 )(( SUPER_TYPE* )( TYPE* )ptr ) - ( s32 )( ptr );
	}
};

//================================================================//
// RTTIRecord
//================================================================//
/**	@brief Class to store run time type information for a single type.
*/
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
/**	@brief RTTIBase class for any class hierarchy requiring RTTI.
*/
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
	/**	@brief Extends the RTTI class hierarchy.  This should be called by the constructor
		of each class in the hierarchy.
		@tparam TYPE Superclass of type being constructed.
	*/
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
/**	@brief Convenience template class for automatically extending the RTTI hierarchy.
	To use, inherit it and pass in the current class (the class that you are defining) along
	with its super class.  A 'shim' class will be created in the hierarchy (between the class
	and its superclass) which will call RTTIBase::ExtendRTTI() automatically in its constructor.
	Note that the shim class itself will be excluded from the RTTI hierarchy.
	
	@tparam TYPE Type of the leaf class (which should inherit from RTTI).
	@tparam TYPE Type of the superclass.
*/
template < typename TYPE, typename SUPER >
class RTTI :
	public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	/**	@brief Constructor.
	*/
	RTTI () {
		this->template SimpleRTTI < SUPER >(( TYPE* )this );
	}
	
	//----------------------------------------------------------------//
	/**	@brief Destructor.
	*/
	~RTTI () {
	}
};

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* USCast ( RTTIBase* object ) {

	if ( object ) {
		return object->AsType < TYPE >();
	}
	return 0;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* USCast ( RTTIBase& object ) {

	return object.AsType < TYPE >();
}

#endif
