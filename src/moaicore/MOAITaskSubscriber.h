// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKSUBSCRIBER_H
#define MOAITASKSUBSCRIBER_H

#include <moaicore/MOAIMutex.h>

class MOAITaskBase;

//================================================================//
// MOAITaskSubscriber
//================================================================//
class MOAITaskSubscriber {
private:

	friend class MOAITaskBase;
	friend class MOAITaskThread;

	USLeanList < MOAITaskBase* >	mCompletedTasks;
	USLeanList < MOAITaskBase* >	mCompletedTasksLatent;
	
	MOAIMutex						mMutex;
	double							mLatentPublishDuration;

	//----------------------------------------------------------------//
	void			Process					();
	void			PushTask				( USLeanLink< MOAITaskBase* >& link );
	void			PushTaskLatent			( USLeanLink< MOAITaskBase* >& link );

public:

	GET_SET ( double, LatentPublishDuration, mLatentPublishDuration )

	//----------------------------------------------------------------//
	void			Publish					();
					MOAITaskSubscriber		();
					~MOAITaskSubscriber		();
};

#endif
