// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAISwapChainVK.h>

//================================================================//
// MOAIGfxUtilVK
//================================================================//

//----------------------------------------------------------------//
VkFormat FindDepthFormat (
	VkPhysicalDevice physicalDevice,
	VkFormatFeatureFlags optimalTilingFeatures,
	VkFormatFeatureFlags linearTilingFeatures,
	VkFormatFeatureFlags bufferFeatures
) {

	// Since all depth formats may be optional, we need to find a suitable depth format to use
	// Start with the highest precision packed format
	const VkFormat depthFormats [] = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM,
		VK_FORMAT_UNDEFINED,
	};

	for ( size_t i = 0; depthFormats [ i ] == VK_FORMAT_UNDEFINED; ++i ) {
		VkFormat format = depthFormats [ i ];
	
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties ( physicalDevice, format, &props );
		
		if ((( props.linearTilingFeatures & optimalTilingFeatures ) == optimalTilingFeatures ) &&
			(( props.optimalTilingFeatures & linearTilingFeatures ) == linearTilingFeatures ) &&
			(( props.bufferFeatures & bufferFeatures ) == bufferFeatures )) {
			return format;
		}
	}
	return VK_FORMAT_UNDEFINED;
}

//----------------------------------------------------------------//
cc8* MOAIGfxUtilVK::GetErrorString ( VkResult errorCode ) {

	switch ( errorCode ) {
		#define STR(r) case VK_ ##r: return #r
		STR ( NOT_READY );
		STR ( TIMEOUT );
		STR ( EVENT_SET );
		STR ( EVENT_RESET );
		STR ( INCOMPLETE );
		STR ( ERROR_OUT_OF_HOST_MEMORY );
		STR ( ERROR_OUT_OF_DEVICE_MEMORY );
		STR ( ERROR_INITIALIZATION_FAILED );
		STR ( ERROR_DEVICE_LOST );
		STR ( ERROR_MEMORY_MAP_FAILED );
		STR ( ERROR_LAYER_NOT_PRESENT );
		STR ( ERROR_EXTENSION_NOT_PRESENT );
		STR ( ERROR_FEATURE_NOT_PRESENT );
		STR ( ERROR_INCOMPATIBLE_DRIVER );
		STR ( ERROR_TOO_MANY_OBJECTS );
		STR ( ERROR_FORMAT_NOT_SUPPORTED );
		STR ( ERROR_SURFACE_LOST_KHR );
		STR ( ERROR_NATIVE_WINDOW_IN_USE_KHR );
		STR ( SUBOPTIMAL_KHR );
		STR ( ERROR_OUT_OF_DATE_KHR );
		STR ( ERROR_INCOMPATIBLE_DISPLAY_KHR );
		STR ( ERROR_VALIDATION_FAILED_EXT );
		STR ( ERROR_INVALID_SHADER_NV );
		#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}

//----------------------------------------------------------------//
uint32_t MOAIGfxUtilVK::GetMemoryTypeIndex ( uint32_t typeBits, VkPhysicalDeviceMemoryProperties memoryProperties, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound ) {

	for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++ ) {
		if (( typeBits & 1 ) == 1 ) {
			if ((memoryProperties.memoryTypes [ i ].propertyFlags & properties ) == properties ) {
				if ( memTypeFound ) {
					*memTypeFound = true;
				}
				return i;
			}
		}
		typeBits >>= 1;
	}

	if ( memTypeFound ) {
		*memTypeFound = false;
		return 0;
	}
	assert ( false );
}

////----------------------------------------------------------------//
//VkShaderModule MOAIGfxUtilVK::LoadShaderSPIRV ( cc8* fileName, VkDevice device ) {
//	size_t shaderSize;
//		char* shaderCode = NULL;
//		VkShaderModule shader = VK_NULL_HANDLE;
//
//		std::ifstream is ( filename, std::ios::binary | std::ios::in | std::ios::ate );
//
//		if ( is.is_open ()) {
//			shaderSize = is.tellg ();
//			is.seekg ( 0, std::ios::beg );
//			// Copy file contents into a buffer
//			shaderCode = new char [ shaderSize ];
//			is.read ( shaderCode, shaderSize );
//			is.close ();
//			assert ( shaderSize > 0 );
//			
//			shader = loadShaderSPIRV ( shaderCode, shaderSize, device );
//			delete [] shaderCode;
//		}
//		return shader;
//}

//----------------------------------------------------------------//
VkShaderModule MOAIGfxUtilVK::LoadShaderSPIRV ( const void* shaderCode, size_t shaderSize, VkDevice device ) {

	// Create a new shader module that will be used for pipeline creation
	VkShaderModuleCreateInfo moduleCreateInfo = MOAIGfxStructVK::shaderModuleCreateInfo (( uint32_t* )shaderCode, shaderSize );

	VkShaderModule shaderModule;
	VK_CHECK_RESULT ( vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule ));

	return shaderModule;
}
