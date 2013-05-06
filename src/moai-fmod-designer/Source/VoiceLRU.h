#ifndef _VOICE_LRU_H
#define _VOICE_LRU_H

#include "LineCode.h"
#include "Common.h"

namespace FMODDesigner
{
    struct EventProperties;

    class VoiceLRU
    {
        typedef void* InternalData;
    public:
        VoiceLRU()
            : m_iSoftSizeLimit(0), m_iHardSizeLimit(0), m_iCurrentSize(0)
        {
        }

        void Init(u32 iSoftLimitMB, u32 iHardLimitMB)
        {
            m_iSoftSizeLimit = iSoftLimitMB * MEG;
            m_iHardSizeLimit = iHardLimitMB * MEG;

            assert(m_iCurrentSize == 0);

            m_iCurrentSize = 0;

            assert(m_iSoftSizeLimit <= m_iHardSizeLimit);
        }

        // we are enabled if we have a non-zero size
        bool IsEnabled() const
        {
            return m_iHardSizeLimit > 0;
        }

        u32 GetCurrentSize() const
        {
            return m_iCurrentSize;
        }

        // empty out everything
        void ReleaseAllSound();   

        InternalData GetSound_NoLoad(const LineCode& lineCode);
        InternalData GetSound(const LineCode& lineCode, u32 iCurrentTime, const EventProperties* pProperties);    

        void Update();    

    private:
        class VoiceLRUEntry
        {
        public:
            VoiceLRUEntry() : m_size(0), m_lastUsed(0) { }

            VoiceLRUEntry(const VoiceLRUEntry& copyMe)
                : m_pSound(copyMe.m_pSound), m_size(copyMe.m_size), m_lastUsed(copyMe.m_lastUsed)
            {

            }

            bool IsSoundPlaying(const LineCode& lineCode) const;

            InternalData m_pSound;
            u32 m_size;
            u32 m_lastUsed;
        };

        u32 GetSize(const LineCode& lineCode) const;   
        bool FreeSpaceToSize(u32 iDesiredUsage);    

        typedef STLMap<LineCode, VoiceLRUEntry> VoiceLRUTable;
        VoiceLRUTable m_aVoiceTable;

        // we have a soft and a hard size limit
        // the soft limit is where we start kicking stuff out
        u32 m_iSoftSizeLimit;
        // the hard limit is where we can't add anything new
        // so it had best be bigger than the soft limit
        u32 m_iHardSizeLimit;

        // current usage?
        u32 m_iCurrentSize;
    };
};

#endif