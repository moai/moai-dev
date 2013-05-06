#include "Event.h"

#include "EventParameter.h"
#include "EventProperties.h"
#include "EventManager.h"
#include "LineCode.h"

using namespace FMODDesigner;

Event::Event() :
    m_pInternalData((InternalData)kINVALID_EVENT_INDEX),    
    m_rawSound(false)
{
}

Event::Event( const STLString& eventName ) :
    m_eventName(eventName),
    m_pInternalData((InternalData)kINVALID_EVENT_INDEX),    
    m_rawSound(false)
{
}

Event::Event( InternalData pData ) :    
    m_pInternalData(pData),
    m_rawSound(true)
{
}

float Event::GetDuration(const LineCode* pLineCode) const
{
    PROFILE_TIME_REGION_FAST(sg_SoundZone);

    if ( IsValid() )
    {        
        FMOD_RESULT result = FMOD_OK;
        if (pLineCode && *pLineCode != kINVALID_LINECODE && pLineCode->GetIndex() < g_WavbankEntries.size())
        {
            return g_WavbankEntries[pLineCode->GetIndex()].GetLength();            
        }

        if( !m_rawSound )
        {		    
            const EventProperties* pProperties = tEventManager.GetEventProperties( *this );        
            if( pProperties )
            {
                return pProperties->m_duration;
            }
        }
        else
        {
            FMOD::Sound* pSound = (FMOD::Sound*)m_pInternalData;
            u32 length = 0;
            result = pSound->getLength( &length, FMOD_TIMEUNIT_MS );
            if( result == FMOD_OK )
            {
                return length * 0.001f;                                
            }
        }

        HANDLE_FMOD_ERROR( result );        
    }
    return 0.f;
}

void Event::GetParameterIndex( EventParameter& param, const STLString& paramName ) const
{   
    PROFILE_TIME_REGION_FAST(sg_SoundZone);    

    assert( !m_rawSound );

    param.m_index = EventParameter::kINVALID_INDEX;

    if( IsValid() && !m_rawSound )
    {        
        const EventProperties* pProperties = tEventManager.GetEventProperties( *this );        
        if( pProperties )
        {
            param.m_index = EventParameter::kINVALID_INDEX;
            STLMap<STLString, int>::const_iterator it = pProperties->m_aParams.find( paramName );
            if( it != pProperties->m_aParams.end() )
            {
                param.m_index = it->second;
            }
        }
    }    
}

bool Event::IsValid() const
{
    PROFILE_TIME_REGION_FAST(sg_SoundZone);

    if( !m_rawSound )
    {
        assert( !m_rawSound );

        // This is a lazy setting of the cached ID
        if( (int)m_pInternalData == kINVALID_EVENT_INDEX &&
            m_eventName.length() > 0 &&
            s_pFMODEventSystem )             
        {            
            FMOD_RESULT result = FMOD_OK;

            FMOD::Event* pEvent = NULL;

            const EventProperties* pProperties = tEventManager.GetEventProperties( *this );            
            if( pProperties )
            {
                FMOD::EventProject* pProject = NULL;
                result = s_pFMODEventSystem->getProjectByIndex( pProperties->m_projectId, &pProject );
                if( result == FMOD_OK )
                {
                    result = pProject->getEventByProjectID( pProperties->m_id, FMOD_EVENT_INFOONLY, &pEvent );
                }
            }

            if( result == FMOD_OK &&
                pEvent )
            {
                int groupIndex = -1;
                char* pszName = NULL;
                FMOD_EVENT_INFO info;
                memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
                result = pEvent->getInfo(&groupIndex, &pszName, &info);
                if (result == FMOD_OK)
                {
                    m_pInternalData = (InternalData)info.systemid;
                }
            }

            HANDLE_FMOD_ERROR( result );     
        }        

        return  ( (int)m_pInternalData != kINVALID_EVENT_INDEX );
    }
    else
    {
        return m_pInternalData != NULL;
    }
}

void Event::DownsampleInto(vector<u8>& aDstBuffer, u32 targetFrequency) const
{
    if( m_rawSound )
    {
        FMOD::Sound* pSound = (FMOD::Sound*)m_pInternalData;

        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );
        int systemSampleRate = 0;
        result = pSystem->getSoftwareFormat( &systemSampleRate, NULL, NULL, NULL, NULL, NULL );

        FMOD_SOUND_TYPE soundType = FMOD_SOUND_TYPE_UNKNOWN;
        FMOD_SOUND_FORMAT soundFormat = FMOD_SOUND_FORMAT_NONE;
        result = pSound->getFormat(&soundType, &soundFormat, NULL, NULL);
        assert( soundType == FMOD_SOUND_TYPE_RAW && soundFormat == FMOD_SOUND_FORMAT_PCM16 );

        u32 length = 0;
        result = pSound->getLength( &length, FMOD_TIMEUNIT_PCM );        
        u8* pSrcBuffer = NULL;
        u32 len1, len2;
        result = pSound->lock(0, length * 2, (void**)&pSrcBuffer, NULL, &len1, &len2 );

        u32 skipRate = systemSampleRate / targetFrequency;
        aDstBuffer.clear();
        aDstBuffer.resize((length / skipRate) * 2);
        for( u32 i = 0; i < aDstBuffer.size(); i += 2 )
        {
            aDstBuffer[i] = pSrcBuffer[i * skipRate];
            aDstBuffer[i+1] = pSrcBuffer[i * skipRate + 1];
        }

        result = pSound->unlock(pSrcBuffer, NULL, len1, len2);
    }
    else
    {
        Trace(TT_Sound, TL_Error, "Can only downsample raw sounds.");
    }
}

void Event::CopyInto(vector<u8>& aDstBuffer) const
{
    if( m_rawSound )
    {
        FMOD::Sound* pSound = (FMOD::Sound*)m_pInternalData;

        FMOD_SOUND_TYPE soundType = FMOD_SOUND_TYPE_UNKNOWN;
        FMOD_SOUND_FORMAT soundFormat = FMOD_SOUND_FORMAT_NONE;
        FMOD_RESULT result = pSound->getFormat(&soundType, &soundFormat, NULL, NULL);
        assert( soundType == FMOD_SOUND_TYPE_RAW && soundFormat == FMOD_SOUND_FORMAT_PCM16 );

        u32 length = 0;
        result = pSound->getLength( &length, FMOD_TIMEUNIT_PCM );        
        u8* pSrcBuffer = NULL;
        u32 len1, len2;
        result = pSound->lock(0, length * 2, (void**)&pSrcBuffer, NULL, &len1, &len2 );

        aDstBuffer.clear();
        aDstBuffer.resize(length * 2);
        for( u32 i = 0; i < aDstBuffer.size(); i += 2 )
        {
            aDstBuffer[i] = pSrcBuffer[i];
            aDstBuffer[i+1] = pSrcBuffer[i + 1];
        }

        result = pSound->unlock(pSrcBuffer, NULL, len1, len2);
    }
    else
    {
        Trace(TT_Sound, TL_Error, "Can only copy into raw sounds.");
    }
}
