#include "EventInstance.h"

#include "EventManager.h"
#include "EventParameter.h"
#include "EventManager.h"

using namespace FMODDesigner;

EventInstance::EventInstance( InternalData pInternalData, const Event& sourceEvent, bool rawSound ) :
    m_pInternalData(pInternalData),    
    m_sourceEvent(sourceEvent),
    m_pVoiceSoundPointer(NULL), // Can only init one member of the union, so zero seems saftest
    m_uRetestMs(0),    
    m_flags(0)
{        
    SetFlag(kSIF_RawSound, rawSound);
    SetFlag(kSIF_Unloaded, true);
    m_vPosition.Init(0.f, 0.f, 0.f);
#if SUPPORT_FULL_3D_SOUNDS
    m_vVelocity.Init(0.f, 0.f, 0.f);
    m_vForward.Init(0.f, 0.f, 1.f);    
#endif
}

EventInstance::~EventInstance()
{
}

void EventInstance::ClearVoiceData()
{     
    assert(GetFlag(kSIF_ResidentVoice)); 
    m_pVoiceSoundPointer = NULL;
    SetFlag(kSIF_ResidentVoice, false);     
}

void EventInstance::StopInternal( bool noFadeOut )
{   
    tEventManager.UnDuckSoundCategories(m_hDuckingRequest);

    if (IsValid())
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {	        
            result = ((FMOD::Event*)m_pInternalData)->stop( noFadeOut );            
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            result = pChannel->stop();
        }

        HANDLE_FMOD_ERROR( result );
    }
    else
    {
        // Make sure that the sound system destroys this sound instance.
        SetFlag(kSIF_Loops, false);
    }
}

float EventInstance::GetDominantFrequency() const
{
    if (IsValid())
    {
        if (GetFlag(kSIF_RawSound))
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;

            const int kOUTPUT_RATE = 48000;
            const u32 kSPECTRUM_SIZE = 8192;
            const float kSPECTRUM_RANGE = ((float)kOUTPUT_RATE / 2.0f);      /* 0 to nyquist */
            const float kBIN_SIZE = ((float)kSPECTRUM_RANGE / (float)kSPECTRUM_SIZE);
            float spectrum[kSPECTRUM_SIZE];

            FMOD_RESULT result = pChannel->getSpectrum(spectrum, kSPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE); 
            if (result == FMOD_OK)
            {
                int	bin = 0;
                float max = 0;

                // XXX: We intentionally use kSPECTRUM_SIZE - 1 because for some reason we were getting 
                // junk data in the last slot (by junk data I mean a high value, causing us to select it).
                // This was yielding frequency values at the very top of the frequency range.
                for (int count = 0; count < kSPECTRUM_SIZE - 1; count++)
                {
                    if (spectrum[count] > 0.01f && spectrum[count] > max)
                    {
                        max = spectrum[count];
                        bin = count;
                    }
                }        

                float dominanthz  = (float)bin * kBIN_SIZE;       /* dominant frequency min */
                return dominanthz;
            }
            else 
            {
                Trace(TT_Sound, TL_Warning, "Couldn't get spectrum of sound channel: %s", GetName().c_str());
            }
        }
    }

    return -1.f;
}

float EventInstance::GetTempo() const
{    
    if (IsValid())
    {
        const float elapsedTime = GetTime( true );
        vector<float> aPoints;
        GetSyncPoints( aPoints, NULL, false );
        // 2 sync points might be just due to an inner loop window. We need lots of them!
        if( aPoints.size() > 2 )
        {
            int currentBeat = 0;
            for( u32 i = 0; i < aPoints.size() - 1; ++i )
            {
                if( elapsedTime > aPoints[i] && elapsedTime < aPoints[i+1] )
                {
                    currentBeat = i;
                    break;
                }
            }

            static int nBeatsToAverage = 8;
            float beatCounter = 0.f;
            float avgBeatDuration = 0.f;
            for( int i = Max(currentBeat - 1, 0); i < (int)aPoints.size() - 1 && i < currentBeat + nBeatsToAverage; ++i )
            {
                avgBeatDuration += aPoints[i+1] - aPoints[i];
                beatCounter += 1.f;
            }
            avgBeatDuration /= beatCounter;
            return 60.f / avgBeatDuration;
        } 
    }       

    return 0.f;
}

