#include "EventManager.h"

#include "SoundDSP.h"
#include "EventInstance.h"
#include "ReverbInstance.h"
#include "ReverbHandle.h"
#include "EventParameter.h"
#include "EventProperties.h"
#include "DuckingRequest.h"
#include "VoiceLRU.h"
#include <algorithm>
#include <cmath>

using namespace FMODDesigner;

#ifdef _DEBUG
    #include <fmod_event_net.hpp>
#endif

#ifdef MOAI_OS_IPHONE
    #include <fmodiphone.h>
#endif

#ifdef MOAI_OS_LINUX
    #include <stdint.h>
#endif

#ifdef MOAI_OS_WINDOWS
	#include <windows.h>
#endif

// We don't need to update more than every 30th of a second or so
float FMODDesigner::g_AudioUpdateInterval = (1.f/30.f);

// how long to wait to unload a soundgroup
const float kSOUNDGROUP_UNLOAD_DELAY = 5.f;

FMODDesigner::EventManager FMODDesigner::tEventManager;

// Irritatingly enough, FMOD_OS_GetMs is not an exported symbol;
// OSX moai uses dynamic linking, so we can't just sneakily use it.
u32 FMODDesigner::GetTimeMs()
{
    return (u32)(ZLDeviceTime::GetTimeInSeconds() * 1000.0);
}

//----------------------------------------------------------------------------------------
// FMOD Sound System Globals
//----------------------------------------------------------------------------------------

static const u32 kMAX_VOICE_LINES = MAX_STREAMS_PER_BANK; // Doesn't have to be the same, just a decent baseline.

FMOD::EventSystem*       FMODDesigner::s_pFMODEventSystem = NULL;
static vector<FMOD::DSP*> s_aHighPassFilters;
static vector<FMOD::DSP*> s_aLowPassFilters;
static void*             s_pMainMemPool = NULL;

static u32	             s_uTotalMemSize = 0;
static u32	             s_uMainMemSize = 0;

static STLMap<STLString, int>     s_ReverbIDTable;


//----------------------------------------------------------------------------------------
// Wavbank Data
//----------------------------------------------------------------------------------------

vector<FMODDesigner::WavbankEntry> FMODDesigner::g_WavbankEntries;
vector<STLString> FMODDesigner::g_WavbankNames;
STLMap<STLString, int> FMODDesigner::g_ProjectIDs;

bool EventManager::Init(const SoundInitParams& initParams)
{
	if (m_enabled)
	{
		return false;
	}

    m_initParams = initParams;
    
    if( m_initParams.m_enableSoundSystem )
    {        
        _InitInternals();        
    }
    return m_enabled;
}

void EventManager::Shutdown() 
{  
    if (m_enabled)
    {
        _ShutdownInternals();

        for( u32 i = 0; i < m_aEventInstances.size(); ++i )
        {
            m_handleFactory.Free( m_aEventInstances[i]->m_myHandle );
            delete m_aEventInstances[i];
        }
        m_aEventInstances.clear();

        for( SoundCategoryTable::iterator it = m_apSoundCategories.begin(); it != m_apSoundCategories.end(); ++it )
        {
            delete it->second;
        }
        m_apSoundCategories.clear();

		for( u32 i = 0; i < m_aReverbInstances.size(); ++i )
		{
			m_ReverbHandleFactory.Free( m_aReverbInstances[i]->m_myHandle );
			delete m_aReverbInstances[i];
		}
		m_aReverbInstances.clear();

    }
}

EventManager::EventManager() :   
    m_nextDuckingRequestHandle(0),    
    m_distantLowpassRange(0.f,0.f),
    m_maxDistantOcclussion(0.f),
    m_near2DBlendRange(0.f,0.f),
    m_near2DBlendLevel(0.f),
    m_timeUntilNextUpdate(0.f),
    m_enabled(false),
    m_timeSinceLastStateChange(0.f),
    m_timeSinceLastMusicSetChange(0.f),
    m_timeUntilNextMusicLoop(0.f),
    m_musicDelay(0.f),
    m_musicIsAmbient(false),
    m_iNumActiveVoices(0)
{
    // of: Why are people using GetPtr? This causes a lot of issues when projects get loaded and unloaded
    //     Work around the problem by reserving enough entries
    m_aGroups.get_allocator().allocate(512);
    m_aEventProperties.get_allocator().allocate(1024);

    m_vListenerPosition.Init(0.f, 0.f, 0.f);
    m_vListenerVelocity.Init(0.f, 0.f, 0.f);
    m_vListenerForward.Init(0.f, 0.f, 1.f);
    m_vListenerUp.Init(0.f, 1.f, 0.f); 
    m_vListenerRight.Init(-1.f,0.f,0.f);
}

EventManager::~EventManager()
{    
}

void EventManager::SetMicrophoneTransform(const ZLVec3D& vPos, const ZLVec3D& vVelocity, const ZLVec3D& vLook, const ZLVec3D& vUp, const ZLVec3D& vRight)
{    
    m_vListenerPosition = vPos;
    m_vListenerVelocity = vVelocity;
    m_vListenerForward = vLook;
    m_vListenerUp = vUp;
    m_vListenerRight = vRight;

	if (m_enabled)
	{
		_SetMicrophoneTransform(vPos, vVelocity, vLook, vUp);
	}
}

void EventManager::PauseSoundCategory(const STLString& category, bool bPause)
{
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory && 
        pSoundCategory->m_bPaused != bPause )
    {
        pSoundCategory->m_bPaused = bPause;    
        _PauseSoundCategory( pSoundCategory, bPause );
    }
}

bool EventManager::IsSoundCategoryPaused(const STLString& category) const
{
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory )
    {
        return pSoundCategory->m_bPaused;
    }
    else
    {
        return false;
    }
}

void EventManager::SetSoundCategoryVolume(const STLString& category, float fVolume)
{
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory &&
        pSoundCategory->m_fVolume != fVolume )
    {    
        pSoundCategory->m_fVolume = fVolume;
        _UpdateSoundCategoryVolume( pSoundCategory );
    }
}

float EventManager::GetSoundCategoryVolume(const STLString& category) const
{
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory )
    {
        return pSoundCategory->m_fVolume;
    }
    else
    {
        return 0.f;
    }    
}

void EventManager::DuckSoundCategories(DuckingRequestHandle& handle, const vector<STLString>& aCategories, const vector<float>& aVolumes, float duration)
{
    // check for ducking request made without releasing the previous request.
    assert(handle.m_id == DuckingRequestHandle::kINVALID_HANDLE );

	// Ensure that we have created entries in the m_aSubCategories structure.
	for( u32 i = 0; i < aCategories.size(); ++i )
	{
		_GetSoundCategoryState( aCategories[i] );
	}
    m_aPendingDuckingRequests.push_back( DuckingRequest( aCategories, aVolumes, duration, m_nextDuckingRequestHandle ) );
    handle.m_id = m_nextDuckingRequestHandle++;
}

void EventManager::ChangeDuckVolumes(DuckingRequestHandle& handle, const vector<float>& aVolumes)
{
    if( handle.m_id != DuckingRequestHandle::kINVALID_HANDLE )
    {
        for( DuckingRequestList::iterator it = m_aPendingDuckingRequests.begin(); it != m_aPendingDuckingRequests.end(); ++it )
        {
            if( it->m_handleIndex == handle.m_id )
            {
                assert(it->m_aVolumes.size() == aVolumes.size());
                it->m_aVolumes = aVolumes;
                break;
            }
        }        
    }
}

void EventManager::UnDuckSoundCategories(DuckingRequestHandle& handle)
{
    if( handle.m_id != DuckingRequestHandle::kINVALID_HANDLE )
    {
        for( DuckingRequestList::iterator it = m_aPendingDuckingRequests.begin(); it != m_aPendingDuckingRequests.end(); ++it )
        {
            if( it->m_handleIndex == handle.m_id )
            {
                m_aPendingDuckingRequests.erase( it );
                break;
            }
        }
        handle.m_id = DuckingRequestHandle::kINVALID_HANDLE;
    }
}

void EventManager::MuteSoundCategory(const STLString& category, bool bMute)
{    
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory &&
        pSoundCategory->m_bMuted != bMute )
    {
        pSoundCategory->m_bMuted = bMute;    
        _MuteSoundCategory( pSoundCategory, bMute );    
    }
}

bool EventManager::IsSoundCategoryMuted(const STLString& category) const
{
    SoundCategoryState* pSoundCategory = _GetSoundCategoryState(category);
    if( pSoundCategory )
    {
        return pSoundCategory->m_bMuted;
    }
    else
    {
        return false;
    }    
}

bool EventManager::LoadGroup(const STLString& groupPath, bool bPersistant, bool blockOnLoad)
{    
    if( groupPath.length() > 0 )
    {        
        GroupData* pGroup = _GetGroupData( groupPath );            
        if( pGroup )
        {
            vector<GroupData>::iterator it = find( m_aPendingUnloads.begin(), m_aPendingUnloads.end(), *pGroup);                        
            if ( it != m_aPendingUnloads.end() )
            {
                m_aPendingUnloads.erase( it );
                Trace(TT_Sound, TL_Info, "Load soundgroup %s while it was pending unload", pGroup->m_path.c_str());
            }
            else if( pGroup->m_refCount == 0 )
            {
                _LoadGroup( pGroup, blockOnLoad );
            }
            ++pGroup->m_refCount;

            return true;
        }
        else if( m_enabled )
        {
            Trace(TT_Sound, TL_Warning, "Tried to load non-extant sound group %s", groupPath.c_str());
        }
    }    

    return false;
}

