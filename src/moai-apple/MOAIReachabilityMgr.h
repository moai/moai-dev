// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIREACHABILITYMGR_H
#define	MOAIREACHABILITYMGR_H

#import <moai-core/headers.h>

@class MOAIReachabilityListener;

//================================================================//
// MOAIReachabilityMgr
//================================================================//
class MOAIReachabilityMgr :
	public ZLContextClass < MOAIReachabilityMgr > {
private:
	
	MOAIReachabilityListener*	mReachabilityListener;
	
	//----------------------------------------------------------------//
	void			ZLContextClass_Finalize			();
	
public:

	//----------------------------------------------------------------//
					MOAIReachabilityMgr			();
					~MOAIReachabilityMgr		();
	void			UpdateReachability			();
};

#endif
