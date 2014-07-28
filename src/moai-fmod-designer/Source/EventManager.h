//----------------------------------------------------------------------------------------
// EventManager.h
//----------------------------------------------------------------------------------------

#if defined(_MSC_VER) && !defined(NO_PRAGMA_ONCE)
#  pragma once
#endif

#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

#include "Common.h"
#include "Event.h"
#include "EventHandle.h"
#include "ReverbHandle.h"
#include "VoiceLRU.h"
#include "SoundInitParams.h"
#include "SoundOccluder.h"
#include <zl-util/headers.h>
#include <fmod_event.hpp>
#include <fmod_errors.h>

namespace FMODDesigner
{
    // Our streaming sounds are 40 KBps.  Deafult stream buffer is 16K (400ms).
    // We have up to 4 streams (2 ambient, 2 music), each double-buffered.
    // Total memory use is therefore
    // 
    //   (4 * STREAM_BUFFER_SIZE) * 2
    //
    // FILE_BLOCK_ALIGN is increased to attempt to keep FMOD from making
    // numerous tiny reads, which destroys performance.  Setting it to 0
    // causes all the reads to be raw (not truncated to buffer size, but
    // also not increased to buffer size either), at the expense of them not
    // being aligned.
    // 
    #  define STREAM_BUFFER_SIZE          (16 * KILO)
    #  define VOICE_STREAM_BUFFER_SIZE    (16 * KILO)  /* currently unused */
    #  define FILE_BLOCK_ALIGN            ( 2 * KILO) // this line should be revisited per platform

    #define MAX_STREAMS_PER_BANK 3

    #ifdef _DEBUG
    extern void PrintFMODError( FMOD_RESULT result );
    #define HANDLE_FMOD_ERROR( result ) FMODDesigner::PrintFMODError( result );    
    #else
    #define HANDLE_FMOD_ERROR( result ) (void)result
    #endif

    extern FMOD::EventSystem* s_pFMODEventSystem;

    class PlatformData;
    class EventInstance;
    class ReverbInstance;
    class DuckingRequestHandle;
    struct EventProperties;
    class LineCode;
    class SoundDSP;

    struct EventAndInstance
    {
        EventAndInstance( STLString eventString, int eventInstance ) : 
            m_eventString( eventString ), 
            m_eventInstance( eventInstance )
        {}

        STLString m_eventString;
        int       m_eventInstance;
    };
    typedef vector< EventAndInstance > EventAndInstanceArray;

    #define kMinDB (-60.f)
    const float cg_fITLowpassLimit = 22000.f;
    const float cg_fITHighpassLimit = 22000.f;

    void CopyDFVectorToFMOD( FMOD_VECTOR& vFMOD, const ZLVec3D& vDF );
    void CopyFMODVectorToDF( ZLVec3D& vDF, const FMOD_VECTOR& vFMOD );
    void GetEventParts( FMOD::ChannelGroup* pChannelGroup, vector<FMOD::Channel*>& apChannels, vector<FMOD::Sound*>* papSounds = NULL );
    bool IsSoundLoading( FMOD::Sound* pSound );
    void GetAudioPath( STLString& outPath, bool getFullPath );
    u32 GetTimeMs();

    inline float DecibelsToNormalizedVolume(float volumeDb)         
    { 
        return Pin<float>(pow(10.f, volumeDb/20.f), 0.f, 1.f);
    }

    inline float NormalizedVolumeToDecibels(float volumeNormalized)
    { 
        // log10 will explode if we give it 0, but should be ok with very small values
        return FSel( volumeNormalized - EPSILON, Pin(20.f * log10f(volumeNormalized), kMinDB, 0.f), kMinDB );    
    }


    /**
    * \class EventManager
    * \brief Manages loading and playback of sounds and sound "events"
    * 
    * The EventManager is responsible for allocation of resources for sounds
    * and sound instances.  It cleans up all events and instances when the
    * sound completes or the app shuts down.
    */
    class EventManager
    {
        struct SoundCategoryState;
        struct GroupData;
    public:
        /** @return true if project exists. */
        static bool VoiceDataExists( const char* szProject );
        static const char* GetVoiceEncryptionKey();
        static const char* GetSoundEncryptionKey();

        //---------------------------------------------------------------------------------------
        // Initialization functions

        EventManager();
        ~EventManager();

        /** Pass m_enableSoundSystem=false to initialize EventManager, but leave it disabled
         * @return true if the sound system is enabled.
         */
        bool Init(const SoundInitParams& );

