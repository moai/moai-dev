// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USTASKSUBSCRIBER_H
#define USTASKSUBSCRIBER_H

#include <uslscore/USLeanList.h>
#include <uslscore/USMutex.h>

class USTaskBase;

//================================================================//
// USTaskSubscriber
//================================================================//
class USTaskSubscriber {
private:

	friend class USTaskBase;
	friend class USTaskThread;

	USLeanList < USTaskBase* >	mCompletedTasks;
	USLeanList < USTaskBase* >	mCompletedTasksLatent;
	
	USMutex						mMutex;
	double						mLatentPublishDuration;

	//----------------------------------------------------------------//
	void			Process					();
	void			PushTask				( USLeanLink< USTaskBase* >& link );
	void			PushTaskLatent			( USLeanLink< USTaskBase* >& link );

public:

	GET_SET ( double, LatentPublishDuration, mLatentPublishDuration )

	//----------------------------------------------------------------//
	void			Publish					();
					USTaskSubscriber		();
					~USTaskSubscriber		();
};

#endif