bool EventManager::UnloadGroup(const STLString& groupPath, bool bUnloadImmediately)
{    
    if( groupPath.length() > 0 )
    {        
        GroupData* pGroup = _GetGroupData( groupPath );  
        if( pGroup )
        {
            if( pGroup->m_refCount > 0 )
            {            
                --pGroup->m_refCount;
                if( pGroup->m_refCount == 0 )
                {         
                    if( bUnloadImmediately && _UnloadGroup( pGroup, false ) )
                    {                    
                        return true;
                    }
                    else
                    {
                        Trace(TT_Sound, TL_Info, "Adding %s to PendingUnloads list", groupPath.c_str());

                        // add it to pending unloads and set the time until unload
                        m_aPendingUnloads.push_back( *pGroup );
                        int iNewIndex = m_aPendingUnloads.size() - 1;
                        // the time defaults to 0, so if we are supposed to unload immediately
                        // but the unload failed, don't set this
                        if (!bUnloadImmediately)
                        {
                            m_aPendingUnloads[iNewIndex].m_fTimeUntilUnload = kSOUNDGROUP_UNLOAD_DELAY;
                        }

                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                Trace( TT_Sound, TL_Warning, "Called UnloadGroup on %s without matching LoadGroup. This is very likely an audio memory leak.", groupPath.c_str() );
                return true;
            }
        }                    
        else if( m_enabled )
        {
            Trace(TT_Sound, TL_Warning, "Tried to unload non-extant sound group %s", groupPath.c_str());
        }
    }    

    return true;
}

void EventManager::UnloadPendingUnloads(bool bBlockOnUnload)
{
    // we're going to TRY to unload everything but it might fail, we don't want to block
    vector<GroupData>::iterator unloadIter = m_aPendingUnloads.begin();
    while( unloadIter != m_aPendingUnloads.end() )
    {
        GroupData& rGroup = *unloadIter;

        // here we're reseting the unload time to 0 so that we'll keep trying to unload it
        // once gameplay resumes
        rGroup.m_fTimeUntilUnload = 0.f;

        if (_UnloadGroup( &rGroup, bBlockOnUnload ))
        {
            unloadIter = m_aPendingUnloads.erase( unloadIter );
        }
        else
        {
            ++unloadIter;
        }
    }

}


EventHandle EventManager::PlayEvent2D(const Event& soundEvent, bool loopSound, const LineCode* pLineCode)
{
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        if( soundEvent.IsValid() )
        {
            Trace(TT_Sound, TL_Spam, "Playing sound: %s", soundEvent.GetName().c_str() );                
            EventInstance* pInstance = _PlayEvent( soundEvent, loopSound, NULL, NULL, pLineCode );
            if( pInstance )
            {
                if(pInstance->GetFlag(EventInstance::kSIF_3D))
                {
                    Trace(TT_Sound, TL_Warning, "The FMOD Event %s is 3D, but the game is trying to play it as 2D.", soundEvent.GetName().c_str());
                }
                pInstance->m_myHandle = m_handleFactory.Alloc(pInstance);
                m_aEventInstances.push_back( pInstance );                
                return pInstance->m_myHandle;
            }
            else
            {
                Trace(TT_Sound, TL_Info, "Failed to play 2D sound: %s", soundEvent.GetName().c_str() );
            }
        }        
    }    
    return EventHandle();    
}

EventHandle EventManager::PlayEvent3D(const Event& soundEvent, const ZLVec3D& vPos, bool loopSound, const ZLVec3D& vVelocity, const LineCode* pLineCode)
{   
    if( soundEvent.IsValid() )
    {   
        PROFILE_TIME_REGION(sg_SoundZone);
        Trace(TT_Sound, TL_Spam, "Playing 3D sound: %s", soundEvent.GetName().c_str() );        
        EventInstance* pNewInstance = _PlayEvent(soundEvent, loopSound, &vPos, NULL, pLineCode);
        if( pNewInstance )
        {            
            if(!pNewInstance->GetFlag(EventInstance::kSIF_3D))
            {                        
                Trace(TT_Sound, TL_Info, "The FMOD Event %s is 2D, but the game is trying to play it as 3D.", soundEvent.GetName().c_str());
            }
            pNewInstance->SetPosition(vPos, vVelocity);            
            pNewInstance->m_myHandle = m_handleFactory.Alloc( pNewInstance );            
            m_aEventInstances.push_back( pNewInstance );            
            return pNewInstance->m_myHandle;
        }
        else
        {
            Trace(TT_Sound, TL_Info, "Failed to play 3D sound: %s", soundEvent.GetName().c_str() );
        }        
    }

    return EventHandle();        
}

void EventManager::StopAllEvents()
{
    for( u32 i = 0; i < m_aEventInstances.size(); ++i )
    {
        EventInstance* pInstance = m_aEventInstances[i];
        pInstance->Stop(true);
    }

    m_voiceLRU.ReleaseAllSound();
    m_iNumActiveVoices = 0;
}

void EventManager::MuteAllEvents(bool bSetting)
{
    for( u32 i = 0; i < m_aEventInstances.size(); ++i )
    {
        EventInstance* pInstance = m_aEventInstances[i];
        pInstance->Mute(bSetting);
    }
}

/* decrement the current voice count */
void EventManager::DecrementCurrentlyPlayingVoices()
{    
    // underflow check here rather than an assert because some order of operations
    // issues can cause this number to be set to 0 before sounds are totally stopped.    
    if( m_iNumActiveVoices > 0 )
    {
        --m_iNumActiveVoices;
    }
}


const EventProperties* EventManager::GetEventProperties(const Event& soundEvent) const
{
    // check first if we have it in the cache
    EventProperties* pEventProperties = NULL;
    if( m_aEventProperties.contains( soundEvent.GetName() ) )
    {
        pEventProperties = &m_aEventProperties[soundEvent.GetName()];
    }

    // if not, then try to load its properties and cache them
    if( !pEventProperties )
    {   
        EventProperties temp;
        if( _LoadSoundPropertyData( soundEvent, &temp ) )
        {                
            m_aEventProperties.insert( pair<STLString, EventProperties>( soundEvent.GetName(), temp ) );    
            // pass back a non-temp version
            assert( m_aEventProperties.contains( soundEvent.GetName() ) );
            EventProperties liveproperties = m_aEventProperties[soundEvent.GetName()];
            pEventProperties = &liveproperties;            
        }            
    }

    return pEventProperties;
}

EventManager::GroupData* EventManager::_GetGroupData(const STLString& groupPath) const
{
    // check first if we have it in the cache
    GroupData* pGroupData = NULL;
    if( m_aGroups.contains( groupPath ) )
    {
        pGroupData = &m_aGroups[groupPath];        
    }

    // if not, then try to load the group data and cache it
    if( !pGroupData )
    {        
        GroupData temp;

        if( _LoadGroupData( groupPath, &temp ) )
        {                
            m_aGroups.insert( pair<STLString, GroupData>( groupPath, temp ) );
            // pass back a non-temp version
            assert( m_aGroups.contains( groupPath ) );
            pGroupData = &m_aGroups[groupPath];              
        }
    }

    return pGroupData;
}

void EventManager::Update(float fDeltaTime)
{
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_SoundZone);

        // we update this every frame
        // we want to use the game dt
        // and it isn't doing anything too expensive, hopefully
        _UpdateUnloads(fDeltaTime);
        m_voiceLRU.Update();

        m_timeUntilNextUpdate -= fDeltaTime;
        if( m_timeUntilNextUpdate < 0.f )
        {
            float fActualInterval = g_AudioUpdateInterval - m_timeUntilNextUpdate;

            m_timeUntilNextUpdate = g_AudioUpdateInterval;

            _UpdateDuckingRequests(fActualInterval);                
            _UpdateEventInstances(fActualInterval);
            _UpdateMinRetriggerTimers(fActualInterval);        
            _UpdateReverbInstances(fActualInterval);
            _InternalUpdate(fActualInterval);
        }
    }    
}

void EventManager::_UpdateUnloads(float fDeltaTime)
{
    // Try to unload any pending groups
    vector<GroupData>::iterator unloadIter = m_aPendingUnloads.begin();
    while( unloadIter != m_aPendingUnloads.end() )
    {
        GroupData& rGroup = *unloadIter;
        rGroup.m_fTimeUntilUnload -= fDeltaTime;

        if ( rGroup.m_fTimeUntilUnload <= 0.f && _UnloadGroup( &rGroup, false ) )
        {
            Trace(TT_Sound, TL_Info, "Removing %s from PendingUnloads list", rGroup.m_path.c_str());

            unloadIter = m_aPendingUnloads.erase( unloadIter );
        }
        else
        {
            ++unloadIter;
        }
    }
}

void EventManager::_UpdateDuckingRequests(float fDeltaTime)
{    
    // Should probably check for things being paused here...
    STLMap<STLString, float> duckedCategories;   

    DuckingRequestList::iterator requestIter = m_aPendingDuckingRequests.begin();
    while( requestIter != m_aPendingDuckingRequests.end() )
    {
        DuckingRequest& rCurrentRequest = *requestIter;
        const bool doesNotExpire = rCurrentRequest.m_remainingTime == -1.f;
        if( rCurrentRequest.m_remainingTime > 0.f || doesNotExpire )
        {
            for( u32 i = 0; i < rCurrentRequest.m_aCategories.size(); ++i )
            {
                STLString category = rCurrentRequest.m_aCategories[i];
                float prevVolume = FLT_MAX;
                if( duckedCategories.contains( category ) )
                {
                    prevVolume = duckedCategories[category];
                }
                
                if( rCurrentRequest.m_aVolumes[i] < prevVolume )
                {
                    duckedCategories.insert( pair<STLString, float>( category, rCurrentRequest.m_aVolumes[i] ) );
                }
            }

            if( !doesNotExpire )
            {
                rCurrentRequest.m_remainingTime -= fDeltaTime;
            }

            ++requestIter;
        }
        else
        {
            requestIter = m_aPendingDuckingRequests.erase( requestIter );
        }
    }	

    for( SoundCategoryTable::iterator soundCategoryIter = m_apSoundCategories.begin(); soundCategoryIter != m_apSoundCategories.end(); ++soundCategoryIter )
    {
        SoundCategoryState* pCategoryState = soundCategoryIter->second;
        float goalDuckVolume = 1.f;
        if( duckedCategories.contains( soundCategoryIter->first ) )
        {
            goalDuckVolume = duckedCategories[soundCategoryIter->first];
        }            
            
        float deltaDuckVolume = goalDuckVolume - pCategoryState->m_fDuckedVolume;
        if( std::abs( deltaDuckVolume ) > EPSILON )
        {
            const float maxDuckVelocity = 2.5f;
            const float maxDuckVolumeChange = fDeltaTime * maxDuckVelocity;
            pCategoryState->m_fDuckedVolume += Pin( deltaDuckVolume, -maxDuckVolumeChange, maxDuckVolumeChange );
            pCategoryState->m_fDuckedVolume = Pin( pCategoryState->m_fDuckedVolume, 0.f, 1.f );
            _UpdateSoundCategoryVolume( pCategoryState );            
        }
    }
}

void EventManager::_UpdateEventInstances(float fDeltaTime)
{
	const u32 uDeltaMs = (u32)(fDeltaTime * 1000.f);

    vector<EventInstance*>::iterator it = m_aEventInstances.begin();
    while( it != m_aEventInstances.end() )
    {
		// Branchless min of (curIndex + 8, maxIndex)
		const int curIndex = it - m_aEventInstances.begin();
		const int maxIndex = m_aEventInstances.size()-1;
		const int mask = ((curIndex+8) - maxIndex) >> 31;
		const u32 nextIndex = (mask & (curIndex+8)) | (~mask & maxIndex);

        EventInstance *pInstance = m_aEventInstances[curIndex];

        // NULL intances in the array mean that something has gone terribly, horribly wrong.
        assert( pInstance );

        // For sounds that have unloadOnSilence set, we'll try to unload them here.
        if( pInstance->m_pInternalData == NULL &&
			pInstance->TestFlagMask(EventInstance::kSIF_UnloadOnSilence|EventInstance::kSIF_Unloaded, EventInstance::kSIF_UnloadOnSilence))
        {
            // Only unload when we are the last instance of the sound.
            // Presumably the other instances will also have this flag set.
            if( tEventManager.GetNumInstances( pInstance->m_sourceEvent ) == 0 )
            {
                pInstance->SetFlag(EventInstance::kSIF_Unloaded, tEventManager.UnloadEvent( pInstance->m_sourceEvent, false ));
            }
        }

        if( (uintptr_t)pInstance->m_pInternalData |
            pInstance->GetFlags(EventInstance::kSIF_Loops | EventInstance::kSIF_Delayed) )
        {
            if( pInstance->m_uRetestMs > uDeltaMs )
            {
                pInstance->m_uRetestMs -= uDeltaMs;
            }            
            else
			{
				// Update 3D sound instances
				// Mostly to see if any have become (in)active due to retrigger radius or
				// cutoff radius restrictions.
				if( pInstance->GetFlag(EventInstance::kSIF_3D) )
				{                                   
					pInstance->SetFlag(EventInstance::kSIF_Delayed, false);

					if( !pInstance->m_pInternalData )
					{
						// We pass the existing instance in so that any external handles to this sound
						// stay copasetic.
						_PlayEvent( pInstance->m_sourceEvent, pInstance->GetFlag(EventInstance::kSIF_Loops), &(pInstance->m_vPosition), pInstance, NULL );
						if( pInstance->m_pInternalData )
						{
#if SUPPORT_FULL_3D_SOUNDS
							pInstance->SetPosition( pInstance->m_vPosition, pInstance->m_vVelocity, pInstance->m_vForward );
#else
							pInstance->SetPosition( pInstance->m_vPosition );
#endif
						}
					}
					else if(pInstance->GetFlags(EventInstance::kSIF_HasCutoffRadius | EventInstance::kSIF_HasRetriggerRadius))
					{
						// The source might be head relative or world space relative,
						// so we need to be sure that we get the proper world space transform.
						ZLVec3D vSourcePosition = _GetWorldspacePosition(pInstance->m_vPosition, pInstance->GetFlag(EventInstance::kSIF_HeadRelative));

						// Compute the squared distance to the listener.
						// That's sufficient for most of the calcs in this sequence.
						float listenerDistanceSqr = m_vListenerPosition.DistSqrd( vSourcePosition );

						// This will track if we failed any of our distance based tests.
						// When that happens, the sound is shut down.
						bool passedDistanceTests = true;

						const EventProperties* pProperties = GetEventProperties( pInstance->m_sourceEvent );

						// Check the cutoff radius,
						// which is the max radius at which this sound can be heard.
						if( pInstance->GetFlag(EventInstance::kSIF_HasCutoffRadius) &&
							listenerDistanceSqr > Square( pProperties->m_cutoffRadius ) )
						{
							passedDistanceTests = false;
						}
						// Check the retrigger radius,
						// which is the min distance between this event and other events of the same type.
						else if( pInstance->GetFlag(EventInstance::kSIF_HasRetriggerRadius) &&
							!pInstance->GetFlag(EventInstance::kSIF_RetriggerDisabled) &&
							!_PassesRetriggerTest( vSourcePosition, *pProperties, pInstance->m_sourceEvent, pInstance ) )
						{
							passedDistanceTests = false;
						}

						if( !passedDistanceTests )
						{
							// For looping sounds we want the instance to persist,
							// call the internal stop function,                       
							// which shuts the FMOD event shut down,
							// but keeps the m_loops flag intact.
							pInstance->StopInternal(false);
						}
					}

					// Recheck roughly every second
					pInstance->m_uRetestMs = (rand() % 256) + 896;
				}
				else
				{
					pInstance->SetFlag(EventInstance::kSIF_Delayed, false);
                    if( !pInstance->m_pInternalData )
                    {
                        // We pass the existing instance in so that any external handles to this sound stay copasetic.                        
                        _PlayEvent( pInstance->m_sourceEvent, pInstance->GetFlag(EventInstance::kSIF_Loops), NULL, pInstance, NULL );
                        if( pInstance->m_pInternalData )
                        {
#if SUPPORT_FULL_3D_SOUNDS
                            pInstance->SetPosition( pInstance->m_vPosition, pInstance->m_vVelocity, pInstance->m_vForward );
#else
                            pInstance->SetPosition( pInstance->m_vPosition );
#endif
                        }
                    }
                }                
            }

            ++it;
        }        
        else if( !pInstance->GetFlag(EventInstance::kSIF_UnloadOnSilence) |
                  pInstance->GetFlag(EventInstance::kSIF_Unloaded) )
        {
            // Goodbye!
            m_handleFactory.Free(pInstance->m_myHandle);                
            delete pInstance;
            it = m_aEventInstances.erase( it );
        }
        else
        {
            // Done but not unloaded, so just move on.
            ++it;
        }
    }
}

