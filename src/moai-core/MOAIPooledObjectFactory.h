// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOLEDOBJECTFACTORY_H
#define	MOAIPOOLEDOBJECTFACTORY_H

#include <moai-core/MOAILua.h>

class MOAIScope;

//================================================================//
// MOAIPooledObjectFactory
//================================================================//
class MOAIPooledObjectFactory {
public:

	MOAILuaStrongRef		mFactory;
	MOAILuaStrongRef		mOnProvision;
	MOAILuaStrongRef		mOnRemit;
	
	//----------------------------------------------------------------//
	MOAILuaObject*			Create 				();
	void 					OnProvision 		( MOAILuaObject& object, MOAIScope& scope );
	void 					OnRemit 			( MOAILuaObject& object );
};

#endif