float EventInstance::GetBeatFraction() const
{   
    if (IsValid())
    {
        const float elapsedTime = GetTime(true);
        vector<float> aPoints;
        GetSyncPoints( aPoints, NULL, false );
        // 2 sync points might be just due to an inner loop window. We need lots of them!
        if( aPoints.size() > 2 )
        {
            for( u32 i = 0; i < aPoints.size() - 1; ++i )
            {
                if( elapsedTime > aPoints[i] && elapsedTime < aPoints[i+1] )
                {                
                    return (elapsedTime - aPoints[i]) / (aPoints[i+1] - aPoints[i]);                
                }
            }
        } 
    }    

    return 0.f;   
}

float EventInstance::GetMeasureFraction() const
{
    if (IsValid())
    {
        const float elapsedTime = GetTime(true);
        vector<float> aPoints;
        GetSyncPoints( aPoints, NULL, false );
        // 2 sync points might be just due to an inner loop window. We need lots of them!
        if( aPoints.size() > 2 )
        {
            for( u32 i = 0; aPoints.size() >= 4 && i < aPoints.size() - 4; i += 4 )
            {
                if( elapsedTime > aPoints[i] && elapsedTime < aPoints[i+4] )
                {
                    return (elapsedTime - aPoints[i]) / (aPoints[i+4] - aPoints[i]);
                }
            }
        }
    }    

    return 0.f;
}


void EventInstance::SetPositionInternal( const ZLVec3D& vPosition, const ZLVec3D& vVelocity, const ZLVec3D& vForward )
{       
    if (IsValid())
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD_VECTOR vFMPosition;
        CopyDFVectorToFMOD( vFMPosition, vPosition );
#if SUPPORT_FULL_3D_SOUNDS
        FMOD_VECTOR vFMVelocity;
        CopyDFVectorToFMOD( vFMVelocity, vVelocity );
        FMOD_VECTOR vFMForward;
        CopyDFVectorToFMOD( vFMForward, vForward );
#endif
        if( !GetFlag(kSIF_RawSound) )
        {
            FMOD::Event* pEvent = ((FMOD::Event*)m_pInternalData);
#if SUPPORT_FULL_3D_SOUNDS
            result = pEvent->set3DAttributes(&vFMPosition, &vFMVelocity, &vFMForward);
#else
            result = pEvent->set3DAttributes(&vFMPosition, NULL, NULL);
#endif
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
#if SUPPORT_FULL_3D_SOUNDS
            result = pChannel->set3DAttributes(&vFMPosition, &vFMVelocity);
            result = pChannel->set3DConeOrientation(&vFMForward);
#else
            result = pChannel->set3DAttributes(&vFMPosition, NULL);
#endif
        }        

        HANDLE_FMOD_ERROR( result );
    }    
}

void EventInstance::SetTime( float fTime )
{
    assert( fTime >= 0.f );
    SetTimeMs((u32)(fTime * 1000.f));
}

void EventInstance::SetTimeMs( u32 time )
{   
    if (IsValid())
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;        
        if( !GetFlag(kSIF_RawSound) )
        {
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {                
                vector<FMOD::Channel*> apChannels; // POTENTIAL OPTIMIZATION SPOT - used to be InPlaceArray<FMOD::Channel*, 8>
                GetEventParts( pChannelGroup, apChannels );
                for( u32 i = 0; i < apChannels.size(); ++i )
                {
                    FMOD::Channel* pChannel = apChannels[i];
                    if( pChannel )
                    {
                        result = pChannel->setPosition( time, FMOD_TIMEUNIT_MS );
                    }            
                }
            }          
        }
        else
        {            
            result = ((FMOD::Channel*)m_pInternalData)->setPosition( time, FMOD_TIMEUNIT_MS );
        }

        HANDLE_FMOD_ERROR( result );
    }
}