void EventManager::_UpdateMinRetriggerTimers(float fDeltaTime)
{
    STLMap<STLString, float>::iterator it = m_aMinRetriggerTimers.begin();
    while( it != m_aMinRetriggerTimers.end() )
    {
        it->second -= fDeltaTime;
        if( it->second < 0.f )
        {
            m_aMinRetriggerTimers.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

void EventManager::EventInstanceCompleted( EventInstance* pInstance )
{
    assert( pInstance );
    pInstance->m_pInternalData = NULL;        
    tEventManager.UnDuckSoundCategories(pInstance->m_hDuckingRequest);    

    _EventInstanceCompleted(pInstance);
}

void EventManager::SetDistantOcclusion( float minRange, float maxRange, float maxOcclussion )
{
    m_distantLowpassRange.min = minRange;
    m_distantLowpassRange.max = maxRange;
    m_maxDistantOcclussion = maxOcclussion;
}

void EventManager::SetNear2DBlend( float minRange, float maxRange, float maxLevel )
{
    m_near2DBlendRange.min = minRange;
    m_near2DBlendRange.max = maxRange;
    m_near2DBlendLevel = maxLevel;
}

EventManager::SoundCategoryState* EventManager::_GetSoundCategoryState(const STLString& category) const
{
    SoundCategoryState* pNewCategoryState = NULL;

    if( m_apSoundCategories.contains( category ) )
    {
        pNewCategoryState = m_apSoundCategories[category];
    }
    
    if( !pNewCategoryState )
    {
        pNewCategoryState = _InitSoundCategory(category);
        if( pNewCategoryState )
        {
            m_apSoundCategories.insert( pair<STLString, SoundCategoryState*>( category, pNewCategoryState ) );
        }
    }
    return pNewCategoryState;
}

ZLVec3D EventManager::_GetWorldspacePosition( const ZLVec3D& vPosition, bool headRelative ) const
{
    if( headRelative )
    {
        ZLMatrix4x4 mtxlistener;
        ZLVec3D vTransformedVec;

        mtxlistener.Init( m_vListenerRight, m_vListenerUp, m_vListenerForward, m_vListenerPosition );        
        mtxlistener.TransformVec( vTransformedVec );

        return vTransformedVec;
    }
    else
    {
        return vPosition;
    }
}

//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------
// Utility functions
//----------------------------------------------------------------------------------------

// Temporary, because we have to work around a bug where voice banks don't encrypt properly.
static int sg_nEncryptedWavbanks = 0;
static void
_CheckIfEncrypted(const STLString& bankname)
{
    char buf[3] = {'\0'};
    {
        ZLFileStream file;
        if (! file.OpenRead(bankname.c_str())) { return; }
        file.ReadBytes(buf, sizeof(buf));
        file.Close();
    }

    // First 4 characters are a filetype token -- FSB4 or FSB5.
    // Check if they're plaintext.
    if (buf[0]!='F' || buf[1]!='S' || buf[2]!='B')
    {
        sg_nEncryptedWavbanks += 1;
    }
}

const char* // static
EventManager::GetVoiceEncryptionKey()
{
    // Voice banks might not be encrypted
    if (sg_nEncryptedWavbanks) { return "DFm3t4lFTW"; }
    return NULL;
}

const char* // static
EventManager::GetSoundEncryptionKey()
{
    return "DFm3t4lFTW";
}

static const char*
_VoiceProjectNameToLanguageName(const char* voiceProjectName)
{
	const char* result = strchr(voiceProjectName, '_');
	if (result)
	{
		return result+1;
	}

	return NULL;
}


bool _VoiceProjectNameWithoutLang(const char* voiceProjectName, STLString& outStr)
{
	outStr = voiceProjectName;
	int underscoreIndex = outStr.find('_');

	if (underscoreIndex >= 0)
	{
		outStr.erase(underscoreIndex, outStr.size()-underscoreIndex);
		return true;
	}

	return false;
}

#ifdef _DEBUG
void FMODDesigner::PrintFMODError( FMOD_RESULT result )
{
    if( result != FMOD_OK &&                                // Ok sounds are, well, ok
        result != FMOD_ERR_EVENT_MAXSTREAMS &&              // Failing on max-streams is the desired behavior
        result != FMOD_ERR_EVENT_FAILED &&                  // Failing on a just-fail is the desired behavior
        result != FMOD_ERR_CHANNEL_STOLEN &&                // This is ok-ish, because we hold onto EventInstances after the sound has stopped playing
        result != FMOD_ERR_NOTREADY                         // Sometimes this happens when you use the non-blocking interfaces.
        )
    {
        Trace(TT_Sound, TL_Info, "FMOD error! %s\n", FMOD_ErrorString(result));
    }
}
#endif

void FMODDesigner::CopyDFVectorToFMOD( FMOD_VECTOR& vFMOD, const ZLVec3D& vDF )
{
    vFMOD.x = vDF.mX;
    vFMOD.y = vDF.mY;
    vFMOD.z = vDF.mZ;
}

void FMODDesigner::CopyFMODVectorToDF( ZLVec3D& vDF, const FMOD_VECTOR& vFMOD )
{
    vDF.mX = vFMOD.x;
    vDF.mY = vFMOD.y;
    vDF.mZ = vFMOD.z;
}



void CopyFMODToReverb(FMOD_REVERB_PROPERTIES& rFMODReverb, ReverbInstance* pReverbInstance)
{
	pReverbInstance->m_fDecayTime			= rFMODReverb.DecayTime;
	pReverbInstance->m_fDecayHFRatio		= rFMODReverb.DecayHFRatio; 
	pReverbInstance->m_fPreDelay			= rFMODReverb.ReflectionsDelay;	
	pReverbInstance->m_fLateDelay			= rFMODReverb.ReverbDelay;		
	pReverbInstance->m_fEarlyReflections	= (float) rFMODReverb.Reflections;		
	pReverbInstance->m_fLateReflections		= (float) rFMODReverb.Reverb;	
	pReverbInstance->m_fDiffusion			= rFMODReverb.Diffusion;		
	pReverbInstance->m_fDensity				= rFMODReverb.Density;		
	pReverbInstance->m_fHFGain				= (float) rFMODReverb.RoomHF;			
	pReverbInstance->m_fLFGain				= (float) rFMODReverb.RoomLF;			
	pReverbInstance->m_fHFCrossover			= rFMODReverb.HFReference;		
	pReverbInstance->m_fLFCrossOver			= rFMODReverb.LFReference;		
}



void CopyReverbToFMOD(FMOD_REVERB_PROPERTIES& rFMODReverb, ReverbInstance* pReverbInstance)
{
	rFMODReverb.DecayTime			= pReverbInstance->m_fDecayTime;
	rFMODReverb.DecayHFRatio		= pReverbInstance->m_fDecayHFRatio;
	rFMODReverb.ReflectionsDelay	= pReverbInstance->m_fPreDelay	;
	rFMODReverb.ReverbDelay			= pReverbInstance->m_fLateDelay	;
	rFMODReverb.Reflections			= (int) floor(pReverbInstance->m_fEarlyReflections);
	rFMODReverb.Reverb				= (int) floor(pReverbInstance->m_fLateReflections);
	rFMODReverb.Diffusion			= pReverbInstance->m_fDiffusion;
	rFMODReverb.Density				= pReverbInstance->m_fDensity;
	rFMODReverb.RoomHF				= (int) floor(pReverbInstance->m_fHFGain);
	rFMODReverb.RoomLF				= (int) floor(pReverbInstance->m_fLFGain);
	rFMODReverb.HFReference			= pReverbInstance->m_fHFCrossover;
	rFMODReverb.LFReference			= pReverbInstance->m_fLFCrossOver;
}

void FMODDesigner::GetEventParts( FMOD::ChannelGroup* pChannelGroup, vector<FMOD::Channel*>& apChannels, vector<FMOD::Sound*>* papSounds )
{
    FMOD_RESULT result = FMOD_OK;
    if( pChannelGroup )
    {
        int nChannels = 0;
        result = pChannelGroup->getNumChannels(&nChannels);
        if( result == FMOD_OK )
        {
            for( int i = 0; i < nChannels; ++i )
            {
                FMOD::Channel* pChannel = NULL;
                result = pChannelGroup->getChannel(i, &pChannel);
                if( result == FMOD_OK && pChannel != NULL )
                {
                    apChannels.push_back(pChannel);
                    if( papSounds )
                    {
                        FMOD::Sound* pSound = NULL;
                        result = pChannel->getCurrentSound(&pSound);
                        if( result == FMOD_OK && pSound != NULL )
                        {
                            papSounds->push_back(pSound);                        
                        }
                    }
                }
            }
        }

        int nChannelGroups = 0;
        result = pChannelGroup->getNumGroups(&nChannelGroups);
        if( result == FMOD_OK )
        {
            for( int i = 0; i < nChannelGroups; ++i )
            {
                FMOD::ChannelGroup* pGroup = NULL;
                result = pChannelGroup->getGroup(i, &pGroup);
                if( result == FMOD_OK )
                {
                    FMODDesigner::GetEventParts( pGroup, apChannels, papSounds );
                }
            }
        }
    }
}

bool FMODDesigner::IsSoundLoading( FMOD::Sound* pSound )
{    
    if( pSound )
    {                
        FMOD_OPENSTATE state = FMOD_OPENSTATE_ERROR;
        pSound->getOpenState(&state, NULL, NULL, NULL);
        return state == FMOD_OPENSTATE_LOADING || state == FMOD_OPENSTATE_CONNECTING || state == FMOD_OPENSTATE_BUFFERING || state == FMOD_OPENSTATE_SEEKING;
    }
    else
    {
        return false;
    }
}

// Return a path, with a trailing path separator
void FMODDesigner::GetAudioPath( STLString& outPath, bool getFullPath )
{
    outPath.clear();
    outPath.write( ZLFileSys::GetCurrentPath().c_str() );

    if ( getFullPath )
    {
        STLString mediaPath = ZLFileSys::GetAbsoluteDirPath( outPath.c_str() );        
        outPath = mediaPath;
        if (outPath.length() && (outPath.at(outPath.length()-1) != FILE_SEP_CHAR))
        {
            outPath += FILE_SEP_CHAR;
        }
    }
}

static FMOD::EventGroup* _GetGroup( int projectId, int id )
{
    FMOD_RESULT result = FMOD_OK;
    FMOD::EventGroup* pGroup = NULL;
    
    FMOD::EventProject* pProject = NULL;
    result = FMODDesigner::s_pFMODEventSystem->getProjectByIndex( projectId, &pProject );
    if( result == FMOD_OK )
    {
        result = pProject->getGroupByIndex( id, false, &pGroup );        
    }    

    HANDLE_FMOD_ERROR( result );
    return pGroup;
}

bool // static
EventManager::VoiceDataExists(const char* szProject)
{
    STLString path; // in place string
    FMODDesigner::GetAudioPath(path, false);

    STLString fsb; // in place string
    fsb.write("%s%s/%s_Streaming.fsb", path.c_str(), szProject, _VoiceProjectNameToLanguageName(szProject));
    return (ZLFileSys::CheckFileExists(fsb.c_str()));
}

FMOD_RESULT F_CALLBACK _FMODEventCallback( FMOD_EVENT* pEvent, FMOD_EVENT_CALLBACKTYPE type, void* pParam1, void* pParam2, void* pUserData )
{    
    assert(pUserData);
    EventInstance* pInstance = (EventInstance*)pUserData;
    switch(type)
    {
        case FMOD_EVENT_CALLBACKTYPE_EVENTFINISHED:
        case FMOD_EVENT_CALLBACKTYPE_STOLEN:
        {
            ((FMOD::Event*)pEvent)->setCallback(NULL, NULL);
            FMODDesigner::tEventManager.EventInstanceCompleted( pInstance );
            return FMOD_OK;
        }        
        case FMOD_EVENT_CALLBACKTYPE_SOUNDDEF_CREATE:
        {        
            if( pInstance->GetSourceEvent().IsRawSound() )
            {
                *((FMOD::Sound**)pParam2) = (FMOD::Sound*)pInstance->GetSourceEvent().GetInternals();                
                return FMOD_OK;
            }
            else
            {            
                FMOD::Sound* pRootSound = NULL;
                u16 wavIndex = 0;
                if (pInstance->ContainsResidentVoice())
                {
                    // wavIndex is always 0 for this case
                    pRootSound = ((FMOD::Sound*)pInstance->GetVoiceMemoryPointer());
                }

                if( pRootSound )
                {
                    return pRootSound->getSubSound( wavIndex, ((FMOD::Sound**)pParam2) );                
                }
                else
                {
                    Trace(TT_Sound, TL_Info, "Programmer sound %s trying to start that hasn't been setup properly", pInstance->GetName().c_str());

                    // ideally we would do something that indicates an error
                    // but it isn't clear what the caller would do with it
                    return FMOD_OK;
                }
            }
        }
        case FMOD_EVENT_CALLBACKTYPE_SOUNDDEF_RELEASE:
        {            
            if( pInstance->ContainsVoiceData() )
            {                
                pInstance->ClearVoiceData();
                FMODDesigner::tEventManager.DecrementCurrentlyPlayingVoices();
            }            

            return FMOD_OK;
        }
        case FMOD_EVENT_CALLBACKTYPE_SYNCPOINT:
        {            
            return FMOD_OK;
        }
        default:
            return FMOD_OK;
    }
}
                                             
FMOD_RESULT F_CALLBACK _FMODChannelCallback( FMOD_CHANNEL* pChannel, FMOD_CHANNEL_CALLBACKTYPE type, void* commanddata1, void* commanddata2 )
{   
    if(type == FMOD_CHANNEL_CALLBACKTYPE_END)
    {
        // This implementation is obviously very inefficient (O(n) search).
        // It's only meant to keep support for the low level interface intact.
        // If we ever make heavy use of raw channels this will need to be revisited.
        const vector<EventInstance*>& raEventInstances = FMODDesigner::tEventManager.GetEventInstances();
        for( u32 i = 0; i < raEventInstances.size(); ++i )
        {
            EventInstance* pInstance = raEventInstances[i];
            if( pInstance && pInstance->GetInternalData() == pChannel )
            {
                ((FMOD::Channel*)pChannel)->setCallback(NULL);
                FMODDesigner::tEventManager.EventInstanceCompleted( pInstance );
                break;
            }
        }
    }    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK _FMODSystemCallback( FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACKTYPE type, void* commanddata1, void* commanddata2 )
{
    static bool sbWarnedAboutMemory = false;

    switch( type )
    {
    case FMOD_SYSTEM_CALLBACKTYPE_MEMORYALLOCATIONFAILED:        
        if (!sbWarnedAboutMemory)
        {
            Trace( TT_Sound, TL_Error, "ZOMG, we ran out of sound memory. Someone tell team Audio to look into it ASAP!" );
            sbWarnedAboutMemory = true;
        }
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPCONNECTION:
        // we made a bad DSP Connection. :(
        assert( false );
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPLEVEL:
        // we made too many fancy DSP effects, FMOD is sad and will probably die soon :(
        assert( false );
        break;
    default:
        break;
    };
    return FMOD_OK;
}

//----------------------------------------------------------------------------------------
// Experimental sound processing
//----------------------------------------------------------------------------------------

struct ProcessSoundRequest
{
    ProcessSoundRequest() :
        soundDataBufferEnd(0)
    {
        memset( soundDataBuffer, 0, sizeof( soundDataBuffer ) );        
    }

    short soundDataBuffer[MEG];
    u32 soundDataBufferEnd;    
};

static ProcessSoundRequest* sg_pCurrentProcessRequest;

FMOD_RESULT F_CALLBACK copyDSP(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, u32 length, int inchannels, int outchannels)
{
    u32 userdata;    
    char name[256]; 
    FMOD::DSP* pThisDSP = (FMOD::DSP *)dsp_state->instance; 

    /* 
    This redundant call just shows using the instance parameter of FMOD_DSP_STATE and using it to 
    call a DSP information function. 
    */
    pThisDSP->getInfo(name, 0, 0, 0, 0);
    pThisDSP->getUserData((void **)&userdata);

    assert(inchannels == 1);
    assert(inchannels == outchannels);
    for (u32 i = 0; i < length; ++i) 
    {                
        outbuffer[i] = inbuffer[i];         
        sg_pCurrentProcessRequest->soundDataBuffer[sg_pCurrentProcessRequest->soundDataBufferEnd] = (short)Pin( inbuffer[i] * 30000.f, -32767.f, 32767.f );
        ++sg_pCurrentProcessRequest->soundDataBufferEnd;
    }

    return FMOD_OK; 
}

void EventManager::CreateSoundFromLastProcess(Event& newSound)
{
    if( sg_pCurrentProcessRequest && sg_pCurrentProcessRequest->soundDataBufferEnd > 0 )
    {
        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );

        int sampleRate = 0;        
        result = pSystem->getSoftwareFormat( &sampleRate, NULL, NULL, NULL, NULL, NULL );

        CreateSound( (char*)sg_pCurrentProcessRequest->soundDataBuffer,
            (sg_pCurrentProcessRequest->soundDataBufferEnd - 1) * 2, 
            sampleRate,
            newSound );
        delete sg_pCurrentProcessRequest;
        sg_pCurrentProcessRequest = NULL;        
    }
    else
    {
        newSound = Event();
    }
}

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

#define OUTPUTRATE          48000
#define SPECTRUMSIZE        8192
#define SPECTRUMRANGE       ((float)OUTPUTRATE / 2.0f)      /* 0 to nyquist */
#define BINSIZE      (SPECTRUMRANGE / (float)SPECTRUMSIZE)
static FMOD::Channel* sg_pPlaybackChannel = NULL;

float EventManager::GetFrequencyOfRecordingSound()
{
	if (!sg_pPlaybackChannel)
	{
		return 0.f;
	}

	static float spectrum[SPECTRUMSIZE];
	float        dominanthz = 0;
	float        max;
	int			 bin = 0;

	FMOD_RESULT result = sg_pPlaybackChannel->getSpectrum(spectrum, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE); 
	ERRCHECK(result);

	max = 0;

	for (int count = 0; count < SPECTRUMSIZE; count++)
	{
		if (spectrum[count] > 0.01f && spectrum[count] > max)
		{
			max = spectrum[count];
			bin = count;
		}
	}        

	dominanthz  = (float)bin * BINSIZE;       /* dominant frequency min */

	return dominanthz;
}


EventHandle EventManager::ProcessSound( const Event& soundEvent, const SoundDSP* pDSPChain, bool processSilently )
{
    if( soundEvent.IsValid() && !sg_pCurrentProcessRequest )
    {
        assert( soundEvent.m_rawSound ); // For now, only works on raw sounds

        sg_pCurrentProcessRequest = new ProcessSoundRequest();

        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );
        if( result == FMOD_OK )
        {
            FMOD::Sound* pSound = (FMOD::Sound*)soundEvent.m_pInternalData;
            FMOD::Channel* pChannel = NULL;
			result = pSystem->playSound( FMOD_CHANNEL_FREE, pSound, false, &pChannel );
            if( result == FMOD_OK )
            {
                result = pChannel->setCallback( _FMODChannelCallback );
                assert( result == FMOD_OK );
                // So that the user doesn't hear the processing
                if( processSilently )
                {
                    pChannel->setVolume(0.f);
                }       

                static STLString voiceCat("Voice/Main");
                if( SoundCategoryState* pCategoryState = _GetSoundCategoryState(voiceCat) )
                {
                    FMOD::ChannelGroup* pChannelGroup = NULL;
                    result = ((FMOD::EventCategory*)pCategoryState->m_pInternalData)->getChannelGroup(&pChannelGroup);
                    if( result == FMOD_OK )
                    {
                        pChannel->setChannelGroup(pChannelGroup);
                    }                                               
                }

                pDSPChain->Apply( pChannel );                
                                                
                FMOD_DSP_DESCRIPTION  copyDSPDesc;                 
                memset(&copyDSPDesc, 0, sizeof(FMOD_DSP_DESCRIPTION)); 
                strcpy(copyDSPDesc.name, "Copy DSP"); 
                copyDSPDesc.channels     = 0;                   // 0 = whatever comes in, else specify. 
                copyDSPDesc.read         = copyDSP; 
                copyDSPDesc.userdata     = (void *)0x12345678; 

                FMOD::DSP* pMyDSP = NULL;
                result = pSystem->createDSP(&copyDSPDesc, &pMyDSP);
                if( result == FMOD_OK )
                {
                    pChannel->addDSP( pMyDSP, NULL );
                    EventInstance* pNewInstance = new EventInstance( pChannel, soundEvent, true );                    
                    pNewInstance->m_myHandle = m_handleFactory.Alloc(pNewInstance);
                    m_aEventInstances.push_back( pNewInstance );
                    return pNewInstance->m_myHandle;
                }
            }
        }

        HANDLE_FMOD_ERROR( result );
    }    

    return EventHandle();
}

//----------------------------------------------------------------------------------------
// EventManager Methods
//----------------------------------------------------------------------------------------

void* EventManager::GetPlatformOutputHandle()
{
    if (s_pFMODEventSystem)
    {
        FMOD::System* pSystem = NULL;
        if (s_pFMODEventSystem->getSystemObject(&pSystem) == FMOD_OK)
        {
            void* platformHandle = NULL;
            if (pSystem->getOutputHandle(&platformHandle) == FMOD_OK)
            {
                return platformHandle;
            }
        }
    }
    return NULL;
}

bool EventManager::LoadProject( const char* szProject )
{
    assert( szProject );

    if( m_enabled )
    {        
        FMOD::EventProject* pProject = NULL;
        FMOD_RESULT result = FMOD_OK;

        STLString sProjectName(szProject);
                
        // This loadInfo is for bank decryption.  Storing the key in the

        FMOD_EVENT_LOADINFO loadInfo;
        memset( &loadInfo, 0, sizeof(FMOD_EVENT_LOADINFO) );
        loadInfo.size = sizeof(FMOD_EVENT_LOADINFO);
        loadInfo.encryptionkey = (char*)EventManager::GetSoundEncryptionKey();

        result = s_pFMODEventSystem->load( sProjectName.c_str(), &loadInfo, &pProject );

        if( result == FMOD_OK )
        {
            FMOD_EVENT_PROJECTINFO info;
            memset(&info, 0, sizeof(FMOD_EVENT_PROJECTINFO));
            result = pProject->getInfo( &info );
            if( result == FMOD_OK )
            {
                g_ProjectIDs.insert( pair<STLString, int>( sProjectName, info.index ) );
            }
        }
        else if( result == FMOD_ERR_FILE_EOF )
        {
            Trace(TT_Sound, TL_Error, "%s is garbage and will crash the game. Someone probably forgot to check in a wav file.", sProjectName.c_str() );
        }
        else
        {
            // people might want to know about this
            Trace(TT_Sound, TL_Warning, "Sound project %s failed to load, '%s'", szProject, FMOD_ErrorString(result));
        }

        HANDLE_FMOD_ERROR( result );

        return result == FMOD_OK;
    }

    return false;
}

bool EventManager::UnloadProject( const char* szProject )
{   
    assert( szProject );

    if( m_enabled )
    {                
        FMOD::EventProject* pProject = NULL;
        FMOD_RESULT result = FMOD_OK;
        result = s_pFMODEventSystem->getProject( szProject, &pProject );
        if( result == FMOD_OK )
        {
            result = pProject->release();
        }

        HANDLE_FMOD_ERROR( result );

        return result == FMOD_OK;
    }

    return true;
}

// Misleading name -- actually takes a .fsb, not a project file
bool EventManager::LoadVoiceProject( const char* szProject, std::vector<std::pair<cc8*, float> >* lineCodeInfo )
{
    if (!m_enabled) { return false; }

    FMOD_RESULT result = FMOD_OK;

    // we should only come in here for a voice project
    FMOD::System* pSystem = NULL;
    s_pFMODEventSystem->getSystemObject( &pSystem );
    assert(pSystem);

    STLString audioPath;
    FMODDesigner::GetAudioPath( audioPath, true );

    STLString sWavbankFilename; 
    sWavbankFilename.write( "%s%s", audioPath.c_str(), szProject );
    g_WavbankNames.push_back( sWavbankFilename );
    const u16 iCurWavbank = g_WavbankNames.size() - 1;

    // Updates encryption status (which is global, barf)
    _CheckIfEncrypted( sWavbankFilename );

    FMOD::Sound* pSound = NULL;
    {
        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);                        
        exinfo.encryptionkey = EventManager::GetVoiceEncryptionKey();
        exinfo.suggestedsoundtype = FMOD_SOUND_TYPE_FSB;
        const FMOD_MODE mode = FMOD_SOFTWARE | FMOD_CREATESTREAM | FMOD_LOOP_OFF;            
        result = pSystem->createSound( sWavbankFilename.c_str(), mode, &exinfo, &pSound );
        if (result != FMOD_OK) { return false; }
    }
	
	int numSounds = -1;     
	result = pSound->getNumSubSounds( &numSounds );
	if (result != FMOD_OK) { return false; }

    // Grab all the sound information into g_WavbankEntries
	bool bLoadDataFromFsb = true;
	if ( lineCodeInfo ) {

		size_t numLineCodes = lineCodeInfo->size();		
		if ( numLineCodes == numSounds ) {

			// Use the cached data
			bLoadDataFromFsb = false;

			g_WavbankEntries.resize ( numLineCodes + 1 );

			for ( size_t i=0; i<numLineCodes; i++ ) {

				cc8* szName = (*lineCodeInfo) [ i ].first;
				float fLengthSec = (*lineCodeInfo) [ i ].second;
				
				u32 linecodeIndex = LineCode(szName).GetIndex();

				if ( linecodeIndex >= g_WavbankEntries.size () ) {
					g_WavbankEntries.resize ( linecodeIndex + 1 );
					assert ( 0 ); // This is very slow!
				}

				g_WavbankEntries[linecodeIndex] =
					WavbankEntry( linecodeIndex, NULL, fLengthSec, iCurWavbank );
			}
		}
	}
	if ( bLoadDataFromFsb ) {

		g_WavbankEntries.resize ( numSounds + 1 );

		for( int i = 0; i < numSounds; ++i ) {

			FMOD::Sound* pSubSound = NULL;
			result = pSound->getSubSound( i, &pSubSound );
			if (result != FMOD_OK) { continue; }

			u32 lenMs = 0;        
			result = pSubSound->getLength( &lenMs, FMOD_TIMEUNIT_MS );   
			if (result != FMOD_OK) { continue; }

			u32 linecodeIndex;
			{
				// Get the name	
				const size_t kNameLength = 32;
				char szName[kNameLength];
				result = pSubSound->getName( szName, kNameLength );
				if (result != FMOD_OK) { continue; }

				// Strip the extension
				for ( int i=0; i<kNameLength; i++ ) {
					if ( szName [ i ] == '.' ) {
						szName [ i ] = 0;
						break;
					}
				}
			
				// Get the index
				linecodeIndex = LineCode(szName).GetIndex();
			}

			if ( linecodeIndex >= g_WavbankEntries.size () ) {
				g_WavbankEntries.resize ( linecodeIndex + 1 );
				assert ( 0 ); // This is very slow!
			}

			g_WavbankEntries[linecodeIndex] =
				WavbankEntry( linecodeIndex, NULL, lenMs * 0.001f, iCurWavbank );
		}
	}

	pSound->release();

    return true;
}

bool EventManager::UnloadVoiceProjects()
{
    if( m_enabled )
    {         
        // empty the voice lru
        // but we're not allowed to do that directly so stop all sounds and it will empty it
        //StopAllSounds();
        // CAUTION: This is not safe to do DURING a mission
        // but since we are changing it to make you unable to change languages mid-mission
        // this is ok.  But if you crash here, you probably did something bad with that
        m_voiceLRU.ReleaseAllSound();
        m_iNumActiveVoices = 0;

        // empty wavbank entries (stuff from wavlist)
        g_WavbankEntries.clear();        
        g_WavbankNames.clear();
    }
    
    return true;
}

/*!
*   High demand voice line that must go into the voice LRU now
*/
void EventManager::AddLineToLRU(const Event& soundEvent, const LineCode& lineCode)
{
    if (m_voiceLRU.IsEnabled())
    {
        const EventProperties* pProperties = GetEventProperties( soundEvent );
        if (pProperties)
        {
            const u32 time = FMODDesigner::GetTimeMs();
            m_voiceLRU.GetSound(lineCode, time, pProperties);
        }
    }
}

bool EventManager::IsLineLoading(const LineCode& lineCode)
{
    if (m_voiceLRU.IsEnabled())
    {
        return IsSoundLoading( (FMOD::Sound*)m_voiceLRU.GetSound_NoLoad(lineCode) );        
    }
    return false;
}

void EventManager::_EventInstanceCompleted( EventInstance* pInstance )
{
    // make sure we release the stream if we haven't
    if ( pInstance->ContainsVoiceData()  )
    {
        pInstance->ClearVoiceData();
        DecrementCurrentlyPlayingVoices();
    }    
}

bool EventManager::_LoadGroup( const GroupData* pGroupData, bool blockOnLoad )
{  
	// preload all the events from this group (eventually this could use the group
	//  "persistent" flag
	const bool bPreloadEvents = false;

    assert(pGroupData);
    if( m_enabled )
    {    
        Trace(TT_Sound, TL_Info, "Attempting to load group %s", pGroupData->m_path.c_str());

        FMOD_RESULT result = FMOD_OK;        
        FMOD::EventGroup* pGroup = _GetGroup( pGroupData->m_projectId, pGroupData->m_id );        
        if(pGroup && bPreloadEvents)
        {     
            result = pGroup->loadEventData( FMOD_EVENT_RESOURCE_SAMPLES, blockOnLoad ? FMOD_EVENT_DEFAULT : FMOD_EVENT_NONBLOCKING );
        }
                
        HANDLE_FMOD_ERROR( result );

        return result == FMOD_OK;
    }

    return false;
}

bool EventManager::_UnloadGroup( const GroupData* pGroupData, bool blockOnUnload )
{   
    assert(pGroupData);
    if( m_enabled )
    {
        Trace(TT_Sound, TL_Info, "Attempting to unload group %s", pGroupData->m_path.c_str());

        FMOD_RESULT result = FMOD_OK;
        FMOD::EventGroup* pGroup = _GetGroup( pGroupData->m_projectId, pGroupData->m_id );
        if(pGroup)
        {
            result = pGroup->freeEventData( NULL, blockOnUnload );
        }

        HANDLE_FMOD_ERROR( result );

        return result == FMOD_OK;
    }

    return true;
}

void EventManager::SetStreamBufferSize(u32 nBytes)
{
    if( m_enabled )
    {
        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );
        if (result == FMOD_OK)
        {            
            pSystem->setStreamBufferSize(nBytes, FMOD_TIMEUNIT_RAWBYTES);
        }
    }
}

