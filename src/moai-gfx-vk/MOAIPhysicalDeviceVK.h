// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPHYSICALDEVICEVK_H
#define MOAIPHYSICALDEVICEVK_H

#include <vector>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>

//================================================================//
// MOAIGfxQueueSetVK
//================================================================//
class MOAIGfxQueueSetVK {
public:

	static const VkQueueFlags KNOWN_QUEUE_TYPES		= VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
	static const u32 MAX_QUEUES							= 4;

	VkQueueFlags	mQueueTypes;
	bool			mCanPresent;

	u32		mGraphicsQueueIndex;
	u32		mComputeQueueIndex;
	u32		mPresentQueueIndex;
	u32		mTransferQueueIndex;
	
	ZLLeanArray < VkDeviceQueueCreateInfo > mCreateInfos;
	
	//----------------------------------------------------------------//
	MOAIGfxQueueSetVK () :
		mQueueTypes ( 0 ),
		mCanPresent ( false ),
		mGraphicsQueueIndex (( u32 )-1 ),
		mComputeQueueIndex (( u32 )-1 ),
		mPresentQueueIndex (( u32 )-1 ),
		mTransferQueueIndex (( u32 )-1 ) {
	}
};

//================================================================//
// MOAIPhysicalDeviceVK
//================================================================//
class MOAIPhysicalDeviceVK :
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAIGfxInstanceVK > {
private:

	typedef const VkCompositeAlphaFlagBitsKHR* AlphaRank;
	typedef const VkFormat* FormatRank;

	// for internal use (normalizes 'present' as a bit flag for cleaner selection code)
	enum {
		QUEUE_GRAPHICS_BIT		= 0x01,
		QUEUE_COMPUTE_BIT		= 0x02,
		QUEUE_TRANSFER_BIT		= 0x04,
		QUEUE_PRESENT_BIT		= 0x08,
	};

	//----------------------------------------------------------------//
	VkFormat			FindFormat						( const VkFormat* rankedFormats, VkFormatFeatureFlags optimalTilingFeatures, VkFormatFeatureFlags linearTilingFeatures = 0, VkFormatFeatureFlags bufferFeatures = 0 );
	bool				FindQueue						( u32& queueIndex, u32 criteria, u32 exclude = 0 );
	u32					GetQueueFlags					( ZLIndex queueIndex );
	void				InitBasicInfo					( VkPhysicalDevice device );
	void				InitFullInfo					( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
	void				InitPresentModes				( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
	void				InitQueueFamilyProperties		( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
	void				InitSupportedExtensions			();
	void				InitSurfaceCapabilities			( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
	void				InitSurfaceFormats				( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
	AlphaRank			RankCompositeAlphaBits			() const;
	FormatRank			RankDepthFormats				() const;
	size_t				ScoreDeviceType					() const; // TODO: make user configurable
	static size_t		ScorePresentMode				( VkPresentModeKHR presentMode ); // TODO: make user configurable

public:

	IMPLEMENT_FINALIZABLE ( MOAIPhysicalDeviceVK )

	VkPhysicalDevice							mDevice;
	VkPhysicalDeviceProperties					mProperties;
	VkPhysicalDeviceFeatures					mFeatures;
	VkPhysicalDeviceMemoryProperties			mMemoryProperties;
	ZLLeanArray < VkQueueFamilyProperties >		mQueueFamilyProperties;
	ZLLeanArray < bool >						mQueueFamilySupportsPresent;
	ZLLeanArray < VkSurfaceFormatKHR >			mSurfaceFormats;
	STLSet < STLString >						mSupportedExtensions;
	VkFormat									mDepthFormat;
	
	VkSurfaceCapabilitiesKHR					mSurfaceCapabilities;
	ZLLeanArray < VkPresentModeKHR >			mPresentModes;
	
	VkQueueFlags								mQueueFlags;
	bool										mSupportsPresent;
	
	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mDevice != VK_NULL_HANDLE );
	}

	//----------------------------------------------------------------//
	bool operator < ( const MOAIPhysicalDeviceVK& compare ) const {
		
		size_t selfDeviceTypeScore = this->ScoreDeviceType ();
		size_t otherDeviceTypeScore = compare.ScoreDeviceType ();
		
		// TODO: this is pretty naive. itegrated GPU always wins. need more scoring after this
		// that takes into accounts features, etc.
		if ( selfDeviceTypeScore != otherDeviceTypeScore ) {
			return selfDeviceTypeScore < otherDeviceTypeScore;
		}
		return false;
	}

	//----------------------------------------------------------------//
	operator VkPhysicalDevice () {
		return this->mDevice;
	}

	//----------------------------------------------------------------//
	VkCompositeAlphaFlagBitsKHR		FindCompositeAlpha				();
	bool							FindMemoryTypeIndex				( u32& index, u32 typeBits, VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	VkPresentModeKHR				FindPresentMode					();
	void							FindQueues						( MOAIGfxQueueSetVK& queueSet, VkQueueFlags requestedQueueTypes, bool requestPresent );
	VkSurfaceFormatKHR				FindSurfaceFormat				( VkFormat colorFormat, VkColorSpaceKHR colorSpace );
	VkSurfaceTransformFlagBitsKHR	FindSurfaceTransform			( VkSurfaceTransformFlagBitsKHR preferredTransform );
	VkExtent2D						GetExtent						( u32 preferredWidth, u32 preferredHeight );
	void							Initialize						( MOAIGfxInstanceVK& instance, MOAISurfaceVK& surface );
									MOAIPhysicalDeviceVK			();
									~MOAIPhysicalDeviceVK			();
	u32								SuggestSwapChainImageCount		( VkPresentModeKHR presentMode );
	bool							SupportsExtension				( cc8* extensionName ) const;
	bool							SupportsRenderAndPresent		() const;
};

#endif
