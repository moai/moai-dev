/*
* Vulkan Example base class
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <SDL_vulkan.h>
#include <string>
#include <vector>
#include "vulkan/vulkan.h"

//================================================================//
// VulkanHost
//================================================================//
class VulkanHost {
private:

	std::string 	mAssetPath;
	SDL_Window*		mWindow;

public:


    //----------------------------------------------------------------//
    virtual bool createSurface ( VkInstance instance, VkSurfaceKHR* surface ) {
		
		return SDL_Vulkan_CreateSurface ( this->mWindow, instance, surface );
		
//		SDL_bool SDL_Vulkan_GetInstanceExtensions ( SDL_Window* window, unsigned int* pCount, const char** pNames );
//		void SDL_Vulkan_GetDrawableSize ( SDL_Window* window, int* w, int* h );
//		SDL_bool SDL_Vulkan_CreateSurface ( SDL_Window* window, VkInstance instance, VkSurfaceKHR* surface );
    }
	
	//----------------------------------------------------------------//
	const std::string getAssetPath () {
		//return [ NSBundle.mainBundle.resourcePath stringByAppendingString: @"/data/" ].UTF8String;
		return this->mAssetPath;
	}
	
	//----------------------------------------------------------------//
	void setAssetPath ( const char* path = NULL ) {
		this->mAssetPath = path ? path : "./";
	}
	
    //----------------------------------------------------------------//
    void setWindow ( SDL_Window* window ) {
			this->mWindow = window;
    }

	//----------------------------------------------------------------//
	VulkanHost () :
		mAssetPath ( "./" ) {
	}
	
	//----------------------------------------------------------------//
	virtual ~VulkanHost () {
	}
};

#define VULKAN_EXAMPLE_MAIN()
