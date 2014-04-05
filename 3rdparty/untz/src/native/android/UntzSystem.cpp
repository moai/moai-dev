#include "UntzSystem.h"
#include "SystemData.h"
#include <jni.h>
#include <android/log.h>

extern JavaVM *jvm;

using namespace UNTZ;

class PlaybackThread;

class AndroidSystemData : public UNTZ::SystemData
{
public:
    AndroidSystemData() : SystemData() {}
    ~AndroidSystemData() {}
    UInt32 getNumFrames();
    UInt32 getNumOutputChannels();
    PlaybackThread *mpAudioThread;

    UInt32 mSampleRate;
    UInt32 mFramesPerBuffer;
    UInt32 mOptions;
};


UInt32 AndroidSystemData::getNumFrames()
{
	return 8192;//4096;
}
UInt32 AndroidSystemData::getNumOutputChannels()
{
    return 2;
}


#include <threading/Threading.h>
#include <vector>

class PlaybackThread : public RThread
{
public:
    // Singleton
    static PlaybackThread *getInstance();
    static void deleteInstance();

    void setData(AndroidSystemData *d);

protected:
    // RThread
    void run();

private:
    PlaybackThread();
    ~PlaybackThread();

    static PlaybackThread* mInstance;
    AndroidSystemData *mpSystemData;
};

// Some defines needed by java.
#define CHANNEL_OUT_MONO 0x00000004
#define CHANNEL_OUT_STEREO 0x0000000c
#define ENCODING_PCM_16BIT 0x00000002
#define ENCODING_PCM_8BIT 0x00000003
#define STREAM_MUSIC 0x00000003
#define MODE_STREAM 0x00000001

PlaybackThread* PlaybackThread::mInstance;

PlaybackThread* PlaybackThread::getInstance()
{
    if(!mInstance)
    {
        mInstance = new PlaybackThread();
    }
    return mInstance;
}

void PlaybackThread::deleteInstance()
{
    mInstance->stop();
    mInstance->wait();

    delete mInstance;
    mInstance = NULL;
}


PlaybackThread::PlaybackThread()
{
}

PlaybackThread::~PlaybackThread()
{
}

void PlaybackThread::setData(AndroidSystemData *d)
{
    mpSystemData = d;
}

