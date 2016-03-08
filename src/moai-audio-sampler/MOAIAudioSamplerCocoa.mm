// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-audio-sampler/MOAIAudioSamplerCocoa.h>
#include <Foundation/Foundation.h>

//----------------------------------------------------------------//
void RegisterMOAIAudioSamplerCocoa () {
	REGISTER_LUA_CLASS ( MOAIAudioSamplerCocoa );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int	MOAIAudioSamplerCocoa::_getLevels ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
	
	double average;
	double peak;
	
	self->GetLevels ( average, peak );
	
	state.Push ( average );
	state.Push ( peak );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_init ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )

	u32 sampleRate		= state.GetValue < u32 >( 2, 44100 );
	u32 channels		= state.GetValue < u32 >( 3, 1 );
	u32 sampleSize		= state.GetValue < u32 >( 4, sampleRate >> 1 );
	u32 totalBuffers	= state.GetValue < u32 >( 5, 5 );

	self->Init ( sampleRate, channels, sampleSize, totalBuffers );

    return 0;
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_pause ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
    self->Pause ();
    return 0;
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_resume ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
    self->Resume ();
    return 0;
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_setStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
	self->mStream.Set ( *self, state.GetLuaObject < MOAIStream >( 2, false ));
	self->mStreamFormat = state.GetValue < u32 >( 3, ZLSample::SAMPLE_S16 );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_start ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
    self->Start ();
    return 0; 
}

//----------------------------------------------------------------//
int MOAIAudioSamplerCocoa::_stop ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSamplerCocoa, "U" )
    self->Stop ();
    return 0;
}

