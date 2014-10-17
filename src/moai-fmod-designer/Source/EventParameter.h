#ifndef _EVENT_PARAMETER_H_
#define _EVENT_PARAMETER_H_

namespace FMODDesigner
{
    class Event;

    // A key/value pair passed into a EventInstance for use by the sound designer.
    class EventParameter
    {
    public:
        EventParameter() : m_index(kINVALID_INDEX) {}
        bool IsValid() const        { return m_index != kINVALID_INDEX; }
        int GetIndex() const        { return m_index; }

    private:
        friend class Event; 
        int m_index;

        static const int kINVALID_INDEX = -1;
    };
};

#endif