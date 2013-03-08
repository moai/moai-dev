// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERSCOPE_H
#define	MOAIPROFILERSCOPE_H

#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIProfilerReport.h>
#include <moaicore/MOAIProfilerAutoScope.h>

//================================================================//
// MOAIProfilerScope
//================================================================//
class MOAIProfilerScope {
private:
	
	friend class MOAIProfilerContext;
	
	//----------------------------------------------------------------//
	MOAIProfilerScope*				mNext;
	MOAIProfilerScope*				mParent;
	USHashedString					mName;
	USDeviceTime::TimeStamp			mStartTime;
	USDeviceTime::TimeStamp			mDuration;

public:

	//----------------------------------------------------------------//
	void		Enter					( MOAIProfilerScope* parent, const USHashedString& name );
	void		Leave					( const USHashedString& name );
	void		Reset					();
				MOAIProfilerScope		( const USHashedString& name = USHashedString::Empty );
};

#endif
