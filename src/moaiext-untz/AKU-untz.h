//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_UNTZ_H
#define AKU_MOAIEXT_UNTZ_H

#include <aku/AKU.h>

//#define AKU_UNTZ_DEFAULT_SAMPLE_RATE		= 44100;
//#define DEFAULT_FRAMES_PER_BUFFER			= 4096;
//#define DEFAULT_OPTIONS						= 0;
//
//AKU_API void	AKUUntzInit			( int sampleRate, int numFrames, int options );
//AKU_API void	AKUUntzShutdown		();

AKU_API void	AKUUntzInit			();
AKU_API void	AKUUntzResume		();
AKU_API void	AKUUntzSuspend		();

#endif
