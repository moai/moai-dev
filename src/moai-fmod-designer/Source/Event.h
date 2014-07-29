#ifndef _EVENT_H_
#define _EVENT_H_

#define kINVALID_EVENT_INDEX -1
#include "Common.h"
#include <zl-util/pch.h>
#include <zl-util/STLString.h>

namespace FMODDesigner
{
    class EventManager;
    class EventParameter;
    class LineCode;

    // The definition of a sound.
    class Event
    {
        friend class EventManager;
        typedef void* InternalData;
    public:
        Event();
        explicit Event( const STLString& eventName );
        explicit Event( InternalData pData );
        // default copy-constructor and assignment operator

        bool IsValid() const;
        float GetDuration(const LineCode* pLineCode = NULL) const; //< 0 is looping
        void GetParameterIndex( EventParameter& param, const STLString& paramName ) const;    

        const STLString& GetName() const                 { return m_eventName; }  
        void SetName( const STLString& eventName )	     { m_eventName = eventName; }
        bool IsRawSound() const                     { return m_rawSound; }
        void* GetInternals() const                  { return m_pInternalData; }

        void DownsampleInto(vector<u8>& aDstBuffer, u32 targetFrequency) const;
        void CopyInto(vector<u8>& aDstBuffer) const;

        bool operator==( const Event& rhs ) const { return m_eventName == rhs.m_eventName; }
        bool operator!=( const Event& rhs ) const { return m_eventName != rhs.m_eventName; }

    private:    
        void _SetSoundID() const;    

        STLString			m_eventName;    
        mutable InternalData m_pInternalData;
        bool            m_rawSound;
    };
};


#endif
