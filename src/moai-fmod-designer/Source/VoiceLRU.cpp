#include "VoiceLRU.h"

#include "EventManager.h"
#include "EventProperties.h"

using namespace FMODDesigner;

const u32      kCONSERVATIVE_BITRATE = 15 * KILO;
const float     kPLAYBACK_BUFFER = 1.f;    // in seconds

// empty out everything
void VoiceLRU::ReleaseAllSound()
{
    VoiceLRUTable::iterator iter = m_aVoiceTable.begin();
    while (iter != m_aVoiceTable.end())
    {
        // I'm just not going to ever call this function without calling StopAllSounds first
        // that should be safe enough
        // we don't have a good way to stop the sounds inside here

        FMOD::Sound* pSound = (FMOD::Sound*)((iter->second).m_pSound);
        pSound->release();
        m_aVoiceTable.erase(iter);
    }

    m_iCurrentSize = 0;
}

VoiceLRU::InternalData VoiceLRU::GetSound_NoLoad(const LineCode& lineCode)
{
    if (m_aVoiceTable.contains(lineCode))
    {
        VoiceLRUEntry& lruEntry = m_aVoiceTable[lineCode];
        return lruEntry.m_pSound;
    }
    
    return NULL;
}

VoiceLRU::InternalData VoiceLRU::GetSound(const LineCode& lineCode, u32 iCurrentTime, const EventProperties* pProperties)
{
    // look for the entry to see if we already have it
    if (m_aVoiceTable.contains(lineCode))
    {
        // woot, easy mode!
        VoiceLRUEntry& lruEntry = m_aVoiceTable[lineCode];        
        lruEntry.m_lastUsed = iCurrentTime;
        return lruEntry.m_pSound;
    }    

    // how big is this thing?
    u32 size = GetSize(lineCode);

    // if it failed to get a size, forget it
    if (size == 0)
    {
        return NULL;
    }

    // try to make sure we will still be below the soft cap
    // after we create this guy
    FreeSpaceToSize(m_iSoftSizeLimit - size);

    if (m_iCurrentSize + size <= m_iHardSizeLimit && lineCode.GetIndex() < g_WavbankEntries.size())
    {
        FMOD::System* pSystem = NULL;
        FMOD_RESULT result = s_pFMODEventSystem->getSystemObject( &pSystem );

        if (result == FMOD_OK)
        {
            const WavbankEntry& rEntry = g_WavbankEntries[lineCode.GetIndex()];            

            FMOD::Sound* pSound = NULL;

            FMOD_CREATESOUNDEXINFO exinfo;
            memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
            exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);                        
            exinfo.encryptionkey = EventManager::GetVoiceEncryptionKey();

            exinfo.suggestedsoundtype = FMOD_SOUND_TYPE_FSB;

            // only load this one entry
            int wavIndex = rEntry.GetWavIndex();
            exinfo.inclusionlist = &wavIndex;
            exinfo.inclusionlistnum = 1;
            exinfo.numsubsounds = 1;

            FMOD_MODE mode = FMOD_SOFTWARE | FMOD_CREATECOMPRESSEDSAMPLE | FMOD_LOWMEM | FMOD_LOOP_OFF | FMOD_NONBLOCKING;
            
            if (pProperties->m_is3D)
            {
                mode |= FMOD_3D;

                if (pProperties->m_headRelative)
                {
                    mode |= FMOD_3D_HEADRELATIVE;
                }
                else
                {
                    mode |= FMOD_3D_WORLDRELATIVE;
                }
            }
            else
            {
                mode |= FMOD_2D;
            }

            result = pSystem->createSound(g_WavbankNames[rEntry.GetWavBankIndex()].c_str(), mode, &exinfo, &pSound);            

            if (result == FMOD_OK)
            {
                // just add it
                VoiceLRUEntry newEntry;
                newEntry.m_lastUsed = iCurrentTime;
                newEntry.m_size = size;
                newEntry.m_pSound = pSound;

                m_aVoiceTable.insert( pair< LineCode, VoiceLRUEntry >( lineCode, newEntry ) );

                // add to our current size
                m_iCurrentSize += size;

                return newEntry.m_pSound;
            }
        }

    }

    return NULL;
}

u32 VoiceLRU::GetSize(const LineCode& lineCode) const
{
    // get the size to keep this in memory
    if (lineCode != kINVALID_LINECODE && lineCode.GetIndex() < g_WavbankEntries.size() )
    {
        float fDuration = g_WavbankEntries[lineCode.GetIndex()].GetLength();
        return (u32)(fDuration * kCONSERVATIVE_BITRATE);
    }

    // FAILURE
    //Trace(TT_Sound, TL_Warning, "Failed to get size of linecode %s", lineCode.c_str());
    return 0;
}

bool VoiceLRU::FreeSpaceToSize(u32 iDesiredUsage)
{
    // nothing to do, we're undersized
    if (m_iCurrentSize <= iDesiredUsage)
    {
        return true;
    }

    // we need to find who to kick out
    // just find the oldest non-playing sound and kick it out
    LineCode bestCandidateLineCode;
    VoiceLRUEntry bestCandidateEntry;
    u32 oldestTime = UINT_MAX;

    for (VoiceLRUTable::iterator iter = m_aVoiceTable.begin(); iter != m_aVoiceTable.end(); ++iter)
    {
        VoiceLRUEntry& lruEntry = iter->second;
        if (lruEntry.m_lastUsed < oldestTime)
        {
            // now check that it's not playing
            if (!lruEntry.IsSoundPlaying(iter->first))
            {
                bestCandidateEntry = lruEntry;
                bestCandidateLineCode = iter->first;
                oldestTime = lruEntry.m_lastUsed;
            }
        }
    }

    if (oldestTime < UINT_MAX)
    {
        // we found one, kick it out
        if (((FMOD::Sound*)bestCandidateEntry.m_pSound)->release() == FMOD_OK)
        {
            m_iCurrentSize -= GetSize(bestCandidateLineCode);
            m_aVoiceTable.erase(bestCandidateLineCode);            

            // technically we might not have freed enough
            // but oh well.  that's why we have 2 caps, right?  I hope?
            return true;
        }            
    }

    // not gonna try again, forget it!
    return false;
}

//////////////////////////////////////////////////////////////////////////
bool VoiceLRU::VoiceLRUEntry::IsSoundPlaying(const LineCode& lineCode) const
{
    // use the duration and current time to see if it's playing
    if( lineCode.GetIndex() < g_WavbankEntries.size() )
    {
        const u32 curTime = FMODDesigner::GetTimeMs();

        u32 elapsedTime = curTime - m_lastUsed;
        // this is in seconds, so let's get it in milliseconds and add some buffer
        u32 duration = u32((g_WavbankEntries[lineCode.GetIndex()].GetLength() + kPLAYBACK_BUFFER) * 1000);

        // we are playing if the elapsed time is inside the duration
        return (elapsedTime <= duration);
    }
    else
    {
        return false;
    }
}

void VoiceLRU::Update()
{
    FreeSpaceToSize( m_iSoftSizeLimit );
}
