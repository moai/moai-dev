#include <moaiext-audiosampler/MOAIAudioSampler.h>

MOAIAudioSampler::MOAIAudioSampler ()  :
mNumFrequency(0),
        mNumChannels(0),
    mMaxBufferSizeInBytes ( 0 ),
    mBufferAryLen(0),
    mBufferAry(NULL),
    mBufferReadSizeInBytes (NULL),
    currentWriteIndex(0),
    currentReadIndex(0),
    isActive(false),
    isQueueInitialized(false)
{
    //fprintf(stderr,"MOAIAudioSampler construct. %p\n", this );
    RTTI_SINGLE ( MOAINode )
}

MOAIAudioSampler::~MOAIAudioSampler () {
    //fprintf(stderr,"MOAIAudioSampler destruct. %p\n", this );

    if(this->mBufferAry){
        for(u32 i=0;i<this->mBufferAryLen;i++){
            free( this->mBufferAry[i] );
        }
        free(this->mBufferAry);
        free(this->mBufferReadSizeInBytes);
    }

    if( this->isQueueInitialized ){
        AudioQueueDispose( this->queue, TRUE);
    }    
}

int MOAIAudioSampler::_setFrequency ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "UN" )
        ;
    self->mNumFrequency = state.GetValue < u32 >( 2, 44100 );        
    return 0;
}

int	MOAIAudioSampler::_setNumChannels ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "UN" )
        ;
    self->mNumChannels = state.GetValue < u32 >(2, 2 );
    return 0;
}





// Determine the size, in bytes, of a buffer necessary to represent the supplied number
// of seconds of audio data.
static int MyComputeRecordBufferSize(const AudioStreamBasicDescription *format, AudioQueueRef queue, float seconds)
{
	int packets, frames, bytes;
	
	frames = (int)ceil(seconds * format->mSampleRate);
	
	if (format->mBytesPerFrame > 0)
		bytes = frames * format->mBytesPerFrame;
	else {
		UInt32 maxPacketSize;
		if (format->mBytesPerPacket > 0)
			maxPacketSize = format->mBytesPerPacket;	// constant packet size
		else {
			UInt32 propertySize = sizeof(maxPacketSize); 
			OSStatus result = AudioQueueGetProperty(queue, kAudioConverterPropertyMaximumOutputPacketSize, &maxPacketSize,
                                                    &propertySize );
            if(result){
                return -1; // couldn't get queue's maximum output packet size
            }
		}
		if (format->mFramesPerPacket > 0)
			packets = frames / format->mFramesPerPacket;
		else
			packets = frames;	// worst-case scenario: 1 frame in a packet
		if (packets == 0)		// sanity check
			packets = 1;
		bytes = packets * maxPacketSize;
	}
	return bytes;
}




void MOAIAudioSampler::globalCallback( void *inUserData,
                                       AudioQueueRef inAQ,
                                       AudioQueueBufferRef inBuffer,
                                       const AudioTimeStamp *inStartTime,
                                       UInt32 inNumPackets,
                                       const AudioStreamPacketDescription *inPacketDesc )
{
    UNUSED ( inStartTime );
    UNUSED ( inPacketDesc );        

    MOAIAudioSampler *sampler = (MOAIAudioSampler*) inUserData;

//        fprintf(stderr, "callback. nPkt:%d active:%d buf:%p sz:%d curWI:%d\n",
//                inNumPackets, (int)sampler->isActive, inBuffer->mAudioData, inBuffer->mAudioDataByteSize, sampler->currentWriteIndex  );
    
    OSStatus result = AudioQueueEnqueueBuffer( inAQ, inBuffer, 0, NULL );
    if(result) printf("cannot enque buffer\n" );

    if( sampler->isActive ){        
        short *outbuf = sampler->mBufferAry[ sampler->currentWriteIndex];
        short *inbuf = (short*) inBuffer->mAudioData;
        for(u32 i=0;i<inNumPackets;i++){
            outbuf[i] = inbuf[i];
        }
        sampler->mBufferReadSizeInBytes[ sampler->currentWriteIndex] = inBuffer->mAudioDataByteSize;
    
        sampler->currentWriteIndex ++;
        if( sampler->currentWriteIndex >= sampler->mBufferAryLen ){
            sampler->currentWriteIndex = 0;
        }
    }
}

