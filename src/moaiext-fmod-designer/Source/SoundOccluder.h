#ifndef _SOUND_OCCLUDDER_H_
#define _SOUND_OCCLUDDER_H_

#include "Common.h"
#include <uslscore/USVec3D.h>

namespace FMODDesigner
{
    typedef void* InstanceHandle;

    class SoundOccluder
    {
    public:
        SoundOccluder() :
          m_hInstance( NULL )
          {
          }

          SoundOccluder( InstanceHandle hInstance ) :
          m_hInstance( hInstance )
          {
          }

          void SetPosition( const USVec3D& vPos );
          void SetScale( const USVec3D& vScale );
          void Destroy();

          //Assignment Operator
          SoundOccluder& operator=( const SoundOccluder& rhs )
          {
              m_hInstance = rhs.m_hInstance;        
              return *this;
          }
          /// Copy-constructor
          SoundOccluder( const SoundOccluder& rhs )
          {
              m_hInstance = rhs.m_hInstance;        
          }

    private:
        bool IsValid() const;

    private:
        mutable InstanceHandle m_hInstance;
    };
};

#endif