u32 EventManager::GetStreamBufferSize() const
{
    if( m_enabled )
    {
        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );
        if (result == FMOD_OK)
        {
            u32 bufferSize = 0;
            FMOD_TIMEUNIT timeuint;
            result = pSystem->getStreamBufferSize(&bufferSize, &timeuint);
            if( result == FMOD_OK )
            {
                return bufferSize;
            }
        }
    }
    return 0;    
}

void EventManager::_InitInternals()
{           
    FMOD_RESULT result = FMOD_OK;
    
    void* pExtraDriverData = NULL;
    FMOD_INITFLAGS systemInitFlags = FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED;

#ifdef _DEBUG
    if( m_initParams.m_enableAuditioning )
    {
        systemInitFlags |= FMOD_INIT_ENABLE_PROFILE;
    }    
#endif
    FMOD_EVENT_INITFLAGS eventInitFlags = FMOD_EVENT_INIT_NORMAL | FMOD_EVENT_INIT_FAIL_ON_MAXSTREAMS;    

    if( m_initParams.m_enableDistantLowpass )
    {         
        systemInitFlags |= FMOD_INIT_OCCLUSION_LOWPASS;
    }

    FMOD_ADVANCEDSETTINGS advancedSettings;
    memset( &advancedSettings, 0, sizeof( FMOD_ADVANCEDSETTINGS ) );
    advancedSettings.cbsize = sizeof( FMOD_ADVANCEDSETTINGS );    
    advancedSettings.maxADPCMcodecs = m_initParams.m_nADPCMCodecs;    

    FMOD::System* pSystem = NULL;   

    // FMOD wants this as a multiple of 512 (and xenon wants a multiple of 4096)    
    s_uTotalMemSize = m_initParams.m_soundMemoryMB * MEG;
    s_uMainMemSize = s_uTotalMemSize;

// Setup the event flags and other platform specific settings
// TODO: add all shipping platforms
#ifdef MOAI_OS_WINDOWS
    advancedSettings.maxMPEGcodecs = m_initParams.m_nCompressedCodecs;
#endif   

    // Initialize FMOD's memory subsystem        
    s_pMainMemPool = malloc(s_uMainMemSize);

    result = FMOD::Memory_Initialize(s_pMainMemPool, s_uMainMemSize, NULL, NULL, NULL);    
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }

    //create the FMOD event system, the FMOD system should be automatically created
    result = FMOD::EventSystem_Create(&s_pFMODEventSystem);
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }
    
    result = s_pFMODEventSystem->getSystemObject( &pSystem );
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }

    pSystem->setCallback( _FMODSystemCallback );

    result = pSystem->setAdvancedSettings( &advancedSettings );
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }      

