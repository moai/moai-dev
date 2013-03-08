#include "SoundDSP.h"

#include "EventManager.h"

using namespace FMODDesigner;

SoundDSP::SoundDSP() : 
    m_pNextDSP(NULL)    
{
}

SoundDSP::~SoundDSP()
{
    delete m_pNextDSP;    
}

void SoundDSP::Apply(void* pChannel) const
{
    _Apply(pChannel);

    if( m_pNextDSP )
    {
        m_pNextDSP->Apply(pChannel);
    }
}

SoundDSP* SoundDSP::Clone() const
{
    SoundDSP* pNewDSP = _Clone();
    if( m_pNextDSP )
    {
        pNewDSP->m_pNextDSP = m_pNextDSP->Clone();
    }
    return pNewDSP;
}

LowpassDSP::LowpassDSP() :
    m_cutoffFrequency(5000.f),
    m_resonance(1.f)
{
}

SoundDSP* LowpassDSP::_Clone() const
{
    return new LowpassDSP(*this);
}

void* LowpassDSP::_Apply(void* pChannel) const
{        
    FMOD::System* pSystem = NULL;
    FMOD_RESULT result = FMODDesigner::s_pFMODEventSystem->getSystemObject( &pSystem );

    FMOD::DSP* pDSP = NULL;
    result = pSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &pDSP);
    if( result == FMOD_OK )
    {
        pDSP->setParameter( FMOD_DSP_LOWPASS_CUTOFF, m_cutoffFrequency );
        pDSP->setParameter( FMOD_DSP_LOWPASS_RESONANCE, m_resonance );
        result = ((FMOD::Channel*)pChannel)->addDSP(pDSP, NULL);     
        if( result == FMOD_OK )
        {
            return pChannel;
        }
    }

    return NULL;
}

HighpassDSP::HighpassDSP() :
    m_cutoffFrequency(5000.f),
    m_resonance(1.f)
{
}

SoundDSP* HighpassDSP::_Clone() const
{
    return new HighpassDSP(*this);
}

void* HighpassDSP::_Apply(void* pChannel) const
{
    FMOD::System* pSystem = NULL;
    FMOD_RESULT result = FMODDesigner::s_pFMODEventSystem->getSystemObject( &pSystem );

    FMOD::DSP* pDSP = NULL;
    result = pSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &pDSP);
    if( result == FMOD_OK )
    {
        pDSP->setParameter( FMOD_DSP_HIGHPASS_CUTOFF, m_cutoffFrequency );
        pDSP->setParameter( FMOD_DSP_HIGHPASS_RESONANCE, m_resonance );        
        result = ((FMOD::Channel*)pChannel)->addDSP(pDSP, NULL);    
        if( result == FMOD_OK )
        {
            return pChannel;
        }
    }

    return NULL;
}

NormalizeDSP::NormalizeDSP() :
    m_fadeTime(0.f),
    m_threshold(0.f),
    m_maxAmp(0.f)
{
}

SoundDSP* NormalizeDSP::_Clone() const
{
    return new NormalizeDSP(*this);
}

void* NormalizeDSP::_Apply(void* pChannel) const
{
    FMOD::System* pSystem = NULL;
    FMOD_RESULT result = FMODDesigner::s_pFMODEventSystem->getSystemObject( &pSystem );

    FMOD::DSP* pDSP = NULL;
    result = pSystem->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &pDSP);
    if( result == FMOD_OK )
    {
        pDSP->setParameter( FMOD_DSP_NORMALIZE_FADETIME, m_fadeTime );
        pDSP->setParameter( FMOD_DSP_NORMALIZE_THRESHHOLD, m_threshold );
        pDSP->setParameter( FMOD_DSP_NORMALIZE_MAXAMP, m_maxAmp );
        result = ((FMOD::Channel*)pChannel)->addDSP(pDSP, NULL);
        if( result == FMOD_OK )
        {
            return pChannel;
        }
    }

    return NULL;
}

CompressorDSP::CompressorDSP() :
    m_makeUpGain(0.f),
    m_threshold(0.f),
    m_attack(50.f),
    m_release(50.f)
{
}

SoundDSP* CompressorDSP::_Clone() const
{
    return new CompressorDSP(*this);
}

void* CompressorDSP::_Apply(void* pChannel) const
{
    FMOD::System* pSystem = NULL;
    FMOD_RESULT result = FMODDesigner::s_pFMODEventSystem->getSystemObject( &pSystem );

    FMOD::DSP* pDSP = NULL;
    result = pSystem->createDSPByType(FMOD_DSP_TYPE_COMPRESSOR, &pDSP);
    if( result == FMOD_OK )
    {
        pDSP->setParameter( FMOD_DSP_COMPRESSOR_THRESHOLD, m_threshold );
        pDSP->setParameter( FMOD_DSP_COMPRESSOR_ATTACK, m_attack );
        pDSP->setParameter( FMOD_DSP_COMPRESSOR_RELEASE, m_release );
        pDSP->setParameter( FMOD_DSP_COMPRESSOR_GAINMAKEUP, m_makeUpGain );
        result = ((FMOD::Channel*)pChannel)->addDSP(pDSP, NULL);
        if( result == FMOD_OK )
        {
            return pChannel;
        }
    }

    return NULL;
}

PitchShiftDSP::PitchShiftDSP() :
    m_pitch(1.f),
    m_fftSize(256)
{
}

SoundDSP* PitchShiftDSP::_Clone() const
{
    return new PitchShiftDSP(*this);
}

void* PitchShiftDSP::_Apply(void* pChannel) const
{
    FMOD::System* pSystem = NULL;
    FMOD_RESULT result = FMODDesigner::s_pFMODEventSystem->getSystemObject( &pSystem );

    FMOD::DSP* pDSP = NULL;
    result = pSystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pDSP);
    if( result == FMOD_OK )
    {
        pDSP->setParameter( FMOD_DSP_PITCHSHIFT_PITCH, m_pitch );
        pDSP->setParameter( FMOD_DSP_PITCHSHIFT_FFTSIZE, m_fftSize );
        result = ((FMOD::Channel*)pChannel)->addDSP(pDSP, NULL);
        if( result == FMOD_OK )
        {
            return pChannel;
        }
    }

    return NULL;
}
