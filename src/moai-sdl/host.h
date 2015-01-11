//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_SDL_HOST_H
#define MOAI_SDL_HOST_H

#include <moai-core/host.h>
#include <SDL.h>

AKU_API void	AKUSdlAppFinalize					();
AKU_API void	AKUSdlAppInitialize					();
AKU_API void	AKUSdlContextInitialize				();
AKU_API void	AKUSdlSetWindow						( SDL_Window* window );

#endif
