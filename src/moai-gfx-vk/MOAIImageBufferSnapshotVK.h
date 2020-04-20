// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEBUFFERSNAPSHOTVK_H
#define	MOAIIMAGEBUFFERSNAPSHOTVK_H

#include <moai-gfx-vk/MOAISnapshotVK.h>

//================================================================//
// MOAIImageBufferSnapshotVK
//================================================================//
// TODO: doxygen
class MOAIImageBufferSnapshotVK :
	public virtual MOAISnapshotVK < MOAIImageBufferSnapshotVK >,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK >{
protected:

	ZL_FINALIZATION_VISITOR_FRIEND

	VkImage				mImage;
	VkDeviceMemory		mImageMemory;
	VkImageView			mImageView; // assume a primary image view; additional image vies can be added later

	//----------------------------------------------------------------//
	void							_Finalize								();
	void							MOAIAbstractSnapshotVK_OnPin			( MOAICommandBufferVK& commandBuffer );
	void							MOAIAbstractSnapshotVK_OnUnpin			();
	MOAIImageBufferSnapshotVK*		MOAISnapshotFactoryVK_GetSnapshot		();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIImageBufferSnapshotVK )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mImage != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkDeviceMemory& () {
		return this->mImageMemory;
	}
	
	//----------------------------------------------------------------//
	operator VkImage& () {
		return this->mImage;
	}
	
	//----------------------------------------------------------------//
	operator VkImageView& () {
		return this->mImageView;
	}
	
	//----------------------------------------------------------------//
	void		Init								(	MOAILogicalDeviceVK& logicalDevice,
														VkImage image,
														VkImageViewType viewType,
														VkFormat format,
														VkComponentMapping components,
														VkImageSubresourceRange subresourceRange,
														VkImageViewCreateFlags flags
													);
	void		Init								( MOAILogicalDeviceVK& logicalDevice, const VkImageCreateInfo& imageCreateInfo, VkImageViewCreateInfo imageViewCreateInfo );
	void		Init								( MOAILogicalDeviceVK& logicalDevice, VkImageView imageView );
				MOAIImageBufferSnapshotVK			();
				~MOAIImageBufferSnapshotVK			();
};

#endif