#if defined(MOAI_OS_WINDOWS)
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];
 
    if (m_initParams.m_disableSound)
    {
        pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
    }
    
    result = pSystem->getNumDrivers(&numdrivers);
    //numdrivers = 1;
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }
    
    if (numdrivers == 0)
    {
        result = pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        if (result != FMOD_OK)
        {
            goto fail_with_result;
        }
    } else {
        result = pSystem->getDriverCaps(0, &caps, 0, &speakermode);
        if (result != FMOD_OK)
        {
            goto fail_with_result;
        }
        
        /*
          Set the user selected speaker mode.
        */
        result = pSystem->setSpeakerMode(speakermode);
        if (result != FMOD_OK)
        {
                goto fail_with_result;
            }

            if (caps & FMOD_CAPS_HARDWARE_EMULATED)
            {
                /*
                  The user has the 'Acceleration' slider set to off! This is really bad
                  for latency! You might want to warn the user about this.
                 */
                result = pSystem->setDSPBufferSize(1024, 10);
                if (result != FMOD_OK)
                {
                    goto fail_with_result;
                }
            }
            result = pSystem->getDriverInfo(0, name, 256, 0);
            if (result != FMOD_OK)
            {
                goto fail_with_result;
            }

            if (strstr(name, "SigmaTel"))
            {
                /*
                  Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
                  PCM floating point output seems to solve it.
                */
            result = pSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, m_initParams.m_nMaxInputChannels, FMOD_DSP_RESAMPLER_LINEAR);
                if (result != FMOD_OK)
                {
                    goto fail_with_result;
                }
            }
        else
  		{
  		    int sampleRate;
  		    FMOD_SOUND_FORMAT soundFormat;
  		    int numOutputChannels;
  		    int maxInputChannels;
  		    FMOD_DSP_RESAMPLER dsp;
  		    int bits;
  		  
  		    result = pSystem->getSoftwareFormat( &sampleRate, &soundFormat, &numOutputChannels, &maxInputChannels, &dsp, &bits );
                if (result != FMOD_OK)
                {
                    goto fail_with_result;
                }

            // Passing 0 in to numOutputChannels so that we preserve the default speaker mode.
  		    result = pSystem->setSoftwareFormat( sampleRate, soundFormat, 0, m_initParams.m_nMaxInputChannels, dsp);
            if( result != FMOD_OK )
            {
                goto fail_with_result;
            }
        }

            result = s_pFMODEventSystem->init(m_initParams.m_nVirtualChannels, systemInitFlags, pExtraDriverData, eventInitFlags);  // Initialize FMOD.
            if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
            {
                /*
                  Ok, the speaker mode selected isn't supported by this soundcard. Switch it
                  back to stereo...
                */
                result = pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
                if (result != FMOD_OK)
                {
                    goto fail_with_result;
                }

                /*
                  ... and re-init.
                */
                result = s_pFMODEventSystem->init(m_initParams.m_nVirtualChannels, systemInitFlags, pExtraDriverData, eventInitFlags);  // Initialize FMOD.
                if (result != FMOD_OK)
                {
                    goto fail_with_result;
                }
            }
        }
#elif 0 && defined(MOAI_OS_NACL)
    FMOD_NACL_EXTRADRIVERDATA extraDriverData;
    memset(&extraDriverData, 0, sizeof(FMOD_NACL_EXTRADRIVERDATA)); 
    extraDriverData.instance = g_instance->pp_instance();
    pExtraDriverData = &extraDriverData;

    result = s_pFMODEventSystem->init(m_initParams.m_nVirtualChannels, systemInitFlags, pExtraDriverData, eventInitFlags);	// Initialize FMOD.
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }    
    s_pFMODEventSystem->setDSPBufferSize( 1024, 4 );    
#else
    result = s_pFMODEventSystem->init(m_initParams.m_nVirtualChannels, systemInitFlags, pExtraDriverData, eventInitFlags);	// Initialize FMOD.
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }    
#endif

    // set the media path
    {
        STLString sAudioPath;    
        FMODDesigner::GetAudioPath( sAudioPath, true );
        assert(sAudioPath.size() > 0);
        result = s_pFMODEventSystem->setMediaPath(sAudioPath.c_str());				
        if (result != FMOD_OK)
        {
            goto fail_with_result;            
        }
    }   
                
    // Scale doppler
    result = pSystem ->set3DSettings( m_initParams.m_dopplerScale, 1.f, 1.f );
    if (result != FMOD_OK)
    {
        goto fail_with_result;
    }    

    {
        FMOD_SPEAKERMODE mode = FMOD_SPEAKERMODE_STEREO;
        (void) pSystem->getSpeakerMode(&mode);
        if (mode >= FMOD_SPEAKERMODE_SURROUND)
    {
        // Disable FX on the center channel. Keep it clear for VO.
        result = pSystem->set3DSpeakerPosition(FMOD_SPEAKER_FRONT_CENTER, 0.0f, 1.0f, false);
        if (result != FMOD_OK)
        {
            goto fail_with_result;
        }
    }
    }

    m_voiceLRU.Init(m_initParams.m_voiceLRUMaxMB - m_initParams.m_voiceLRUBufferMB, m_initParams.m_voiceLRUMaxMB);

#ifdef _DEBUG
    if( m_initParams.m_enableAuditioning )
    {
        FMOD::NetEventSystem_Init( s_pFMODEventSystem );
    } 
