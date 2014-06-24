//
//  UntzSystem.cpp - OSX / iOS
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSystem.h"
#include "SystemData.h"
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioServices.h>
#import <AudioToolbox/AudioToolbox.h>
#import <Foundation/Foundation.h>

#define kOutputBus 0
#define kInputBus  1

using namespace UNTZ;

#pragma mark Declarations of static C functions, callbacks, etc.
static void errorCallback (void                 *inRefCon,
                           AudioUnit            inUnit,
                           AudioUnitPropertyID  inID,
                           AudioUnitScope       inScope,
                           AudioUnitElement     inElement);

static OSStatus recordCallback(void *inRefCon,
							   AudioUnitRenderActionFlags *ioActionFlags,
							   const AudioTimeStamp *inTimeStamp,
							   UInt32 inBusNumber,
							   UInt32 inNumberFrames,
							   AudioBufferList *ioData);

static OSStatus playbackCallback(void *inRefCon, 
                                 AudioUnitRenderActionFlags *ioActionFlags, 
                                 const AudioTimeStamp *inTimeStamp, 
                                 UInt32 inBusNumber, 
                                 UInt32 inNumberFrames, 
                                 AudioBufferList *ioData);
#ifndef MACOSX
static void audioPropertyListener(void *inClientData,
                                  AudioSessionPropertyID inID,
                                  UInt32 inDataSize,
                                  const void *inData);

void audioSessionInterruptionListener(void *inClientData, 
									  UInt32 inInterruptionState);
#endif

void checkStatus(OSStatus status);


#pragma mark Class definition MacOSX / iOS SystemData

class IosSystemData : public UNTZ::SystemData
{
public:
    IosSystemData() : SystemData(), mInterruptedOnPlayback(false) {}
    virtual ~IosSystemData();
    UInt32 getNumFrames();
    UInt32 getNumOutputChannels();
	void setActive(bool active);
	
	bool mInterruptedOnPlayback;
	AUGraph mGraph;
	AUNode ioNode;
    
    AudioComponentInstance mAudioUnit;
    AudioStreamBasicDescription mAudioOutputFormat;
    std::vector< float > mOutputBuffer;
	
    AudioStreamBasicDescription mAudioInputFormat;
    std::vector< SInt16 > mInputBuffer;
	
	UInt32 getAudioSessionCategory(UInt32 options);
	void setupAudioSessionCategory(UInt32 options);
	void setupAudioGraph(UInt32 sampleRate, UInt32 options);
	void tearDownAudioGraph();
};

IosSystemData::~IosSystemData()
{
    tearDownAudioGraph();
}

UInt32 IosSystemData::getNumFrames()
{
    Float32 durationSeconds;
    UInt32 size = sizeof(durationSeconds);
#ifndef MACOSX
    AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, &size, &durationSeconds);
    return durationSeconds*mAudioOutputFormat.mSampleRate;
#endif
    return mAudioOutputFormat.mSampleRate;
}

UInt32 IosSystemData::getNumOutputChannels()
{
    return mAudioOutputFormat.mChannelsPerFrame;
}

void IosSystemData::setActive(bool active)
{
	if (active && mInterruptedOnPlayback) {
		fprintf(stderr, "was interrupted, waiting for interruption handler\n");
		return;
	}
	
	OSStatus result;
	
#ifndef MACOSX
	result = AudioSessionSetActive(active);
	checkStatus(result);
#endif
	
	if (active && !SystemData::isActive()) {
		[[NSNotificationCenter defaultCenter] postNotificationName:@"UntzSessionActive" object:nil];
	} else if (!active && SystemData::isActive()){
		[[NSNotificationCenter defaultCenter] postNotificationName:@"UntzSessionInactive" object:nil];
	}
	
    Boolean isRunning = false;
	result = AUGraphIsRunning(mGraph, &isRunning);
	if(result == noErr) {
		if(!isRunning && active) {
			result = AUGraphStart(mGraph);
			checkStatus(result);
		} else if(isRunning && !active) {
			result = AUGraphStop(mGraph);
			checkStatus(result);
		}
	}

	SystemData::setActive(active);
}

