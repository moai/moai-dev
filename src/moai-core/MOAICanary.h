// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICANARY_H
#define MOAICANARY_H

class MOAIObject;

//================================================================//
// MOAICanary
//================================================================//
class MOAICanary {
protected:

	u32				mRefCount;
	u32				mStrongRefs;
	MOAIObject*		mObject;

	//----------------------------------------------------------------//	
					MOAICanary		();
	
public:

	friend class MOAIObject;

	//----------------------------------------------------------------//
	void			ForceDelete		();
	MOAIObject*		GetObject		();
	bool			IsValid			();
	void			Release			( bool strong );
	void			Retain			( bool strong );
					~MOAICanary		();
};

#endif
