// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIGfxInstanceVK
//================================================================//

//----------------------------------------------------------------//
VkResult MOAIGfxInstanceVK::GetPhysicalDeviceSurfaceCapabilitiesKHR ( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities ) {

	assert ( this->mGetPhysicalDeviceSurfaceCapabilitiesKHR );
	return this->mGetPhysicalDeviceSurfaceCapabilitiesKHR ( physicalDevice, surface, pSurfaceCapabilities );
}

//----------------------------------------------------------------//
VkResult MOAIGfxInstanceVK::GetPhysicalDeviceSurfaceFormatsKHR ( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats ) {

	assert ( this->mGetPhysicalDeviceSurfaceFormatsKHR );
	return this->mGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
}

//----------------------------------------------------------------//
VkResult MOAIGfxInstanceVK::GetPhysicalDeviceSurfacePresentModesKHR ( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes ) {

	assert ( this->mGetPhysicalDeviceSurfacePresentModesKHR );
	return this->mGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, surface, pPresentModeCount, pPresentModes );
}

//----------------------------------------------------------------//
VkResult MOAIGfxInstanceVK::GetPhysicalDeviceSurfaceSupportKHR ( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported ) {
	
	assert ( this->mGetPhysicalDeviceSurfaceSupportKHR );
	return this->mGetPhysicalDeviceSurfaceSupportKHR ( physicalDevice, queueFamilyIndex, surface, pSupported );
}

//----------------------------------------------------------------//
void MOAIGfxInstanceVK::Init ( cc8* name, uint32_t apiVersion, cc8** hostInstanceExtensions, bool enableValidation ) {

	printf ( "VK_LAYER_PATH: %s\n", getenv ( "VK_LAYER_PATH" ));
	printf ( "VK_ICD_FILENAMES: %s\n", getenv ( "VK_ICD_FILENAMES" ));

	uint32_t instance_layer_count;
	VK_CHECK_RESULT ( vkEnumerateInstanceLayerProperties ( &instance_layer_count, nullptr ));
	std::cout << instance_layer_count << " layers found!\n";
	if ( instance_layer_count > 0 ) {
		std::unique_ptr<VkLayerProperties []> instance_layers ( new VkLayerProperties [ instance_layer_count ]);
		VK_CHECK_RESULT ( vkEnumerateInstanceLayerProperties ( &instance_layer_count, instance_layers.get ()));
		for ( uint32_t i = 0; i < instance_layer_count; ++i ) {
			std::cout << instance_layers [ i ].layerName << "\n";
		}
	}

	VkApplicationInfo appInfo = MOAIGfxStructVK::applicationInfo ( name, VK_MAKE_VERSION ( 0, 0, 0 ), name, VK_MAKE_VERSION ( 0, 0, 0 ), apiVersion );

	// need to build an array of c strings for VkInstanceCreateInfo. can't be an array of stl strings.
	std::vector < const char* > instanceExtensions;
	std::vector < const char* > validationLayers;

	instanceExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );

	for ( size_t i = 0; hostInstanceExtensions [ i ]; ++i ) {
		instanceExtensions.push_back ( hostInstanceExtensions [ i ]);
	}

	if ( enableValidation ) {
		instanceExtensions.push_back ( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
		validationLayers.push_back ( "VK_LAYER_LUNARG_standard_validation" );
	}

	VkInstanceCreateInfo instanceCreateInfo = MOAIGfxStructVK::instanceCreateInfo (
		&appInfo,
		instanceExtensions.data (),
		( uint32_t )instanceExtensions.size (),
		validationLayers.data (),
		( uint32_t )validationLayers.size ()
	 );

	VkInstance instance;
	VK_CHECK_RESULT ( vkCreateInstance ( &instanceCreateInfo, nullptr, &instance ));

	//    if ( this->mEnableValidation ) {
	//        VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	//        vks::debug::setupDebugging ( this->mInstance, debugReportFlags, VK_NULL_HANDLE );
	//    }

	this->mInstance = instance;
	this->mEnableValidation = enableValidation;

	this->mGetPhysicalDeviceSurfaceCapabilitiesKHR		= VK_GET_INSTANCE_PROC_ADDR ( instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR );
	this->mGetPhysicalDeviceSurfaceFormatsKHR			= VK_GET_INSTANCE_PROC_ADDR ( instance, vkGetPhysicalDeviceSurfaceFormatsKHR );
	this->mGetPhysicalDeviceSurfacePresentModesKHR		= VK_GET_INSTANCE_PROC_ADDR ( instance, vkGetPhysicalDeviceSurfacePresentModesKHR );
	this->mGetPhysicalDeviceSurfaceSupportKHR			= VK_GET_INSTANCE_PROC_ADDR ( instance, vkGetPhysicalDeviceSurfaceSupportKHR );
}

//----------------------------------------------------------------//
MOAIGfxInstanceVK::MOAIGfxInstanceVK () :
	mGetPhysicalDeviceSurfaceCapabilitiesKHR ( 0 ),
	mGetPhysicalDeviceSurfaceFormatsKHR ( 0 ),
	mGetPhysicalDeviceSurfacePresentModesKHR ( 0 ),
	mGetPhysicalDeviceSurfaceSupportKHR ( 0 ),
	mInstance ( VK_NULL_HANDLE ),
	mEnableValidation ( false ) {
}

//----------------------------------------------------------------//
MOAIGfxInstanceVK::~MOAIGfxInstanceVK () {

	this->Destruct ();
}