#endif    
    
    m_enabled = true;

    SetStreamBufferSize(STREAM_BUFFER_SIZE);
    
	// setup default reverb
	{
		FMOD_REVERB_PROPERTIES reverbProperties = FMOD_PRESET_OFF;
		ReverbInstance* pNewInstance = new ReverbInstance("DEFAULT_REVERB"); 
		pNewInstance->m_myHandle = m_ReverbHandleFactory.Alloc(pNewInstance);
		CopyFMODToReverb(reverbProperties, pNewInstance);
		pNewInstance->m_fAlpha = 1.f;
		m_aReverbInstances.push_back(pNewInstance);
	}
    
    return;

 fail_with_result:
    HANDLE_FMOD_ERROR( result );
    _ShutdownInternals();
    return;
}

void
EventManager::_ShutdownInternals()
{
#ifdef _DEBUG
    if( m_initParams.m_enableAuditioning )
    {
        FMOD::NetEventSystem_Shutdown();
    }   
#endif

    // release our highpass dsps
    for (u32 i = 0; i < s_aHighPassFilters.size(); ++i)
    {
        s_aHighPassFilters[i]->release();
    }
    // and empty the array
    s_aHighPassFilters.clear();

    //same with low pass dsps
    for( u32 i = 0; i < s_aLowPassFilters.size(); ++i )
    {
        s_aLowPassFilters[i]->release();
    }
    s_aLowPassFilters.clear();

    if(s_pFMODEventSystem)
    {
        s_pFMODEventSystem->release();
    }
    s_pFMODEventSystem = NULL;	
    m_enabled = false;

    if (s_pMainMemPool)
    {
        free(s_pMainMemPool);
    }

    s_pMainMemPool = NULL;    
    s_uTotalMemSize = 0;
    s_uMainMemSize = 0;    
}

bool EventManager::_PassesRetriggerTest( ZLVec3D vSourcePosition, const EventProperties& properties, const Event& soundEvent, EventInstance* pInstance )
{
    if( m_enabled && !properties.m_headRelative )
    {        
        PROFILE_TIME_REGION(sg_FMODZone);
        if( soundEvent.IsValid() && properties.m_retriggerRadius > 0.f )
        {
            FMOD_RESULT result;
            FMOD::Event* pEvent = NULL;
            result = s_pFMODEventSystem->getEventBySystemID( (int)soundEvent.m_pInternalData, FMOD_EVENT_INFOONLY, &pEvent ); 
            if( result == FMOD_OK )
            {
                FMOD_RESULT result = FMOD_OK;
                FMOD_EVENT_INFO info;
                memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
                const int kMaxInstances = 100;
                FMOD::Event apInstances[kMaxInstances];
                info.instances = (FMOD_EVENT**)&apInstances;
                info.numinstances = kMaxInstances;
                result = pEvent->getInfo( NULL, NULL, &info );
                if( result == FMOD_OK && info.instances != NULL )
                {
                    int nProximateInstances = 0;
                    float sqrRetriggerRadius = Square( properties.m_retriggerRadius );
                    for( int i = 0; i < info.numinstances; ++i )
                    {
                        FMOD::Event* pEvent = (FMOD::Event*)info.instances[i];
                        if( pInstance && pInstance->m_pInternalData == pEvent )
                        {
                            continue;
                        }

                        FMOD_VECTOR FMInstancePosition;
                        result = pEvent->get3DAttributes( &FMInstancePosition, NULL, NULL );
                        if( result == FMOD_OK )
                        {
                            ZLVec3D vInstancePosition;
                            FMODDesigner::CopyFMODVectorToDF( vInstancePosition, FMInstancePosition );
                            const float sqrDistance = vInstancePosition.DistSqrd( vSourcePosition );                            
                            if( sqrDistance <= sqrRetriggerRadius )
                            {
                                ++nProximateInstances;
                                if( nProximateInstances > properties.m_maxRetriggerInstances )
                                {
                                    return false;
                                }
                            }
                        }                        
                    }
                }
            }
        }
    }
    return true;
}

EventInstance* EventManager::_PlayEvent( const Event& soundEvent, bool loops, const ZLVec3D* pvPosition, EventInstance* pInstanceOverride, const LineCode* pLineCode )
{
    EventInstance* pInstance = NULL;
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_FMODZone);
        FMOD_RESULT result = FMOD_OK;

        const bool isVoiceLine = pLineCode && *pLineCode != kINVALID_LINECODE;

        if( isVoiceLine && m_iNumActiveVoices >= kMAX_VOICE_LINES )
        {
            return NULL;
        }
                
        {
            const EventProperties* pProperties = GetEventProperties( soundEvent );    

#ifdef _DEBUG
            if (pProperties->m_duration == 0.f)
            {
                Trace(TT_Sound, TL_Warning, "Trying to play sound %s with duration 0", soundEvent.GetName().c_str());
                return NULL;
            }
#endif

            if( isVoiceLine || soundEvent.IsRawSound() )
            {
                loops = false;
            }
            else if( pProperties->m_loops )
            {
                loops = true;
            }

            if( pProperties )
            {
                if( pProperties->m_minRetriggerTime > 0.f )
                {
                    if( m_aMinRetriggerTimers.contains(soundEvent.GetName()) )
                    {
                        return pInstanceOverride;
                    }
                    else
                    {
                        m_aMinRetriggerTimers.insert( pair<STLString, float>( soundEvent.GetName(), pProperties->m_minRetriggerTime ) );
                    }
                }

                // Delayed sounds just create a stub sound instance.
                // They'll do the distance tests when the delay ends if need be.
                if( pProperties->m_soundDelayTime > 0.f && !pInstanceOverride )
                {
                    pInstance = new EventInstance( NULL, soundEvent, soundEvent.m_rawSound ); 
                    pInstance->SetFlag(EventInstance::kSIF_Delayed, true);                    
                }
                // All 3D sounds played as 3D get distance tested.                
                else if( pProperties->m_is3D && pvPosition )
                {
                    // The source might be head relative or world space relative,
                    // so we need to be sure that we get the proper world space transform.
                    ZLVec3D vSourcePosition = _GetWorldspacePosition(*pvPosition, pProperties->m_headRelative);

                    // We need to check to see if the sound passes the retrigger and cutoff checks.
                    // If we don't, it's possible to hear sounds play and get clipped out by the update code.
                    const float cutoffRadius = FSel( pProperties->m_cutoffRadius - EPSILON, pProperties->m_cutoffRadius, FLT_MAX );
                    if( ( m_vListenerPosition.DistSqrd( vSourcePosition ) > Square( cutoffRadius ) ) ||
                        ( !_PassesRetriggerTest( vSourcePosition, *pProperties, soundEvent, NULL ) ) )
                    {
                        // For looping sounds, we assume that even if they aren't audible now, they might be later,
                        // so we'll create a dummy sound instance and let the update loop take care of the rest.
                        if( loops && !pInstanceOverride )
                        {
                            assert( pLineCode == NULL );
                            pInstance = new EventInstance( NULL, soundEvent, soundEvent.m_rawSound );
                        }
                        // For non looping sounds, we assume they are short enough that the world state
                        // won't change enough during the lifetime of the sound to be worth worrying about.
                        // This check could be made smarter by looking at the duration of the sound, and
                        // the player velocity, but, well, this is good enough for now.
                        else
                        {
                            return pInstanceOverride;
                        }
                    }
                }                               

                // If we didn't create a stub instance, let's actually play an event.
                if( !pInstance )
                {
                    FMOD::Sound* pSound = NULL;

                    // For programmer sounds, replicate the behavior of FAIL_ON_MAX_STREAMS.
                    if( isVoiceLine )
                    {                        
                        if( pLineCode->GetIndex() < g_WavbankEntries.size() )
                        {
                            Trace(TT_VoiceEvent, TL_Info, "%s", pLineCode->c_str());
                            if (m_voiceLRU.IsEnabled())
                            {
                                // get the time from fmod
                                const u32 time = FMODDesigner::GetTimeMs();

                                pSound = ( FMOD::Sound* )m_voiceLRU.GetSound(*pLineCode, time, pProperties);

                            }

                            if( !pSound )
                            {
                                return NULL;
                            }
                        }
                        else
                        {                    
                            Trace(TT_Sound, TL_Warning, "Programmer sound in %s is unsupported", soundEvent.GetName().c_str() );                    
                            return NULL;
                        }
                    }


                    FMOD::Event* pEvent = NULL;
                    if( soundEvent.IsRawSound() )
                    {                                                
                        FMOD::EventProject* pProject = NULL;
                        result = s_pFMODEventSystem->getProjectByIndex( pProperties->m_projectId, &pProject );
                        if( result == FMOD_OK )
                        {
                            result = pProject->getEventByProjectID( pProperties->m_id, FMOD_EVENT_DEFAULT | FMOD_EVENT_NONBLOCKING, &pEvent );
                        }
                    }
                    else
                    {
                        result = s_pFMODEventSystem->getEventBySystemID( (int)soundEvent.m_pInternalData, FMOD_EVENT_DEFAULT | FMOD_EVENT_NONBLOCKING, &pEvent ); 
                    }

                    if( result == FMOD_OK )
                    {                    
                        result = pEvent->start();
                        if( result == FMOD_OK )
                        {
                            if( pInstanceOverride )
                            {  
                                pInstance = pInstanceOverride;
                                assert( pInstance->m_pInternalData == NULL );
                                pInstance->m_pInternalData = pEvent;                                
                            }
                            else
                            {
                                pInstance = new EventInstance( pEvent, soundEvent, false );

                                if (pSound)
                                {
                                    pInstance->SetVoiceMemoryPointer(pSound);            
                                }

                                if( pInstance->ContainsVoiceData() )
                                {                                    
                                    assert(m_iNumActiveVoices < kMAX_VOICE_LINES); // Guard against multiple increments on the same instance
                                    ++m_iNumActiveVoices;
                                }
                            }  
                            
                            result = pEvent->setCallback( _FMODEventCallback, pInstance );
                            assert( result == FMOD_OK );
                        }
                    }
                }

                if( pInstance )
                {
                    pInstance->Set3D(pProperties->m_is3D);
                    pInstance->SetHeadRelative(pProperties->m_headRelative);
                    pInstance->SetLoops(loops);
                    pInstance->UnloadOnSilence(pProperties->m_unloadOnSilence || pInstance->GetFlag(EventInstance::kSIF_UnloadOnSilence));
                    if( !pInstance->GetFlag(EventInstance::kSIF_RetriggerDisabled) && pProperties->m_retriggerRadius > 0.f )
                    {
                        pInstance->SetRetriggerRadius(pProperties->m_retriggerRadius, pProperties->m_maxRetriggerInstances);
                    }
                    if( pProperties->m_cutoffRadius > 0.f )
                    {
                        pInstance->SetCuttoffRadius( pProperties->m_cutoffRadius );
                    }
                    
                    if( pInstance->m_pInternalData )
                    {
                        // We played started a new sound instance, so we're not unloaded.
                        pInstance->SetFlag(EventInstance::kSIF_Unloaded, false);
                    }

                    // Make sure that we preserve the status of the mute flag.
                    if( pInstance->GetFlag(EventInstance::kSIF_Muted) )
                    {        
                        pInstance->MuteInternal(true);
                    }

                    if( pInstance->GetFlag(EventInstance::kSIF_Delayed ) )
                    {
                        // Delayed sounds cheat using the retest time.
                        pInstance->m_uRetestMs = (u32)(pProperties->m_soundDelayTime * 1000.f);
                    }
                    else
                    {
                        // Recheck roughly every second
                        pInstance->m_uRetestMs = (rand() % 256) + 896;
                    }

                    pInstance->MuteInternal(false);

                    // We only want to activate ducks on sounds that are actually playing/audible.
                    // Otherwise there would be a rash of seemingly inexplicable ducks.
                    if( pInstance->m_pInternalData && pProperties->m_categoriesToDuck.size() > 0 )
                    {
                        DuckSoundCategories(pInstance->m_hDuckingRequest, pProperties->m_categoriesToDuck, pProperties->m_duckingVolumes, -1.f );                
                    }
                }     
            }           
        }
        HANDLE_FMOD_ERROR( result );          
    }
   
    return pInstance;
}

bool EventManager::UnloadEvent(const Event& soundEvent, bool blockOnUnload) const
{
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_FMODZone);
        if( soundEvent.IsValid() )
        {
            Trace(TT_Sound, TL_Info, "Attempting to unload %s", soundEvent.GetName().c_str());

            FMOD_RESULT result = FMOD_OK;
            if( !soundEvent.m_rawSound )
            {
                //unload the sound
                FMOD::Event* pEvent = NULL;
                result = s_pFMODEventSystem->getEventBySystemID( (int)soundEvent.m_pInternalData, FMOD_EVENT_INFOONLY, &pEvent );
                if( result == FMOD_OK )
                {
                    FMOD::EventGroup* pGroup = NULL;
                    result = pEvent->getParentGroup( &pGroup );
                    if( result == FMOD_OK )
                    {
                        result = pGroup->freeEventData( pEvent, blockOnUnload );
                    }
                }
            }            
            HANDLE_FMOD_ERROR( result );

            return result == FMOD_OK;
        }
    }

    return true;
}

