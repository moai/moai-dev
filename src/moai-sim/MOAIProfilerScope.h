// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERSCOPE_H
#define	MOAIPROFILERSCOPE_H

#include <moai-sim/MOAIProfilerReport.h>
#include <moai-sim/MOAIProfilerAutoScope.h>

//================================================================//
// MOAIProfilerScope
//================================================================//
class MOAIProfilerScope {
private:
	
	friend class MOAIProfilerContext;
	
	//----------------------------------------------------------------//
	MOAIProfilerScope*				mNext;
	MOAIProfilerScope*				mParent;
	ZLHashedString					mName;
	ZLDeviceTime::TimeStamp			mStartTime;
	ZLDeviceTime::TimeStamp			mDuration;

public:

	//----------------------------------------------------------------//
	void		Enter					( MOAIProfilerScope* parent, const ZLHashedString& name );
	void		Leave					( const ZLHashedString& name );
	void		Reset					();
				MOAIProfilerScope		( const ZLHashedString& name = ZLHashedString::Empty );
};

#endif
