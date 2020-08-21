// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICMDMEDIUMWITHAPI_H
#define	MOAICMDMEDIUMWITHAPI_H

#include <moai-core/MOAIAbstractCmdMedium.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAICmdMediumWithAPI
//================================================================//
template < typename API_TYPE >
class MOAICmdMediumWithAPI :
	public virtual API_TYPE,
	public virtual MOAIAbstractCmdMedium {
protected:

	//----------------------------------------------------------------//
	void* MOAIAbstractCmdMedium_GetMediumWithAPI ( ZLTypeID apiTypeID ) {
		return ( apiTypeID == ZLType::GetID < API_TYPE >()) ? this : NULL;
	}

public:

	//----------------------------------------------------------------//
	MOAICmdMediumWithAPI () {
	}

	//----------------------------------------------------------------//
	MOAICmdMediumWithAPI ( MOAICmdMediumAdapter& adapter ) {
		this->mAdapter = &adapter;
	}
	
	//----------------------------------------------------------------//
	~MOAICmdMediumWithAPI () {
	}
};

#endif
