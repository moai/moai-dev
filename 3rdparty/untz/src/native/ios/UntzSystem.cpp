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

#define kOutputBus 0
#define kInputBus 1

using namespace UNTZ;

#pragma mark Declarations of static C functions, callbacks, etc.

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
#endif

void audioSessionInterruptionListener(void *inClientData, 
									  UInt32 inInterruptionState);
void checkStatus(OSStatus status);


#pragma mark Class definition MacOSX SystemData
class IosSystemData : public UNTZ::SystemData
{
public:
    IosSystemData() : SystemData() {}
    ~IosSystemData()
    {
        OSStatus status = AudioOutputUnitStop(mAudioUnit);
        checkStatus(status);
    }
    UInt32 getNumFrames();
    UInt32 getNumOutputChannels();
    
    AudioComponentInstance mAudioUnit;
    AudioStreamBasicDescription mAudioFormat;
    std::vector< float > mOutputBuffer;
    int mBufferListSize;
    AudioBufferList *mBufferList;
};

UInt32 IosSystemData::getNumFrames()
{
    Float32 durationSeconds;
    UInt32 size = sizeof(durationSeconds);
#ifndef MACOSX
    AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, &size, &durationSeconds);
    return durationSeconds*mAudioFormat.mSampleRate;
#endif
    return mAudioFormat.mSampleRate;
}
UInt32 IosSystemData::getNumOutputChannels()
{
    return mAudioFormat.mChannelsPerFrame;
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
		return 0;
	}
		
    if(sysData->mOutputBuffer.size() == 0)
    {
        sysData->mOutputBuffer.resize(outBuffer->mBuffers[0].mNumberChannels*framesPerBuffer);
    }
    
    // Grab audio from our mixer.
    float *out_bufs = (float*)&sysData->mOutputBuffer[0];
    sysData->mMixer.process(0, NULL, 2, out_bufs, framesPerBuffer);

	float volume = sysData->mMixer.getVolume();
	
    // Clip nicely.
	// HBS
    //for(int i=0; i< outBuffer->mBuffers[0].mNumberChannels; i++)
    //    limit_float(&sysData->mOutputBuffer[i*framesPerBuffer], framesPerBuffer);
    
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
	
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);  // handles one event from the run loop.	
    return noErr;
}

#ifndef MACOSX
static void audioPropertyListener(void *inClientData,
								  AudioSessionPropertyID inID,
								  UInt32 inDataSize,
								  const void *inData)
{
#if 0
	AudioIO* aio = (AudioIO*)inClientData;
    
    //FIXME: Implement in C++
    
	// This is triggered by plugging a headset into a iPod touch
	if(inID == kAudioSessionProperty_AudioInputAvailable)
	{	
		fprintf(stderr, "restarting audio...");
		[aio performSelectorOnMainThread:@selector(stop) withObject:nil waitUntilDone:YES];
		AudioSessionSetActive(YES);  // Will not work if the session is not active
        
		[aio performSelectorOnMainThread:@selector(start) withObject:nil waitUntilDone:YES];
	}
	else if(inID == kAudioSessionProperty_AudioRouteChange)
	{
        UInt32 routeSize = sizeof (CFStringRef);
        CFStringRef route;
        OSStatus status = AudioSessionGetProperty(kAudioSessionProperty_AudioRoute,&routeSize,&route);
        NSString *ns_route = (NSString *)route;
        if ([ns_route isEqualToString:@"ReceiverAndMicrophone"])
        { // don't let the audio come out of the crappy phone speaker.  Use the one on the bottom instead.
            UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;  // 1
            
            status = AudioSessionSetProperty (
											  kAudioSessionProperty_OverrideAudioRoute,                         // 2
											  sizeof (audioRouteOverride),                                      // 3
											  &audioRouteOverride                                               // 4
											  );
            checkStatus(status);
        }		
	}
#endif
}

void audioSessionInterruptionListener(void *inClientData, 
									  UInt32 inInterruptionState)
{
	//System* sys = (System*)inClientData;
	if(inInterruptionState == kAudioSessionBeginInterruption)
	{
		AudioSessionSetActive(false);
	}
	else
	{
		AudioSessionSetActive(true);
        
#if 0
        //FIXME: implement in C++
		[aio performSelectorOnMainThread:@selector(stop) withObject:nil waitUntilDone:YES];
		[aio performSelectorOnMainThread:@selector(start) withObject:nil waitUntilDone:YES];			
#endif
	}
}
#endif



