// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>

//================================================================//
// MOAIPhysicalDeviceVK
//================================================================//

//----------------------------------------------------------------//
VkCompositeAlphaFlagBitsKHR MOAIPhysicalDeviceVK::FindCompositeAlpha () {

	AlphaRank rank = this->RankCompositeAlphaBits ();
	for ( size_t i = 0; rank [ i ]; ++i ) {
		VkCompositeAlphaFlagBitsKHR flag = rank [ i ];
		if ( this->mSurfaceCapabilities.supportedCompositeAlpha & flag ) return flag;
	}
	assert ( false );
	return VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
}

//----------------------------------------------------------------//
VkFormat MOAIPhysicalDeviceVK::FindFormat (
	const VkFormat* rankedFormats,
	VkFormatFeatureFlags optimalTilingFeatures,
	VkFormatFeatureFlags linearTilingFeatures,
	VkFormatFeatureFlags bufferFeatures
) {

	for ( size_t i = 0; rankedFormats [ i ] != VK_FORMAT_UNDEFINED; ++i ) {
		VkFormat format = rankedFormats [ i ];
	
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties ( this->mDevice, format, &props );
		
		if ((( props.optimalTilingFeatures & optimalTilingFeatures ) == optimalTilingFeatures ) &&
			(( props.linearTilingFeatures & linearTilingFeatures ) == linearTilingFeatures ) &&
			(( props.bufferFeatures & bufferFeatures ) == bufferFeatures )) {
			return format;
		}
	}
	return VK_FORMAT_UNDEFINED;
}

//----------------------------------------------------------------//
bool MOAIPhysicalDeviceVK::FindMemoryTypeIndex ( u32& index, u32 typeBits, VkMemoryPropertyFlags properties ) {

	for ( u32 i = 0; i < this->mMemoryProperties.memoryTypeCount; i++ ) {
		if (( typeBits & 1 ) == 1 ) {
			if (( this->mMemoryProperties.memoryTypes [ i ].propertyFlags & properties ) == properties ) {
				index = i;
				return true;
			}
		}
		typeBits >>= 1;
	}
	return false;
}

//----------------------------------------------------------------//
VkPresentModeKHR MOAIPhysicalDeviceVK::FindPresentMode () {

	assert ( this->mPresentModes.Size ());

	size_t bestScore = 0;
	VkPresentModeKHR bestPresentMode;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mPresentModes.Size (); ++i ) {
	
		VkPresentModeKHR comp = this->mPresentModes [ i ];
		size_t score = MOAIPhysicalDeviceVK::ScorePresentMode ( comp );
	
		if ( bestScore < score ) {
			bestScore = score;
			bestPresentMode = comp;
		}
	}
	assert ( bestScore > 0 );
	return bestPresentMode;
}

//----------------------------------------------------------------//
bool MOAIPhysicalDeviceVK::FindQueue ( u32& queueIndex, u32 criteria, u32 exclude ) {

	queueIndex = ( u32 )-1;
	bool found = false;
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mQueueFamilyProperties.Size (); ++i ) {
		
		u32 queueFlags = this->GetQueueFlags ( i );
		
		bool excludeOK = (( queueFlags & exclude ) == 0 );
	
		if ( queueFlags & criteria ) {
			
			queueIndex = ( u32 )i;
			found = true;
			
			if ( excludeOK ) {
				return true;
			}
		}
	}
	return found;
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::FindQueues ( MOAIGfxQueueSetVK& queueSet, VkQueueFlags requestedQueueTypes, bool requestPresent ) {

	queueSet = MOAIGfxQueueSetVK ();

	STLSet < u32 > foundSet;

	// try to find a dedicated compute queue
	if ( requestedQueueTypes & VK_QUEUE_COMPUTE_BIT ) {
		u32 index;
		if ( this->FindQueue ( index, QUEUE_COMPUTE_BIT, QUEUE_GRAPHICS_BIT )) {
			foundSet.affirm ( index );
			queueSet.mComputeQueueIndex = index;
		}
	}
	
	// try to find a dedicated transfer queue
	if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
		u32 index;
		if ( this->FindQueue ( index, QUEUE_TRANSFER_BIT, QUEUE_GRAPHICS_BIT | QUEUE_COMPUTE_BIT )) {
			foundSet.affirm ( index );
			queueSet.mTransferQueueIndex = index;
		}
	}
	
	// try to find a dedicated graphics queue (without present)
	if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
		u32 index;
		if ( this->FindQueue ( index, QUEUE_GRAPHICS_BIT, QUEUE_PRESENT_BIT )) {
			foundSet.affirm ( index );
			queueSet.mGraphicsQueueIndex = index;
		}
	}
	
	// try to find a dedicated presentation queue (without graphics)
	if ( requestPresent ) {
		u32 index;
		if ( this->FindQueue ( index, QUEUE_PRESENT_BIT, QUEUE_GRAPHICS_BIT )) {
			foundSet.affirm ( index );
			queueSet.mPresentQueueIndex = index;
			queueSet.mCanPresent = true;
		}
	}
	
	queueSet.mCreateInfos.Init ( foundSet.size ());
	
	STLSet < u32 >::const_iterator foundSetIt = foundSet.cbegin ();
	for ( ZLIndex i = ZLIndexOp::ZERO; foundSetIt != foundSet.cend (); ++foundSetIt, ++i ) {
		queueSet.mCreateInfos [ i ] = MOAIGfxStructVK::deviceQueueCreateInfo ( *foundSetIt );
		queueSet.mQueueTypes |= this->mQueueFamilyProperties [ i ].queueFlags;
	}
}