int	MOAIAudioSampler::_prepareBuffer ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "UNN" )
        ;
    double sec = state.GetValue < float > (2, 1);
    self->mBufferAryLen = state.GetValue < u32> (3, 5 );

    memset( &self->recFmt, 0, sizeof(self->recFmt));

    self->recFmt.mSampleRate = self->mNumFrequency; 
    self->recFmt.mChannelsPerFrame = self->mNumChannels; // 2 for stereo
    self->recFmt.mFormatID = kAudioFormatLinearPCM;
    self->recFmt.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    self->recFmt.mBitsPerChannel = 16;
    self->recFmt.mFormatFlags |= kLinearPCMFormatFlagIsBigEndian; // to be apparent
    self->recFmt.mBytesPerPacket = self->recFmt.mBytesPerFrame = ( self->recFmt.mBitsPerChannel / 8) * self->recFmt.mChannelsPerFrame;
    self->recFmt.mFramesPerPacket = 1;
    self->recFmt.mReserved = 0;    
    
    OSStatus result = AudioQueueNewInput( &self->recFmt, globalCallback,
                                          self, /*userdata*/
                                          NULL, /*run loop */
                                          NULL, /*run loop mode */
                                          0, /*flags */
                                          &self->queue );
    if(result){
        printf("AudioQueueNewInput failed\n");
        return 0;
    }

    int bufsize = MyComputeRecordBufferSize( &self->recFmt, self->queue, ( float )sec );
    if( bufsize < 0 ){
        printf("invalid arg?");
        return 0;
    }

    fprintf(stderr, "bufsize:%d\n", bufsize );
    self->mMaxBufferSizeInBytes = bufsize;

    self->mBufferAry = (short**) malloc( self->mBufferAryLen * sizeof(short*));
    self->mBufferReadSizeInBytes = (size_t*) malloc( self->mBufferAryLen * sizeof(size_t));
    memset( self->mBufferReadSizeInBytes, 0, self->mBufferAryLen * sizeof(size_t));
    
    assert(self->mBufferAry);
    for(u32 i=0;i< self->mBufferAryLen;++i){
        self->mBufferAry[i] = (short*) malloc( bufsize );        
        assert(self->mBufferAry[i]);
        
        AudioQueueBufferRef buffer;
        result = AudioQueueAllocateBuffer( self->queue, bufsize, &buffer );
        if(result){
            printf("AudioQueueAllocateBuffer failed\n");
            return 0;
        }
        result = AudioQueueEnqueueBuffer( self->queue, buffer, 0, NULL );
        if(result){
            printf("AudioQueueEnqueueBuffer failed\n");
            return 0;
        }
    }
    self->isQueueInitialized = true;
    return 0;    
}

int MOAIAudioSampler::_start ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "U" )
        ;
    
    OSStatus result = AudioQueueStart( self->queue, NULL );
    if(result){
        printf( "AudioQueueStart failed\n");
        return 0;
    }

    self->isActive = true;
    return 0; 
}

// @out data: array table
int	MOAIAudioSampler::_read ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "U" )
        ;
    cc8 *tn = state.GetValue < cc8 * > ( 2, "float" );    

    int tnid = -1;
    if( strcmp( tn, "float" ) == 0 ){
        tnid = 0;
    } else if( strcmp( tn, "char" ) == 0 ){
        tnid = 8;
    } else if( strcmp( tn, "short" ) == 0 ){
        tnid = 16;
	} else if( strcmp( tn, "raw" ) == 0 ){
		tnid = 255;
    } else {
        lua_pushnil(L);
        return 1;
    }
    
    for(u32 i=0;i< self->mBufferAryLen;i++){
        u32 useInd = ( self->currentReadIndex + i ) % self->mBufferAryLen;
        if( self->mBufferReadSizeInBytes[useInd] > 0 ){

            short *data = self->mBufferAry[ useInd ];
            int datanum = self->mBufferReadSizeInBytes[ useInd ] / sizeof(short);

			if(tnid==255){
				short *outdata = (short*) malloc(sizeof(short) * datanum);
				for(int j=0;j<datanum;j++){ outdata[j] = ntohs( data[j] ); }
				lua_pushlstring(L, (char*)outdata, sizeof(short)*datanum);
				free(outdata);
			} else {
				lua_createtable(L,datanum,0);
				for(int j=0;j< datanum;j++){
					short sval = ntohs(data[j] );
					switch(tnid){
						case 0:
							lua_pushnumber(L, (double)( sval / 32768.0 ) );
							break;
						case 8:
							lua_pushinteger(L,(char)( sval / 256 ) );
							break;
						case 16:
							lua_pushinteger(L, sval );
							break;
					} 
					lua_rawseti(L,-2,j+1);
				}
            }
            self->mBufferReadSizeInBytes[useInd ] = 0; // set 0 after read
            self->currentReadIndex ++;
            if( self->currentReadIndex >= self->mBufferAryLen ){
                self->currentReadIndex = 0;
            }
            return 1; // return a table
        }
    }
    // no data is incoming.
    lua_pushnil(L);
    return 1;    
}
int MOAIAudioSampler::_stop ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "U" )
        ;
    self->isActive = false;

    AudioQueueStop( self->queue, TRUE);

    return 0;
}
int MOAIAudioSampler::_pause ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "U" )
        ;
    self->isActive = false;
    return 0;
}
int MOAIAudioSampler::_resume ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIAudioSampler, "U" )
        ;
    self->isActive = true;
    return 0;
}

void MOAIAudioSampler::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );

    //	state.SetField ( -1, "ATTR_VOLUME", MOAIUntzSoundAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIAudioSampler::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setNumChannels",		_setNumChannels },
		{ "setFrequency",		_setFrequency },
		{ "prepareBuffer",		_prepareBuffer },
		{ "start",				_start },
		{ "read",				_read },
		{ "stop",				_stop },
		{ "pause",				_pause },
		{ "resume",				_resume },                
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}


