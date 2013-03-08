#include "ReverbInstance.h"

#include "EventManager.h"

using namespace FMODDesigner;

ReverbInstance::ReverbInstance( const STLString& reverbName ) :
m_Name( reverbName),
m_fAlpha(0.f),
m_fDecayTime(0.f),
m_fDecayHFRatio(0.f),
m_fPreDelay(0.f),	
m_fLateDelay(0.f),		
m_fEarlyReflections(0.f),
m_fLateReflections(0.f),
m_fDiffusion(0.f),
m_fDensity(0.f),
m_fHFGain(0.f),		
m_fLFGain(0.f),			
m_fHFCrossover(0.f),
m_fLFCrossOver(0.f)
{        

}

ReverbInstance::~ReverbInstance()
{
}

void ReverbInstance::AddReverb(const ReverbInstance* pReverb, float fAlpha /* = 1.f */)
{
	m_fDecayTime		+= fAlpha * pReverb->m_fDecayTime;
	m_fDecayHFRatio		+= fAlpha * pReverb->m_fDecayHFRatio;
	m_fPreDelay			+= fAlpha * pReverb->m_fPreDelay;
	m_fLateDelay		+= fAlpha * pReverb->m_fLateDelay;
	m_fEarlyReflections += fAlpha * pReverb->m_fEarlyReflections;
	m_fLateReflections	+= fAlpha * pReverb->m_fLateReflections;
	m_fDiffusion		+= fAlpha * pReverb->m_fDiffusion;
	m_fDensity			+= fAlpha * pReverb->m_fDensity;
	m_fHFGain			+= fAlpha * pReverb->m_fHFGain;
	m_fLFGain			+= fAlpha * pReverb->m_fLFGain;
	m_fHFCrossover		+= fAlpha * pReverb->m_fHFCrossover;
	m_fLFCrossOver		+= fAlpha * pReverb->m_fLFCrossOver;
}
