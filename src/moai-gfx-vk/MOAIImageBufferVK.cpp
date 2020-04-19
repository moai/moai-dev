// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIImageBufferVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

//================================================================//
// MOAIImageBufferSnapshotVK
//================================================================//

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::Init (
	MOAILogicalDeviceVK& logicalDevice,
	VkImage image,
	VkImageViewType viewType,
	VkFormat format,
	VkComponentMapping components,
	VkImageSubresourceRange subresourceRange,
	VkImageViewCreateFlags flags
) {
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mImage = image;
	
	VkImageViewCreateInfo imageViewCreateInfo = MOAIGfxStructVK::imageViewCreateInfo (
		image,
		viewType,
		format,
		components,
		subresourceRange,
		flags
	);
	VK_CHECK_RESULT ( vkCreateImageView ( logicalDevice, &imageViewCreateInfo, NULL, &this->mImageView ));
}

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::Init (
	MOAILogicalDeviceVK& logicalDevice,
	const VkImageCreateInfo& imageCreateInfo,
	VkImageViewCreateInfo imageViewCreateInfo
) {
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );

	this->mImage = logicalDevice.CreateImage ( imageCreateInfo );
	imageViewCreateInfo.image = this->mImage;

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements ( logicalDevice, this->mImage, &memReqs );

	u32 memoryTypeIndex;
	MOAIPhysicalDeviceVK& physicalDevice = logicalDevice.GetDependency < MOAIPhysicalDeviceVK >();
	bool didFindMemoryTypeIndex = physicalDevice.FindMemoryTypeIndex ( memoryTypeIndex, memReqs.memoryTypeBits );
	assert ( didFindMemoryTypeIndex );

	VkMemoryAllocateInfo memAllocInfo = MOAIGfxStructVK::memoryAllocateInfo ( memReqs.size, memoryTypeIndex );
	this->mImageMemory = logicalDevice.AllocateMemory ( memAllocInfo );
	logicalDevice.BindImageMemory ( this->mImage, this->mImageMemory );

	this->mImageView = logicalDevice.CreateImageView ( imageViewCreateInfo );
}

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::Init (
	MOAILogicalDeviceVK& logicalDevice, VkImageView imageView
) {
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mImageView = imageView;
}

//----------------------------------------------------------------//
MOAIImageBufferSnapshotVK::MOAIImageBufferSnapshotVK () :
	mImage ( VK_NULL_HANDLE ),
	mImageMemory ( VK_NULL_HANDLE ),
	mImageView ( VK_NULL_HANDLE ) {
	
	RTTI_BEGIN ( MOAIImageBufferSnapshotVK )
		RTTI_EXTEND ( ZLFinalizable )
		RTTI_VISITOR ( ZLAbstractFinalizationVisitor, ZLFinalizationVisitor < MOAIImageBufferSnapshotVK >)
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImageBufferSnapshotVK::~MOAIImageBufferSnapshotVK () {

	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::_Finalize () {
	
	if ( this->mImageView ) {
		vkDestroyImageView ( this->GetDependency < MOAILogicalDeviceVK >(), this->mImageView, NULL );
	}
}

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::MOAIAbstractSnapshotVK_OnPin ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );
}

//----------------------------------------------------------------//
void MOAIImageBufferSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {
}
