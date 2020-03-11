// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLEVIEWLAYER_H
#define	MOAITABLEVIEWLAYER_H

#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAITableLayer.h>

//================================================================//
// MOAITableViewLayer
//================================================================//
// TODO: doxygen
class MOAITableViewLayer :
	public virtual MOAITableLayer,
	public virtual MOAIAbstractViewLayer {
private:

	//----------------------------------------------------------------//
	void				MOAIAbstractViewLayer_Draw			();
	void				MOAIDrawable_Draw					( int subPrimID );

public:
	
	DECL_LUA_FACTORY ( MOAITableViewLayer )
	
	//----------------------------------------------------------------//
						MOAITableViewLayer			();
						~MOAITableViewLayer			();
};

#endif
