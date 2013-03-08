#ifndef _SOUND_INIT_PARAMS_H_
#define _SOUND_INIT_PARAMS_H_

#include "Event.h"

namespace FMODDesigner
{
    class PlatformData;

    struct SoundInitParams
    {
        explicit SoundInitParams(PlatformData* pPlatformData = NULL) :
            m_pPlatformData(NULL),
            m_soundMemoryMB(32),
            m_rsxMemoryMB(0),
            m_voiceLRUMaxMB(0),
            m_voiceLRUBufferMB(0),
            m_nVirtualChannels(256),
            m_nRealChannels(32),
            m_nPCMCodecs(16),
            m_nADPCMCodecs(32),
            m_nCompressedCodecs(32),
            m_nMaxInputChannels(6),
            m_enableSoundSystem(true),
            m_enableDistantLowpass(false),
            m_enableNear2DBlend(true),
            m_enableEnvironmentalReverb(false),                
            m_enableAuditioning(false),
            m_enableProfiling(false),
            m_enableFsCallbacks(false),
            m_disableSound(false),
            m_dopplerScale(0.f)        
        {
        }

        PlatformData* m_pPlatformData;
        u32 m_soundMemoryMB;
        u32 m_rsxMemoryMB;
        u32 m_voiceLRUMaxMB;
        u32 m_voiceLRUBufferMB;
        u32 m_nVirtualChannels;
        u32 m_nRealChannels;
        u32 m_nPCMCodecs;
        u32 m_nADPCMCodecs;
        u32 m_nCompressedCodecs;
        u32 m_nMaxInputChannels;
        bool m_enableSoundSystem;
        bool m_enableDistantLowpass;
        bool m_enableEnvironmentalReverb; 
        bool m_enableNear2DBlend;
        bool m_enableAuditioning;
        bool m_enableProfiling;
        bool m_enableFsCallbacks;
        bool m_disableSound;
        float m_dopplerScale;    
        STLString m_processingEventName;
    };
};


#endif