//================================================================//
// MOAIAudioSamplerCocoa
//================================================================//

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Clear () {

	this->Stop ();

	[[ NSNotificationCenter defaultCenter ] removeObserver:( id )mSessionActiveObserver ];
    [[ NSNotificationCenter defaultCenter ] removeObserver:( id )mSessionInactiveObserver ];

    if ( this->mQueue ){
	
        AudioQueueDispose ( this->mQueue, TRUE );

		for ( u32 i = 0; i < this->mBuffers.Size (); i++ ) {
			AudioQueueFreeBuffer ( this->mQueue, this->mBuffers [ i ]);
		}
		this->mQueue = 0;
    }
	this->mBuffers.Clear ();
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::GetLevels ( double& average, double& peak ) {

	average = 0.0;
	peak = 0.0;

	if ( !this->mIsRunning ) {
        return;
    }
	
	UInt32 channelsPerFrame = this->mStreamDescription.mChannelsPerFrame;
	UInt32 size = sizeof ( AudioQueueLevelMeterState ) * channelsPerFrame;
	AudioQueueLevelMeterState* levels = ( AudioQueueLevelMeterState* )alloca ( size );
	
	assert ( levels );
	
    OSErr result = AudioQueueGetProperty ( this->mQueue, kAudioQueueProperty_CurrentLevelMeterDB, levels, &size );
	
    if ( result ) {
        fprintf ( stderr, "AudioQueueGetProperty kAudioQueueProperty_CurrentLevelMeterDB failed with %ld\n", result );
        return;
    }
    
    // average both channels for detection
    // if the device has a stereo mic (the macbook has) it's helpful
	
    for ( u32 i = 0; i < channelsPerFrame; i++ ) {
        average += ( float )( levels [ 0 ].mAveragePower );
        peak    += ( float )( levels [ 0 ].mPeakPower );
    }
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::HandleInput ( AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, UInt32 inNumPackets ) {
		
	short* inbuf = ( short* )inBuffer->mAudioData;
	
	if ( this->mStream ) {
	
		for ( u32 i = 0; i < inNumPackets; i++, this->mSamplesWritten++ ) {
			
			ZLSample::WriteSample ( *this->mStream, this->mStreamFormat, &inbuf [ i ], ZLSample::SAMPLE_S16 );
			
			if (( this->mSampleSize > 0 ) && ( this->mSamplesWritten >= ( this->mSampleSize - 1 ))) {
			
				this->InvokeListenerWithSelf ( EVENT_BUFFER );
				this->mSamplesWritten = 0;
			}
		}
	}
	
	if ( this->mIsRunning ) {
		OSStatus result = AudioQueueEnqueueBuffer ( inAQ, inBuffer, 0, NULL );
        if ( result ) {
			printf ( "cannot enqueue buffer %d\n", ( int )result );
		}
	}
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Init ( u32 sampleRate, u32 channels, u32 sampleSize, u32 totalBuffers ) {

	this->Clear ();

    memset ( &this->mStreamDescription, 0, sizeof ( this->mStreamDescription ));

    this->mStreamDescription.mSampleRate			= sampleRate;
    this->mStreamDescription.mChannelsPerFrame		= channels; // 2 for stereo
    this->mStreamDescription.mFormatID				= kAudioFormatLinearPCM;
    this->mStreamDescription.mFormatFlags			= kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    this->mStreamDescription.mBitsPerChannel		= 16;
    this->mStreamDescription.mBytesPerPacket		= ( this->mStreamDescription.mBitsPerChannel >> 3 ) * channels;
	this->mStreamDescription.mBytesPerFrame			= this->mStreamDescription.mBytesPerPacket;
    this->mStreamDescription.mFramesPerPacket		= 1;
    this->mStreamDescription.mReserved				= 0;
    
    OSStatus result = AudioQueueNewInput (
		&this->mStreamDescription,
		InputCallback,
		this,						// userdata
		CFRunLoopGetMain (),		// run loop
		kCFRunLoopCommonModes,		// run loop mode
		0,							// flags
		&this->mQueue
	);
	
    if ( result ){
        fprintf ( stderr, "AudioQueueNewInput failed\n" );
        return;
    }

	size_t bufsize = this->mStreamDescription.mBytesPerFrame * sampleSize;
    if ( bufsize <= 0 ) {
        fprintf ( stderr, "invalid arg?" );
        return;
    }

    //fprintf ( stderr, "bufsize:%d\n", bufsize );

    // where
	this->mBuffers.Init ( totalBuffers );
	
    for ( u32 i = 0; i < this->mBuffers.Size (); ++i ){
	
        result = AudioQueueAllocateBuffer ( this->mQueue, ( UInt32 )bufsize, &this->mBuffers [ i ]);
        if ( result ){
            fprintf ( stderr, "AudioQueueAllocateBuffer failed with %ld\n", result );
            return;
        }
	}
    
    // enable level metering
    UInt32 val = 1;
    result = AudioQueueSetProperty (
		this->mQueue,
		kAudioQueueProperty_EnableLevelMetering,
		&val,
		sizeof ( UInt32 )
	);
	
	if ( result ) {
        fprintf ( stderr, "AudioQueueSetProperty kAudioQueueProperty_EnableLevelMetering failed with %ld\n", result );
    }
	
	this->mSampleSize = sampleSize;
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::InputCallback ( void* inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime, UInt32 inNumPackets, const AudioStreamPacketDescription* inPacketDesc ) {

    UNUSED ( inStartTime );
    UNUSED ( inPacketDesc );

    MOAIAudioSamplerCocoa* sampler = ( MOAIAudioSamplerCocoa* )inUserData;

	sampler->HandleInput ( inAQ, inBuffer, inNumPackets );
}

//----------------------------------------------------------------//
MOAIAudioSamplerCocoa::MOAIAudioSamplerCocoa () :
    mQueue ( 0 ),
    mIsRunning ( false ),
	mIsPaused ( false ),
    mSessionActiveObserver ( 0 ),
    mSessionInactiveObserver ( 0 ),
	mStreamFormat ( 0 ),
	mSampleSize ( 0 ),
	mSamplesWritten ( 0 ) {
	
    RTTI_SINGLE ( MOAIInstanceEventSource )
    
    mSessionActiveObserver = [[ NSNotificationCenter defaultCenter ] addObserverForName:@"UntzSessionActive"
		object:nil
		queue:nil
		usingBlock:^( NSNotification* ) {
			this->Resume ();
		}
	];
    
    mSessionInactiveObserver = [[ NSNotificationCenter defaultCenter ] addObserverForName:@"UntzSessionInactive"
		object:nil
		queue:nil
		usingBlock:^( NSNotification* ) {
			this->Pause ();
		}
	];
}

//----------------------------------------------------------------//
MOAIAudioSamplerCocoa::~MOAIAudioSamplerCocoa () {

	this->mStream.Set ( *this, 0 );
    this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Pause () {

    if ( this->mIsRunning && ( !this->mIsPaused )) {
        AudioQueuePause ( this->mQueue );
		this->mIsPaused = true;
    }
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Resume () {

    if ( this->mIsRunning && this->mIsPaused ) {
        OSStatus result = AudioQueueStart ( this->mQueue, NULL );
        if ( result ) {
            fprintf ( stderr, "AudioQueueStart failed %ld\n", result );
        }
		this->mIsPaused = false;
    }
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_BUFFER",	( u32 )EVENT_BUFFER );
	
	state.SetField ( -1, "SAMPLE_S8",		( u32 )ZLSample::SAMPLE_S8 );
	state.SetField ( -1, "SAMPLE_U8",		( u32 )ZLSample::SAMPLE_U8 );
	state.SetField ( -1, "SAMPLE_S16",		( u32 )ZLSample::SAMPLE_S16 );
	state.SetField ( -1, "SAMPLE_U16",		( u32 )ZLSample::SAMPLE_U16 );
	state.SetField ( -1, "SAMPLE_S32",		( u32 )ZLSample::SAMPLE_S32 );
	state.SetField ( -1, "SAMPLE_U32",		( u32 )ZLSample::SAMPLE_U32 );
	state.SetField ( -1, "SAMPLE_FLOAT",	( u32 )ZLSample::SAMPLE_FLOAT );
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLevels",				_getLevels },
		{ "init",					_init },
		{ "pause",					_pause },
		{ "resume",					_resume },
		{ "setStream",				_setStream },
		{ "start",					_start },
		{ "stop",					_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Start () {

	if ( !this->mIsRunning ) {
	 
        for ( u32 i = 0; i < this->mBuffers.Size (); ++i ) {
		
            OSStatus result = AudioQueueEnqueueBuffer ( this->mQueue, this->mBuffers [ i ], 0, NULL );
			
            if ( result ) {
                fprintf ( stderr, "AudioQueueEnqueueBuffer failed with %ld\n", result );
                return;
            }
        }
    
		OSStatus result = AudioQueueStart ( this->mQueue, NULL );
		if ( result ) {
			fprintf ( stderr, "AudioQueueStart failed with %ld\n", result );
			return;
		}
		
		this->mIsRunning = true;
	}
}

//----------------------------------------------------------------//
void MOAIAudioSamplerCocoa::Stop () {

	if ( this->mIsRunning ) {
		this->mIsRunning = false;
		AudioQueueStop ( this->mQueue, YES );
    }
}
