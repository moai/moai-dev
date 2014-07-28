#ifndef _EVENT_INSTANCE_H_
#define _EVENT_INSTANCE_H_

#include "EventHandle.h"
#include "Event.h"
#include "DuckingRequest.h"
#include <zl-util/ZLVec3D.h>

namespace FMODDesigner
{
    class EventParameter;

    // If you need support for oriented sound emitters or doppler, enable this,
    // but it'll cost you 24 bytes per sound instance.
#define SUPPORT_FULL_3D_SOUNDS 0

    // An instance of a playing Event.
    class EventInstance
    {
        friend class EventManager;
        typedef void* InternalData;
    public:
        static const u16 kINVALID_STREAM_INDEX = -1;

    public:
        bool IsValid() const;
        void Stop( bool noFadeOut = false );    
        void Mute( bool bMute );
        void SetPosition( const ZLVec3D& vPosition, const ZLVec3D& vVelocity = ZLVec3D(0.f, 0.f, 0.f), const ZLVec3D& vForward = ZLVec3D(0.f, 0.f, 1.f));
        void SetOrientation( const ZLVec3D& vForward, const ZLVec3D& vUp = ZLVec3D( 0.f, 1.f, 0.f ) );
        void SetTime( float fTime );
        void SetTimeMs( u32 time );
        void SetDelayMs( u32 delay, bool start = true );
        void SetPitch( float fPitch );
        float GetPitch() const;
        void SetVolume( float fVolume );      
        float GetVolume() const;      
        void SetParameter( const EventParameter& param, float fValue );
        float GetParameter( const EventParameter& param );
        void KeyOff( const EventParameter& param );
        void SetSpeakerMix( float fLeft, float fRight, float fCenter, float fSub, float fSurroudLeft, float fSurroundRight );
        void SetLoopPoints( float fStartTime, float fEndTime = -1.f );    
        void UnloadOnSilence(bool unloadOnSilence);
        void DisableRetriggerRadius(bool disable);
        void Pause(bool pause);

        // for determining the currently playing music, need the actual wave that's playing right now
        bool GetCurrentlyPlayingWavName(STLString& wavName) const;

        const STLString& GetName() const                             { return m_sourceEvent.GetName(); }
        const ZLVec3D& GetPosition() const                         { return m_vPosition; }    

        // ensureCompleteCoverage means that if the sound has syncpoints,
        // make sure that there is a point at the very start and very end.
        void GetSyncPoints( vector<float>& aSyncPointTimes, vector<STLString>* pSyncPointLabels, bool ensureCompleteCoverage = false ) const;

        int GetNumChannels() const;        

        // Returns time within the event, or if useSubsoundTime, will return the time
        // within the _the first subsound only_
        float GetTime(bool useSubsoundTime) const;
        u32 GetTimeMs(bool useSubsoundTime) const;    

        // Return ratio of time / duration. If you request the use of subSoundTime, in the event 
        // there are more than a single sub-sound, -1 will be returned.
        float GetElapsed(bool useSubSoundTime) const;

        // Returns the fundamental frequency of this sound instance.
        float GetDominantFrequency() const;

        // Music API
        float GetTempo() const;
        float GetBeatFraction() const;
        float GetMeasureFraction() const;

        // Voice API    
        bool ContainsVoiceData() const                                  { return GetFlag(kSIF_ResidentVoice); }    
        void SetVoiceMemoryPointer(void* ptr);
        bool ContainsResidentVoice() const                              { return GetFlag(kSIF_ResidentVoice); }
        InternalData GetVoiceMemoryPointer() const                      { assert(GetFlag(kSIF_ResidentVoice)); return m_pVoiceSoundPointer; }    

        void ClearVoiceData();    

        // If you call these, you'd better know what you are doing
        InternalData GetInternalData()                                  { return m_pInternalData; } 
        const Event& GetSourceEvent() const                       { return m_sourceEvent; }
        bool IsLoading() const;

    private: 
        enum StateFlags
        {           
            kSIF_RawSound               = 1 << 0,
            kSIF_3D                     = 1 << 1,
            kSIF_HasRetriggerRadius     = 1 << 2,
            kSIF_HasCutoffRadius        = 1 << 3,
            kSIF_UnloadOnSilence        = 1 << 4,
            kSIF_Unloaded               = 1 << 5,
            kSIF_Loops                  = 1 << 6,
            kSIF_Muted                  = 1 << 7,
            kSIF_HeadRelative           = 1 << 8,
            kSIF_RetriggerDisabled      = 1 << 9,
            kSIF_Delayed                = 1 << 10,
            kSIF_Bleeping               = 1 << 11,
            kSIF_ResidentVoice          = 1 << 12,
        };

    private:
        EventInstance( InternalData pInternalData, const Event& sourceEvent, bool rawSound );
        ~EventInstance();