void EventInstance::SetDelayMs( u32 delay, bool start )
{
    if (IsValid())
    {    
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::System* pSystem = NULL;
        result = s_pFMODEventSystem->getSystemObject(&pSystem);
        if( result == FMOD_OK )
        {        
            vector<FMOD::Channel*> apChannels; // POTENTIAL OPTIMIZATION SPOT - used to be InPlaceArray<FMOD::Channel*, 8>
            if( !GetFlag(kSIF_RawSound) )
            {
                FMOD::ChannelGroup* pChannelGroup = NULL;
                result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
                if( result == FMOD_OK )
                {                
                    GetEventParts( pChannelGroup, apChannels );   
                }
            }
            else
            {
                apChannels.push_back((FMOD::Channel*)m_pInternalData);            
            }            

            int sampleRate = 0;
            pSystem->getSoftwareFormat(&sampleRate, NULL, NULL, NULL, NULL, NULL);
            sampleRate /= 1000;

            u32 dspHi = 0;
            u32 dspLo = 0;
            pSystem->getDSPClock(&dspHi, &dspLo);
            for( u32 i = 0; i < apChannels.size(); ++ i )
            {
                FMOD_64BIT_ADD(dspHi, dspLo, 0, delay * sampleRate);
                FMOD::Channel* pChannel = apChannels[i];
                if( pChannel )
                {                    
                    pChannel->setDelay(start ? FMOD_DELAYTYPE_DSPCLOCK_START : FMOD_DELAYTYPE_DSPCLOCK_END, dspHi, dspLo );
                }
            }
        }

        HANDLE_FMOD_ERROR(result);
    }
}

void EventInstance::SetPitch(float fPitch)
{
    if (IsValid())
    {    
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {	        
            result = ((FMOD::Event*)m_pInternalData)->setPitch(Log2(fPitch), FMOD_EVENT_PITCHUNITS_OCTAVES);
        }
        else
        {
            // I'm not sure what channel group this is returning, it may
            // not be unique to this channel...
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            FMOD::ChannelGroup* pGroup = NULL;
            result = pChannel->getChannelGroup( &pGroup );
            if( result == FMOD_OK )
            {
                result = pGroup->setPitch( fPitch );
            }
        }

        HANDLE_FMOD_ERROR( result );
    }    
}

float EventInstance::GetPitch() const
{
    float fPitch = 0.f;

    if (IsValid())
    {    
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {	        
            result = ((FMOD::Event*)m_pInternalData)->getPitch( &fPitch );
        }
        else
        {
            // I'm not sure what channel group this is returning, it may
            // not be unique to this channel...
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            FMOD::ChannelGroup* pGroup = NULL;
            result = pChannel->getChannelGroup( &pGroup );
            if( result == FMOD_OK )
            {
                result = pGroup->getPitch( &fPitch );
            }
        }

        HANDLE_FMOD_ERROR( result );
    }    

    return fPitch;
}

void EventInstance::SetVolume(float fVolume)
{
    assert( fVolume >= 0.f && fVolume <= 1.f );

    if (IsValid())
    {          
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {
            result = ((FMOD::Event*)m_pInternalData)->setVolume(fVolume);            
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            result = pChannel->setVolume(fVolume);
        }

        HANDLE_FMOD_ERROR( result );
    }    
}

float EventInstance::GetVolume() const
{
    float fVolume = 0.f;

    if (IsValid())
    {          
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {
            result = ((FMOD::Event*)m_pInternalData)->getVolume( &fVolume );            
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            result = pChannel->getVolume( &fVolume );
        }

        HANDLE_FMOD_ERROR( result );
    }    

    return fVolume;
}

void EventInstance::MuteInternal( bool bMute )
{
    if (IsValid())
    {    
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {            
            result = ((FMOD::Event*)m_pInternalData)->setMute(bMute);            
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            result = pChannel->setMute( bMute );
        }

        HANDLE_FMOD_ERROR( result );
    }        
}

void EventInstance::SetParameter(const EventParameter& param, float fValue)
{
    assert( !GetFlag(kSIF_RawSound) );

    if( IsValid() && param.IsValid() )
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::EventParameter *pParam = NULL;
        result = ((FMOD::Event*)m_pInternalData)->getParameterByIndex(param.GetIndex(), &pParam);
        if (result == FMOD_OK)
        {
            result = pParam->setValue(fValue);
        }

        HANDLE_FMOD_ERROR( result );
    }
}

