// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASK_H
#define MOAIHTTPTASK_H

#if USE_CURL
#include <moaicore/MOAIHttpTask_curl.h>
#elif MOAI_OS_NACL
#include <moaicore/MOAIHttpTaskInfo_nacl.h>
#endif

#endif
