#include "SoundOccluder.h"

#include "EventManager.h"

using namespace FMODDesigner;

void SoundOccluder::SetPosition( const ZLVec3D& vPos )
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::Geometry* pGeom = (FMOD::Geometry*)m_hInstance;
        FMOD_VECTOR vFMPos;
        FMODDesigner::CopyDFVectorToFMOD( vFMPos, vPos );        
        result = pGeom->setScale( &vFMPos );

        HANDLE_FMOD_ERROR( result );                
    }
}

void SoundOccluder::SetScale( const ZLVec3D& vScale )
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::Geometry* pGeom = (FMOD::Geometry*)m_hInstance;
        FMOD_VECTOR vFMScale;
        FMODDesigner::CopyDFVectorToFMOD( vFMScale, vScale );        
        result = pGeom->setScale( &vFMScale );

        HANDLE_FMOD_ERROR( result );
    }
}

void SoundOccluder::Destroy()
{
    if( IsValid() )
    {
        PROFILE_TIME_REGION(sg_SoundZone);
        FMOD_RESULT result = FMOD_OK;
        FMOD::Geometry* pGeom = (FMOD::Geometry*)m_hInstance;        
        result = pGeom->release();
        m_hInstance = NULL;

        HANDLE_FMOD_ERROR( result );        
    }
}

bool SoundOccluder::IsValid() const
{
    return m_hInstance != NULL;    
}
