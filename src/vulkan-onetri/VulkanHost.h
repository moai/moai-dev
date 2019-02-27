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
		
//		// Create the os-specific surface
//        #if defined(VK_USE_PLATFORM_IOS_MVK)
//            VkIOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
//            surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
//            surfaceCreateInfo.pNext = NULL;
//            surfaceCreateInfo.flags = 0;
//            surfaceCreateInfo.pView = view;
//            err = vkCreateIOSSurfaceMVK(instance, &surfaceCreateInfo, nullptr, &surface);
//        #elif defined(VK_USE_PLATFORM_MACOS_MVK)
//            VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
//            surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
//            surfaceCreateInfo.pNext = NULL;
//            surfaceCreateInfo.flags = 0;
//            surfaceCreateInfo.pView = view;
//            err = vkCreateMacOSSurfaceMVK(instance, &surfaceCreateInfo, NULL, &surface);
//        #endif
		
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
	void getDrawableSize ( int* w, int* h ) {
	
		SDL_Vulkan_GetDrawableSize ( this->mWindow, w, h );
	}
	
	//----------------------------------------------------------------//
	size_t getInstanceExtensions ( unsigned int* count = NULL, const char** names = NULL ) {
	
		unsigned int tempCount = 0;
		count = count ? count : &tempCount;
		
		SDL_bool result = SDL_Vulkan_GetInstanceExtensions ( this->mWindow, count, names );
		assert ( result );
		return ( size_t )*count;
	}
	
	//----------------------------------------------------------------//
	void pushDeviceExtensions ( std::vector < const char* >& extensions ) {
	
		extensions.push_back ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
	}
	
	//----------------------------------------------------------------//
	void pushInstanceExtensions ( std::vector < const char* >& extensions ) {
	
//		instanceExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );
//
//		#if defined ( VK_USE_PLATFORM_IOS_MVK )
//			instanceExtensions.push_back ( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
//		#elif defined ( VK_USE_PLATFORM_MACOS_MVK )
//			instanceExtensions.push_back ( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
//		#endif
	
		extensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );
        extensions.push_back ( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
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