float EventInstance::GetParameter(const EventParameter& param)
{
    assert( !GetFlag(kSIF_RawSound) );

    if( IsValid() && param.IsValid() )
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::EventParameter *pParam = NULL;
        result = ((FMOD::Event*)m_pInternalData)->getParameterByIndex(param.GetIndex(), &pParam);
        if (result == FMOD_OK)
        {
            float fValue = 0.f;
            result = pParam->getValue(&fValue);
            if (result == FMOD_OK)
            {
                return fValue;
            }
        }

        HANDLE_FMOD_ERROR( result );
    }
    else
    {
        Trace(TT_Sound, TL_Warning, "Attempted to set parameter on invalid sound event.");
    }
    return 0.f;
}

void EventInstance::KeyOff(const EventParameter& param)
{
    assert( !GetFlag(kSIF_RawSound) );

    if( IsValid() && param.IsValid() )
    {
        //make sure looping flag is cleared when we try to key off a 3-part sound
        SetLoops(false);

        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::EventParameter *pParam = NULL;
        result = ((FMOD::Event*)m_pInternalData)->getParameterByIndex(param.GetIndex(), &pParam);
        if (result == FMOD_OK)
        {
            result = pParam->keyOff();
        }

        HANDLE_FMOD_ERROR( result );
    }
}

// for determining the currently playing music, need the actual wave that's playing right now
bool EventInstance::GetCurrentlyPlayingWavName(STLString& wavName) const
{
    if (IsValid())
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::Sound* pSound = NULL;
        if( !GetFlag(kSIF_RawSound) )
        {            
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if (result == FMOD_OK)
            {
                vector<FMOD::Channel*> apChannels; // POTENTIAL OPTIMIZATION SPOT - used to be InPlaceArray<FMOD::Channel*, 8>    
                vector<FMOD::Sound*> apSounds; // POTENTIAL OPTIMIZATION SPOT - used to be InPlaceArray<FMOD::Sound*, 8>
                GetEventParts( pChannelGroup, apChannels, &apSounds );
                if (apSounds.size() > 0)
                {
                    // just return the first one
                    pSound = apSounds[0];                    
                }
            }
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;            
            result = pChannel->getCurrentSound( &pSound );            
        }

        if( pSound )
        {
            char szName[31];
            result = pSound->getName(szName, 31);
            if (result == FMOD_OK)
            {
                szName[30] = 0;
                wavName = szName;
                return true;
            }
        }

        HANDLE_FMOD_ERROR(result);
    }

    return false;
}

float EventInstance::GetDuration(bool subsoundTime) const
{
    if ( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {
            FMOD_EVENT_INFO info;
            memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
            u32 duration = 0;

            if( subsoundTime )
            {
                FMOD::ChannelGroup* pChannelGroup = NULL;
                result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
                if( result == FMOD_OK )
                {
                    vector<FMOD::Channel*> apChannels;                    
                    GetEventParts( pChannelGroup, apChannels );    
                    if( apChannels.size() > 0 )
                    {
                        if( apChannels.size() > 1 )
                        {
                            Trace(TT_Sound, TL_Warning, "GetDuration() called on a EventInstance with more than 1 sub-sound. May yield inaccurate results.");
                        }
                        FMOD::Sound* pSound = NULL;
                        result = apChannels[0]->getCurrentSound( &pSound );
                        if(result == FMOD_OK)
                        {
                            result = pSound->getLength( &duration, FMOD_TIMEUNIT_MS );
                        }
                    }
                    else subsoundTime = false;
                }
            }                
            if(!subsoundTime || result != FMOD_OK)
            {
                result = ((FMOD::Event*)m_pInternalData)->getInfo(NULL, NULL, &info);
                duration = info.lengthms;
            }
            if (result == FMOD_OK)
            {
                return duration * .001f;
            }
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            FMOD::Sound* pSound = NULL;
            result = pChannel->getCurrentSound( &pSound );
            if( result == FMOD_OK )
            {
                u32 duration = 0;
                result = pSound->getLength( &duration, FMOD_TIMEUNIT_MS );
                if( result == FMOD_OK )
                {
                    return duration * 0.001f;
                }
            }
        }

        HANDLE_FMOD_ERROR( result );
    }
    return 0.f;
}