        // Functions that should only be called by the EventManager
        void Set3DOcclussion( float fOcclussion );
        void Set3DPanLevel( float fPanLevel );
        void SetCuttoffRadius( float fRadius );    
        void SetLoops(bool loops);
        void Set3D(bool is3D);
        void SetHeadRelative(bool headRelative);    
        void SetRetriggerRadius( float fRetriggerRadius, int maxRetriggerInstances );

        // Internal functions for middleware implementation of the feature.
        void StopInternal( bool noFadeOut );
        void MuteInternal( bool bMute );
        void SetPositionInternal(const ZLVec3D& vPos, const ZLVec3D& vVelocity, const ZLVec3D& vForward);

        // For marker use
        // default to false to preserve existing calls.
        // Correct default may actually be true?
        float GetDuration(bool subsoundTime=false) const;

        // Utility functions for the EventInstance and EventManager        
        void SetFlag(StateFlags flag, bool value);
        bool GetFlag(StateFlags flag) const;
        u32 GetFlags(u32 flags) const;
        bool TestFlagMask(u32 mask, u32 desired) const;

    private:   
        // Flags
        u16 m_flags;

        // Update staggering
        u32 m_uRetestMs;    

        // Instance Data
        mutable InternalData m_pInternalData;

        Event m_sourceEvent;
        EventHandle m_myHandle;

        void* m_pVoiceSoundPointer;    

        // 3D Data
        ZLVec3D m_vPosition;
#if SUPPORT_FULL_3D_SOUNDS
        ZLVec3D m_vVelocity;
        ZLVec3D m_vForward;    
#endif

        // Ducking data
        DuckingRequestHandle m_hDuckingRequest;
    };

    inline
        void EventInstance::Stop( bool noFadeOut )
    {
        SetFlag(kSIF_Loops, false);
        SetFlag(kSIF_Delayed, false);
        StopInternal( noFadeOut );
    }

    inline
        void EventInstance::Mute( bool bMute )
    {
        if( GetFlag(kSIF_Muted) != bMute )
        {
            SetFlag(kSIF_Muted, bMute);
            MuteInternal(bMute);
        }
    }

    inline
        void EventInstance::SetPosition(const ZLVec3D& vPosition, const ZLVec3D& vVelocity, const ZLVec3D& vForward)
    {
        if( GetFlag(kSIF_3D) )
        {    
            m_vPosition = vPosition;
#if SUPPORT_FULL_3D_SOUNDS
            m_vVelocity = vVelocity;
            m_vForward = vForward;
#endif

            SetPositionInternal(vPosition, vVelocity, vForward);
        }
    }


    inline
        void EventInstance::SetCuttoffRadius( float cutoffRadius )
    {
        SetFlag(kSIF_HasCutoffRadius, cutoffRadius > 0.f);    
    }

    inline
        void EventInstance::SetLoops(bool loops)
    {
        SetFlag(kSIF_Loops, loops);
    }

    inline
        void EventInstance::Set3D(bool is3D)
    {
        SetFlag(kSIF_3D, is3D);
    }

    inline
        void EventInstance::SetHeadRelative(bool headRelative)
    {
        SetFlag(kSIF_HeadRelative, headRelative);
    }

    inline
        void EventInstance::SetRetriggerRadius( float fRetriggerRadius, int maxRetriggerInstances )
    {
        SetFlag(kSIF_HasRetriggerRadius, fRetriggerRadius > 0.f);    
    }

    inline
        void EventInstance::UnloadOnSilence(bool unloadOnSilence)
    {
        SetFlag(kSIF_UnloadOnSilence, unloadOnSilence);
    }

    inline
        void EventInstance::DisableRetriggerRadius(bool disable)
    {
        SetFlag(kSIF_RetriggerDisabled, disable);
    }

    inline
        bool EventInstance::IsValid() const
    {
        return m_pInternalData != NULL;
    }

    inline
        void EventInstance::SetFlag(StateFlags flag, bool value)
    {
        if( value )
        {
            m_flags |= flag;
        }
        else
        {
            m_flags &= ~flag;
        }
    }

    inline
        bool EventInstance::GetFlag(StateFlags flag) const
    {
        return ((m_flags & flag) != 0);
    }

    inline
        u32 EventInstance::GetFlags(u32 flags) const
    {
        return (m_flags & flags);
    }

    inline
        bool EventInstance::TestFlagMask(u32 mask, u32 desired) const
    {
        return ((m_flags & mask) == desired);
    }

    inline
        void EventInstance::SetVoiceMemoryPointer(void* ptr)
    {
        assert(!GetFlag(kSIF_ResidentVoice));
        assert(ptr != NULL);
        m_pVoiceSoundPointer = ptr;
        SetFlag(kSIF_ResidentVoice, true);    
    }
};


#endif
