// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSINGLETON_H
#define	ZLSINGLETON_H

#include <zl-core/ZLMutex.h>

//================================================================//
// ZLSingleton
//================================================================//
template < typename TYPE >
class ZLSingleton {
protected:

	ZLMutex			mMutex;

public:
	
	typedef ZLScopedRef < TYPE > Ref;

	//----------------------------------------------------------------//
	static Ref Get () {
		
		static TYPE singleton;
		return Ref ( singleton.mMutex, singleton );
	}
};

#endif
