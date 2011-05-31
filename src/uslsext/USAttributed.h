// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USATTRIBUTED_H
#define USATTRIBUTED_H

#include <uslsext/USAttrOp.h>

#define UNPACK_ATTR(attrID)	\
	( attrID & 0x0000ffff )

#define DECL_ATTR_HELPER(type)																										\
	class type##Attr {																												\
		public:																														\
		static inline bool	Check	( u32 attrID ) { return (( USTypeID < type >::GetID ()) == (( attrID >> 16 ) & 0x0000ffff )); }	\
		static inline u32	Pack	( u32 attrID ) { return ( USTypeID < type >::GetID () << 16 ) | ( attrID & 0x0000ffff ); }		\
	};

//================================================================//
// USAttributed
//================================================================//
class USAttributed {
public:

	enum {
		NULL_ATTR = 0xffffffff,
	};

	//----------------------------------------------------------------//
	virtual bool	ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	bool			AttrExists			( u32 attrID );
					USAttributed		();
	virtual			~USAttributed		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline bool CheckAttrID ( u32 attrID ) {
	
		return (( USTypeID < TYPE >::GetID ()) == (( attrID >> 16 ) & 0x0000ffff ));
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetAttributeValue ( u32 attrID, TYPE value ) {
		
		if ( attrID != NULL_ATTR ) {
			USAttrGetter getter;
			this->ApplyAttrOp ( attrID, getter );
			value = getter.Get < TYPE >();
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline u32 PackAttrID ( u32 attrID ) {
	
		return ( USTypeID < TYPE >::GetID () << 16 ) | ( attrID & 0x0000ffff );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetAttributeValue ( u32 attrID, TYPE value ) {
		if ( attrID != NULL_ATTR ) {
			USAttrSetter setter;
			setter.Set < TYPE >( value );
			this->ApplyAttrOp ( attrID, setter );
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline u32 UnpackAttrID ( u32 attrID ) {
		
		return attrID & 0x0000ffff;
	}
};

#endif