        /** Shuts down the sound manager and does cleanup */
        void Shutdown();

        /** Returns true if the sound system is initialized and enabled */
        bool IsEnabled() const                                      { return m_enabled; }

        /**
        * Updates the sound system.
        * Should be called roughly every game tick.
        */
        void Update(float fDeltaTime);

        /** Returns platform-specific low-level sound system handle (XAudio2 on 360) */
        void* GetPlatformOutputHandle();

        /** Sets the sizes of all stream buffers created after this call. */
        void SetStreamBufferSize(u32 nBytes);

        /** Returns the current stream buffer size. */
        u32 GetStreamBufferSize() const;    

        //---------------------------------------------------------------------------------------
        // Data/Memory Management API

        /**
         * Loads a project from disk but does not load any wav or instance data.
         * Projects must be loaded before sounds can be played from them.
         */
        bool LoadProject( const char* szProject );

        /**
         * Unloads all data associated with a particular project.
         * Completely flushes all memory associated with the project.
         * Returns true if the project is no longer loaded after this call.
         */
        bool UnloadProject( const char* szProject );

        /**
         * Calls LoadProject and does all attendant special voice load stuff as well
         */
        bool LoadVoiceProject( const char* szProject, std::vector<std::pair<cc8*, float> >* lineCodeInfo );

        /**
        * Calls UnloadProject and does all attendant special voice unload stuff as well
        */
        bool UnloadVoiceProjects();

        /**
        *   High demand voice line that must go into the voice LRU now.
        *   Returns true if the line is loaded and ready to play.
        */
        void AddLineToLRU(const Event& soundEvent, const LineCode& lineCode);

        /**
        *   Returns true if and only if the given linecode's voice data is in a "loading" state.
        *   Note that this is NOT simply the negation of AddLineToLRU's return value.
        */
        bool IsLineLoading(const LineCode& lineCode);

        /**
        * Loads the wave data and instance data associated with a particular group.
        * Groups are reference counted internally, with each call to LoadGroup incrementing the ref count.
        * szGroup should be of the form ProjectName/GroupName
        * Passing true for persistant means that this group is expected to be around for the entire duration of the app
        * Passing true for blockOnLoad means that the main thread will block while loaded wave data for this group.
        * Returns true if the project is loaded after this call.
        */
        bool LoadGroup( const STLString& groupPath, bool bPersistant, bool blockOnLoad );

        /**
        * Unloads the wave data and instance data associated with a particular group.
        * Groups may not be unloaded immediately either because their reference count is not zero,
        * or because the sound system is not ready to release the group.
        * szGroup should be of the form ProjectName/GroupName
        * if bUnloadImmediately is false, it'll wait X seconds before trying to unload
        * if bUnloadImmediately is true, it'll try to unload now, but never blocks
        * Returns true if the Group is no longer loaded after this call.    
        */
        bool UnloadGroup( const STLString& groupPath, bool bUnloadImmediately = false );  


        /**
        * Unloads all the pending unload groups, use between missions, etc
        */
        void UnloadPendingUnloads(bool bBlockOnUnload = false);

        /** Creates a sound from a buffer of raw PCM data. */
        void CreateSound( const char* pData, u32 size, u32 frequency, Event& newSound );

        /** Destroys a low level FMOD sound contained in a Event. */
        void DestroySound( Event& sound ) const;

        //---------------------------------------------------------------------------------------
        // Play Sounds API

        /**
        * Plays a sound in 2D.
        * Calling this function on 3D sounds is undefined.
        * loopSound will force the sound to loop even if it does not in the data.
        * wavName should be only used for programmer sounds (a very advanced feature).
        */
        EventHandle PlayEvent2D(const Event& soundEvent, bool loopSound = false, const LineCode* pLineCode = NULL);

        /**
        * Plays a sound in 3D.
        * Calling this function on 2D sounds is harmless, but not advised.
        * All 3D sounds must pass in a velocity if using Doppler.
        * loopSound will force the sound to loop even if it does not in the data.
        * wavName should be only used for programmer sounds (a very advanced feature).
        */
        EventHandle PlayEvent3D(const Event& soundEvent, const ZLVec3D& vPos, bool loopSound = false, const ZLVec3D& vVelocity = ZLVec3D( 0.f, 0.f, 0.f ), const LineCode* pLineCode = NULL);

        /** Stops all events/sounds from playing. */
        void StopAllEvents();

        /** Stops all events/sounds from playing. */
        void MuteAllEvents( bool bSetting ); 