u32 EventManager::GetNumInstances(const Event& soundEvent)
{
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_FMODZone);
        if( soundEvent.IsValid() )
        {
            FMOD_RESULT result = FMOD_OK;            
            FMOD::Event* pEvent = NULL;
            if( soundEvent.m_rawSound )
            {
                const EventProperties* pProperties = GetEventProperties( soundEvent );
                if( pProperties )
                {
                    FMOD::EventProject* pProject = NULL;
                    result = s_pFMODEventSystem->getProjectByIndex( pProperties->m_projectId, &pProject );
                    if( result == FMOD_OK )
                    {
                        result = pProject->getEventByProjectID( pProperties->m_id, FMOD_EVENT_INFOONLY, &pEvent );
                    }
                }
            }
            else
            {
                result = s_pFMODEventSystem->getEventBySystemID( (int)soundEvent.m_pInternalData, FMOD_EVENT_INFOONLY, &pEvent );                
            }

            if(result == FMOD_OK && pEvent)
            {
                FMOD_EVENT_INFO info;
                memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
                result = pEvent->getInfo( NULL, NULL, &info );
                if( result == FMOD_OK )
                {
                    return info.instancesactive;
                }
            }                    

            HANDLE_FMOD_ERROR( result );
        }        
    }    
    return 0;
}

static FMOD::Sound* sg_pRecordedSound = NULL;
static const int sg_micIndex = 0;

/*! Starts recording into a new event. */
void EventManager::StartRecording()
{    
    if( m_enabled )
    {
        assert(sg_pRecordedSound == NULL);

        FMOD::System* pSystem = NULL;
        s_pFMODEventSystem->getSystemObject( &pSystem );
        int numdrivers = 0;
        pSystem->getRecordNumDrivers(&numdrivers);
        if( numdrivers > 0 )
        {            
            int recordingFrequency = 0;

            FMOD_GUID guid;
            pSystem->getRecordDriverInfo(sg_micIndex, 0, 0, &guid);
            
            // A best guess for what's common.
            recordingFrequency = 48000;

            if( recordingFrequency > 0 )
            {
                pSystem->recordStop(sg_micIndex);

                FMOD_CREATESOUNDEXINFO exinfo;
                memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

                exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
                exinfo.numchannels      = 1;
                exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
                exinfo.defaultfrequency = recordingFrequency;
                exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;

                pSystem->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sg_pRecordedSound);

                pSystem->recordStart(sg_micIndex, sg_pRecordedSound, false);

				// immediately start playing back the recorded sound so we can sample the pitch
				// horrible hack, seems to need a delay for some reason.  don't question.
#ifndef MOAI_OS_WINDOWS
                usleep ( 200000 );
#else
                // WARNING: sleep on windows is not quite as precise
                Sleep ( 200 );
#endif
				pSystem->playSound(FMOD_CHANNEL_REUSE, sg_pRecordedSound, false, &sg_pPlaybackChannel);
				sg_pPlaybackChannel->setVolume(0);
            }
        }                
    }    
}

void EventManager::StopRecording(Event* pOutSound)
{
    if( m_enabled )
    {
        FMOD::System* pSystem = NULL;
        s_pFMODEventSystem->getSystemObject( &pSystem );
        pSystem->recordStop(sg_micIndex);

        if( sg_pRecordedSound )
        {
            if( pOutSound )
            {            
                if( m_initParams.m_processingEventName.length() > 0 )
                {
                    pOutSound->m_eventName = m_initParams.m_processingEventName;                
                }
                else
                {
                    pOutSound->m_eventName = "RecordedSound";              
                }                
                pOutSound->m_pInternalData = sg_pRecordedSound;               
                pOutSound->m_rawSound = true;
            }
            else
            {
                sg_pRecordedSound->release();
            }
            sg_pRecordedSound = NULL;
        }
    }
}

void EventManager::CreateSound( const char* pData, u32 size, u32 frequency, Event& newSound )
{    
    if( pData != NULL && size > 0 )
    {           
        FMOD::System* pSystem = NULL;
        if( s_pFMODEventSystem->getSystemObject( &pSystem ) == FMOD_OK )
        {        
            char* pLittleEndianBuffer = new char[size];
            // pData is native-endian; needs to be little-endian
            for(u32 i = 0; i < size; i += 2 )
            {
                const u16 sample = *(u16*)(pData+i);
                pLittleEndianBuffer[i  ] = (char)(sample >> 0);
                pLittleEndianBuffer[i+1] = (char)(sample >> 8);
            }

            FMOD_CREATESOUNDEXINFO exinfo;
            memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
            exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
            exinfo.numchannels      = 1;
            exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
            exinfo.defaultfrequency = frequency;
            exinfo.length           = size;

            FMOD::Sound* pSound = NULL;
            FMOD_RESULT result = pSystem->createSound(pLittleEndianBuffer, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_OPENRAW, &exinfo, &pSound);

            delete[] pLittleEndianBuffer;

            if( result == FMOD_OK )
            {
                if( m_initParams.m_processingEventName.length() > 0 )
                {
                    newSound.m_eventName = m_initParams.m_processingEventName;                
                }
                else
                {
                    newSound.m_eventName = "ProcessedSound";               
                }                        
                newSound.m_pInternalData = pSound;
                newSound.m_rawSound = true;
                return;
            }
        }
    }

    newSound = Event();
}

/*! Destroys a low level FMOD sound contained in a Event. */
void EventManager::DestroySound( Event& sound ) const
{
    if( sound.IsValid() )
    {
        if( sound.IsRawSound() )
        {
            FMOD::Sound* pSound = (FMOD::Sound*)sound.m_pInternalData;
            FMOD_RESULT result = pSound->release();
            sound.m_pInternalData = NULL;
            HANDLE_FMOD_ERROR(result);
        }
        else
        {
            Trace(TT_Sound, TL_Warning, "Tried to destroy non-raw sound: %s", sound.GetName().c_str());
        }
    }    
}

void EventManager::_SetMicrophoneTransform(const ZLVec3D &vPos, const ZLVec3D& vVelocity, const ZLVec3D &vLook, const ZLVec3D &vUp )
{
#ifndef MOAI_OS_WINDOWS
    #ifndef _isnan
        #define _isnan isnan
    #endif
#endif
    assert( !_isnan( vPos.mX ) && !_isnan( vPos.mY ) && !_isnan( vPos.mZ ) );
    // check for bad microphone orientation
    assert( vLook.Compare( vLook, 0.f ) && vUp.Compare( vUp, 0.f ) );

    if(m_enabled)
    {
        PROFILE_TIME_REGION_FAST(sg_FMODZone);
        FMOD_RESULT result  = FMOD_OK;
        FMOD_VECTOR vFMPosition;
        FMOD_VECTOR vFMVelocity;
        FMOD_VECTOR vFMLook;
        FMOD_VECTOR vFMUp;
        FMODDesigner::CopyDFVectorToFMOD( vFMPosition, vPos );
        FMODDesigner::CopyDFVectorToFMOD( vFMVelocity, vVelocity );
        FMODDesigner::CopyDFVectorToFMOD( vFMLook, vLook );
        FMODDesigner::CopyDFVectorToFMOD( vFMUp, vUp );        
        result = s_pFMODEventSystem->set3DListenerAttributes(0, &vFMPosition, &vFMVelocity, &vFMLook, &vFMUp);

        HANDLE_FMOD_ERROR( result );
    }    
}

void EventManager::_PauseSoundCategory(const SoundCategoryState* pCategoryState, bool bPause)
{    
    assert(pCategoryState);
    if(m_enabled)
    {
        PROFILE_TIME_REGION(sg_FMODZone);        
        FMOD_RESULT result = FMOD_OK;
        
        //get the category
        FMOD::EventCategory* pEventCat = (FMOD::EventCategory*)pCategoryState->m_pInternalData;
        result = pEventCat->setPaused(bPause);        
        
        HANDLE_FMOD_ERROR( result );
    } 
}

static FMOD_RESULT GetReverbPreset( const STLString& name, FMOD_REVERB_PROPERTIES& reverb )
{
	int* pReverbID = NULL;     
    if( s_ReverbIDTable.contains( name ) )
    {
        pReverbID = &s_ReverbIDTable[name];
    }

	if( pReverbID )
	{
		return FMODDesigner::s_pFMODEventSystem->getReverbPresetByIndex( *pReverbID, &reverb );
	}
	else
	{
        // if there's no cache for it, try to grab the index
        FMOD_RESULT result = FMOD_OK;        

        // Grab the project
        int id = -1;
        result = FMODDesigner::s_pFMODEventSystem->getReverbPreset( name.c_str(), &reverb, &id );
        if( result == FMOD_OK )
        {
            // save the index so we can use that later
            s_ReverbIDTable.insert( pair<STLString, int>( name, id ) );
            return result;
        }  
	}

    Trace(TT_Sound, TL_Warning, "Could not find reverb preset %s", name.c_str());
    return FMOD_ERR_PLUGIN; /* ? */
}


void EventManager::_UpdateReverbInstances(float fDeltaTime)
{
	PROFILE_TIME_REGION(sg_FMODZone);

	// multiply our reverb stack until we hit 1.f alpha
	ReverbInstance totalReverb("");
	float fUsedAlpha = 0.f;
	for(int i = m_aReverbInstances.size() -1; i >= 0; i--)
	{
		float fAlpha = Min( (1.f-fUsedAlpha), m_aReverbInstances[i]->m_fAlpha);

		totalReverb.AddReverb( m_aReverbInstances[i], fAlpha);

		fUsedAlpha += fAlpha;

		if( fUsedAlpha >= 1.f )
		{
			break;
		}
	}

	assert(fUsedAlpha >= 1.f);

	// initialize reverb properties to an fmod data so the values we dont touch are reasonable
	FMOD_REVERB_PROPERTIES reverbProperties = FMOD_PRESET_OFF;
	FMOD_RESULT result = FMOD_OK;
	if (m_DefaultReverb.length() > 0)
	{
		result = GetReverbPreset( m_DefaultReverb, reverbProperties );
	}

	if( result == FMOD_OK )
	{
		CopyReverbToFMOD(reverbProperties, &totalReverb );
		result = s_pFMODEventSystem->setReverbAmbientProperties( &reverbProperties );
	}
	HANDLE_FMOD_ERROR(result);
}

void EventManager::_UpdateSoundCategoryVolume(const SoundCategoryState* pCategoryState)
{
    assert(pCategoryState);
    if(m_enabled)
    {    
        PROFILE_TIME_REGION_FAST(sg_FMODZone);
        FMOD_RESULT result = FMOD_OK;
        
        FMOD::EventCategory* pEventCat = (FMOD::EventCategory*)pCategoryState->m_pInternalData;
        float fVolume = pCategoryState->m_bMuted ? 0.f : pCategoryState->m_fOriginalVolume * pCategoryState->m_fDuckedVolume * pCategoryState->m_fVolume;
        result = pEventCat->setVolume(fVolume);        
        
        HANDLE_FMOD_ERROR( result );
    }
}

