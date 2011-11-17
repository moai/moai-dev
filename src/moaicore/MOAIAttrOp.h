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

	enum {
		TYPE_UNKNOWN,
		TYPE_COMPLEX,
		TYPE_NUMBER,
		TYPE_VALID,
	};
	
	u32 mType;
	u32 mComplexType;
	u32 mFlags;

	union {
		float			mNumber;
		const void*		mPtr;
		u8				mBuffer [ MAX_SIZE ];
	};

public:
	
	enum {
		NONE,
		ADD,
		CHECK,
		GET,
		SET,
	};

	//----------------------------------------------------------------//
	inline float Apply ( float attr, u32 op, u32 flags ) {
	
		this->mFlags = flags;

		switch ( op ) {
			case ADD:
				if ( this->mType == TYPE_NUMBER ) {
					return attr + this->mNumber;
				}
				break;
			case CHECK: {
				this->mType = TYPE_VALID;
				break;
			}
			case GET: {
				this->SetValue ( attr );
				break;
			}
			case SET: {
				return this->GetValue ();
				break;
			}
		}
		return attr;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE* Apply ( TYPE* attr, u32 op, u32 flags ) {

		this->mFlags = flags;
		
		switch ( op ) {
			case CHECK: {
				this->mType = TYPE_VALID;
				break;
			}
			case GET: {
				this->SetValue < TYPE >( attr );
				break;
			}
			case SET: {
				return this->GetValue < TYPE >();
				break;
			}
		}
		return attr;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void Apply ( TYPE& attr, u32 op, u32 flags ) {

		this->mFlags = flags;
		
		switch ( op ) {
			case CHECK: {
				this->mType = TYPE_VALID;
				break;
			}
			case GET: {
				this->SetValue < TYPE >( attr );
				break;
			}
			case SET: {
				this->GetValue < TYPE >( attr );
				break;
			}
		}
	}
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mType = TYPE_UNKNOWN;
	}
	
	//----------------------------------------------------------------//
	inline u32 GetFlags () const {
		return this->mFlags;
	}
	
	//----------------------------------------------------------------//
	float GetValue () const {
		return this->mType == TYPE_NUMBER ? this->mNumber : 0.0f;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE* GetValue () const {

		if (( this->mType == TYPE_COMPLEX ) && ( this->mComplexType == USTypeID < TYPE >::GetID ())) {
			return ( TYPE* )this->mPtr;
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline bool GetValue ( TYPE& value ) const {

		if (( this->mType == TYPE_COMPLEX ) && ( this->mComplexType == USTypeID < TYPE >::GetID ())) {
			memcpy ( &value, this->mBuffer, sizeof ( TYPE ));
			return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	inline bool IsNumber () const {
		return ( this->mType == TYPE_NUMBER );
	}
	
	//----------------------------------------------------------------//
	inline bool IsValid () const {
		return ( this->mType != TYPE_UNKNOWN );
	}

	//----------------------------------------------------------------//
	MOAIAttrOp () :
		mType ( TYPE_UNKNOWN ),
		mComplexType ( 0 ),
		mFlags ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIAttrOp () {
	}

	//----------------------------------------------------------------//
	inline void SetValue ( float value ) {

		this->mType = TYPE_NUMBER;
		this->mNumber = value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void SetValue ( const TYPE* value ) {

		this->mType = TYPE_COMPLEX;
		this->mComplexType = USTypeID < TYPE >::GetID ();
		this->mPtr = value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void SetValue ( const TYPE& value ) {

		assert ( sizeof ( TYPE ) <= MAX_SIZE );

		this->mType = TYPE_COMPLEX;
		this->mComplexType = USTypeID < TYPE >::GetID ();
		memcpy ( &this->mBuffer, &value, sizeof ( TYPE ));
	}
	
	//----------------------------------------------------------------//
	inline void SetValid ( bool isValid, u32 flags ) {
	
		this->mFlags = flags;
		this->mType = isValid ? TYPE_VALID : TYPE_UNKNOWN;
	}
};

#endif