UInt32 IosSystemData::getAudioSessionCategory(UInt32 options)
{
#ifndef MACOSX
	// Check for audio input
	UInt32 inputAvailable;
	UInt32 ui32PropertySize = sizeof(inputAvailable);
	OSStatus status = AudioSessionGetProperty(kAudioSessionProperty_AudioInputAvailable,
											  &ui32PropertySize, &inputAvailable);
    checkStatus(status);
	
	// Set up the audio session category
	UInt32 category = kAudioSessionCategory_SoloAmbientSound;
	if(options & UNTZ::RECORDABLE && inputAvailable) {
		category = kAudioSessionCategory_PlayAndRecord;
	} else if(options & UNTZ::MIX_WITH_OTHERS) {
		category = kAudioSessionCategory_AmbientSound;
	}
	return category;
#else
	return 0;
#endif
}

void IosSystemData::setupAudioSessionCategory(UInt32 options)
{
#ifndef MACOSX
	// Set up the audio session category
	UInt32 category = getAudioSessionCategory(options);
    OSStatus status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
									 sizeof(category), &category);
    checkStatus(status);
	
	// Set audio session property "allow mixing" to true so audio can be recorded while it is playing
	UInt32 allowMixing = options & UNTZ::MIX_WITH_OTHERS ? 1 : 0;
	status = AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryMixWithOthers,
											  sizeof(allowMixing), &allowMixing);
	checkStatus(status);
	
	// Set up the session mode
	UInt32 mode = kAudioSessionMode_Default;
	if(options & UNTZ::VOICECHAT_MODE) {
		mode = kAudioSessionMode_VoiceChat;
	}
	status = AudioSessionSetProperty(kAudioSessionProperty_Mode,
									 sizeof(mode), &mode);
	
#if !TARGET_IPHONE_SIMULATOR
	// Set up to allow bluetooth inputs
	UInt32 allowBluetoothInput = 1;
	AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryEnableBluetoothInput,
							sizeof (allowBluetoothInput),
							&allowBluetoothInput);
	
	// Make the default sound route for the session be to use the speaker
	// instead of receiver when kAudioSessionCategory_PlayAndRecord category is used
	// This persists through interruptions and route changes such as when
	// plugging/unplugging headsets.
	UInt32 doChangeDefaultRoute = 1;
	AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryDefaultToSpeaker,
							sizeof(doChangeDefaultRoute), &doChangeDefaultRoute);
#endif
#endif
}

