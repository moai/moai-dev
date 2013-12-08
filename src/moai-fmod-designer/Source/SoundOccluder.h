#ifndef _SOUND_OCCLUDDER_H_
#define _SOUND_OCCLUDDER_H_

#include "Common.h"
#include <zl-util/ZLVec3D.h>

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

          void SetPosition( const ZLVec3D& vPos );
          void SetScale( const ZLVec3D& vScale );
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