void PlaybackThread::run()
{  	
    __android_log_write(ANDROID_LOG_DEBUG, "UntzJNI", "Started audio push thread");

	// Set up our environment
    JNIEnv* env;
    jvm->AttachCurrentThread(&env, NULL);
    env->PushLocalFrame(2);

    // Get the class object for AudioTrack
    jclass audioTrackClass = env->FindClass("android/media/AudioTrack");
    if (!audioTrackClass)
    {
        __android_log_write(ANDROID_LOG_ERROR,"UntzJNI","android.media.AudioTrack class is not found. Are you running at least 1.5 version?");
        return;
    }
    env->NewGlobalRef(audioTrackClass);

    // Get method ids for the methods in AudioTrack
    static jmethodID audioTrackConstructor = env->GetMethodID(audioTrackClass, "<init>", "(IIIIII)V");
    static jmethodID getMinBufferSizeMethod = env->GetStaticMethodID(audioTrackClass, "getMinBufferSize", "(III)I");
    static jmethodID playMethod = env->GetMethodID(audioTrackClass, "play", "()V");
    static jmethodID stopMethod = env->GetMethodID(audioTrackClass, "stop", "()V");
    static jmethodID pauseMethod = env->GetMethodID(audioTrackClass, "pause", "()V");
    static jmethodID releaseMethod = env->GetMethodID(audioTrackClass, "release", "()V");
    static jmethodID writeMethod = env->GetMethodID(audioTrackClass, "write", "([BII)I");
	static jmethodID setPlaybackRateMethod = env->GetMethodID(audioTrackClass, "setPlaybackRate", "(I)I");

	int sampleRateInHz = mpSystemData->mSampleRate;//44100;
    int channelConfig = CHANNEL_OUT_STEREO;
    const int numChannels = 2;
    int audioFormat = ENCODING_PCM_16BIT;

    int bufferSizeInBytes = env->CallStaticIntMethod(audioTrackClass,
                                                     getMinBufferSizeMethod,
                                                     sampleRateInHz,
                                                     channelConfig,
                                                     audioFormat);

    int framesPerBuffer = (float)bufferSizeInBytes / sizeof(SInt16) / numChannels;

    // Create and start a new AudioTrack
    jobject track = env->NewObject(audioTrackClass,
                                   audioTrackConstructor,
                                   STREAM_MUSIC, sampleRateInHz, channelConfig,
                                   audioFormat,
                                   bufferSizeInBytes,
                                   MODE_STREAM);
    env->CallNonvirtualVoidMethod(track, audioTrackClass, playMethod);
	bool isPlaying = true;

	// Set the playback rate
//  int error = env->CallNonvirtualIntMethod(track, audioTrackClass, setPlaybackRateMethod, mpSystemData->mSampleRate);
//	if(error != 0)
//		__android_log_write(ANDROID_LOG_ERROR, "UntzJNI", "Failed to set playback rate");
    
    // Get our buffer
    jarray buffer = env->NewByteArray(bufferSizeInBytes);

    float float_buf[framesPerBuffer*numChannels];

    struct timespec nextCallTime;
    struct timespec now;
    struct timespec sleepTime;
    struct timespec timeLeft;

    // Initialize
    clock_gettime(CLOCK_MONOTONIC, &nextCallTime);
    sleepTime.tv_sec = 0; // sleepTime.tv_nsec gets set in the loop

    char str[500];
    long nsec_per_buffer = ((double)framesPerBuffer / sampleRateInHz ) * 1000000000;
	int bufferCount = 0;
    while (!shouldThreadExit())
    {		
        // Grab the float samples from the mixer.
        mpSystemData->mMixer.process(0, NULL, numChannels, float_buf, framesPerBuffer);

		float volume = mpSystemData->mMixer.getVolume();
        
		/*
        // Clip nicely.
        for(int i=0; i< outBuffer->mBuffers[0].mNumberChannels; i++)
            limit_float(&float_buf[i*framesPerBuffer], framesPerBuffer);
        */

        // Convert the float samples to short and pass into AudioTrack.
        void* pBuffer = env->GetPrimitiveArrayCritical(buffer, NULL);
        if (pBuffer)
        {
            // Convert left-right buffers to short samples and interleave.
            SInt16 *outbuf = (SInt16*)pBuffer;
            for(int i=0; i<framesPerBuffer; i++)
            {
                for(int j=0; j<numChannels; j++)
                {
                    //*(outbuf++) = 32767 * float_buf[j*framesPerBuffer+i];
					*(outbuf++) = 32767 * volume * float_buf[j*framesPerBuffer+i]; // from HBS
                }
            }
            env->ReleasePrimitiveArrayCritical(buffer, pBuffer, 0);
            env->CallNonvirtualIntMethod(track, audioTrackClass, writeMethod, buffer, 0, bufferSizeInBytes);
//			__android_log_write(ANDROID_LOG_DEBUG, "UntzJNI", "pushed buffer.");
        }
        else
        {
            __android_log_write(ANDROID_LOG_ERROR,"UntzJNI","Failed to get pointer to array bytes");
        }

		if(++bufferCount == 3)
		{
			do
			{
				if(!mpSystemData->isActive() && isPlaying)
				{
					__android_log_write(ANDROID_LOG_INFO,"UntzJNI","Pausing audio");
				    env->CallNonvirtualVoidMethod(track, audioTrackClass, pauseMethod);
					isPlaying = false;
				}
				else if(mpSystemData->isActive() && !isPlaying)
				{
					__android_log_write(ANDROID_LOG_INFO,"UntzJNI","Playing audio");
				    env->CallNonvirtualVoidMethod(track, audioTrackClass, playMethod);
					isPlaying = true;
				}
				
				// Calculate when the next callback should happen (based on buffer size)
				long next_nsecs = nextCallTime.tv_nsec+nsec_per_buffer;
				nextCallTime.tv_nsec = next_nsecs % 1000000000;
				nextCallTime.tv_sec = nextCallTime.tv_sec + next_nsecs / 1000000000;

				// Sleep until the next callback should come in.
				clock_gettime(CLOCK_MONOTONIC, &now);
				sleepTime.tv_nsec = (nextCallTime.tv_sec-now.tv_sec)*1000000000+(nextCallTime.tv_nsec-now.tv_nsec);
				while(sleepTime.tv_nsec < 0)
					sleepTime.tv_nsec += nsec_per_buffer;			
			}
			while(nanosleep(&sleepTime, &timeLeft) < 0 || !mpSystemData->isActive());

			--bufferCount;
		}
    }

    env->CallNonvirtualVoidMethod(track, audioTrackClass, stopMethod);
    env->CallNonvirtualVoidMethod(track, audioTrackClass, releaseMethod);

    env->PopLocalFrame(NULL);
    jvm->DetachCurrentThread();
}


System* System::msInstance = 0;

System* System::initialize(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
    __android_log_write(ANDROID_LOG_DEBUG, "UntzJNI", "Initializing System");
	if(!msInstance)
    {
		msInstance = new System(sampleRate, numFrames, options);
        msInstance->mpData->mMixer.init();
    }
	return msInstance;
}

void System::shutdown()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
}

System* System::get()
{
    return msInstance;
}

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
    mpData = new AndroidSystemData();
    AndroidSystemData *d = (AndroidSystemData*)mpData;
    d->mSampleRate = sampleRate;
    d->mFramesPerBuffer = numFrames;
    d->mOptions = options;

    // set up audio thread.
    d->mpAudioThread = PlaybackThread::getInstance();
    // FIXME: set up audio thread.
    d->mpAudioThread->setData(d);
    d->mpAudioThread->start();
}

System::~System()
{
    AndroidSystemData *d = (AndroidSystemData *)mpData;
    d->mpAudioThread->deleteInstance();
    delete d;
}

UInt32 System::getSampleRate()
{
    AndroidSystemData *d = (AndroidSystemData *)mpData;
    return d->mSampleRate;
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
	__android_log_write(ANDROID_LOG_DEBUG, "UntzJNI", "suspending...");
}

void System::resume()
{
	msInstance->mpData->setActive(true);
	__android_log_write(ANDROID_LOG_DEBUG, "UntzJNI", "resuming...");
}
