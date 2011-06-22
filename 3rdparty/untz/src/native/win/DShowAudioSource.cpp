#include "DShowAudioSource.h"
#include "SystemData.h"
#include "SoundData.h"

HRESULT FindUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    CComPtr<IEnumPins> pEnum = 0;
    CComPtr<IPin> pPin = 0;
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
			*ppPin = pPin.Detach();
            return S_OK;
        }
		pPin = NULL;
    }
    return E_FAIL;  
}

HRESULT ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest)
{
	CComPtr<IPin> pOut = 0;
	CComPtr<IPin> pIn = 0;

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


DShowAudioSource::DShowAudioSource()
{
	mIsCoInitialized = false;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(!FAILED(hr))
		mIsCoInitialized = true;
}

DShowAudioSource::~DShowAudioSource()
{
	if(mIsCoInitialized)
		CoUninitialize();
}

bool DShowAudioSource::load(const RString& path)
{
	mPath = path;
	HRESULT hr = S_OK;

	// Create the graph builder
	CComPtr<IGraphBuilder> pGraphBuilder;
	hr = pGraphBuilder.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER);
    if(FAILED(hr)) 
		return false;

	// Create the sample grabber filter
	CComPtr<IBaseFilter> pSampleGrabberBaseFilter;
	hr = pSampleGrabberBaseFilter.CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr))
		return false;

	// Add the sample grabber filter to the graph
	hr = pGraphBuilder->AddFilter(pSampleGrabberBaseFilter, L"Sample Grabber");
	if(FAILED(hr))
		return false;

	// Get the Sample Grabber interface
	CComQIPtr<ISampleGrabber> pSampleGrabberFilter(pSampleGrabberBaseFilter);
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
	mpSampleGrabberCallback.Attach(new SampleGrabberCallback(this));
	hr = pSampleGrabberFilter->SetCallback(mpSampleGrabberCallback, 1);
	if(FAILED(hr))
		return false;

	// Add the source filter to the graph
	WCHAR filename[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0,mPath.c_str(), -1, filename, MAX_PATH);
	CComPtr<IBaseFilter> pSourceFilter;
	hr = pGraphBuilder->AddSourceFilter(filename, L"Audio Source", &pSourceFilter);
	if(FAILED(hr)) 
		return false;

	// Create the NULL renderer.  We are only decoding
	CComPtr<IBaseFilter> pNullRenderer;
	hr = pNullRenderer.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER);
	hr = pGraphBuilder->AddFilter(pNullRenderer, L"NULL Renderer");

	// Connect input to sample grabber
	hr = ConnectFilters(pGraphBuilder, pSourceFilter, pSampleGrabberBaseFilter);
	if(FAILED(hr))
		return false;

	// Connect sample grabber to NULL renderer
	hr = ConnectFilters(pGraphBuilder, pSampleGrabberBaseFilter, pNullRenderer);
	if(FAILED(hr))
		return false;

	// Get some useful interfaces
	mpMediaControl = pGraphBuilder;
	if(mpMediaControl == 0)
		return false;

	mpMediaEvent = pGraphBuilder;
	if(mpMediaEvent == 0)
		return false;

	mpMediaSeeking = pGraphBuilder;
	if(mpMediaSeeking == 0)
		return false;

	// Get the waveformatex structure
	pSampleGrabberFilter->GetConnectedMediaType(&mt);
    mpWaveFormatEx = reinterpret_cast<WAVEFORMATEX*>(mt.pbFormat);

	hr = mpMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	if(FAILED(hr)) 
		return false;

	CComQIPtr<IMediaFilter> pMediaFilter(pGraphBuilder);
	pMediaFilter->SetSyncSource(0);

	mCurrentPosition = 0;
	mEOF = false;

	printf("# channels = %d\n", mpWaveFormatEx->nChannels);
	printf("Sample rate = %d\n", mpWaveFormatEx->nSamplesPerSec);

	return true;
}