void EventInstance::GetSyncPoints( vector<float>& aSyncPointTimes, vector<STLString>* pSyncPointLabels, bool ensureCompleteCoverage ) const
{
    if ( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        aSyncPointTimes.clear();
        if( pSyncPointLabels )
        {
            pSyncPointLabels->clear();
        }
        FMOD_RESULT result = FMOD_OK;
        FMOD::Sound* pSound = NULL;
        if( !GetFlag(kSIF_RawSound) )
        {
            FMOD::Event* pEvent = (FMOD::Event*)m_pInternalData;            
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = pEvent->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {
                vector<FMOD::Channel*> apChannels;
                vector<FMOD::Sound*> apSounds;
                GetEventParts( pChannelGroup, apChannels, &apSounds );
                if( apSounds.size() > 0 )
                {
                    if( apSounds.size() > 1 )
                    {                        
                        //Trace(TT_Sound, TL_Warning, "GetSyncPoints called on an Event which has more than one sound. This may cause unexpected results." );
                    }
                    for( u32 i = 0; i < apSounds.size(); ++i )
                    {
                        int nSyncPoints = 0;
                        result = apSounds[i]->getNumSyncPoints(&nSyncPoints);
                        if( nSyncPoints > 0 )
                        {
                            pSound = apSounds[i];
                            break;
                        }
                    }                    
                }
            }
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;            
            result = pChannel->getCurrentSound( &pSound );
        }

        if( pSound )
        {
            int nSyncPoints = 0;
            result = pSound->getNumSyncPoints(&nSyncPoints);
            if( result == FMOD_OK && nSyncPoints > 0 )
            {                

                for( int i = 0; i < nSyncPoints; ++i )
                {
                    FMOD_SYNCPOINT* pSyncPoint = NULL;
                    result = pSound->getSyncPoint( i, &pSyncPoint );
                    if( result == FMOD_OK )
                    {
                        u32 offset = 0;
                        const u32 kNameLen = 256;
                        char name[kNameLen];
                        pSound->getSyncPointInfo( pSyncPoint, pSyncPointLabels ? name : NULL, kNameLen, &offset, FMOD_TIMEUNIT_MS );
                        float fTime = offset * 0.001f;
                        if(aSyncPointTimes.size() == 0 || !Equal(aSyncPointTimes[aSyncPointTimes.size()-1], fTime, .01f))
                        {
                            aSyncPointTimes.push_back(fTime);
                            if(pSyncPointLabels)
                            {
                                pSyncPointLabels->push_back(STLString(name));
                            }
                        }
                        else
                        {
                            Trace(TT_Sound, TL_Info, "Removing duplicate timing data at %f.", fTime);
                        }
                    }
                }

                if( ensureCompleteCoverage )
                {   
                    if( !Equal( aSyncPointTimes[0], 0.f, EPSILON ) )
                    {
                        aSyncPointTimes.insert(aSyncPointTimes.begin(), 0.f);
                        if(pSyncPointLabels)
                        {
                            pSyncPointLabels->insert(pSyncPointLabels->begin(), STLString());
                        }
                    }  

                    if( !Equal( aSyncPointTimes[aSyncPointTimes.size()-1], GetDuration(), EPSILON ) )
                    {
                        aSyncPointTimes.push_back( GetDuration() + EPSILON );
                        if(pSyncPointLabels)
                        {
                            pSyncPointLabels->push_back(STLString());
                        }
                    }                    
                }
            }
        }

        HANDLE_FMOD_ERROR( result );        
    }
}

float EventInstance::GetTime(bool useSubsoundTime) const
{
    return GetTimeMs(useSubsoundTime) * 0.001f;
}

u32 EventInstance::GetTimeMs(bool subsoundTime) const
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {            
            if( subsoundTime )
            {
                FMOD::ChannelGroup* pChannelGroup = NULL;
                result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
                if( result == FMOD_OK )
                {
                    vector<FMOD::Channel*> apChannels;                    
                    GetEventParts( pChannelGroup, apChannels );    
                    if( apChannels.size() > 0 )
                    {
                        if( apChannels.size() > 1 )
                        {
                            Trace(TT_Sound, TL_Warning, "GetTime() called on a EventInstance with more than 1 sub-sound. May yield inaccurate results.");
                        }

                        u32 position = 0;
                        result = apChannels[0]->getPosition( &position, FMOD_TIMEUNIT_MS );
                        if( result == FMOD_OK )
                        {
                            return position;
                        }
                    }
                }
            }                

            // If we request subsound time, it might fail, so if nothing else
            // report the event time.
            FMOD_EVENT_INFO info;
            memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
            result = ((FMOD::Event*)m_pInternalData)->getInfo(NULL, NULL, &info);
            if( result == FMOD_OK )
            {
                return info.positionms;
            }
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            u32 position = 0;
            result = pChannel->getPosition( &position, FMOD_TIMEUNIT_MS );
            if( result == FMOD_OK )
            {
                return position;
            }
        }

        HANDLE_FMOD_ERROR( result );
    }

    return 0;
}

