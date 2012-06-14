// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIATTROP_H
#define	MOAIATTROP_H

//================================================================//
// MOAIAttrOp
//================================================================//
class MOAIAttrOp  {
protected:

	static const size_t MAX_SIZE = 64;

	u32 mTypeID;
	u32 mFlags;
	
	u8 mBuffer [ MAX_SIZE ];

public:
	
	GET_SET ( u32, Flags, mFlags )
	
	enum {
		NONE,
		ADD,
		CHECK,
		GET,
		SET,
	};
	
	static const u32	NULL_ATTR			= 0x3fffffff;
	static const u32	ATTR_ID_MASK		= 0x0000ffff;
	static const u32	CLASS_ID_MASK		= 0x3fff0000;
	static const u32	ATTR_FLAGS_MASK		= 0xC0000000;
	
	static const u32	ATTR_READ			= 0x40000000;
	static const u32	ATTR_WRITE			= 0x80000000;
	static const u32	ATTR_READ_WRITE		= 0xC0000000;
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE Apply ( const TYPE& attr, u32 op, u32 flags ) {
		
		this->mFlags = flags;
		
		switch ( op ) {
			case ADD:
				return attr + this->GetValue < TYPE >( attr );
				break;
			case GET: {
				this->SetValue < TYPE >( attr );
				break;
			}
			case SET: {
				return this->GetValue < TYPE >( attr );
				break;
			}
		}
		return attr;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE ApplyNoAdd ( const TYPE& attr, u32 op, u32 flags ) {
		
		this->mFlags = flags;
		
		switch ( op ) {
			case GET: {
				this->SetValue < TYPE >( attr );
				break;
			}
			case SET: {
				return this->GetValue < TYPE >( attr );
				break;
			}
		}
		return attr;
	}
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mTypeID = 0;
		this->mFlags = 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE GetValue ( const TYPE& attr ) const {

		if ( this->mTypeID == USTypeID < TYPE >::GetID ()) {
			return *( TYPE* )this->mBuffer;
		}
		return attr;
	}
	
	//----------------------------------------------------------------//
	inline bool IsValid () const {
		return ( this->mFlags != 0 );
	}

	//----------------------------------------------------------------//
	MOAIAttrOp () :
		mTypeID ( 0 ),
		mFlags ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIAttrOp () {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void SetValue ( const TYPE& value ) {

		assert ( sizeof ( TYPE ) <= MAX_SIZE );
		
		this->mTypeID = USTypeID < TYPE >::GetID ();
		*( TYPE* )this->mBuffer = value;
	}
};

#endif
