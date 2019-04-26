// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONSTACKMGR_H
#define	MOAIACTIONSTACKMGR_H

class MOAIAction;
class MOAIActionTree;

//================================================================//
// MOAIActionStackMgr
//================================================================//
class MOAIActionStackMgr :
	public ZLContextClass < MOAIActionStackMgr > {
private:
	
	ZLLeanStack < MOAIAction* > mStack;
	
	//----------------------------------------------------------------//
	MOAIAction*			GetCurrent				();
	MOAIAction*			GetDefaultParent		();
	void				Pop						();
	void				Push					( MOAIAction& action );
	
public:
	
	friend class MOAIAction;
	friend class MOAIActionTree;
	friend class MOAICoroutine;
	
	//----------------------------------------------------------------//
						MOAIActionStackMgr		();
						~MOAIActionStackMgr		();
};

#endif