float EventInstance::GetElapsed(bool subsoundTime) const
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {            
            if( subsoundTime )
            {
                FMOD::ChannelGroup* pChannelGroup = NULL;
                result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
                if( result == FMOD_OK )
                {
                    vector<FMOD::Channel*> apChannels;                    
                    GetEventParts( pChannelGroup, apChannels);    
                    if( apChannels.size() > 0 )
                    {
                        if( apChannels.size() > 1 )
                        {
                            Trace(TT_Sound, TL_Warning, "GetElapsed() called on a EventInstance with more than 1 sub-sound. May yield inaccurate results.");
                        }

                        FMOD::Sound *pSound = NULL;
                        result = apChannels[0]->getCurrentSound( &pSound );
                        if (result == FMOD_OK && pSound != NULL)
                        {
                            u32 duration = 0;
                            result = pSound->getLength(&duration, FMOD_TIMEUNIT_MS);
                            if (result == FMOD_OK)
                            {
                                u32 position = 0;
                                result = apChannels[0]->getPosition( &position, FMOD_TIMEUNIT_MS );
                                if( result == FMOD_OK )
                                {
                                    return (position * .001f) / (duration * .001f);
                                }
                            }
                        }
                    }
                }
            }                

            // If we request subsound time, it might fail, so if nothing else
            // report the event time.
            FMOD_EVENT_INFO info;
            memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
            result = ((FMOD::Event*)m_pInternalData)->getInfo(NULL, NULL, &info);
            if( result == FMOD_OK )
            {
                if (info.lengthms == -1)
                {
                    return -1;
                }

                return (info.positionms * .001f) / (info.lengthms * .001f);
            }
        }
        else
        {
            FMOD::Channel *pChannel = (FMOD::Channel*)m_pInternalData;
            u32 position = 0;
            result = pChannel->getPosition( &position, FMOD_TIMEUNIT_MS );
            if( result == FMOD_OK )
            {
                FMOD::Sound *pSound;
                result = pChannel->getCurrentSound(&pSound);
                if (result == FMOD_OK)
                {
                    u32 duration = 0;
                    result = pSound->getLength(&duration, FMOD_TIMEUNIT_MS);

                    if (result == FMOD_OK)
                    {
                        return (position * .001f) / (duration * .001f);
                    }
                }
            }
        }

        HANDLE_FMOD_ERROR( result );
    }

    return -1.f;
}


int EventInstance::GetNumChannels() const
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        if( !GetFlag(kSIF_RawSound) )
        {
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {
                vector<FMOD::Channel*> apChannels;                
                GetEventParts( pChannelGroup, apChannels ); 
                return apChannels.size();
            }            
        }
        else
        {
            return 1;
        }

        HANDLE_FMOD_ERROR( result );
    }

    return -1;
}

void EventInstance::SetSpeakerMix( float fLeft, float fRight, float fCenter, float fSub, float fSurroudLeft, float fSurroundRight )
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        vector<FMOD::Channel*> apChannels;
        if( !GetFlag(kSIF_RawSound) )
        {      
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {                                
                GetEventParts( pChannelGroup, apChannels );                
            }
        }
        else
        {            
            apChannels.push_back((FMOD::Channel*)m_pInternalData);            
        }

        float aLeftLevels[6] = { fLeft, 0.f, fCenter, fSub, fSurroudLeft, 0.f };
        float aRightLevels[6] = { 0.f, fRight, fCenter, fSub, 0.f, fSurroundRight };
        for( u32 i = 0; i < apChannels.size(); ++i )
        {
            result = apChannels[i]->setSpeakerMix(1.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f); 
            result = apChannels[i]->setSpeakerLevels(FMOD_SPEAKER_FRONT_LEFT, aLeftLevels, 6); 
            result = apChannels[i]->setSpeakerLevels(FMOD_SPEAKER_FRONT_RIGHT, aRightLevels, 6); 
        }

        HANDLE_FMOD_ERROR( result );
    }
}

