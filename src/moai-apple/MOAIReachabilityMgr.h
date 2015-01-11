// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIREACHABILITYMGR_H
#define	MOAIREACHABILITYMGR_H

#import <moai-core/headers.h>

@class MOAIReachabilityListener;

//================================================================//
// MOAIReachabilityMgr
//================================================================//
class MOAIReachabilityMgr :
	public MOAIGlobalClass < MOAIReachabilityMgr > {
private:
	
	MOAIReachabilityListener*	mReachabilityListener;
	
	//----------------------------------------------------------------//
	void			OnGlobalsFinalize			();
	
public:

	//----------------------------------------------------------------//
					MOAIReachabilityMgr			();
					~MOAIReachabilityMgr		();
	void			UpdateReachability			();
};

#endif
