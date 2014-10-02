//
//  DShowAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "DShowAudioSource.h"
#include "SystemData.h"
#include "SoundData.h"

#define SECONDS_TO_BUFFER 2

HRESULT FindUnconnectedPin(IBaseFilter* pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins* pEnum = 0;
    IPin* pPin = 0;
    BOOL bFound = false;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr))
		return hr;

    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION pinDirection;
        pPin->QueryDirection(&pinDirection);
        if (PinDir == pinDirection)
        {
			*ppPin = pPin;
            return S_OK;
        }
		pPin = NULL;
    }
    return E_FAIL;  
}

HRESULT ConnectFilters(IGraphBuilder* pGraph, IBaseFilter* pSrc, IBaseFilter* pDest)
{
	IPin* pOut = 0;
	IPin* pIn = 0;

    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
	if(FAILED(hr))
		return hr;

    hr = FindUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if(FAILED(hr))
		return hr;

	hr = pGraph->Connect(pOut, pIn);
	return hr;
}


class SampleGrabberCallback : public ISampleGrabberCB
{
	ULONG _refCount; 
	DShowAudioSource* mpDShowAudioSource;
public:
	SampleGrabberCallback(DShowAudioSource* pDShowAudioSource) : _refCount(0), mpDShowAudioSource(pDShowAudioSource) {};
	~SampleGrabberCallback() {};

	// IUnknown
	ULONG APIENTRY AddRef() { return ++_refCount; }
	ULONG APIENTRY Release() { return --_refCount; }		
	HRESULT APIENTRY QueryInterface(REFIID iid, void** ppvObject)
	{
		if(iid == IID_IUnknown)
			*ppvObject = dynamic_cast<IUnknown*>(this);
		else if (iid == IID_ISampleGrabberCB)
			*ppvObject = dynamic_cast<ISampleGrabberCB*>(this);
		else 
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		
		AddRef();
		return S_OK;
	}

	// ISampleGrabberCB
	HRESULT APIENTRY BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen)
	{
		mpDShowAudioSource->putData(pBuffer, BufferLen);
		return S_OK;
	}

	HRESULT APIENTRY SampleCB(double SampleTime, IMediaSample *pSample)
	{
		// Do some grabbing...
		return S_OK;
	}
};


DShowAudioSource::DShowAudioSource() : mpSampleGrabberCallback(0)
{	
	mIsCoInitialized = false;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(!FAILED(hr))
		mIsCoInitialized = true;
}

DShowAudioSource::~DShowAudioSource()
{
	stop();

	if(mpSampleGrabberCallback)
		delete mpSampleGrabberCallback;

	if(mIsCoInitialized)
		CoUninitialize();
}