EventManager::SoundCategoryState* EventManager::_InitSoundCategory(const STLString& category) const
{    
    if(m_enabled)
    {
        PROFILE_TIME_REGION_FAST(sg_FMODZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::EventCategory* pEventCat = NULL;    

        result = s_pFMODEventSystem->getCategory( category.c_str(), &pEventCat );

        if (pEventCat != NULL)
        {
            float fOriginalVolume = 0.f;
            result = pEventCat->getVolume(&fOriginalVolume);
            return new SoundCategoryState(category, fOriginalVolume, pEventCat);
        }

        HANDLE_FMOD_ERROR( result );
    }

    return NULL;
}

// Uses SetSoundCategoryVolume to implement muting since
// FMOD doesn't handle regular muting very well
void EventManager::_MuteSoundCategory(const SoundCategoryState* pCategoryState, bool bMute)
{    
    if(m_enabled)
    {
        _UpdateSoundCategoryVolume( pCategoryState );
    }
}



void EventManager::SetDefaultReverb( const STLString& reverbName )
{
    assert( reverbName.length() > 0 );
	assert( m_aReverbInstances.size() > 0);
	FMOD_REVERB_PROPERTIES reverbProperties;
	FMOD_RESULT result = GetReverbPreset( reverbName, reverbProperties );
	if( result == FMOD_OK )
	{
		ReverbInstance* pNewInstance = m_aReverbInstances[0];
		CopyFMODToReverb(reverbProperties, pNewInstance);
		m_DefaultReverb = reverbName;
	}
	else
	{
		Trace(TT_Sound, TL_Warning, "Default Reverb Preset %s not found skipping", reverbName.c_str());
		HANDLE_FMOD_ERROR(result);
		m_DefaultReverb = "";
	}
}

void EventManager::ClearDefaultReverb()
{    
	assert(m_aReverbInstances.size() > 0);

	FMOD_REVERB_PROPERTIES reverbProperties = FMOD_PRESET_OFF;
	ReverbInstance* pDefaultInstance = m_aReverbInstances[0];
	CopyFMODToReverb(reverbProperties, pDefaultInstance);

	m_DefaultReverb = "";
}

void EventManager::ClearReverb(ReverbInstance* pInstance)
{
        PROFILE_TIME_REGION(sg_FMODZone);
	if( pInstance == NULL) return;	
    vector<ReverbInstance*>::iterator it = find( m_aReverbInstances.begin(), m_aReverbInstances.end(), pInstance);                        
    if ( it != m_aReverbInstances.end() )
        {
        m_aReverbInstances.erase( it );
        }
	m_ReverbHandleFactory.Free(pInstance->m_myHandle);                
	delete pInstance;
}


void EventManager::SetReverbAlpha(ReverbInstance* pReverbInstance, float fAlpha)
{
	PROFILE_TIME_REGION(sg_FMODZone);
	if( pReverbInstance == NULL) return;
	for( u32 i =0; i < m_aReverbInstances.size(); i++)
	{
		if( m_aReverbInstances[i] == pReverbInstance )
        {
			m_aReverbInstances[i]->m_fAlpha = fAlpha;
		}
	}
}

ReverbHandle EventManager::AddReverb( const STLString& name)
{
    if( m_enabled &&
        name.length() > 0 &&
        m_initParams.m_enableEnvironmentalReverb )
    {
        PROFILE_TIME_REGION(sg_FMODZone);

		// try to fill data
        FMOD_REVERB_PROPERTIES reverbProperties;
        FMOD_RESULT result = GetReverbPreset( name, reverbProperties );
        if( result == FMOD_OK )
        {
			ReverbInstance* pNewInstance = new ReverbInstance(name ); 
			pNewInstance->m_myHandle = m_ReverbHandleFactory.Alloc(pNewInstance);
			CopyFMODToReverb(reverbProperties, pNewInstance);
			m_aReverbInstances.push_back( pNewInstance );
			return pNewInstance->m_myHandle;
		}
		else
        {
            Trace(TT_Sound, TL_Warning, "Reverb Preset %s not found skipping", name.c_str());
			HANDLE_FMOD_ERROR(result);
        }
    }

    return ReverbHandle();
}

SoundOccluder EventManager::CreateSoundOccluder()
{
    if( m_enabled )
    {
        PROFILE_TIME_REGION(sg_FMODZone);

        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );
        if( result == FMOD_OK )
        {
            FMOD::Geometry* pGeom = NULL;
            result = pSystem->createGeometry( 6, 24, &pGeom );
            if( result == FMOD_OK )
            {                   
                // Verts for the unit cube
                FMOD_VECTOR cube[24] = 
                { 
                    { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, //+X face 
                    {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, //-X face 
                    {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, //+Y face 
                    {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}, //-Y face 
                    {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, //+Z face 
                    {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,- 0.5f, -0.5f}, //-Z face 
                };
                
                for(u32 i = 0; i < 6; ++i)
                { 
                    pGeom->addPolygon( 1, 1, 1, 4, cube + (4* i), NULL); // pointer arithmetic to get face i 
                }
                
                return SoundOccluder( pGeom );
            }
        }

        HANDLE_FMOD_ERROR(result);
    }

    return SoundOccluder();
}

bool EventManager::_LoadSoundPropertyData( const Event& soundEvent, EventProperties* pSoundProperty ) const
{
    FMOD_RESULT result = FMOD_OK;
    FMOD::Event* pEvent = NULL;    

    // Grab the project
    const char* szName = soundEvent.GetName().c_str();
    result = s_pFMODEventSystem->getEvent( szName, FMOD_EVENT_INFOONLY,&pEvent );
    if( result == FMOD_OK && pEvent )
    {
        pSoundProperty->m_name = soundEvent.GetName();

        // Need the project index
        FMOD::EventGroup* pGroup = NULL;        
        result = pEvent->getParentGroup( &pGroup );
        if( result != FMOD_OK )
        {
            return false;
        }
        FMOD::EventProject* pProject = NULL;
        result = pGroup->getParentProject( &pProject );
        if( result != FMOD_OK )
            {
            return false;
                        }
        FMOD_EVENT_PROJECTINFO projinfo;        
        memset( &projinfo, 0, sizeof( FMOD_EVENT_PROJECTINFO ) );                
        pProject->getInfo( &projinfo );
        pSoundProperty->m_projectId = projinfo.index;            

        // Now the event info
        FMOD_EVENT_INFO info;        
        memset( &info, 0, sizeof( FMOD_EVENT_INFO ) );
        pEvent->getInfo( NULL, NULL, &info );
        
        pSoundProperty->m_id = info.projectid;        
        pSoundProperty->m_loops = info.lengthms < 0;
        pSoundProperty->m_duration = info.lengthms / 1000.f;        

        // First, we extract some stock properties.
        int eventMode = 0;            
        pEvent->getPropertyByIndex( FMOD_EVENTPROPERTY_MODE, &eventMode );
        pSoundProperty->m_is3D = ( eventMode == FMOD_3D );

        int headRelative = 0;
        pEvent->getPropertyByIndex( FMOD_EVENTPROPERTY_3D_POSITION, &headRelative );
        pSoundProperty->m_headRelative = ( headRelative == FMOD_3D_HEADRELATIVE );

        pEvent->getPropertyByIndex( FMOD_EVENTPROPERTY_3D_MAXDISTANCE, &pSoundProperty->m_cutoffRadius );

        // Then we extract the user properties.
        // We use names where we can to speed up the compares.

        int nFMODProperties = 0;
        pEvent->getNumProperties( &nFMODProperties );

        for( int i = FMOD_EVENTPROPERTY_USER_BASE; i < nFMODProperties; ++i )
    {
            char* propertyString = NULL;                    
            pEvent->getPropertyInfo( &i, &propertyString );                    
            STLString propertyName( propertyString );

            if( propertyName == "UnloadOnSilence" )
        {
                int unloadOnSilence = 0;
                pEvent->getPropertyByIndex( i, &unloadOnSilence );
                pSoundProperty->m_unloadOnSilence = ( unloadOnSilence != 0 );
            }
            else if( propertyName == "MaxRetriggerInstances" )
            {
                pEvent->getPropertyByIndex( i, &pSoundProperty->m_maxRetriggerInstances );
            }
            else if( propertyName == "MinRetriggerTime" )
                {
                pEvent->getPropertyByIndex( i, &pSoundProperty->m_minRetriggerTime );
            }
            else if( propertyName == "DelayTime" )
                    {
                pEvent->getPropertyByIndex( i, &pSoundProperty->m_soundDelayTime );
            }
            else if( propertyName == "RetriggerRadius" )
                        {
                pEvent->getPropertyByIndex( i, &pSoundProperty->m_retriggerRadius );
                        }
            else if( propertyName == "DisableCutoff" )
            {
                int disableCutoff = 0;
                pEvent->getPropertyByIndex( i, &disableCutoff );
                if( disableCutoff == 1 )
        {
                    pSoundProperty->m_cutoffRadius = -1.f;
        }
    }
            else if( strstr( propertyString, "Duck_" ) != 0 )
    {
                STLString categoryName( propertyString + 5 );
                float duckedVolume = 1.f;
                pEvent->getPropertyByIndex( i, &duckedVolume );
                pSoundProperty->m_categoriesToDuck.push_back( categoryName );
                pSoundProperty->m_duckingVolumes.push_back( duckedVolume );
        }
    }

        return true;
    }

    return false;
}

bool EventManager::_LoadGroupData( const STLString& groupPath, GroupData* pGroupData ) const
{
    FMOD_RESULT result = FMOD_OK;
    FMOD::EventGroup* pGroup = NULL;    

    result = s_pFMODEventSystem->getGroup( groupPath.c_str(), false, &pGroup );    
    if( result == FMOD_OK && pGroup )
    {
        pGroupData->m_path = groupPath;

        int groupId = -1;
        char* pThrowaway = NULL;

        result = pGroup->getInfo( &groupId, &pThrowaway );
        if( result == FMOD_OK )
    {
            FMOD::EventProject* pProject = NULL;
            result = pGroup->getParentProject( &pProject );
            if( result == FMOD_OK )
        {
                FMOD_EVENT_PROJECTINFO info;        
                memset( &info, 0, sizeof( FMOD_EVENT_PROJECTINFO ) );                
                pProject->getInfo( &info );
                pGroupData->m_projectId = info.index;
				pGroupData->m_id = groupId;

                return true;
            }                    
        }
    }

    return false;
}

void EventManager::_InternalUpdate(float fDeltaTime)
{    
    if (m_enabled)
    {
        PROFILE_TIME_REGION(sg_FMODZone);
                
        s_pFMODEventSystem->update();  

        FMOD::System* pSystem = NULL;
        s_pFMODEventSystem->getSystemObject(&pSystem);

        if( m_initParams.m_enableDistantLowpass || m_initParams.m_enableNear2DBlend )
        {
            float fInvDistantLowpassDelta = m_distantLowpassRange.Delta();
            if( m_initParams.m_enableDistantLowpass )
            {
                // this formerly used FastReciprocal -- optimized for specific platforms
                fInvDistantLowpassDelta = FSel( fInvDistantLowpassDelta - EPSILON, ( 1.f / fInvDistantLowpassDelta ), 0.f );
                //TraceIf( fInvDistantLowpassDelta == 0.f, TT_Sound, TL_Warning, "Distant lowpass enabled without a valid range set." );
            }

            float fInvNear2DBlendDelta = m_near2DBlendRange.Delta();
            if( m_initParams.m_enableNear2DBlend )
            {
                // this formerly used FastReciprocal -- optimized for specific platforms
                fInvNear2DBlendDelta = FSel( fInvNear2DBlendDelta - EPSILON, ( 1.f / fInvNear2DBlendDelta ), 0.f );
                //TraceIf( fInvNear2DBlendDelta == 0.f, TT_Sound, TL_Warning, "Near 2D Blend enabled without a valid range set." );
            }

            for( u32 i = 0; i < m_initParams.m_nVirtualChannels; ++i )
            {
                FMOD::Channel* pChannel = NULL;
                if( pSystem->getChannel( i, &pChannel ) == FMOD_OK )
                {
                    FMOD_MODE mode = 0;
                    if( pChannel->getMode(&mode) == FMOD_OK &&
                        (mode & FMOD_3D) && (mode & FMOD_3D_HEADRELATIVE) == 0 )
                    {
                        FMOD_VECTOR position;
                        if( pChannel->get3DAttributes( &position, NULL ) == FMOD_OK )
                        {
                            float fDistance = m_vListenerPosition.Dist( ZLVec3D( position.x, position.y, position.z ) );

                            // Distant lowpass is a lowpass filter added to everything greater
                            // than a certain distance from the microphone to make it sound muffled/far away.                            
                            if( m_initParams.m_enableDistantLowpass )
                            {
                                float occlusion = Lerp( 0.f, m_maxDistantOcclussion, (fDistance - m_distantLowpassRange.min) * fInvDistantLowpassDelta );
                                occlusion = Pin( occlusion, 0.f, m_maxDistantOcclussion );
                                pChannel->set3DOcclusion( occlusion, 0.f );
                            }

                            // Near 2D Blend blends 3D sounds to be 2D within a certain range of the camera.
                            // This mimics a very poor man's HRTF.
                            if( m_initParams.m_enableNear2DBlend )
                            {
                                float fPanLevel = Lerp( m_near2DBlendLevel, 1.f, (fDistance - m_near2DBlendRange.min) * fInvNear2DBlendDelta );
                                fPanLevel = Pin( fPanLevel, m_near2DBlendLevel, 1.f );
                                pChannel->set3DPanLevel( fPanLevel );
                            }
                        }
                    }                                    
                }
            }
        }


#ifdef _DEBUG
        if( m_initParams.m_enableAuditioning )
        {
            FMOD::NetEventSystem_Update();
        }
#endif


    }
}

int EventManager::GetTotalMemory() const
{
	int totalMem = 0;
	FMOD::Memory_GetStats(&totalMem, NULL);
	return totalMem;
}

void EventManager::GetEventList(EventAndInstanceArray& aOutput,int nMinActiveInstances) const
{
	_GetEventListInternal(aOutput, nMinActiveInstances, vector<bool>());
}

void EventManager::_GetEventListInternal(EventAndInstanceArray& aOutput,int nMinActiveInstances, const vector<bool>& desiredProjects) const
{
#ifdef _DEBUG
    int nProjects = 0;
    if( s_pFMODEventSystem->getNumProjects( &nProjects ) == FMOD_OK )
    {
        for( int i = 0; i < nProjects; ++i )
        {
			if ((int)desiredProjects.size() > i && !desiredProjects[i])
			{
				continue;
			}

            FMOD::EventProject* pProject = NULL;            
            if( s_pFMODEventSystem->getProjectByIndex( i, &pProject ) == FMOD_OK )
            {                   
                int nEvents = 0;
                if( pProject->getNumEvents( &nEvents ) == FMOD_OK )
                {                
                    for( int j = 0; j < nEvents; ++j )
                    {                            
                        FMOD::Event* pEvent = NULL;
                        if( pProject->getEventByProjectID( j, FMOD_EVENT_INFOONLY, &pEvent ) == FMOD_OK )
                        {
                            FMOD_EVENT_INFO info;
                            memset( &info, 0, sizeof(FMOD_EVENT_INFO) );
                            char* pEventName = 0;
                            if( pEvent->getInfo( NULL, &pEventName, &info ) == FMOD_OK )
                            {
                                if( info.instancesactive >= nMinActiveInstances )
                                {
                                    new EventAndInstance(STLString(pEventName),info.instancesactive);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#endif //_DEBUG
}
