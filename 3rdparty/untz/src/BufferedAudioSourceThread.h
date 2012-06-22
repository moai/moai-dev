//
//  BufferedAudioSourceThread.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef BUFFEREDAUDIOSOURCETHREAD_H_
#define BUFFEREDAUDIOSOURCETHREAD_H_

#include <threading/Threading.h>
#include <vector>

class BufferedAudioSource;

class BufferedAudioSourceThread : public RThread
{
public:
	static BufferedAudioSourceThread* getInstance();
	static void deleteInstance();
	
	bool addSource(BufferedAudioSource *source);
	bool removeSource(BufferedAudioSource *source);
	void readMore();
	
	RCriticalSection& getLock() { return mLock; }
	
protected:	
	// RThread
	void run();
	
private:
	BufferedAudioSourceThread();
	~BufferedAudioSourceThread();
		
	static BufferedAudioSourceThread* mInstance;
	std::vector<BufferedAudioSource*> mSources;
	RSyncEvent mReadMore;
	RCriticalSection mLock;
};

#endif