//----------------------------------------------------------------//
VkSurfaceFormatKHR MOAIPhysicalDeviceVK::FindSurfaceFormat ( VkFormat colorFormat, VkColorSpaceKHR colorSpace ) {

	VkSurfaceFormatKHR format;
	format.format = VK_FORMAT_UNDEFINED;

	if ( !this->mSurfaceFormats.Size ()) return format;

	if (( this->mSurfaceFormats.Size () == 1 ) && ( this->mSurfaceFormats [ ZLIndexOp::ZERO ].format == VK_FORMAT_UNDEFINED )) {
		format.format = VK_FORMAT_B8G8R8A8_UNORM;
		format.colorSpace = this->mSurfaceFormats [ ZLIndexOp::ZERO ].colorSpace;
		return format;
	}

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mSurfaceFormats.Size (); ++i ) {
		const VkSurfaceFormatKHR& comp = this->mSurfaceFormats [ i ];
		if ( comp.format == colorFormat ){
		
			if ( comp.colorSpace == colorSpace ) {
				return comp;
			}
			
			if ( format.format == VK_FORMAT_UNDEFINED ) {
				format = comp;
			}
		}
	}
	return format;
}

//----------------------------------------------------------------//
VkSurfaceTransformFlagBitsKHR MOAIPhysicalDeviceVK::FindSurfaceTransform ( VkSurfaceTransformFlagBitsKHR preferredTransform ) {

	if ( this->mSurfaceCapabilities.supportedTransforms & preferredTransform ) {
		return preferredTransform;
	}
	return this->mSurfaceCapabilities.currentTransform;
}

//----------------------------------------------------------------//
VkExtent2D MOAIPhysicalDeviceVK::GetExtent ( u32 preferredWidth, u32 preferredHeight ) {

	VkExtent2D extent = this->mSurfaceCapabilities.currentExtent;
	if ( extent.width == ( u32 )-1 ) {
		extent.width = preferredWidth;
		extent.height = preferredHeight;
	}
	return extent;
}

