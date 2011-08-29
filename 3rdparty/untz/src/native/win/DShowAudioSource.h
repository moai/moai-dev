#pragma once

#include "Untz.h"
#include "AudioSource.h"
#include <Threading/Threading.h>
#include <tchar.h>
#include <dshow.h>
#include <qedit.h>

class SampleGrabberCallback;

template <typename T>
class RComPtr
{
	T* mP;
public:
	RComPtr() : mP(0) {}
	~RComPtr() {
		if(mP)
			((IUnknown*)mP)->Release();
	}

	T* get() { return mP; }
	operator T*() { return mP; }
	T* operator->() const { return mP; }
	RComPtr<T>& operator=(RComPtr<T>& p) { mP = p; }
	T** operator&() { return &mP; }
	bool operator==(T* p) { return mP == p; }
};


// supported formats: aiff, wav, mp3,

class DShowAudioSource : public AudioSource
{
	friend class SampleGrabberCallback;

public:
	DShowAudioSource();
	~DShowAudioSource();
	virtual bool init(const RString& path, bool loadIntoMemory);

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numSamples);
	virtual void setPosition(double position);
	virtual double getPosition();
	virtual UInt32 getBitsPerSample();
	virtual double getSampleRate();
	virtual UInt32 getNumChannels();
	virtual double getLength();

protected:
	void start();
	void stop();
	void putData(BYTE *data, long length);
	void setDecoderPosition(double position);

private:
	std::vector<float> mBuffer;
	bool mLoadedInMemory;
	RString mPath;
	bool mIsCoInitialized;
	SampleGrabberCallback* mpSampleGrabberCallback;
	RComPtr<IMediaControl> mpMediaControl;	
	RComPtr<IMediaEventEx> mpMediaEvent;	
	RComPtr<IMediaSeeking> mpMediaSeeking;	
	WAVEFORMATEX* mpWaveFormatEx;
	RCriticalSection mLock;
};
