#ifndef _DUCKING_REQUEST_H_
#define _DUCKING_REQUEST_H_

namespace FMODDesigner
{
    class DuckingRequestHandle
    {
    public:    
        explicit DuckingRequestHandle( int id = kINVALID_HANDLE ) : m_id( id ) {}

        int GetID() const { return m_id; }

    private:
        friend class EventManager;
        int m_id;

        static const int kINVALID_HANDLE = -1;
    };
};

#endif