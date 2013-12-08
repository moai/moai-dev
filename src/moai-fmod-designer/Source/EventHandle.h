#ifndef _SOUND_HANDLE_H_
#define _SOUND_HANDLE_H_

#include "HandleFactory.h"

namespace FMODDesigner
{
    class EventInstance;

    typedef HandleFactory<EventInstance>::Handle EventHandle;
};

#endif