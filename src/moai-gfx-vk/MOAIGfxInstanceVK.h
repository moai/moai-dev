// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXINSTANCEVK_H
#define MOAIGFXINSTANCEVK_H

#include <vector>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>

//================================================================//
// MOAIGfxInstanceVK
//================================================================//
class MOAIGfxInstanceVK {
private:

	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	mGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		mGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	mGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		mGetPhysicalDeviceSurfaceSupportKHR;
	
public:

	VkInstance		mInstance;
	bool			mEnableValidation;
	
	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mInstance != VK_NULL_HANDLE );
	}

	//----------------------------------------------------------------//
	operator VkInstance () {
		return this->mInstance;
	}

	//----------------------------------------------------------------//
	VkResult	GetPhysicalDeviceSurfaceCapabilitiesKHR		( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities );
	VkResult	GetPhysicalDeviceSurfaceFormatsKHR			( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats );
	VkResult	GetPhysicalDeviceSurfacePresentModesKHR		( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes );
	VkResult	GetPhysicalDeviceSurfaceSupportKHR			( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported );
	void		Init										( cc8* name, uint32_t apiVersion, cc8** hostInstanceExtensions, bool enableValidation );
				MOAIGfxInstanceVK							();
};

#endif
