// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USATTRIBUTED_H
#define USATTRIBUTED_H

#include <uslsext/USAttrOp.h>

//================================================================//
// USAttributed
//================================================================//
class USAttributed {
public:

	enum {
		NULL_ATTR = 0xffffffff,
	};

	//----------------------------------------------------------------//
	virtual void	ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	bool			AttrExists			( u32 attrID );
					USAttributed		();
	virtual			~USAttributed		();
	
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
	void SetAttributeValue ( u32 attrID, TYPE value ) {
		if ( attrID != NULL_ATTR ) {
			USAttrSetter setter;
			setter.Set < TYPE >( value );
			this->ApplyAttrOp ( attrID, setter );
		}
	}
};

#endif