        /**
        * Unloads the data associated with an Event.
        * All instances of this Event will be stopped when this call is made.
        * Returns true if the Event is no longer loaded after this call.
        */
        bool UnloadEvent(const Event& soundEvent, bool blockOnUnload) const;

        /** Returns the number of active instances of a given sound. */
        u32 GetNumInstances(const Event& soundEvent);

        /** Returns the properties associated with an Event */
        const EventProperties* GetEventProperties(const Event& soundEvent) const;

        //---------------------------------------------------------------------------------------
        // Microphone Recording API

        /** Starts recording. */
        void StartRecording();

        /**
         * Stops recording into the original sound event.
         * Passing NULL will wipe the record buffer.
         */    
        void StopRecording(Event* pOutSound = NULL);

        //---------------------------------------------------------------------------------------
        // Process Sounds API
            
        /** WIP system to DSP process the sound contained in the event. */
        EventHandle ProcessSound( const Event& soundEvent, const SoundDSP* pDSPChain, bool processSilently );

        /** Creates a new Event from the results of the last call to ProcessSound. */
        void CreateSoundFromLastProcess( Event& newSound );    

	    /** Very basic pitch detection */
	    float GetFrequencyOfRecordingSound();

        //---------------------------------------------------------------------------------------
        // 3D Microphone/Listener API
    	
        /**
        * Sets the transform for the microphone
        * All a velocity must be passed in if using Doppler.
        */
        void SetMicrophoneTransform(const ZLVec3D &vPos, const ZLVec3D& vVelocity = ZLVec3D(0.f,0.f,0.f), const ZLVec3D &vLook = ZLVec3D(0.f, 0.f, 1.f), const ZLVec3D &vUp = ZLVec3D(0.f, 1.f, 0.f), const ZLVec3D& vRight = ZLVec3D(-1.f,0.f,0.f) );    

        /** Returns the position of the microphone */
        const ZLVec3D& GetMicrophonePosition() const   { return m_vListenerPosition; }
        const ZLVec3D& GetMicrophoneVelocity() const   { return m_vListenerVelocity; }
        const ZLVec3D& GetMicrophoneForward() const    { return m_vListenerForward; }
        const ZLVec3D& GetMicrophoneUp() const         { return m_vListenerUp; }

        //---------------------------------------------------------------------------------------
        // Category (Mixing bus) API    
        
        /** Mutes all sounds in a particular sound category. */
        void MuteSoundCategory(const STLString& category, bool bMute);	

        /** Returns the mute status of a particular sound category. */
        bool IsSoundCategoryMuted(const STLString& category) const;    

        /** Pauses all sounds in a given category */
        void PauseSoundCategory(const STLString& category, bool bPause);

        /** Returns the pause status of a given category */
        bool IsSoundCategoryPaused(const STLString& category) const;    

        /** Sets the volume for a sound category */
        void SetSoundCategoryVolume(const STLString& category, float fVolume);

        /** Returns the volume of a given category */
        float GetSoundCategoryVolume(const STLString& category) const;

        //---------------------------------------------------------------------------------------
        // Ducking API

        /**
        * Ducks the specified categories for the specified amount of time.
        * A duration of -1 means "forever" or "until I tell you otherwise".
        */
	    void DuckSoundCategories(DuckingRequestHandle& handle, const vector<STLString>& aAffectedCategories, const vector<float>& aVolumes, float duration );

        /**
        * Changes the volumes of an existing duck.    
        */
        void ChangeDuckVolumes(DuckingRequestHandle& handle, const vector<float>& aVolumes );

        /* Removes the ducking on a specific sound category */
        void UnDuckSoundCategories(DuckingRequestHandle& handle);	

        //---------------------------------------------------------------------------------------
        // Environmental Reverb API   

        /** Set the default regional reverb */
        void SetDefaultReverb( const STLString& name );

        /** Clear the default regional reverb */
        void ClearDefaultReverb();

        /** Create an regional reverb */
        ReverbHandle AddReverb( const STLString& name );

	    void ClearReverb(ReverbInstance* pInstance);

	    void SetReverbAlpha(ReverbInstance* pReverbInstance, float fAlpha);

	    const vector<ReverbInstance*>& GetActiveReverbs() const { return m_aReverbInstances; }

        /** Sets a lowpass filter on distant sounds */
        void SetDistantOcclusion( float minRange, float maxRange, float maxOcclussion );

        /** Blend sounds near the microphone to 2D sounds */
        void SetNear2DBlend( float minRange, float maxRange, float maxLevel );

