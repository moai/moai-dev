/*
* Initializers for Vulkan structures and objects used by the examples
* Saves lot of VK_STRUCTURE_TYPE assignments
* Some initializers are parameterized for convenience
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef MOAIGFXUTILVK_H
#define MOAIGFXUTILVK_H

#include <vector>

#define VK_CHECK_RESULT(f)																	\
{																							\
	VkResult res = (f);																		\
	if (res != VK_SUCCESS)																	\
	{																						\
		std::cout << "Fatal : VkResult is \"" << MOAIGfxUtilVK::GetErrorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
		assert(res == VK_SUCCESS);															\
	}																						\
}

#define VK_GET_DEVICE_PROC_ADDR(instance, entrypoint)										\
	MOAIGfxUtilVK::GetDeviceProcAddr < PFN_vk##entrypoint >( instance, "vk"#entrypoint )

#define VK_GET_INSTANCE_PROC_ADDR(instance, entrypoint)										\
	MOAIGfxUtilVK::GetInstanceProcAddr < PFN_vk##entrypoint >( instance, "vk"#entrypoint )

//================================================================//
// MOAIGfxUtilVK
//================================================================//
class MOAIGfxUtilVK {
public:

	//----------------------------------------------------------------//
	// Create a command pool for allocation command buffers from
	static VkCommandPool CreateCommandPool ( VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT ) {
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
		cmdPoolInfo.flags = createFlags;
		VkCommandPool cmdPool;
		VK_CHECK_RESULT ( vkCreateCommandPool ( device, &cmdPoolInfo, nullptr, &cmdPool ));
		return cmdPool;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	static TYPE GetDeviceProcAddr ( VkDevice device, cc8* entrypoint ) {
		TYPE addr = reinterpret_cast < TYPE >( vkGetDeviceProcAddr ( device, entrypoint ));
		assert ( addr );
		return addr;
	}

	//----------------------------------------------------------------//
	static cc8* GetErrorString ( VkResult errorCode ) {
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
	template < typename TYPE >
	static TYPE GetInstanceProcAddr ( VkInstance device, cc8* entrypoint ) {
		TYPE addr = reinterpret_cast < TYPE >( vkGetInstanceProcAddr ( device, entrypoint ));
		assert ( addr );
		return addr;
	}

	//----------------------------------------------------------------//
	// Get the index of a queue family that supports the requested queue flags
	static uint32_t GetQueueFamilyIndex ( const std::vector < VkQueueFamilyProperties >& queueFamilyProperties, VkQueueFlagBits queueFlags, uint32_t fallback ) {
		
		// Dedicated queue for compute
		// Try to find a queue family index that supports compute but not graphics
		if ( queueFlags & VK_QUEUE_COMPUTE_BIT ) {
			for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
				if (( queueFamilyProperties [ i ].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 )) {
					return i;
				}
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
			for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
				if (( queueFamilyProperties [ i ].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
					return i;
				}
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
		for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
			if ( queueFamilyProperties [ i ].queueFlags & queueFlags) {
				return i;
			}
		}
		return fallback;
	}
};

#endif
