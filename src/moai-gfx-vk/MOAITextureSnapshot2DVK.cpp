// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/ZLTextureFormat.h>

void transitionImageLayout ( VkCommandBuffer& commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout ) {

	VkImageMemoryBarrier barrier = MOAIGfxStructVK::imageMemoryBarrier ( image, oldLayout, newLayout );
	
	vkCmdPipelineBarrier (
		commandBuffer,
		0, // TODO
		0, // TODO
		0,
		0, NULL,
		0, NULL,
		1, &barrier
	);
}

void copyBufferToImage ( VkCommandBuffer& commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height ) {

	VkBufferImageCopy bufferImageCopy = MOAIGfxStructVK::bufferImageCopy (
		MOAIGfxStructVK::offset3D ( 0, 0, 0 ),
		MOAIGfxStructVK::extent3D ( width, height, 1 )
	);

	vkCmdCopyBufferToImage (
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&bufferImageCopy
	);
}

//================================================================//
// MOAITextureSnapshot2DVK
//================================================================//

//----------------------------------------------------------------//
bool MOAITextureSnapshot2DVK::Initialize ( ZLImage& srcImage ) {

	if ( !srcImage.IsOK ()) return false;

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );

	ZLImage altImage;

	ZLColor::ColorFormat colorFormat = srcImage.GetColorFormat ();
	if ( colorFormat == ZLColor::CLR_FMT_UNKNOWN ) return false;

	if (( colorFormat == ZLColor::A_1 ) || ( colorFormat == ZLColor::A_4 )) {
		colorFormat = ZLColor::A_8;
	}
	else {
		colorFormat = ZLColor::RGBA_8888; // TODO: fix to select this more intelligently based on actually available color formats
	}
	
	if (( colorFormat != srcImage.GetColorFormat ()) || ( srcImage.GetPixelFormat () != ZLImage::TRUECOLOR )) {
		altImage.Convert ( srcImage, colorFormat, ZLImage::TRUECOLOR );
	}
	
	ZLImage& image = altImage.IsOK () ? altImage : srcImage;
	if ( !image.IsOK ()) return false;

	// get the dimensions before trying to get the OpenGL texture ID
//	this->mWidth = image.GetWidth ();
//	this->mHeight = image.GetHeight ();

	VkFormat format;

	switch ( colorFormat ) {
		
		case ZLColor::A_8:
			format = VK_FORMAT_R8_UNORM;
			break;
		
		case ZLColor::LA_8:
			format = VK_FORMAT_R8G8_UNORM;
			break;
		
		case ZLColor::RGB_888:
			format = VK_FORMAT_R8G8B8_UNORM;
			break;
		
		case ZLColor::RGB_565:
			format = VK_FORMAT_B5G6R5_UNORM_PACK16;
			break;
		
		case ZLColor::RGBA_5551:
			format = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
			break;
		
		case ZLColor::RGBA_4444:
			format = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
			break;
		
		case ZLColor::RGBA_8888:
			format = VK_FORMAT_R8G8B8A8_UNORM;
			break;
			
		default:
			return false;
	}
	
	size_t bitmapSize = image.GetBitmapSize ();
	const void* bitmap = image.GetBitmap ();
	u32 width = image.GetWidth ();
	u32 height = image.GetHeight ();

	MOAIGfxBufferSnapshotVK stageImage;
	stageImage.Initialize ( logicalDevice, bitmapSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
	stageImage.MapAndCopy ( bitmap, bitmapSize );
	
	VkImageCreateInfo imageInfo = MOAIGfxStructVK::imageCreateInfo (
		VK_IMAGE_TYPE_2D,
		format,
		MOAIGfxStructVK::extent3D ( width, height, 1 )
	);
	
	this->mTextureImage;
    VK_CHECK_RESULT ( vkCreateImage ( logicalDevice, &imageInfo, NULL, &this->mTextureImage ));

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements ( logicalDevice, this->mTextureImage, &memReqs );

	u32 memoryTypeIndex;
	bool didFindMemoryTypeIndex = physicalDevice.FindMemoryTypeIndex ( memoryTypeIndex, memReqs.memoryTypeBits );
	assert ( didFindMemoryTypeIndex );

	VkMemoryAllocateInfo memoryAllocateInfo = MOAIGfxStructVK::memoryAllocateInfo ( memReqs.size, memoryTypeIndex );

	this->mTextureImageMemory;
	VK_CHECK_RESULT ( vkAllocateMemory ( logicalDevice, &memoryAllocateInfo, nullptr, &this->mTextureImageMemory ));
	VK_CHECK_RESULT ( vkBindImageMemory ( logicalDevice, this->mTextureImage, this->mTextureImageMemory, 0 ));
	
	// Buffer copies have to be submitted to a queue, so we need a command buffer for them
	// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
	MOAIQueueVK& graphicsQueue = logicalDevice.GetGraphicsQueue ();
	
	MOAICommandBufferVK commandBuffer;
	graphicsQueue.CreateCommandBuffer ( commandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY, true );

	transitionImageLayout ( commandBuffer, this->mTextureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	copyBufferToImage ( commandBuffer, stageImage, this->mTextureImage, width, height );
	transitionImageLayout ( commandBuffer, this->mTextureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
	graphicsQueue.FlushAndFreeCommandBuffer ( commandBuffer );
	
	VkSamplerCreateInfo samplerInfo = MOAIGfxStructVK::samplerCreateInfo ();
	VK_CHECK_RESULT ( vkCreateSampler ( logicalDevice, &samplerInfo, nullptr, &this->mTextureSampler ));
	
	VkImageViewCreateInfo viewInfo = MOAIGfxStructVK::imageViewCreateInfo (
		this->mTextureImage,
		VK_IMAGE_VIEW_TYPE_2D,
		format
	);
	VK_CHECK_RESULT ( vkCreateImageView ( logicalDevice, &viewInfo, nullptr, &this->mTextureImageView ));
	
	this->mTextureDescriptor = MOAIGfxStructVK::descriptorImageInfo ( this->mTextureSampler, this->mTextureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	
	this->mTextureSize = bitmapSize;
	
//	if ( this->ShouldGenerateMipmaps ()) {
//
//		u32 mipLevel = 1;
//
//		ZLImage mipmap;
//		mipmap.Copy ( image );
//
//		while ( mipmap.MipReduce ()) {
//
//			gfx.TexImage2D (
//				mipLevel++,
//				this->mGLInternalFormat,
//				mipmap.GetWidth (),
//				mipmap.GetHeight (),
//				this->mGLInternalFormat,
//				this->mGLPixelType,
//				mipmap.GetBitmapBuffer ()
//			);
//
//			if ( this->mGfxMgr->LogErrors ()) {
//				this->CleanupOnError ();
//				return false;
//			}
//			this->mTextureSize += mipmap.GetBitmapSize ();
//		}
//	}
		
	return true;
}

//----------------------------------------------------------------//
MOAITextureSnapshot2DVK::MOAITextureSnapshot2DVK () :
	mTextureImageView ( VK_NULL_HANDLE ),
	mTextureSize ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextureSnapshot2DVK::~MOAITextureSnapshot2DVK () {

	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITextureSnapshot2DVK::Visitor_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

		vkDestroyImageView ( logicalDevice, this->mTextureImageView, NULL );
		vkDestroyImage ( logicalDevice, this->mTextureImage, NULL);
		vkFreeMemory ( logicalDevice, this->mTextureImageMemory, NULL );
	}
}