        //---------------------------------------------------------------------------------------
        // Geometric Sound Occludders API

        /** Create a sound occluder */
        SoundOccluder CreateSoundOccluder();

        //---------------------------------------------------------------------------------------
        // Profiling API

        // Helper function to get a list of all events
        // Used internally by the debug display and by the cutscene editor
        void GetEventList(EventAndInstanceArray& aOutput, int nMinActiveInstances) const;

	    int  GetTotalMemory() const;

        //---------------------------------------------------------------------------------------
        // Sound Instance API

        /**
        * Gets a non-const reference to the sound instance array.
        * Meant for use by people who promise not do do anything nefarious with it.
        */    
        const vector<EventInstance*>& GetEventInstances() const                  { return m_aEventInstances; }

        /**
        * Informs the sound manager that a EventInstance has completed.
        * Intended to be called by platform specific trampoline functions.
        */
        void EventInstanceCompleted( EventInstance* pInstance );            

        //-----------------------------------------------------------------------------------------
        // Not really part of the public interface: used with FMOD callbacks
        
        /* decrement the current voice count */
        void DecrementCurrentlyPlayingVoices();

     private:

        SoundCategoryState* _GetSoundCategoryState(const STLString& category) const;
        ZLVec3D _GetWorldspacePosition( const ZLVec3D& vPosition, bool headRelative ) const;    

        //---------------------------------------------------------------------------------------
        // Internal/Platform Specific Implementation Functions

	    /// Should set m_bInitialized and m_bEnabled to true on success.
	    void _InitInternals();
    	
	    /// Should set m_bInitialized and m_bEnabled to false.
	    void _ShutdownInternals();

        bool _PassesRetriggerTest( ZLVec3D vSourcePosition, const EventProperties& properties, const Event& soundEvent, EventInstance* pInstance );    
        
        EventInstance* _PlayEvent(const Event& soundEvent, bool loops, const ZLVec3D* pvPosition, EventInstance* pInstanceOverride, const LineCode* pLineCode);    

        SoundCategoryState* _InitSoundCategory(const STLString& category) const;    
        void _PauseSoundCategory(const SoundCategoryState* pCategoryState, bool bPause);
        void _MuteSoundCategory(const SoundCategoryState* pCategoryState, bool bMute);    
        void _UpdateSoundCategoryVolume(const SoundCategoryState* pCategoryState);        

        void _SetMicrophoneTransform(const ZLVec3D &vPos, const ZLVec3D& vVelocity, const ZLVec3D &vLook, const ZLVec3D &vUp );
        
        bool _LoadGroup(const GroupData* pGroupData, bool blockOnLoad);
        bool _UnloadGroup(const GroupData* pGroupData, bool blockOnUnload);

        void _UpdateUnloads(float fDeltaTime);
        void _UpdateDuckingRequests(float fDeltaTime);
        void _UpdateMusic(float fDeltaTime);
        void _UpdateBleeping(float fDeltaTime);
        void _UpdateEventInstances(float fDeltaTime);
	    void _UpdateReverbInstances(float fDeltaTime);
        void _UpdateMinRetriggerTimers(float fDeltaTime);        
        void _InternalUpdate(float fDeltaTime);    

        void _EventInstanceCompleted( EventInstance* pInstance );    
        
        bool _LoadSoundPropertyData( const Event& soundEvent, EventProperties* pSoundProperty ) const;
        bool _LoadGroupData( const STLString& groupPath, GroupData* pGroupData ) const;

	    void _GetEventListInternal(EventAndInstanceArray& aOutput, int nMinActiveInstances, const vector<bool>& desiredProjects) const;    

        /* Returns the group data associated with the group path -- keeping private, because EventManager is the only that uses it */
        GroupData* _GetGroupData(const STLString& groupPath) const;

     private:
        struct GroupData
        {        
            GroupData( STLString path = "", int projectId = -1, int id = -1 ) :
                m_path(path),
                m_persistant(false),
                m_refCount(0),
                m_projectId(projectId),     
                m_fTimeUntilUnload(0.f),
                m_id(id)
            {
            }

            STLString m_path;
            bool m_persistant;
            int m_refCount;
            int m_projectId;
            float m_fTimeUntilUnload;
            int m_id;

            bool operator ==( const GroupData& rhs )
            {
                return ( m_path == rhs.m_path );
            }
        };

