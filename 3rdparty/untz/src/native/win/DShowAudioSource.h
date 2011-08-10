#pragma once

#include "Untz.h"
#include "AudioSource.h"
#include <Threading/Threading.h>
#include <tchar.h>
#include <dshow.h>
#include <qedit.h>
#include <atlbase.h>

class SampleGrabberCallback;

// supported formats: aiff, wav, mp3,

class DShowAudioSource : public AudioSource
{
	friend class SampleGrabberCallback;

public:
	DShowAudioSource();
	~DShowAudioSource();
	bool load(const RString& path);

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numSamples);
	virtual void start();
	virtual void stop();
	virtual void setPosition(double position);
	virtual double getPosition();
	virtual UInt32 getBitsPerSample();
	virtual double getSampleRate();
	virtual UInt32 getNumChannels();
	virtual double getLength();

protected:
	void putData(BYTE *data, long length);
	void setDecoderPosition(double position);
private:
	std::vector<float> mBuffer;
	Int64 mCurrentPosition;
	RString mPath;
	bool mIsCoInitialized;
	CComPtr<SampleGrabberCallback> mpSampleGrabberCallback;
	CComQIPtr<IMediaControl> mpMediaControl;
	CComQIPtr<IMediaEventEx > mpMediaEvent;
	CComQIPtr<IMediaSeeking> mpMediaSeeking;
	WAVEFORMATEX* mpWaveFormatEx;
	RCriticalSection mLock;
	bool mEOF;
};