void DShowAudioSource::start()
{
	// Start decoding...
	if(mpMediaControl)
		mpMediaControl->Run();
}

void DShowAudioSource::stop()
{
	// Stop decoding...
	if(mpMediaControl)
		mpMediaControl->Stop();
}

void DShowAudioSource::putData(BYTE *data, long length)
{
	mCriticalSection.lock();
//	printf("total frames = %d, time = %f\n", (int)(getLength() * mpWaveFormatEx->nSamplesPerSec), getLength());

	// Read samples and convert from short to float
	Int16* p = (Int16*)data;
	UInt32 samples = length / 2;
	for(UInt32 i = 0; i < samples; ++i)
	{
		float temp = *(p++) / 32767.0f;
		mBuffer.push_back(temp);
	}

//	printf("put %d frames\n", samples / mpWaveFormatEx->nChannels);
//	printf("%d frames in buffer\n", mBuffer.size() / mpWaveFormatEx->nChannels);

	mCriticalSection.unlock();

	// Stop decoding once we have 2 seconds of data
	if(mBuffer.size() > UNTZ::System::get()->getSampleRate() * mpWaveFormatEx->nChannels * 2)
		mpMediaControl->Pause();
}

UInt32 DShowAudioSource::readFrames(float* data, UInt32 numChannels, UInt32 numFrames)
{
	memset(data, 0, sizeof(float) * numChannels * numFrames);

	// Check if we are done decoding...
	long evCode;
	mpMediaControl->Run();
	HRESULT hr = mpMediaEvent->WaitForCompletion(0, &evCode);
	if(evCode == EC_COMPLETE)
	{
//		printf("done decoding...");
		mEOF = true;
	}

	mCriticalSection.lock();

//	printf("read frames: %d frames in buffer\n", mBuffer.size() / mpWaveFormatEx->nChannels);

	UInt32 framesRead = numFrames;
	if(mBuffer.size() > 0)
	{
		if(mBuffer.size() / mpWaveFormatEx->nChannels < framesRead)
		{
			framesRead = mBuffer.size() / mpWaveFormatEx->nChannels;
		}

		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = NULL;
			if(mpWaveFormatEx->nChannels == 1)
				in = &mBuffer[0];
			else
				in = &mBuffer[j];

			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(data++) = *in;
				in += mpWaveFormatEx->nChannels;
			}
		}

		mBuffer.erase(mBuffer.begin(), mBuffer.begin()+(framesRead * mpWaveFormatEx->nChannels));
	}
	else
//		printf("no data in buffer.\n");

	mCurrentPosition += framesRead;
//	printf("current position = %d\n", mCurrentPosition);

	// Check if we need to decode more data
	if(mBuffer.size() < numFrames * mpWaveFormatEx->nChannels * 2)
	{
		if(mEOF && isLooping())
		{
//			printf("looping...\n");
			setDecoderPosition(0);
		}
	}	

	mCriticalSection.unlock();

	if(mEOF && framesRead == 0)
	{
		return -1;
//		printf("stopping.\n");
//		mpSound->stop();
	}

	return framesRead;
}

void DShowAudioSource::setDecoderPosition(double position)
{
	mEOF = false;
	mpMediaControl->Stop();
	REFERENCE_TIME current = (REFERENCE_TIME)(position * 1000 * 10000);
	HRESULT hr = mpMediaSeeking->SetPositions(&current, AM_SEEKING_AbsolutePositioning, NULL, 0);
	mpMediaControl->Run();
}

void DShowAudioSource::setPosition(double position)
{
	setDecoderPosition(position);

	mCriticalSection.lock();
	mBuffer.clear();
	mCurrentPosition = (Int64)(position * getSampleRate());
	mpMediaControl->Run();
	mCriticalSection.unlock();
}

double DShowAudioSource::getPosition()
{
	return (double)mCurrentPosition / getSampleRate();
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
