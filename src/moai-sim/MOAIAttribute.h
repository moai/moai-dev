// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIATTRIBUTE_H
#define	MOAIATTRIBUTE_H

#define ATTR_OP_DECLARE_SETTER(type,typeID)			\
	inline void SetValue ( const type& value ) {	\
		this->mAttrTypeID = typeID;					\
		*( type* )this->mBuffer = value;			\
	}

//================================================================//
// MOAIAttribute
//================================================================//
class MOAIAttribute  {
protected:

	static const size_t MAX_SIZE = sizeof ( ZLMatrix4x4 );

	u32 mAttrTypeID;
	u32 mVariantTypeID;
	u32 mFlags;
	
	u8 mBuffer [ MAX_SIZE ];

public:
	
	GET_SET ( u32, Flags, mFlags )
	GET_CONST ( u32, TypeID, mAttrTypeID )
	
	enum {
		NONE,
		ADD,
		CHECK,
		GET,
		SET,
	};
	
	enum {
		ATTR_TYPE_COLOR_VEC_4,
		ATTR_TYPE_FLOAT_32,
		ATTR_TYPE_INDEX_32,
		ATTR_TYPE_INT_32,
		ATTR_TYPE_AFFINE_3D,
		ATTR_TYPE_MATRIX_3X3,
		ATTR_TYPE_MATRIX_4X4,
		ATTR_TYPE_QUATERNION,
		ATTR_TYPE_VARIANT,
		ATTR_TYPE_VEC_3,
		ATTR_TYPE_VEC_4,
	};
	
	static const u32	NULL_ATTR			= 0x3fffffff;
	static const u32	ATTR_ID_MASK		= 0x0000ffff;
	static const u32	CLASS_ID_MASK		= 0x3fff0000;
	static const u32	ATTR_FLAGS_MASK		= 0xC0000000;
	
	static const u32	ATTR_READ			= 0x40000000;
	static const u32	ATTR_WRITE			= 0x80000000;
	static const u32	ATTR_READ_WRITE		= 0xC0000000;
	