void EventInstance::SetLoopPoints( float fStartTime, float fEndTime )
{
    assert( fStartTime >= 0.f );
    assert( fEndTime >= 0.f || fEndTime == -1.f );

    if( IsValid() )
    {
        PROFILE_TIME_REGION_FAST(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        vector<FMOD::Channel*> apChannels;
        vector<FMOD::Sound*> apSounds;
        if( !GetFlag(kSIF_RawSound) )
        {    
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {                
                GetEventParts( pChannelGroup, apChannels, &apSounds );                
            }
        }
        else
        {
            FMOD::Channel* pChannel = (FMOD::Channel*)m_pInternalData;
            FMOD::Sound* pSound = NULL;
            result = pChannel->getCurrentSound( &pSound );
            apChannels.push_back(pChannel);
            apSounds.push_back(pSound);
        }

        assert( apChannels.size() == apSounds.size() );
        for( u32 i = 0; i < apChannels.size(); ++i )
        {               
            u32 startTime = (u32)( fStartTime * 1000.f );
            u32 length = 0;
            apSounds[i]->getLength( &length, FMOD_TIMEUNIT_MS );
            u32 endTime = fEndTime == -1.f ? length - 1 : (u32)( fEndTime * 1000.f );
            result = apChannels[i]->setLoopPoints( startTime, FMOD_TIMEUNIT_MS, endTime, FMOD_TIMEUNIT_MS );
        }

        HANDLE_FMOD_ERROR( result );
    }
}

void EventInstance::Pause(bool pause)
{
    if( IsValid() )
    {
        FMOD_RESULT result = FMOD_OK;
        if( GetFlag(kSIF_RawSound) )
        {
            result = ((FMOD::Channel*)m_pInternalData)->setPaused(pause);
        }
        else
        {
            result = ((FMOD::Event*)m_pInternalData)->setPaused(pause);
        }
        HANDLE_FMOD_ERROR( result );
    }
}

void EventInstance::Set3DOcclussion( float fOcclussion )
{
    if( IsValid() )
    {
        FMOD_RESULT result = FMOD_OK;
        if( GetFlag(kSIF_RawSound) )
        {
            result = ((FMOD::Channel*)m_pInternalData)->set3DOcclusion( fOcclussion, 0.f );
        }
        else
        {
            result = ((FMOD::Event*)m_pInternalData)->set3DOcclusion( fOcclussion, 0.f );
        }
        HANDLE_FMOD_ERROR( result );
    }
}

void EventInstance::Set3DPanLevel( float fPanLevel )
{
    if( IsValid() )
    {
        FMOD_RESULT result = FMOD_OK;
        if( GetFlag(kSIF_RawSound) )
        {
            result = ((FMOD::Channel*)m_pInternalData)->set3DPanLevel( fPanLevel );
        }
        else
        {
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {
                vector<FMOD::Channel*> aChannels;
                GetEventParts( pChannelGroup, aChannels );
                for( u32 i = 0; i < aChannels.size(); ++i )
                {
                    FMOD::Channel* pChannel = aChannels[i];
                    if( pChannel )
                    {
                        result = ((FMOD::Channel*)m_pInternalData)->set3DPanLevel( fPanLevel );
                    }
                }
            }            
        }
        HANDLE_FMOD_ERROR( result );
    }
}

bool EventInstance::IsLoading() const
{
    if( IsValid() )
    {
        FMOD_RESULT result = FMOD_OK;
        if( GetFlag(kSIF_RawSound) )
        {
            FMOD::Sound* pSound = NULL;
            result = ((FMOD::Channel*)m_pInternalData)->getCurrentSound(&pSound);
            if( result == FMOD_OK )
            {
                return IsSoundLoading(pSound);
            }
        }
        else
        {
            FMOD::ChannelGroup* pChannelGroup = NULL;
            result = ((FMOD::Event*)m_pInternalData)->getChannelGroup( &pChannelGroup );
            if( result == FMOD_OK )
            {
                vector<FMOD::Channel*> aChannels;
                vector<FMOD::Sound*> aSounds;                
                GetEventParts(pChannelGroup, aChannels, &aSounds);
                for( u32 i = 0; i < aSounds.size(); ++i )
                {
                    if( IsSoundLoading( aSounds[i] ) )
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}