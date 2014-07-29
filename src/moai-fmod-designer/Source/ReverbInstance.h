#ifndef _REVERB_INSTANCE_H_
#define _REVERB_INSTANCE_H_

#include "ReverbHandle.h"
#include <zl-util/STLString.h>

namespace FMODDesigner
{
    // An instance of a loaded Reverb setting
    class ReverbInstance
    {
        friend class EventManager;
    public:

        ReverbInstance( const STLString& reverbName );
        ~ReverbInstance();

        void AddReverb(const ReverbInstance* pReverb, float fAlpha = 1.f);


        float			m_fAlpha;
        STLString   	m_Name;
        ReverbHandle	m_myHandle;

        float			m_fDecayTime;
        float			m_fDecayHFRatio;
        float			m_fPreDelay;				// Fmod ReflectionsDelay
        float			m_fLateDelay;				// Fmod ReverbDelay
        float			m_fEarlyReflections;		// Fmod Relections?
        float			m_fLateReflections;		// Fmod Reverb (int)
        float			m_fDiffusion;
        float			m_fDensity;
        float			m_fHFGain;				// FMOD RoomHF
        float			m_fLFGain;				// Fmod RoomLF
        float			m_fHFCrossover;			// Fmod HFReference
        float			m_fLFCrossOver;			// Fmod LFReference

    };
};

#endif
