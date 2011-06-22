#pragma once

#include <Threading/Threading.h>
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