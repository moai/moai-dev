#ifndef _SOUNDDSP_H_
#define _SOUNDDSP_H_

namespace FMODDesigner
{
    class SoundDSP
    {    
    public:
        SoundDSP();
        virtual ~SoundDSP();

        void Apply(void* pChannel) const;

        SoundDSP* Clone() const;

    private:
        virtual SoundDSP* _Clone() const = 0;
        virtual void* _Apply(void* pChannel) const = 0;

    private:    
        SoundDSP* m_pNextDSP;
    };

    class LowpassDSP : public SoundDSP
    {      
    public:
        LowpassDSP();

    private:
        virtual SoundDSP* _Clone() const;
        virtual void* _Apply(void* pChannel) const;

    private:
        float m_cutoffFrequency;
        float m_resonance;
    };

    class HighpassDSP : public SoundDSP
    {    
    public:
        HighpassDSP();

    private:
        virtual SoundDSP* _Clone() const;
        virtual void* _Apply(void* pChannel) const;

    private:
        float m_cutoffFrequency;
        float m_resonance;
    };

    class NormalizeDSP : public SoundDSP
    {    
    public:
        NormalizeDSP();

    private:
        virtual SoundDSP* _Clone() const;
        virtual void* _Apply(void* pChannel) const;

    private:    
        float m_fadeTime;
        float m_threshold;
        float m_maxAmp;
    };

    class CompressorDSP : public SoundDSP
    {    
    public:
        CompressorDSP();

    private:
        virtual SoundDSP* _Clone() const;
        virtual void* _Apply(void* pChannel) const;

    private:    
        float m_makeUpGain;
        float m_threshold;
        float m_attack;
        float m_release;
    };

    class PitchShiftDSP : public SoundDSP
    {    
    public:
        PitchShiftDSP();

    private:
        virtual SoundDSP* _Clone() const;
        virtual void* _Apply(void* pChannel) const;

    private:    
        float m_pitch;
        float m_fftSize;    
    };
};

#endif