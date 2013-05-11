/*
  Copyright (C) 2011 Markus Kauppila <markus.kauppila@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "support.h"

#include <SDL/SDL_config.h>

int
PlatformSupportsAudio()
{
	int retValue = 0;

#ifdef SDL_AUDIO_DRIVER_COREAUDIO
	retValue = 1;
#endif
#ifdef SDL_AUDIO_DRIVER_OSS
	retValue = 1;
#endif

	return retValue;
}

int
PlatformSupportsStdio()
{
	int retValue = 0;

#ifdef HAVE_STDIO_H
	retValue = 1;
#endif

	return retValue;
}


/*

Example of implementing new PlatformSupportXXX functions. The function
should return 1 if the feature is supported. Otherwise return 0.

Add call to the implemented function to runner.c in function
CheckTestRequirements. Use the current implementation as a guide.

Also add TEST_REQUIRES_XXX to SDL_test.h and use it in your tests
TestCaseReference. In this case, you'd add TEST_REQUIRES_OPENGL to
SDL_test.h

int
PlatformSupportsOpenGL() {
	int retValue = 0;
#define SDL_VIDEO_OPENGL
	retValue = 1;
#endif

	return retValue;
}

*/