void IosSystemData::setupAudioGraph(UInt32 sampleRate, UInt32 options)
{
	OSStatus status;
	
    // Create a new audio processing mGraph.
    status = NewAUGraph(&mGraph);
	checkStatus(status);
    
    // Describe IO audio component
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
#ifndef MACOSX
	// Use VoiceProcessing IO unit
	if(options & UNTZ::USE_VPIO) {
		desc.componentSubType = kAudioUnitSubType_VoiceProcessingIO;
	} else {
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
	}
#else
//	if(options & UNTZ::USE_VPIO || options & UNTZ::RECORDABLE) {
//		desc.componentSubType = kAudioUnitSubType_VoiceProcessingIO;
//	} else {
		desc.componentSubType = kAudioUnitSubType_DefaultOutput;
//	}
#endif
	
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
    // add component to mGraph
	status = AUGraphAddNode(mGraph,
							&desc,
							&ioNode);
    checkStatus(status);
	
	// Open the mGraph
    status = AUGraphOpen(mGraph);
    checkStatus(status);
	
	// Obtain the I/O unit instance from the corresponding node.
	status = AUGraphNodeInfo(mGraph,
							 ioNode,
							 &desc,
							 &mAudioUnit);
    checkStatus(status);
	
    // Set output callback
    AURenderCallbackStruct callbackStruct;
    callbackStruct.inputProc = playbackCallback;
    callbackStruct.inputProcRefCon = this;
	status = AUGraphSetNodeInputCallback(mGraph,
                                         ioNode,
                                         kOutputBus,
                                         &callbackStruct);
    checkStatus(status);
	
#ifndef MACOSX
    // Enable IO for playback
    UInt32 flag = 1;
    status = AudioUnitSetProperty(mAudioUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Output,
                                  kOutputBus,
                                  &flag,
                                  sizeof(flag));
    checkStatus(status);
#endif
	
	// Read format before setting output
	AudioStreamBasicDescription audioStreamBasicDescription;
	UInt32 audioStreamBasicDescriptionsize = sizeof(AudioStreamBasicDescription);
	AudioUnitGetProperty(mAudioUnit,
						 kAudioUnitProperty_StreamFormat,
						 kAudioUnitScope_Input,
						 kOutputBus,
						 &audioStreamBasicDescription,
						 &audioStreamBasicDescriptionsize);
	
#ifndef MACOSX
    // Describe output format
	// iOS prefers SInt16 interleaved
    mAudioOutputFormat.mSampleRate			= sampleRate;
    mAudioOutputFormat.mFormatID			= kAudioFormatLinearPCM;
    mAudioOutputFormat.mFormatFlags			= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    mAudioOutputFormat.mFramesPerPacket		= 1;
    mAudioOutputFormat.mChannelsPerFrame	= 2;
    mAudioOutputFormat.mBitsPerChannel		= 16;
    mAudioOutputFormat.mBytesPerPacket		= 4;
    mAudioOutputFormat.mBytesPerFrame		= 4;
	mAudioOutputFormat.mReserved			= 0;
#else
    // Describe output format
	// OSX prefers float non interleaved when using VPIO
    mAudioOutputFormat.mSampleRate			= sampleRate;
    mAudioOutputFormat.mFormatID			= kAudioFormatLinearPCM;
    mAudioOutputFormat.mFormatFlags			= kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
    mAudioOutputFormat.mFramesPerPacket		= 1;
    mAudioOutputFormat.mChannelsPerFrame	= 2;
    mAudioOutputFormat.mBitsPerChannel		= 32;
    mAudioOutputFormat.mBytesPerPacket		= 4;
    mAudioOutputFormat.mBytesPerFrame		= 4;
	mAudioOutputFormat.mReserved			= 0;
#endif
    
    // Apply output format
    status = AudioUnitSetProperty(mAudioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  kOutputBus,
                                  &mAudioOutputFormat,
                                  sizeof(mAudioOutputFormat));
    checkStatus(status);
	
	// Read format after setting it
	AudioUnitGetProperty(mAudioUnit,
						 kAudioUnitProperty_StreamFormat,
						 kAudioUnitScope_Input,
						 kOutputBus,
						 &audioStreamBasicDescription,
						 &audioStreamBasicDescriptionsize);
	
	if( options & UNTZ::RECORDABLE ) {
#ifndef MACOSX
		// Enable IO for recording
		UInt32 flag = 1;
		status = AudioUnitSetProperty(mAudioUnit,
									  kAudioOutputUnitProperty_EnableIO,
									  kAudioUnitScope_Input,
									  kInputBus,
									  &flag,
									  sizeof(flag));
		checkStatus(status);
#endif
		
		//		AudioStreamBasicDescription audioStreamBasicDescription;
		//		UInt32 audioStreamBasicDescriptionsize = sizeof (AudioStreamBasicDescription);
		//		AudioUnitGetProperty(mAudioUnit,
		//							 kAudioUnitProperty_StreamFormat,
		//							 kAudioUnitScope_Output,
		//							 kInputBus,
		//							 &audioStreamBasicDescription,
		//							 &audioStreamBasicDescriptionsize);
		
		// Describe input format, force mono SInt16 across iOS/OSX
		mAudioInputFormat.mSampleRate			= sampleRate;
		mAudioInputFormat.mFormatID				= kAudioFormatLinearPCM;
		mAudioInputFormat.mFormatFlags			= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		mAudioInputFormat.mFramesPerPacket		= 1;
		mAudioInputFormat.mChannelsPerFrame		= 1;
		mAudioInputFormat.mBitsPerChannel		= 16;
		mAudioInputFormat.mBytesPerPacket		= 2;
		mAudioInputFormat.mBytesPerFrame		= 2;
		mAudioInputFormat.mReserved				= 0;
		
		// Apply input format
		status = AudioUnitSetProperty(mAudioUnit,
									  kAudioUnitProperty_StreamFormat,
									  kAudioUnitScope_Output,
									  kInputBus,
									  &mAudioInputFormat,
									  sizeof(mAudioInputFormat));
		checkStatus(status);
		
		// Set input callback
		AURenderCallbackStruct callbackStruct;
		callbackStruct.inputProc = recordCallback;
		callbackStruct.inputProcRefCon = this;
        status = AudioUnitSetProperty(mAudioUnit,
                                      kAudioOutputUnitProperty_SetInputCallback,
                                      kAudioUnitScope_Global,
                                      kInputBus,
                                      &callbackStruct,
                                      sizeof(callbackStruct));
		checkStatus(status);
	}
	
	// Setting a callback to receive errors from the unit
	status = AudioUnitAddPropertyListener(mAudioUnit,
										  kAudioUnitProperty_LastRenderError,
										  errorCallback,
										  NULL);
	checkStatus(status);
    
	// Initialize the mGraph
    status = AUGraphInitialize(mGraph);
    checkStatus(status);
    
	// Some diagnostic
	CAShow(mGraph);
}

