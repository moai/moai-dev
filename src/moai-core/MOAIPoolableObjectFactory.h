// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOLABLEOBJECTFACTORY_H
#define	MOAIPOOLABLEOBJECTFACTORY_H

#include <moai-core/MOAILua.h>

class MOAIScope;
class MOAIPoolableObject;

//================================================================//
// MOAIPoolableObjectFactory
//================================================================//
class MOAIPoolableObjectFactory {
public:

	MOAILuaStrongRef		mFactory;
	MOAILuaStrongRef		mOnProvision;
	MOAILuaStrongRef		mOnRemit;
	
	//----------------------------------------------------------------//
	MOAIPoolableObject*		Create 				();
	void 					OnProvision 		( MOAIPoolableObject& resource, MOAIScope& scope );
	void 					OnRemit 			( MOAIPoolableObject& resource );
};

#endif