//----------------------------------------------------------------//
u32 MOAIPhysicalDeviceVK::GetQueueFlags ( ZLIndex queueIndex ) {

	VkQueueFlags vkQueueFlags = this->mQueueFamilyProperties [ queueIndex ].queueFlags;

	u32 queueFlags = 0;
	queueFlags |= ( vkQueueFlags & VK_QUEUE_COMPUTE_BIT ) ? QUEUE_COMPUTE_BIT : 0;
	queueFlags |= ( vkQueueFlags & VK_QUEUE_GRAPHICS_BIT ) ? QUEUE_GRAPHICS_BIT : 0;
	queueFlags |= ( vkQueueFlags & VK_QUEUE_TRANSFER_BIT ) ? QUEUE_TRANSFER_BIT : 0;
	queueFlags |= this->mQueueFamilySupportsPresent [ queueIndex ] ? QUEUE_PRESENT_BIT : 0;
	
	return queueFlags;
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::Initialize ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	u32 deviceCount = 0;
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( instance, &deviceCount, nullptr ));
    assert ( deviceCount > 0 );
	
    std::vector < VkPhysicalDevice > physicalDevices ( deviceCount );
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( instance, &deviceCount, physicalDevices.data ()));

	for ( size_t i = 0; i < deviceCount; ++i ) {
	
		MOAIPhysicalDeviceVK compDevice;
		compDevice.InitBasicInfo ( physicalDevices [ i ]);
		
		// TODO: implement a move sophisticated comparison heuristic
		if ( !this->mDevice || ( *this < compDevice )) {
			
			compDevice.InitFullInfo ( instance, surface );
			if (
				compDevice.SupportsExtension ( VK_KHR_SWAPCHAIN_EXTENSION_NAME ) &&		// can swapchain
				compDevice.SupportsRenderAndPresent () &&								// can present
				( compDevice.mDepthFormat != VK_FORMAT_UNDEFINED )						// supports depth/stencil
			) {
				*this = compDevice;
			}
		}
	}
	assert ( this->mDevice );
	
	instance.AddClient ( instance, *this );
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitBasicInfo ( VkPhysicalDevice device ) {

	this->mDevice = device;
	
	vkGetPhysicalDeviceProperties ( this->mDevice, &this->mProperties );
	vkGetPhysicalDeviceFeatures ( this->mDevice, &this->mFeatures );
	vkGetPhysicalDeviceMemoryProperties ( this->mDevice, &this->mMemoryProperties );
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitFullInfo ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	this->InitPresentModes ( instance, surface );
	this->InitQueueFamilyProperties ( instance, surface );
	this->InitSupportedExtensions ();
	this->InitSurfaceFormats ( instance, surface );
	this->InitSurfaceCapabilities ( instance, surface );
	
	this->mDepthFormat = this->FindFormat ( this->RankDepthFormats (), VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitPresentModes ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	// Get available present modes
	u32 presentModeCount;
	instance.GetPhysicalDeviceSurfacePresentModesKHR ( this->mDevice, surface, &presentModeCount, NULL );
	assert ( presentModeCount > 0 );

	this->mPresentModes.Init ( presentModeCount );
	instance.GetPhysicalDeviceSurfacePresentModesKHR ( this->mDevice, surface, &presentModeCount, this->mPresentModes.GetBuffer ());
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitQueueFamilyProperties ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	assert ( this->mDevice );
	assert ( instance );
	
	// Get available queue family properties
	u32 queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties ( this->mDevice, &queueCount, NULL );

	this->mQueueFamilyProperties.Init ( queueCount );
	this->mQueueFamilySupportsPresent.Init ( queueCount );

	vkGetPhysicalDeviceQueueFamilyProperties ( this->mDevice, &queueCount, this->mQueueFamilyProperties.GetBuffer ());

	this->mQueueFlags = 0;
	this->mSupportsPresent = true;

	for ( ZLIndex i = ZLIndexOp::ZERO; i < queueCount; ++i ) {
	
		VkBool32 supportsPresent = false;
		if ( surface ) {
			instance.GetPhysicalDeviceSurfaceSupportKHR ( this->mDevice, ( u32 )i, surface, &supportsPresent );
		}
		
		this->mQueueFamilySupportsPresent [ i ] = supportsPresent ? true : false;
		
		this->mQueueFlags |= this->mQueueFamilyProperties [ i ].queueFlags;
		this->mSupportsPresent = this->mSupportsPresent || supportsPresent;
	}
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitSupportedExtensions () {

	this->mSupportedExtensions.clear ();

	u32 extCount = 0;
	vkEnumerateDeviceExtensionProperties ( this->mDevice, nullptr, &extCount, nullptr );
	if ( extCount == 0 ) return;
	
	ZLLeanArray < VkExtensionProperties > extensions;
	extensions.Init ( extCount );
	
	if ( vkEnumerateDeviceExtensionProperties ( this->mDevice, NULL, &extCount, extensions.GetBuffer ()) == VK_SUCCESS ) {
		for ( ZLIndex i = ZLIndexOp::ZERO; i < extCount; ++i ) {
			this->mSupportedExtensions.insert ( extensions [ i ].extensionName );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitSurfaceCapabilities ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	VkResult result = instance.GetPhysicalDeviceSurfaceCapabilitiesKHR ( this->mDevice, surface, &this->mSurfaceCapabilities );
	assert ( result == VK_SUCCESS );
}

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::InitSurfaceFormats ( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface ) {

	assert ( instance );
	if ( !surface ) return;

	uint32_t formatCount;
	instance.GetPhysicalDeviceSurfaceFormatsKHR ( this->mDevice, surface, &formatCount, NULL );
	assert ( formatCount > 0 );

	this->mSurfaceFormats.Init ( formatCount );

	std::vector < VkSurfaceFormatKHR > surfaceFormats ( formatCount );
	instance.GetPhysicalDeviceSurfaceFormatsKHR ( this->mDevice, surface, &formatCount, this->mSurfaceFormats.GetBuffer ());
}

//----------------------------------------------------------------//
MOAIPhysicalDeviceVK::MOAIPhysicalDeviceVK () :
	mDevice ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIPhysicalDeviceVK::~MOAIPhysicalDeviceVK () {

	this->Finalize ();
}

//----------------------------------------------------------------//
const VkCompositeAlphaFlagBitsKHR* MOAIPhysicalDeviceVK::RankCompositeAlphaBits () const {

	static const VkCompositeAlphaFlagBitsKHR rankedBits [] = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    	VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR ,
    	VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    	( VkCompositeAlphaFlagBitsKHR )0,
	};
	return rankedBits;
}

//----------------------------------------------------------------//
const VkFormat* MOAIPhysicalDeviceVK::RankDepthFormats () const {

	// start with the highest precision packed format
	static const VkFormat depthFormats [] = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM,
		VK_FORMAT_UNDEFINED,
	};
	return depthFormats;
}

//----------------------------------------------------------------//
size_t MOAIPhysicalDeviceVK::ScoreDeviceType () const {

	switch ( this->mProperties.deviceType ) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:				return 1;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:		return 2;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:				return 3;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:	return 4;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:		return 5;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t MOAIPhysicalDeviceVK::ScorePresentMode ( VkPresentModeKHR presentMode ) {

	// TODO: accoring to https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain,
	// "...some drivers currently don't properly support VK_PRESENT_MODE_FIFO_KHR." which is a
	// mystery. the author recommends preferring VK_PRESENT_MODE_IMMEDIATE_KHR for that reason.
	// so we'll do that for now, but this bears revisiting. (Or, really, customizing through
	// a callback.)

	switch ( presentMode ) {
		case VK_PRESENT_MODE_FIFO_KHR:				return 1; // vsync
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:		return 2; // vsync, possible tearing
		case VK_PRESENT_MODE_IMMEDIATE_KHR:			return 3; // tearing
		case VK_PRESENT_MODE_MAILBOX_KHR:			return 4; // triple buffering
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIPhysicalDeviceVK::SuggestSwapChainImageCount ( VkPresentModeKHR presentMode ) {

	u32 minCount = this->mSurfaceCapabilities.minImageCount;
	u32 maxCount = this->mSurfaceCapabilities.maxImageCount ? this->mSurfaceCapabilities.maxImageCount : 3;

	u32 count = maxCount;

	switch ( presentMode ) {
		case VK_PRESENT_MODE_FIFO_KHR:
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			break;
			
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			count = minCount + 1;
			break;
			
		case VK_PRESENT_MODE_MAILBOX_KHR:
		 	count = 3;
		 	break;
	}
	
	count = ( maxCount < count ) ? maxCount : count;
	count = ( count < minCount ) ? minCount : count;
	
	return count;
}

//----------------------------------------------------------------//
bool MOAIPhysicalDeviceVK::SupportsExtension ( cc8* extensionName ) const {

	return ( this->mSupportedExtensions.find ( extensionName ) != this->mSupportedExtensions.cend ());
}

//----------------------------------------------------------------//
bool MOAIPhysicalDeviceVK::SupportsRenderAndPresent () const {

	return (( this->mQueueFlags | VK_QUEUE_GRAPHICS_BIT ) && this->mSupportsPresent );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPhysicalDeviceVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->FinalizeClients ();

	MOAIGfxInstanceVK& instance = this->GetVulkanInstance ();

//	if ( this->mSwapChain != VK_NULL_HANDLE ) {
//		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mImages.Size (); ++i ) {
//			vkDestroyImageView ( logicalDevice, this->mBuffers [ i ].view, NULL );
//		}
//		logicalDevice.DestroySwapchainKHR ( this->mSwapChain, NULL );
//		this->mSwapChain = VK_NULL_HANDLE;
//	}
	instance.RemoveClient ( *this );
}