void
IosSystemData::tearDownAudioGraph()
{
	Boolean isRunning = false;
    OSStatus result = AUGraphIsRunning(mGraph, &isRunning);
	if(result == noErr && isRunning) {
		AUGraphStop(mGraph);
//		
//		// wait for mGraph to stop so callback doesn't try to access us after we're gone
//		do {
//			result = AUGraphIsRunning(mGraph, &isRunning);
//			CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
//		} while(result == noErr && isRunning);
	}
	
	AudioUnitRemovePropertyListenerWithUserData(mAudioUnit,
												kAudioUnitProperty_LastRenderError,
												errorCallback,
												NULL);
	
	AUGraphUninitialize(mGraph);
	AUGraphClose(mGraph);
	DisposeAUGraph(mGraph);
}

static inline float limit_float(float inValue)
{
	//	const float twoOverPi = 2.0f / 3.14f;
	//    const float gain = 2.0f;
	
	//	int inSign = (1-2*signbit(inValue));
	
    return ((1-2*signbit(inValue)) * atanf(fabs(inValue) * 2.0f) * (2.0f / 3.14f));
}

// from HBS
static inline SInt16 limit_float_conv_SInt16(float inValue)
{
//	const float twoOverPi = 2.0f / 3.14f;
//    const float gain = 2.0f;

//	int inSign = (1-2*signbit(inValue));

    return (SInt16)((1-2*signbit(inValue)) * atanf(fabs(inValue) * 2.0f) * ((2.0f / 3.14f) * 32767));
}

#pragma mark System Callbacks

#include "vector_util.h"