#pragma mark System singleton constructor
System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
    // Allocate and fill out our system data struct.
    mpData = new IosSystemData();
    IosSystemData *data = (IosSystemData *)mpData;
    data->mBufferList = nil;
    data->mBufferListSize = 0;
    
    OSStatus status;
#ifndef MACOSX
    // Set up the audio session interruption callback.
    status = AudioSessionInitialize(NULL, NULL, audioSessionInterruptionListener, this);
    AudioSessionSetActive(true);
    checkStatus(status);
	
	UInt32 category = kAudioSessionCategory_AmbientSound;
	if( options & UNTZ::RECORDABLE ) {
		category = kAudioSessionCategory_PlayAndRecord;
	}
    status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, 4, &category);
    checkStatus(status);
    
    
    // Listen for audio input changes
    status = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioInputAvailable, audioPropertyListener, this);
    checkStatus(status);
    // Listen for route changes
    status = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, audioPropertyListener, this);
    checkStatus(status);

#if !TARGET_IPHONE_SIMULATOR
    UInt32 routeSize = sizeof (CFStringRef);
    CFStringRef route;
    status = AudioSessionGetProperty(kAudioSessionProperty_AudioRoute,&routeSize,&route);
    if (CFStringCompare(route, CFSTR("ReceiverAndMicrophone"), 0) == kCFCompareEqualTo)
    {
        UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;  // 1
        
        status = AudioSessionSetProperty (
                                          kAudioSessionProperty_OverrideAudioRoute,                         // 2
                                          sizeof (audioRouteOverride),                                      // 3
                                          &audioRouteOverride                                               // 4
                                          );
        checkStatus(status);
    }
#endif

    // Set frames per buffer;
    float aBufferLength = (numFrames+1)/(float)sampleRate; // In seconds
    AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, 
                            sizeof(aBufferLength), &aBufferLength);
#endif
    
    // Describe IO audio component
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
#ifndef MACOSX
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
#else
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
#endif
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;

    // Get component
    AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
    
    // Get audio units
    status = AudioComponentInstanceNew(inputComponent, &data->mAudioUnit);
    checkStatus(status);
    
#ifndef MACOSX
    // Enable IO for playback
    UInt32 flag = 1;
    status = AudioUnitSetProperty(data->mAudioUnit, 
                                  kAudioOutputUnitProperty_EnableIO, 
                                  kAudioUnitScope_Output, 
                                  kOutputBus,
                                  &flag, 
                                  sizeof(flag));
    checkStatus(status);
#endif

    // Describe format
    data->mAudioFormat.mSampleRate			= sampleRate;
    data->mAudioFormat.mFormatID			= kAudioFormatLinearPCM;
    data->mAudioFormat.mFormatFlags         = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    data->mAudioFormat.mFramesPerPacket     = 1;
    data->mAudioFormat.mChannelsPerFrame	= 2;
    data->mAudioFormat.mBitsPerChannel		= 16;
    data->mAudioFormat.mBytesPerPacket		= 4;
    data->mAudioFormat.mBytesPerFrame		= 4;
    
    // Apply format
    status = AudioUnitSetProperty(data->mAudioUnit, 
                                  kAudioUnitProperty_StreamFormat, 
                                  kAudioUnitScope_Input, 
                                  kOutputBus, 
                                  &data->mAudioFormat, 
                                  sizeof(data->mAudioFormat));
    checkStatus(status);
    
    
    // Set output callback
    AURenderCallbackStruct callbackStruct;
    callbackStruct.inputProc = playbackCallback;
    callbackStruct.inputProcRefCon = mpData;
    status = AudioUnitSetProperty(data->mAudioUnit, 
                                  kAudioUnitProperty_SetRenderCallback, 
                                  kAudioUnitScope_Global, 
                                  kOutputBus,
                                  &callbackStruct, 
                                  sizeof(callbackStruct));
    checkStatus(status);
    
    status = AudioUnitInitialize(data->mAudioUnit); //can not vibrate after this call ;(
    checkStatus(status);
    
    status = AudioOutputUnitStart(data->mAudioUnit);
    checkStatus(status);
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
    return data->mAudioFormat.mSampleRate;
}


void System::setSampleRate(UInt32 sampleRate)
{
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
    fprintf(stderr, "audioio status: %d\n", (int)status);
#endif
}

