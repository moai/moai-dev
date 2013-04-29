#ifndef _REVERB_HANDLE_H_
#define _REVERB_HANDLE_H_

#include "HandleFactory.h"

namespace FMODDesigner
{
    class ReverbInstance;

    typedef HandleFactory<ReverbInstance>::Handle ReverbHandle;
};

#endif