        struct SoundCategoryState
        {
            explicit SoundCategoryState(const STLString& categoryName, float fOriginalVolume, void* pInternalData) :
                m_categoryName(categoryName),
                m_bMuted(false),            
                m_bPaused(false),
                m_fVolume(1.f),
                m_fDuckedVolume(1.f),
                m_fOriginalVolume(fOriginalVolume),            
                m_pInternalData(pInternalData)
            {
            }

            STLString m_categoryName;
            bool m_bMuted;        
            bool m_bPaused;
            float m_fVolume;
            float m_fDuckedVolume;
            float m_fOriginalVolume;        
            void* m_pInternalData;
        };
        
        struct DuckingRequest
        {
            DuckingRequest( const vector<STLString>& aCategories, const vector<float>& aVolumes, float duration, int handleIndex ) :
                m_remainingTime( duration ),
                m_handleIndex( handleIndex )
            {
            }

            vector<STLString>             m_aCategories;
            vector<float>            m_aVolumes;        
            float                   m_remainingTime;        
            int                     m_handleIndex;
        };        

    private:    
        HandleFactory<EventInstance>	m_handleFactory;
        vector<EventInstance*>			m_aEventInstances;

        HandleFactory<ReverbInstance>	m_ReverbHandleFactory;
	    vector<ReverbInstance*>			m_aReverbInstances;
	    STLString					    m_DefaultReverb;

        typedef vector<DuckingRequest> DuckingRequestList;
        DuckingRequestList m_aPendingDuckingRequests;
        int  m_nextDuckingRequestHandle;

        mutable STLMap<STLString, GroupData>   m_aGroups;
        vector<GroupData> m_aPendingUnloads;       
        
        typedef STLMap<STLString, SoundCategoryState*> SoundCategoryTable;
        mutable SoundCategoryTable m_apSoundCategories;

        typedef STLMap<STLString, float> MinRetriggerTimerTable;
        mutable MinRetriggerTimerTable m_aMinRetriggerTimers;

        typedef STLMap<STLString, EventProperties> EventPropertiesTable;
        mutable EventPropertiesTable m_aEventProperties;

        VoiceLRU        m_voiceLRU;
        u32            m_iNumActiveVoices;

        // Listener
        ZLVec3D            m_vListenerPosition;
        ZLVec3D            m_vListenerVelocity;
        ZLVec3D            m_vListenerForward;
        ZLVec3D            m_vListenerUp;
        ZLVec3D            m_vListenerRight;
        
        // Init
        SoundInitParams m_initParams;
        Range<float>    m_distantLowpassRange;
        float           m_maxDistantOcclussion;
        Range<float>    m_near2DBlendRange;
        float           m_near2DBlendLevel;

        // Update
        float           m_timeUntilNextUpdate;
        bool            m_enabled;  
        
        float               m_timeSinceLastStateChange;
        float               m_timeSinceLastMusicSetChange;
        float               m_timeUntilNextMusicLoop;
        float               m_musicDelay;
        bool                m_musicIsAmbient;
    };

    class WavbankEntry
    {
    public:
        WavbankEntry(u16 wavIndex = (u16)-1, u16 markerData = 0, float length = 0.f, u16 wavBankIndex = (u16)-1, u16 pad = (u16)0) :
              m_wavIndex(wavIndex),
              m_markerData(markerData),              
              m_wavBankIndex(wavBankIndex),
              m_pad(pad),
              m_length(length)
          {
          }

          u16 GetWavIndex() const          { return m_wavIndex; }    
          u16 GetWavBankIndex() const      { return m_wavBankIndex; }

          float GetLength() const             { return m_length; }

          void GetMarkerData(u32 &startIndex, u32& nMarkers) const
          {
              startIndex = (m_markerData >> 4) & 0xFFF;
              nMarkers = m_markerData & 0x000F;
          }

          static u16 RepackMarkerData(u16 originalData, u16 newOffset)
          {
              u32 startIndex = (originalData >> 4) & 0xFFF;
              u32 nMarkers = originalData & 0x000F;

              // add the newoffset in
              startIndex += newOffset;

              return (startIndex << 4) + nMarkers;
          }

    private:
        u16 m_wavIndex;
        u16 m_markerData;
        u16 m_wavBankIndex;
        u16 m_pad;
        float m_length; 
    };

    extern vector<WavbankEntry> g_WavbankEntries;
    extern vector<STLString> g_WavbankNames;
    extern STLMap<STLString, int> g_ProjectIDs;
    extern float g_AudioUpdateInterval;

    extern EventManager tEventManager;
};

#endif // _EVENTMANAGER_H_