static void errorCallback (void                 *inRefCon,
                           AudioUnit            inUnit,
                           AudioUnitPropertyID  inID,
                           AudioUnitScope       inScope,
                           AudioUnitElement     inElement)
{
    UInt32 size = 0;
    Boolean isWritable = false;
    OSStatus result = AudioUnitGetPropertyInfo(inUnit,
                                               inID,
                                               inScope,
                                               inElement,
                                               &size,
                                               &isWritable);
    
    if (result != noErr) {
#ifdef DEBUG
		fprintf(stderr, "In errorCallback: AudioUnitGetPropertyInfo status=%d\n", (int)result);
#endif
    }
    
    if (inID == kAudioUnitProperty_LastRenderError) {
        OSStatus reportedError = noErr;
        result = AudioUnitGetProperty(inUnit,
                                      inID,
                                      inScope,
                                      inElement,
                                      &reportedError,
                                      &size);
        if (result != noErr) {
#ifdef DEBUG
			fprintf(stderr, "In errorCallback: while getting the actual error status=%d\n", (int)result);
#endif
        }
        
        if (reportedError != noErr) {
#ifdef DEBUG
			fprintf(stderr, "In errorCallback: AudioUnit VP reported an error = %d\n", (int)result);
#endif
        }
    }
}

static OSStatus recordCallback(void *userData,
							   AudioUnitRenderActionFlags *actionFlags,
							   const AudioTimeStamp *timeStamp,
							   UInt32 busNumber,
							   UInt32 framesPerBuffer,
							   AudioBufferList *outBuffer)
{
    IosSystemData *sysData = (IosSystemData *)userData;
	
	// Don't do anything if the system is NOT active
	if(!sysData->isActive())
	{
		return 0;
	}
	
	// Resize input buffer if needed
    if(sysData->mInputBuffer.size() < framesPerBuffer)
    {
        sysData->mInputBuffer.resize(framesPerBuffer);
    }
	
	AudioBufferList list;
	list.mNumberBuffers = 1;
	list.mBuffers[0].mData = (SInt16*)&sysData->mInputBuffer[0];
	list.mBuffers[0].mDataByteSize = 2 * framesPerBuffer;
	list.mBuffers[0].mNumberChannels = 1;
	outBuffer = &list;
	
	// Read input data
//	OSStatus result = AudioUnitRender(sysData->mAudioUnit, actionFlags, timeStamp, 1, framesPerBuffer, outBuffer);
//	if(result != noErr) {
//		
//	}
	return noErr;
}

static OSStatus playbackCallback(void *userData,
                                 AudioUnitRenderActionFlags *actionFlags, 
                                 const AudioTimeStamp *timeStamp, 
                                 UInt32 busNumber, 
                                 UInt32 framesPerBuffer, 
                                 AudioBufferList *outBuffer)
{  	
    IosSystemData *sysData = (IosSystemData *)userData;

	// Don't do anything if the system is NOT active (zero buffer)
	if(!sysData->isActive())
	{	
		SInt16 *outbuf = (SInt16 *) outBuffer->mBuffers[0].mData;
		memset(outbuf, 0, sizeof(SInt16) * framesPerBuffer * sysData->getNumOutputChannels());
		
		*actionFlags |= kAudioUnitRenderAction_OutputIsSilence;
		return 0;
	}
	
	// Resize output buffer if needed
    if(sysData->mOutputBuffer.size() < sysData->getNumOutputChannels()*framesPerBuffer)
    {
        sysData->mOutputBuffer.resize(sysData->getNumOutputChannels()*framesPerBuffer);
    }
    
    // Grab audio from our mixer.
    float *out_bufs = (float*)&sysData->mOutputBuffer[0];
    sysData->mMixer.process(0, NULL, sysData->getNumOutputChannels(), out_bufs, framesPerBuffer);

	float volume = sysData->mMixer.getVolume();
	
    // Clip nicely.
	// HBS
    //for(int i=0; i< outBuffer->mBuffers[0].mNumberChannels; i++)
    //    limit_float(&sysData->mOutputBuffer[i*framesPerBuffer], framesPerBuffer);
    
	// Apply volume and limiter
	if(sysData->mAudioOutputFormat.mFormatFlags & kAudioFormatFlagIsSignedInteger) {
		// Convert left-right buffers to short samples and interleave.
		SInt16 *outbuf = (SInt16 *) outBuffer->mBuffers[0].mData;
		for(int i=0; i<framesPerBuffer; i++)
		{
			for(int j=0; j<outBuffer->mBuffers[0].mNumberChannels; j++)
			{
				//*(outbuf++) = 32767 * sysData->mOutputBuffer[j*framesPerBuffer+i];
				*(outbuf++) = limit_float_conv_SInt16(volume * sysData->mOutputBuffer[j*framesPerBuffer+i]); // HBS
			}
		}
	} else {
		float *outbufLeft = (float *) outBuffer->mBuffers[0].mData;
		float *outbufRight = (float *) outBuffer->mBuffers[1].mData;
		for(int i=0; i<framesPerBuffer; i++)
		{
			*(outbufLeft++)  = limit_float(volume * sysData->mOutputBuffer[i]);
			if(outBuffer->mNumberBuffers > 1) {
				*(outbufRight++) = limit_float(volume * sysData->mOutputBuffer[framesPerBuffer+i]);
			}
		}
	}
	
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);  // handles one event from the run loop.
    return noErr;
}