bool DShowAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true; // must have been initialized via copy constructor

	mPath = path;
	HRESULT hr = S_OK;

	// Create the graph builder
	RComPtr<IGraphBuilder> pGraphBuilder;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGraphBuilder);
	if(FAILED(hr))
		return false;

	// Create the sample grabber filter
	RComPtr<IBaseFilter> pSampleGrabberBaseFilter;
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSampleGrabberBaseFilter));
	if(FAILED(hr))
		return false;

	// Add the sample grabber filter to the graph
	hr = pGraphBuilder->AddFilter(pSampleGrabberBaseFilter.get(), L"Sample Grabber");
	if(FAILED(hr))
		return false;

	// Get the Sample Grabber interface
	RComPtr<ISampleGrabber> pSampleGrabberFilter;
	pSampleGrabberBaseFilter->QueryInterface(IID_PPV_ARGS(&pSampleGrabberFilter));
	if(pSampleGrabberFilter == 0)
		return false;

	// Set the sample grabber's media type
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype	= MEDIATYPE_Audio;
	mt.subtype = MEDIASUBTYPE_PCM;
	mt.formattype = FORMAT_WaveFormatEx;
	hr = pSampleGrabberFilter->SetMediaType(&mt);        
	if(FAILED(hr))
		return false;

	// Create the sample grabber callback
	mpSampleGrabberCallback = new SampleGrabberCallback(this);
	hr = pSampleGrabberFilter->SetCallback(mpSampleGrabberCallback, 1);
	if(FAILED(hr))
		return false;

	// Add the source filter to the graph
	WCHAR filename[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0,mPath.c_str(), -1, filename, MAX_PATH);
	RComPtr<IBaseFilter> pSourceFilter;
	hr = pGraphBuilder->AddSourceFilter(filename, L"Audio Source", &pSourceFilter);
	if(FAILED(hr)) 
		return false;

	// Create the NULL renderer.  We are only decoding
	IBaseFilter* pNullRenderer;
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pNullRenderer));
	hr = pGraphBuilder->AddFilter(pNullRenderer, L"NULL Renderer");
	if(FAILED(hr))
		return false;

	// Connect input to sample grabber
	hr = ConnectFilters(pGraphBuilder.get(), pSourceFilter, pSampleGrabberBaseFilter.get());
	if(FAILED(hr))
		return false;

	// Connect sample grabber to NULL renderer
	hr = ConnectFilters(pGraphBuilder.get(), pSampleGrabberBaseFilter.get(), pNullRenderer);
	if(FAILED(hr))
		return false;

	// Get some useful interfaces
	hr = pGraphBuilder->QueryInterface(IID_PPV_ARGS(&mpMediaControl));
	if(mpMediaControl == 0)
		return false;

	hr = pGraphBuilder->QueryInterface(IID_PPV_ARGS(&mpMediaEvent));
	if(mpMediaEvent == 0)
		return false;

	hr = pGraphBuilder->QueryInterface(IID_PPV_ARGS(&mpMediaSeeking));
	if(mpMediaSeeking == 0)
		return false;

	// Get the waveformatex structure
	pSampleGrabberFilter->GetConnectedMediaType(&mt);
    mpWaveFormatEx = reinterpret_cast<WAVEFORMATEX*>(mt.pbFormat);

	hr = mpMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	if(FAILED(hr)) 
		return false;

	RComPtr<IMediaFilter> pMediaFilter;
	hr = pGraphBuilder->QueryInterface(IID_PPV_ARGS(&pMediaFilter));
	pMediaFilter->SetSyncSource(0);

	mEOF = false;
	mLoadedInMemory = loadIntoMemory;

	RPRINT("# channels = %d\n", mpWaveFormatEx->nChannels);
	RPRINT("Sample rate = %d\n", mpWaveFormatEx->nSamplesPerSec);


	if(mLoadedInMemory)
	{
		mBuffer = RAudioBuffer(getNumChannels(), getSampleRate() * getLength());
		start();

		RPRINT("loading sound into memory...\n");
		long evCode = 0;
		HRESULT hr = mpMediaEvent->WaitForCompletion(INFINITE, &evCode);
		if(FAILED(hr))
		{
			RPRINT("failed waiting for file to finish decoding.\n");
			return false;
		}
		mEOF = true;
	}
	else
	{
		mBuffer = RAudioBuffer(getNumChannels(), getSampleRate() * SECONDS_TO_BUFFER);
		start();
	}

	return true;
}

void DShowAudioSource::start()
{
	if(mpMediaControl)
		mpMediaControl->Run();
}

void DShowAudioSource::stop()
{
	if(mpMediaControl)
		mpMediaControl->Stop();
}

void DShowAudioSource::putData(BYTE *data, long length)
{
	RScopedLock l(&mLock);

	// Read samples and convert from short to float
	Int16* p = (Int16*)data;
	UInt32 samples = length / 2;
	for(UInt32 i = 0; i < samples; ++i)
	{
		float temp = *(p++) / 32767.0f;
		mBuffer.putSample(temp);
	}

	RPRINT("just read %d samples (%d)\n", samples, mBuffer.size());

	// Stop decoding once we have 2 seconds of data
	if(!mLoadedInMemory && mBuffer.size() > UNTZ::System::get()->getSampleRate() * mpWaveFormatEx->nChannels * SECONDS_TO_BUFFER)
	{
		RPRINT("pausing decoder\n");
		mpMediaControl->Pause();
	}
}

