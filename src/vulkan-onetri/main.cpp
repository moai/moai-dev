// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VulkanExample.h"

#include <SDL.h>

#ifdef __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
	#include <limits.h>
#endif

#define UNUSED(p) (( void )p)

//================================================================//
// SDL callbacks
//================================================================//

void _SDL_LogOutputFunction	( void *userdata, int category, SDL_LogPriority priority, const char *message );

//----------------------------------------------------------------//
void _SDL_LogOutputFunction ( void *userdata, int category, SDL_LogPriority priority, const char *message ) {
	UNUSED ( userdata );
	UNUSED ( category );
	UNUSED ( priority );

	printf ( "%s", message );
}

//================================================================//
// SDLHost
//================================================================//

//----------------------------------------------------------------//
int main ( int argc, char** argv ) {
	UNUSED ( argc );
	UNUSED ( argv );
	
	setenv ( "VULKAN_SDK", VULKAN_SDK, 0 );
	setenv ( "VK_ICD_FILENAMES", VK_ICD_FILENAMES, 0 );
	setenv ( "VK_LAYER_PATH", VK_LAYER_PATH, 0 );
	
	SDL_LogSetOutputFunction ( _SDL_LogOutputFunction, 0 );
	SDL_LogSetPriority ( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE );

	SDL_Init ( SDL_INIT_EVERYTHING );
	
	static SDL_Window* window = SDL_CreateWindow (
		"SLD Vulkan Onetri",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		( int )640,
		( int )480,
		SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
	);
	
	VulkanHost host;
	host.setWindow ( window );
	if ( argc > 1 ) {
		host.setAssetPath ( argv [ 1 ]);
	}
	VulkanAbstractExample* example = new VulkanExample ( host );
	
	bool running = true;
	while ( running ) {
		
		SDL_Event sdlEvent;
		
		while ( SDL_PollEvent ( &sdlEvent )) {
			switch ( sdlEvent.type )  {
				case SDL_QUIT: {
					running = false;
					break;
				}
			}
		}

		example->renderFrame ();
	}
	
	
	delete example;
	SDL_Quit ();
	return 0;
}