#ifndef MACOSX
static void audioPropertyListener(void *inClientData,
								  AudioSessionPropertyID inID,
								  UInt32 inDataSize,
								  const void *inData)
{
    IosSystemData *sysData = (IosSystemData *)inClientData;

   
	// This is triggered by plugging a headset into a iPod touch
	if(inID == kAudioSessionProperty_AudioInputAvailable &&
	   UNTZ::System::get()->getOptions() & UNTZ::RECORDABLE)
	{
		// Original options required recording but had no inputs then
		// Reinitialize session and graph according to original options
		UNTZ::System::get()->setOptions(UNTZ::System::get()->getOptions(), YES);
	}
	else if(inID == kAudioSessionProperty_AudioRouteChange)
	{
		CFDictionaryRef routeChangeDictionary = (CFDictionaryRef)inData;
        CFNumberRef routeChangeReasonRef =
			(CFNumberRef)CFDictionaryGetValue(routeChangeDictionary,
											  CFSTR(kAudioSession_AudioRouteChangeKey_Reason));
		
        SInt32 routeChangeReason;
        CFNumberGetValue(routeChangeReasonRef, kCFNumberSInt32Type, &routeChangeReason);
		fprintf(stderr, "AudioRouteChange reason:%d\n", routeChangeReason);
		if(routeChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
		{
			fprintf(stderr, "Old device unavailable...\n");
		}
		else if(routeChangeReason == kAudioSessionRouteChangeReason_CategoryChange)
		{
			// when video player starts, the category didn't necessarily changed
			// but we reactivate the session to restart any audio recorder.
			bool wasActive = sysData->isActive();
			sysData->setActive(false);
			
			UInt32 category;
			UInt32 ui32PropertySize = sizeof(category);
			OSStatus status = AudioSessionGetProperty(kAudioSessionProperty_AudioCategory,
													  &ui32PropertySize, &category);
			checkStatus(status);		
			if (status == 0 && category != sysData->getAudioSessionCategory(UNTZ::System::get()->getOptions()))
			{
				fprintf(stderr, "category changed to: %d\n", category);
				// reinitialize to force back our previous category
				UNTZ::System::get()->setOptions(UNTZ::System::get()->getOptions(), YES);
			}
			
			sysData->setActive(wasActive);
		}
	}
}

void audioSessionInterruptionListener(void *inClientData,
									  UInt32 inInterruptionState)
{
    IosSystemData *sysData = (IosSystemData *)inClientData;

	if(inInterruptionState == kAudioSessionBeginInterruption && sysData->isActive())
	{
		fprintf(stderr, "Interruption Started, pausing...\n");
		sysData->mInterruptedOnPlayback = sysData->isActive();
		sysData->setActive(false);
	}
	else if(inInterruptionState == kAudioSessionEndInterruption && sysData->mInterruptedOnPlayback)
	{
		UInt32 interruptionType;
		UInt32 ui32PropertySize = sizeof(interruptionType);
		OSStatus status = AudioSessionGetProperty(kAudioSessionProperty_InterruptionType,
												  &ui32PropertySize, &interruptionType);
		if(interruptionType == kAudioSessionInterruptionType_ShouldResume)
		{		
			fprintf(stderr, "Interruption Ended, resuming...\n");
			// reapply audio session category after interruption
			sysData->setupAudioSessionCategory(UNTZ::System::get()->getOptions());
			sysData->mInterruptedOnPlayback = false;
			sysData->setActive(true);
		}
	}
}
#endif



#pragma mark System singleton constructor
System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options) :
	options(options)
{
    OSStatus status;
	
    // Allocate and fill out our system data struct.
    mpData = new IosSystemData();
    IosSystemData *data = (IosSystemData *)mpData;
	
#ifndef MACOSX
    // Set up the audio session interruption callback.
    status = AudioSessionInitialize(NULL, NULL, audioSessionInterruptionListener, data);
    checkStatus(status);
	
    // Listen for audio input changes
    status = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioInputAvailable, audioPropertyListener, data);
    checkStatus(status);
	
    // Listen for route changes
    status = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, audioPropertyListener, data);
    checkStatus(status);
	
	// Setup Audio session category
	data->setupAudioSessionCategory(options);

    // Set frames per buffer;
    float aBufferLength = (numFrames+1)/(float)sampleRate; // In seconds
    AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, 
                            sizeof(aBufferLength), &aBufferLength);
	
    status = AudioSessionSetActive(true);
    checkStatus(status);