Int64 DShowAudioSource::readFrames(float* data, UInt32 numChannels, UInt32 numFrames, AudioSourceState& state)
{
	memset(data, 0, sizeof(float) * numChannels * numFrames);

	// Check if we are done decoding...
	if(!mEOF && !mLoadedInMemory)
	{
		long evCode;
		HRESULT hr = mpMediaEvent->WaitForCompletion(0, &evCode);
		if(FAILED(hr))
		{
			RPRINT("failed while testing for completion\n");
			if(hr == VFW_E_WRONG_STATE)
				RPRINT("filter graph in wrong state\n");
		}

		if(evCode == EC_COMPLETE)
		{
			RPRINT("reahed EOF\n");
			mEOF = true;
		}
	}

	RScopedLock l(&mLock);

	Int64 framesRead = numFrames;
	Int64 framesAvailable = mBuffer.size() / getNumChannels() - state.mCurrentFrame;
    
	// For disk-streaming sources we calculate available frames using the whole buffer
    if(!mLoadedInMemory)
        framesAvailable = mBuffer.size() / getNumChannels();

	Int64 loopEndFrame = convertSecondsToSamples(state.mLoopEnd);
	Int64 totalFrames = convertSecondsToSamples(getLength());
//	RPRINT("loop end frame = %ld\n", loopEndFrame);
//	RPRINT("current frame = %ld\n", state.mCurrentFrame);
//	RPRINT("total frames = %ld\n", totalFrames);
	bool needToLoop = state.mLooping && ((state.mCurrentFrame >= loopEndFrame && loopEndFrame > 0) || (framesAvailable == 0 && mEOF));
	
	if(framesAvailable > 0 && !needToLoop)
	{
		int sourceChannels = getNumChannels();

		if(framesAvailable < numFrames)
			framesRead = framesAvailable;
        
        // For disk-streaming sources we always start at the beginning of the buffer
        int frameOffset = state.mCurrentFrame;
        if(!mLoadedInMemory)
            frameOffset = 0;
        
		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = NULL;
			if(sourceChannels == 1)
				in = mBuffer.getData(0, frameOffset);
			else
				in = mBuffer.getData(j, frameOffset);

			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(data++) = *in;
				in += sourceChannels;
			}
		}

        state.mCurrentFrame += framesRead;
		
        if(!mLoadedInMemory)
		{
			mBuffer.erase(0, framesRead);
			framesAvailable = mBuffer.size() / getNumChannels();
			UInt32 minimumFrames = getSampleRate() * SECONDS_TO_BUFFER / 2;
			if(framesAvailable <= minimumFrames)
			{
				mpMediaControl->Run();
			}
		}
	}
    else
    {
        framesRead = ERR_BUFFERING;
        if(needToLoop)
        {
			RPRINT("reached loop end...looping again.\n");
			setPosition(state.mLoopStart);
			state.mCurrentFrame = convertSecondsToSamples(state.mLoopStart);
        }
        else if (mEOF)
        {            
            return 0; // signal that we are done
        }
    }
    
	return framesRead;
}

void DShowAudioSource::setDecoderPosition(double seconds)
{
	if(!mLoadedInMemory)
	{
		mEOF = false;
		mpMediaControl->Stop();
		REFERENCE_TIME current = (REFERENCE_TIME)(seconds * 1000 * 10000);
		HRESULT hr = mpMediaSeeking->SetPositions(&current, AM_SEEKING_AbsolutePositioning, NULL, 0);
		if(FAILED(hr))
			RPRINT("failed to set decoder position\n");
		mpMediaControl->Run();
	}
}

void DShowAudioSource::setPosition(double seconds)
{
	seconds = seconds < 0 ? 0.0f : seconds;
	seconds = seconds > getLength() ? getLength() : seconds;
    
    if(!mLoadedInMemory)
    {
		{
			RScopedLock l(&mLock);
			mBuffer.clear();
		}
        setDecoderPosition(seconds);
    }
}

UInt32 DShowAudioSource::getBitsPerSample() 
{ 
	if(mpWaveFormatEx)
		return mpWaveFormatEx->wBitsPerSample;
	return 0; 
}

double DShowAudioSource::getSampleRate() 
{ 
	if(mpWaveFormatEx)
		return (double)mpWaveFormatEx->nSamplesPerSec;
	return 0; 
}

UInt32 DShowAudioSource::getNumChannels() 
{ 
	if(mpWaveFormatEx)
		return mpWaveFormatEx->nChannels;
	return 0; 
}

double DShowAudioSource::getLength() 
{ 
	if(mpWaveFormatEx)
	{
		REFERENCE_TIME duration;
		mpMediaSeeking->GetDuration(&duration);
		return (double)duration / 10000 / 1000;
	}
	return 0; 
}
