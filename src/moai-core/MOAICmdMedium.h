// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICMDMEDIUM_H
#define	MOAICMDMEDIUM_H

#include <moai-core/MOAIAbstractCmdMedium.h>
#include <moai-core/MOAICmdMediumPolymorphic.h>

template < typename API_TYPE > class MOAICmdMediumWithAPI;

//================================================================//
// MOAICmdMedium
//================================================================//
class MOAICmdMedium :
	public ZLRefCountedObject,
	public MOAICmdMediumPolymorphic,
	public MOAIAbstractCmdMedium {
protected:

	friend class MOAIAbstractCmdStream;

	//----------------------------------------------------------------//
	void*				MOAIAbstractCmdMedium_GetMediumWithAPI			( ZLTypeID apiTypeID );

public:

	//----------------------------------------------------------------//
						MOAICmdMedium					( MOAIAbstractCmdStream& stream );
						~MOAICmdMedium					();
};

#endif