#endif
	
	data->setupAudioGraph(sampleRate, options);
}

System::~System()
{
	if(mpData)
		delete mpData;
}

void System::shutdown()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
}

System* System::initialize(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	if(!msInstance)
    {
		msInstance = new System(sampleRate, numFrames, options);
        msInstance->mpData->mMixer.init();
		msInstance->resume();
    }
	return msInstance;
}

System* System::get()
{
    return msInstance;
}

UInt32 System::getSampleRate()
{
    IosSystemData *data = (IosSystemData *)mpData;
    return data->mAudioOutputFormat.mSampleRate;
}

void System::setSampleRate(UInt32 sampleRate)
{
}

void System::setOptions(UInt32 newOptions, bool force)
{
	if(newOptions != options || force)
	{
		IosSystemData *data = (IosSystemData *)msInstance->mpData;
		
		// check if only the voicechat mode changed
		if((options ^ newOptions) & VOICECHAT_MODE) {
			options = newOptions;
#ifndef MACOSX
			AudioSessionSetActive(false);
			data->setupAudioSessionCategory(options);
			AudioSessionSetActive(true);
#endif
			return;
		}
		
		options = newOptions;
		
		bool wasActive = data->isActive();
		data->setActive(false);
		
		data->tearDownAudioGraph();
		
		data->setupAudioSessionCategory(options);
		data->setupAudioGraph(getSampleRate(), newOptions);
		
		// reactivate
		if(wasActive)
		{
			data->setActive(true);
		}
	}
}

void System::setVolume(float volume)
{
	return msInstance->mpData->mMixer.setVolume(volume);
}

float System::getVolume() const
{
	return msInstance->mpData->mMixer.getVolume();
}

void System::suspend()
{
	msInstance->mpData->setActive(false);
}

void System::resume()
{
	msInstance->mpData->setActive(true);
}



#pragma mark Helper functions

void checkStatus(OSStatus status)
{
#ifdef DEBUG
	if (status != noErr) {
		fprintf(stderr, "audioio status: %d\n", (int)status);
	}
#endif
}