	ATTR_OP_DECLARE_SETTER ( ZLColorVec,	ATTR_TYPE_COLOR_VEC_4 )
	ATTR_OP_DECLARE_SETTER ( float,			ATTR_TYPE_FLOAT_32 )
	ATTR_OP_DECLARE_SETTER ( s32,			ATTR_TYPE_INT_32 )
	ATTR_OP_DECLARE_SETTER ( ZLAffine3D,	ATTR_TYPE_AFFINE_3D )
	ATTR_OP_DECLARE_SETTER ( ZLIndex,		ATTR_TYPE_INDEX_32 )
	ATTR_OP_DECLARE_SETTER ( ZLMatrix3x3,	ATTR_TYPE_MATRIX_3X3 )
	ATTR_OP_DECLARE_SETTER ( ZLMatrix4x4,	ATTR_TYPE_MATRIX_4X4 )
	ATTR_OP_DECLARE_SETTER ( ZLQuaternion,	ATTR_TYPE_QUATERNION )
	ATTR_OP_DECLARE_SETTER ( ZLVec3D,		ATTR_TYPE_VEC_3 )
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE Apply ( const TYPE& value, u32 op, u32 flags ) {
		
		this->mFlags = flags;
		
		switch ( op ) {
		
			case ADD:
				return value + this->GetValue ( value );
				
			case GET:
				this->SetValue ( value );
				
			case CHECK:
				this->mFlags = flags;
				break;
				
			case SET:
				return this->GetValue ( value );
		}
		
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE ApplyNoAdd ( const TYPE& value, u32 op, u32 flags ) {
		
		
		switch ( op ) {
		
			case GET:
				this->SetValue ( value );
				
			case CHECK:
				this->mFlags = flags;
				break;
				
			case SET:
				return this->GetValue ( value );
		}
		
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE ApplyVariant ( const TYPE& value, u32 op, u32 flags ) {
		
		this->mFlags = flags;
		
		switch ( op ) {
		
			case ADD:
				return value + this->GetVariant < TYPE >( value );
				
			case GET:
				this->SetVariant < TYPE >( value );
				
			case CHECK:
				this->mFlags = flags;
				break;
				
			case SET:
				return this->GetVariant < TYPE >( value );
		}
		
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE ApplyVariantNoAdd ( const TYPE& value, u32 op, u32 flags ) {
		
		
		switch ( op ) {
		
			case GET:
				this->SetVariant < TYPE >( value );
				
			case CHECK:
				this->mFlags = flags;
				break;
				
			case SET:
				return this->GetVariant < TYPE >( value );
		}
		
		return value;
	}
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mVariantTypeID = 0;
		this->mFlags = 0;
	}
	
	//----------------------------------------------------------------//
	inline bool GetValue ( const bool& value ) const {

		switch ( this->mAttrTypeID ) {
			case ATTR_TYPE_FLOAT_32:	return *( float* )this->mBuffer == 0.0f ? false : true;
			case ATTR_TYPE_INDEX_32:	return ( *( ZLIndex* )this->mBuffer ) == 0 ? false : true;
			case ATTR_TYPE_INT_32:		return *( s32* )this->mBuffer == 0 ? false : true;
			default:					return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLColorVec GetValue ( const ZLColorVec& value ) const {
		
		switch ( this->mAttrTypeID ) {
			case ATTR_TYPE_COLOR_VEC_4:	return *( ZLColorVec* )this->mBuffer;
			case ATTR_TYPE_INT_32: {
				ZLColorVec color;
				color.SetRGBA ( *( u32* )this->mBuffer );
				return color;
			}
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline float GetValue ( const float& value ) const {

		switch ( this->mAttrTypeID ) {
			case ATTR_TYPE_FLOAT_32:	return *( float* )this->mBuffer;
			case ATTR_TYPE_INDEX_32:	return ( float )(( ZLSize )( *( ZLIndex* )this->mBuffer ));
			case ATTR_TYPE_INT_32:		return ( float )( *( s32* )this->mBuffer );
			default:					return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline s32 GetValue ( const s32& value ) const {

		switch ( this->mAttrTypeID ) {
			case ATTR_TYPE_COLOR_VEC_4:	return (( ZLColorVec* )this->mBuffer )->PackRGBA ();
			case ATTR_TYPE_FLOAT_32:	return ( s32 )( *( float* )this->mBuffer );
			case ATTR_TYPE_INDEX_32:	return ( s32 )(( ZLSize )( *( ZLIndex* )this->mBuffer ));
			case ATTR_TYPE_INT_32:		return *( s32* )this->mBuffer;
			default:					return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLAffine3D GetValue ( const ZLAffine3D& value ) const {
		
		switch ( this->mAttrTypeID ) {
			
			case ATTR_TYPE_AFFINE_3D:	return *( ZLAffine3D* )this->mBuffer;
			case ATTR_TYPE_MATRIX_3X3:	return ZLAffine3D ( *( ZLMatrix3x3* )this->mBuffer );
			case ATTR_TYPE_MATRIX_4X4:	return ZLAffine3D ( *( ZLMatrix4x4* )this->mBuffer );
			
			case ATTR_TYPE_QUATERNION: {
				ZLAffine3D mtx;
				(( ZLQuaternion* )this->mBuffer )->Get ( mtx );
				return mtx;
			}
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLIndex GetValue ( const ZLIndex& value ) const {

		switch ( this->mAttrTypeID ) {
			case ATTR_TYPE_FLOAT_32:	return ZLIndexCast (( ZLSize )( *( float* )this->mBuffer ));
			case ATTR_TYPE_INDEX_32:	return *( ZLIndex* )this->mBuffer;
			case ATTR_TYPE_INT_32:		return ZLIndexCast (( ZLSize )( *( s32* )this->mBuffer ));
			default:					return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLMatrix3x3 GetValue ( const ZLMatrix3x3& value ) const {
		
		switch ( this->mAttrTypeID ) {
			
			case ATTR_TYPE_AFFINE_3D:	return ZLMatrix3x3 ( *( ZLAffine3D* )this->mBuffer );
			case ATTR_TYPE_MATRIX_3X3:	return *( ZLMatrix3x3* )this->mBuffer;
			case ATTR_TYPE_MATRIX_4X4:	return ZLMatrix3x3 ( *( ZLMatrix4x4* )this->mBuffer );
			
			case ATTR_TYPE_QUATERNION: {
				ZLMatrix3x3 mtx;
				(( ZLQuaternion* )this->mBuffer )->Get ( mtx );
				return mtx;
			}
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLMatrix4x4 GetValue ( const ZLMatrix4x4& value ) const {
		
		switch ( this->mAttrTypeID ) {
			
			case ATTR_TYPE_AFFINE_3D:	return ZLMatrix4x4 ( *( ZLAffine3D* )this->mBuffer );
			case ATTR_TYPE_MATRIX_3X3:	return ZLMatrix4x4 ( *( ZLMatrix3x3* )this->mBuffer );
			case ATTR_TYPE_MATRIX_4X4:	return *( ZLMatrix4x4* )this->mBuffer;
			
			case ATTR_TYPE_QUATERNION: {
				ZLMatrix4x4 mtx;
				(( ZLQuaternion* )this->mBuffer )->Get ( mtx );
				return mtx;
			}
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLQuaternion GetValue ( const ZLQuaternion& value ) const {
		
		switch ( this->mAttrTypeID ) {
		
			case ATTR_TYPE_AFFINE_3D:	return ZLQuaternion ( *( ZLAffine3D* )this->mBuffer );
			case ATTR_TYPE_MATRIX_3X3:	return ZLQuaternion ( *( ZLMatrix3x3* )this->mBuffer );
			case ATTR_TYPE_MATRIX_4X4:	return ZLQuaternion ( *( ZLMatrix4x4* )this->mBuffer );
			case ATTR_TYPE_QUATERNION:	return *( ZLQuaternion* )this->mBuffer;
			
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLVec3D GetValue ( const ZLVec3D& value ) const {
		
		switch ( this->mAttrTypeID ) {
		
			case ATTR_TYPE_AFFINE_3D: {
				return (( ZLAffine3D* )this->mBuffer )->GetTranslation ();
			}
			
			case ATTR_TYPE_MATRIX_4X4: {
				ZLVec3D vec;
				(( ZLMatrix4x4* )this->mBuffer )->GetTranslation ( vec );
				return vec;
			}
			
			case ATTR_TYPE_VEC_3: {
				return *( ZLVec3D* )this->mBuffer;
			}
			
			case ATTR_TYPE_VEC_4: {
				return ( ZLVec3D )( *( ZLVec4D* )this->mBuffer );
			}
			
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	inline ZLVec4D GetValue ( const ZLVec4D& value ) const {
		
		switch ( this->mAttrTypeID ) {
		
			case ATTR_TYPE_AFFINE_3D: {
				return ZLVec4D ((( ZLAffine3D* )this->mBuffer )->GetTranslation (), 0.0f );
			}
			
			case ATTR_TYPE_MATRIX_4X4: {
				ZLVec3D vec;
				(( ZLMatrix4x4* )this->mBuffer )->GetTranslation ( vec );
				return ZLVec4D ( vec, 0.0f );
			}
			
			case ATTR_TYPE_VEC_3: {
				return ZLVec4D ( *( ZLVec3D* )this->mBuffer, 0.0f );
			}
			
			case ATTR_TYPE_VEC_4: {
				return *( ZLVec4D* )this->mBuffer;
			}
			
			default: return value;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline TYPE GetVariant ( const TYPE& value ) const {

		if ( this->mVariantTypeID == ZLTypeID < TYPE >::GetID ()) {
			return *( TYPE* )this->mBuffer;
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	inline bool IsValid () const {
		return ( this->mFlags != 0 );
	}

	//----------------------------------------------------------------//
	MOAIAttribute () :
		mAttrTypeID ( 0 ),
		mVariantTypeID ( 0 ),
		mFlags ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIAttribute () {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void SetVariant ( const TYPE& value ) {

		assert ( sizeof ( TYPE ) <= MAX_SIZE );
		
		this->mAttrTypeID = ATTR_TYPE_VARIANT;
		this->mVariantTypeID = ZLTypeID < TYPE >::GetID ();
		*( TYPE* )this->mBuffer = value;
	}
};

#endif
