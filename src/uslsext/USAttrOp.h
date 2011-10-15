// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USATTROP_H
#define	USATTROP_H

#include <uslsext/USAffine2D.h>

//================================================================//
// USAttrOp
//================================================================//
class USAttrOp  {
protected:

	enum {
		TYPE_UNKNOWN,
		TYPE_AFFINE_2D,
		TYPE_NUMBER,
		TYPE_RECT,
		TYPE_VALID,
	};
	
	u32 mType;

	union {
		float		mNumber;
		
		u8			mAffine2D [ sizeof ( USAffine2D )];
		u8			mRect [ sizeof ( USAffine2D )];
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
	inline void Apply ( USAffine2D& attr, u32 op ) {

		this->ComplexTypeOp ( attr, op );
	}

	//----------------------------------------------------------------//
	inline bool Apply ( bool attr, u32 op ) {

		if ( op == ADD ) {
			if ( this->mType == TYPE_NUMBER ) {
				return ( this->mNumber == 1.0f ) && attr;
			}
		}
		return this->NumericTypeOp ( attr, op );
	}

	//----------------------------------------------------------------//
	inline float Apply ( float attr, u32 op ) {

		if (( op == ADD ) && ( this->mType == TYPE_NUMBER )) {
			return attr + this->mNumber;
		}
		return this->NumericTypeOp ( attr, op );
	}

	//----------------------------------------------------------------//
	inline int Apply ( int attr, u32 op ) {

		if (( op == ADD ) && ( this->mType == TYPE_NUMBER )) {
			return attr + USFloat::ToInt ( this->mNumber );
		}
		return this->NumericTypeOp ( attr, op );
	}

	//----------------------------------------------------------------//
	inline int Apply ( u32 attr, u32 op ) {

		if (( op == ADD ) && ( this->mType == TYPE_NUMBER )) {
			return attr + ( u32 )USFloat::ToInt ( this->mNumber );
		}
		return this->NumericTypeOp ( attr, op );
	}

	//----------------------------------------------------------------//
	inline void Apply ( USRect& attr, u32 op ) {

		this->ComplexTypeOp ( attr, op );
	}
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mType = TYPE_UNKNOWN;
	}
	
	//----------------------------------------------------------------//
	inline void GetValue ( USAffine2D& value ) {

		if ( this->mType == TYPE_RECT ) {
			memcpy ( &value, &this->mAffine2D, sizeof ( USAffine2D ));
		}
	}
	
	//----------------------------------------------------------------//
	inline void GetValue ( USRect& value ) {

		if ( this->mType == TYPE_RECT ) {
			memcpy ( &value, &this->mRect, sizeof ( USRect ));
		}
	}
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mType != TYPE_UNKNOWN );
	}

	//----------------------------------------------------------------//
	inline void SetValue ( const USAffine2D& value ) {

		this->mType = TYPE_AFFINE_2D;
		memcpy ( &this->mAffine2D, &value, sizeof ( USAffine2D ));
	}

	//----------------------------------------------------------------//
	inline void SetValue ( bool value ) {

		this->mType = TYPE_NUMBER;
		this->mNumber = value ? 1.0f : 0.0f;
	}

	//----------------------------------------------------------------//
	inline void SetValue ( float value ) {

		this->mType = TYPE_NUMBER;
		this->mNumber = value;
	}

	//----------------------------------------------------------------//
	inline void SetValue ( int value ) {

		this->mType = TYPE_NUMBER;
		this->mNumber = ( float )value;
	}

	//----------------------------------------------------------------//
	inline void SetValue ( u32 value ) {

		this->mType = TYPE_NUMBER;
		this->mNumber = ( float )value;
	}

	//----------------------------------------------------------------//
	inline void SetValue ( const USRect& value ) {

		this->mType = TYPE_RECT;
		memcpy ( &this->mRect, &value, sizeof ( USRect ));
	}

	//----------------------------------------------------------------//
	USAttrOp () :
		mType ( TYPE_UNKNOWN ) {
	}
	
	//----------------------------------------------------------------//
	~USAttrOp () {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE > TYPE		GetValue	();

private:

	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void ComplexTypeOp ( TYPE attr, u32 op ) {
	
		switch ( op ) {
			case CHECK: {
				this->mType = TYPE_VALID;
				break;
			}
			case GET: {
				this->SetValue ( attr );
				break;
			}
			case SET: {
				this->GetValue ( attr );
				break;
			}
		}
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE NumericTypeOp ( TYPE attr, u32 op ) {
	
		switch ( op ) {
			case CHECK: {
				this->mType = TYPE_VALID;
				break;
			}
			case GET: {
				this->SetValue ( attr );
				break;
			}
			case SET: {
				attr = this->GetValue < TYPE >();
				break;
			}
		}
		return attr;
	}
};

//----------------------------------------------------------------//
template <> bool	USAttrOp::GetValue < bool >		();
template <> float	USAttrOp::GetValue < float >	();
template <> int		USAttrOp::GetValue < int >		();
template <> u32		USAttrOp::GetValue < u32 >		();

#endif
