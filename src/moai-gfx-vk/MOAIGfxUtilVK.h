// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

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
	MOAIGfxUtilVK::GetDeviceProcAddr < PFN_##entrypoint >( instance, #entrypoint )

#define VK_GET_INSTANCE_PROC_ADDR(instance, entrypoint)										\
	MOAIGfxUtilVK::GetInstanceProcAddr < PFN_##entrypoint >( instance, #entrypoint )

//================================================================//
// MOAIGfxUtilVK
//================================================================//
class MOAIGfxUtilVK {
public:

	//----------------------------------------------------------------//
	static VkFormat			FindDepthFormat				( VkPhysicalDevice physicalDevice, VkFormatFeatureFlags linearTilingFeatures, VkFormatFeatureFlags optimalTilingFeatures, VkFormatFeatureFlags bufferFeatures );
	static cc8*				GetErrorString				( VkResult errorCode );

	//----------------------------------------------------------------//
	template < typename TYPE >
	static TYPE GetDeviceProcAddr ( VkDevice device, cc8* entrypoint ) {
		TYPE addr = reinterpret_cast < TYPE >( vkGetDeviceProcAddr ( device, entrypoint ));
		assert ( addr );
		return addr;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	static TYPE GetInstanceProcAddr ( VkInstance device, cc8* entrypoint ) {
		TYPE addr = reinterpret_cast < TYPE >( vkGetInstanceProcAddr ( device, entrypoint ));
		assert ( addr );
		return addr;
	}
};

#endif
