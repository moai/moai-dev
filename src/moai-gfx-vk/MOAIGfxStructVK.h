// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXSTRUCTVK_H
#define MOAIGFXSTRUCTVK_H

#include <vector>
#include "vulkan/vulkan.h"

#define DECL_VK_STRUCT(type, name)			\
	type name;								\
	memset ( &name, 0, sizeof ( type ));

//================================================================//
// MOAIGfxStructVK
//================================================================//
class MOAIGfxStructVK {
public:

    //----------------------------------------------------------------//
    static VkApplicationInfo applicationInfo ( const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion ) {
        DECL_VK_STRUCT ( VkApplicationInfo, applicationInfo );
        applicationInfo.sType                   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext                   = NULL;
        applicationInfo.pApplicationName        = appName;
        applicationInfo.applicationVersion      = appVersion;
        applicationInfo.pEngineName             = engineName;
        applicationInfo.engineVersion           = engineVersion;
        applicationInfo.apiVersion              = apiVersion;
        return applicationInfo;
    }

    //----------------------------------------------------------------//
    static VkAttachmentDescription attachmentDescription (
        VkFormat format,
        VkSampleCountFlagBits samples,
        VkAttachmentLoadOp loadOp,
        VkAttachmentStoreOp storeOp,
        VkAttachmentLoadOp stencilLoadOp,
        VkAttachmentStoreOp stencilStoreOp,
        VkImageLayout initialLayout,
        VkImageLayout finalLayout,
        VkAttachmentDescriptionFlags flags = 0
    ) {
        DECL_VK_STRUCT ( VkAttachmentDescription, attachmentDescription );
        attachmentDescription.format            = format;
        attachmentDescription.samples           = samples;
        attachmentDescription.loadOp            = loadOp;
        attachmentDescription.storeOp           = storeOp;
        attachmentDescription.stencilLoadOp     = stencilLoadOp;
        attachmentDescription.stencilStoreOp    = stencilStoreOp;
        attachmentDescription.initialLayout     = initialLayout;
        attachmentDescription.finalLayout       = finalLayout;
        attachmentDescription.flags             = flags;
        return attachmentDescription;
    }

    //----------------------------------------------------------------//
    static VkAttachmentReference attachmentReference ( uint32_t attachment, VkImageLayout layout ) {
        DECL_VK_STRUCT ( VkAttachmentReference, attachmentReference );
        attachmentReference.attachment      = attachment;
        attachmentReference.layout          = layout;
        return attachmentReference;
    };

    //----------------------------------------------------------------//
    static VkBindSparseInfo bindSparseInfo () {
        DECL_VK_STRUCT ( VkBindSparseInfo, bindSparseInfo );
        bindSparseInfo.sType                = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
        bindSparseInfo.pNext                = NULL;
        return bindSparseInfo;
    }

    //----------------------------------------------------------------//
    static VkBufferCreateInfo bufferCreateInfo () {
        DECL_VK_STRUCT ( VkBufferCreateInfo, bufCreateInfo );
        bufCreateInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufCreateInfo.pNext                 = NULL;
        return bufCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkBufferCreateInfo bufferCreateInfo ( VkBufferUsageFlags usage, VkDeviceSize size ) {
        DECL_VK_STRUCT ( VkBufferCreateInfo, bufCreateInfo );
        bufCreateInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufCreateInfo.pNext                 = NULL;
        bufCreateInfo.usage                 = usage;
        bufCreateInfo.size                  = size;
        return bufCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkBufferMemoryBarrier bufferMemoryBarrier () {
        DECL_VK_STRUCT ( VkBufferMemoryBarrier, bufferMemoryBarrier );
        bufferMemoryBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferMemoryBarrier.pNext                   = NULL;
        bufferMemoryBarrier.srcQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.dstQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
        return bufferMemoryBarrier;
    }

	//----------------------------------------------------------------//
	static VkClearColorValue clearColorValueF ( float r, float g, float b, float a ) {
		DECL_VK_STRUCT ( VkClearColorValue, clearColorValue );
		clearColorValue.float32 [ 0 ] 		= r;
		clearColorValue.float32 [ 1 ] 		= g;
		clearColorValue.float32 [ 2 ] 		= b;
		clearColorValue.float32 [ 3 ] 		= a;
		return clearColorValue;
	}

	//----------------------------------------------------------------//
	static VkClearDepthStencilValue clearDepthStencilValue ( float depth, uint32_t stencil ) {
		VkClearDepthStencilValue clearDepthStencilValue;
		clearDepthStencilValue.depth		= depth;
		clearDepthStencilValue.stencil		= stencil;
		return clearDepthStencilValue;
	}
	
	//----------------------------------------------------------------//
	static VkClearValue clearValue ( VkClearColorValue color, VkClearDepthStencilValue depthStencil ) {
		VkClearValue clearValue;
		clearValue.color			= color;
		clearValue.depthStencil		= depthStencil;
		return clearValue;
	}

    //----------------------------------------------------------------//
    static VkCommandBufferAllocateInfo commandBufferAllocateInfo ( VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t bufferCount ) {
        DECL_VK_STRUCT ( VkCommandBufferAllocateInfo, commandBufferAllocateInfo );
        commandBufferAllocateInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext                 = NULL;
        commandBufferAllocateInfo.commandPool           = commandPool;
        commandBufferAllocateInfo.level                 = level;
        commandBufferAllocateInfo.commandBufferCount    = bufferCount;
        return commandBufferAllocateInfo;
    }

    //----------------------------------------------------------------//
    static VkCommandBufferBeginInfo commandBufferBeginInfo (
        const VkCommandBufferInheritanceInfo* inheritanceInfo = NULL,
        VkCommandBufferUsageFlags flags = 0
    ) {
        DECL_VK_STRUCT ( VkCommandBufferBeginInfo, cmdBufferBeginInfo );
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBufferBeginInfo.pNext                = NULL;
        cmdBufferBeginInfo.pInheritanceInfo     = inheritanceInfo;
        cmdBufferBeginInfo.flags                = flags;
        return cmdBufferBeginInfo;
    }

    //----------------------------------------------------------------//
    static VkCommandBufferInheritanceInfo commandBufferInheritanceInfo (
        VkRenderPass renderPass,
        uint32_t subpass,
        VkFramebuffer framebuffer,
        VkBool32 occlusionQueryEnable,
        VkQueryControlFlags queryFlags,
        VkQueryPipelineStatisticFlags pipelineStatistics
    ) {
        DECL_VK_STRUCT ( VkCommandBufferInheritanceInfo, cmdBufferInheritanceInfo );
        cmdBufferInheritanceInfo.sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        cmdBufferInheritanceInfo.pNext                  = NULL;
        cmdBufferInheritanceInfo.renderPass             = renderPass;
        cmdBufferInheritanceInfo.subpass                = subpass;
        cmdBufferInheritanceInfo.framebuffer            = framebuffer;
        cmdBufferInheritanceInfo.occlusionQueryEnable   = occlusionQueryEnable;
        cmdBufferInheritanceInfo.queryFlags             = queryFlags;
        cmdBufferInheritanceInfo.pipelineStatistics     = pipelineStatistics;
        return cmdBufferInheritanceInfo;
    }

    //----------------------------------------------------------------//
    static VkCommandPoolCreateInfo commandPoolCreateInfo ( uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0 ) {
        DECL_VK_STRUCT ( VkCommandPoolCreateInfo, cmdPoolCreateInfo );
        cmdPoolCreateInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolCreateInfo.pNext                 = NULL;
        cmdPoolCreateInfo.queueFamilyIndex      = queueFamilyIndex;
        cmdPoolCreateInfo.flags                 = flags;
        return cmdPoolCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkComponentMapping componentMapping (
        VkComponentSwizzle r    = VK_COMPONENT_SWIZZLE_IDENTITY,
        VkComponentSwizzle g    = VK_COMPONENT_SWIZZLE_IDENTITY,
        VkComponentSwizzle b    = VK_COMPONENT_SWIZZLE_IDENTITY,
        VkComponentSwizzle a    = VK_COMPONENT_SWIZZLE_IDENTITY
    ) {
    	VkComponentMapping componentMapping;
        componentMapping.r = r;
        componentMapping.g = g;
        componentMapping.b = b;
        componentMapping.a = a;
        return componentMapping;
    }

    //----------------------------------------------------------------//
    static VkComputePipelineCreateInfo computePipelineCreateInfo ( VkPipelineLayout layout, VkPipelineCreateFlags flags = 0 ) {
        DECL_VK_STRUCT ( VkComputePipelineCreateInfo, computePipelineCreateInfo );
        computePipelineCreateInfo.sType     = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCreateInfo.pNext     = NULL;
        computePipelineCreateInfo.layout    = layout;
        computePipelineCreateInfo.flags     = flags;
        return computePipelineCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkDescriptorImageInfo descriptorImageInfo ( VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout ) {
        DECL_VK_STRUCT ( VkDescriptorImageInfo, descriptorImageInfo );
        descriptorImageInfo.sampler         = sampler;
        descriptorImageInfo.imageView       = imageView;
        descriptorImageInfo.imageLayout     = imageLayout;
        return descriptorImageInfo;
    }

    //----------------------------------------------------------------//
    static VkDescriptorPoolCreateInfo descriptorPoolCreateInfo ( VkDescriptorPoolSize* pPoolSizes, uint32_t poolSizeCount, uint32_t maxSets ) {
        DECL_VK_STRUCT ( VkDescriptorPoolCreateInfo, descriptorPoolInfo );
        descriptorPoolInfo.sType            = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext            = NULL;
        descriptorPoolInfo.poolSizeCount    = poolSizeCount;
        descriptorPoolInfo.pPoolSizes       = pPoolSizes;
        descriptorPoolInfo.maxSets          = maxSets;
        return descriptorPoolInfo;
    }

    //----------------------------------------------------------------//
    static VkDescriptorPoolSize descriptorPoolSize ( VkDescriptorType type, uint32_t descriptorCount ) {
        DECL_VK_STRUCT ( VkDescriptorPoolSize, descriptorPoolSize );
        descriptorPoolSize.type             = type;
        descriptorPoolSize.descriptorCount  = descriptorCount;
        return descriptorPoolSize;
    }

    //----------------------------------------------------------------//
    static VkDescriptorSetAllocateInfo descriptorSetAllocateInfo ( VkDescriptorPool descriptorPool, const VkDescriptorSetLayout* pSetLayouts, uint32_t descriptorSetCount = 1 ) {
        DECL_VK_STRUCT ( VkDescriptorSetAllocateInfo, descriptorSetAllocateInfo );
        descriptorSetAllocateInfo.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.pNext                 = NULL;
        descriptorSetAllocateInfo.descriptorPool        = descriptorPool;
        descriptorSetAllocateInfo.pSetLayouts           = pSetLayouts;
        descriptorSetAllocateInfo.descriptorSetCount    = descriptorSetCount;
        return descriptorSetAllocateInfo;
    }

    //----------------------------------------------------------------//
    static VkDescriptorSetLayoutBinding descriptorSetLayoutBinding (
    	uint32_t binding,
    	VkDescriptorType type,
    	VkShaderStageFlags stageFlags,
    	uint32_t descriptorCount = 1,
    	const VkSampler* pImmutableSamplers = NULL
	) {
        DECL_VK_STRUCT ( VkDescriptorSetLayoutBinding, setLayoutBinding );
        setLayoutBinding.binding                    = binding;
        setLayoutBinding.descriptorType             = type;
        setLayoutBinding.descriptorCount            = descriptorCount;
        setLayoutBinding.stageFlags                 = stageFlags;
        setLayoutBinding.pImmutableSamplers			= pImmutableSamplers;
        return setLayoutBinding;
    }

    //----------------------------------------------------------------//
    static VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo ( const VkDescriptorSetLayoutBinding* pBindings, uint32_t bindingCount = 1 ) {
        DECL_VK_STRUCT ( VkDescriptorSetLayoutCreateInfo, descriptorSetLayoutCreateInfo );
        descriptorSetLayoutCreateInfo.sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.pNext             = NULL;
        descriptorSetLayoutCreateInfo.pBindings         = pBindings;
        descriptorSetLayoutCreateInfo.bindingCount      = bindingCount;
        return descriptorSetLayoutCreateInfo;
    }

	//----------------------------------------------------------------//
    static VkDeviceCreateInfo deviceCreateInfo (
		const VkDeviceQueueCreateInfo* pQueueCreateInfos		= NULL,
		uint32_t queueCreateInfoCount							= 0,
		const char* const* ppEnabledLayerNames					= NULL,
		uint32_t enabledLayerCount								= 0,
		const char* const* ppEnabledExtensionNames				= NULL,
		uint32_t enabledExtensionCount							= 0,
		const VkPhysicalDeviceFeatures* pEnabledFeatures		= NULL,
		VkDeviceCreateFlags flags								= 0
    ) {
        DECL_VK_STRUCT ( VkDeviceCreateInfo, deviceCreateInfo );
        deviceCreateInfo.sType             			= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext             			= NULL;
		deviceCreateInfo.queueCreateInfoCount		= queueCreateInfoCount;
		deviceCreateInfo.pQueueCreateInfos			= pQueueCreateInfos;
		deviceCreateInfo.enabledLayerCount			= enabledLayerCount;
		deviceCreateInfo.ppEnabledLayerNames		= ppEnabledLayerNames;
		deviceCreateInfo.enabledExtensionCount		= enabledExtensionCount;
		deviceCreateInfo.ppEnabledExtensionNames	= ppEnabledExtensionNames;
		deviceCreateInfo.pEnabledFeatures			= pEnabledFeatures;
        deviceCreateInfo.flags             			= flags;
        return deviceCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkDeviceQueueCreateInfo deviceQueueCreateInfo (
        uint32_t queueFamilyIndex,
        uint32_t queueCount                 = 1,
        const float* queuePriorities        = NULL,
        VkDeviceQueueCreateFlags flags      = 0
    ) {
        static const uint32_t MAX_QUEUE_COUNT = 16;
        assert ( queueCount < MAX_QUEUE_COUNT );
        
        static float defaultPriorities [ MAX_QUEUE_COUNT ];
        static bool init = true;
    
        if ( init ) {
            memset ( defaultPriorities, 0, sizeof ( float ) * MAX_QUEUE_COUNT );
        }
    
        DECL_VK_STRUCT ( VkDeviceQueueCreateInfo, deviceQueueCreateInfo );
        deviceQueueCreateInfo.sType                     = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext                     = NULL;
        deviceQueueCreateInfo.queueFamilyIndex          = queueFamilyIndex;
        deviceQueueCreateInfo.queueCount                = queueCount;
        deviceQueueCreateInfo.pQueuePriorities          = queuePriorities ? queuePriorities : defaultPriorities;
        deviceQueueCreateInfo.flags                     = flags;
        
        return deviceQueueCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkEventCreateInfo eventCreateInfo () {
        DECL_VK_STRUCT ( VkEventCreateInfo, eventCreateInfo );
        eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
        return eventCreateInfo;
    }

	//----------------------------------------------------------------//
	static VkExtent2D extent2D ( int32_t width, int32_t height ) {
		VkExtent2D extent2D;
		extent2D.width = width;
		extent2D.height = height;
		return extent2D;
	}
	
	//----------------------------------------------------------------//
	static VkExtent3D extent3D ( int32_t width, int32_t height, int32_t depth ) {
		VkExtent3D extent3D;
		extent3D.width = width;
		extent3D.height = height;
		extent3D.depth = depth;
		return extent3D;
	}

    //----------------------------------------------------------------//
    static VkFramebufferCreateInfo framebufferCreateInfo (
        VkRenderPass renderPass,
        const VkImageView* attachments,
        uint32_t attachmentCount,
        uint32_t width,
        uint32_t height,
        uint32_t layers                     = 1,
        VkFramebufferCreateFlags flags      = 0
    ) {
        DECL_VK_STRUCT ( VkFramebufferCreateInfo, framebufferCreateInfo );
        framebufferCreateInfo.sType             = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext             = NULL;
        framebufferCreateInfo.renderPass        = renderPass;
        framebufferCreateInfo.pAttachments      = attachments;
        framebufferCreateInfo.attachmentCount   = attachmentCount;
        framebufferCreateInfo.width             = width;
        framebufferCreateInfo.height            = height;
        framebufferCreateInfo.layers            = layers;
        framebufferCreateInfo.flags             = flags;
        return framebufferCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkFenceCreateInfo fenceCreateInfo ( VkFenceCreateFlags flags = 0 ) {
        DECL_VK_STRUCT ( VkFenceCreateInfo, fenceCreateInfo );
        fenceCreateInfo.sType               = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext               = NULL;
        fenceCreateInfo.flags               = flags;
        return fenceCreateInfo;
    }

//    //----------------------------------------------------------------//
//    static VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo () {
//        DECL_VK_STRUCT ( VkGraphicsPipelineCreateInfo, graphicsPipelineCreateInfo );
//        graphicsPipelineCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//        graphicsPipelineCreateInfo.pNext                = NULL;
//        graphicsPipelineCreateInfo.basePipelineIndex    = -1;
//        graphicsPipelineCreateInfo.basePipelineHandle   = VK_NULL_HANDLE;
//        return graphicsPipelineCreateInfo;
//    }

    //----------------------------------------------------------------//
    static VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo (
		const VkPipelineShaderStageCreateInfo* stages,
		uint32_t stageCount,
		const VkPipelineVertexInputStateCreateInfo* vertexInputState,
		const VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState,
		const VkPipelineTessellationStateCreateInfo* tessellationState,
		const VkPipelineViewportStateCreateInfo* viewportState,
		const VkPipelineRasterizationStateCreateInfo* rasterizationState,
		const VkPipelineMultisampleStateCreateInfo* multisampleState,
		const VkPipelineDepthStencilStateCreateInfo* depthStencilState,
		const VkPipelineColorBlendStateCreateInfo* colorBlendState,
		const VkPipelineDynamicStateCreateInfo* dynamicState,
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		uint32_t subpass				= 0,
		VkPipeline basePipelineHandle	= VK_NULL_HANDLE,
		int32_t basePipelineIndex		= -1,
		VkPipelineCreateFlags flags		= 0
     ) {
        DECL_VK_STRUCT ( VkGraphicsPipelineCreateInfo, graphicsPipelineCreateInfo );
		graphicsPipelineCreateInfo.sType				= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCreateInfo.pNext				= NULL;
		graphicsPipelineCreateInfo.pStages				= stages;
		graphicsPipelineCreateInfo.stageCount			= stageCount;
		graphicsPipelineCreateInfo.pVertexInputState	= vertexInputState;
		graphicsPipelineCreateInfo.pInputAssemblyState	= inputAssemblyState;
		graphicsPipelineCreateInfo.pTessellationState	= tessellationState;
		graphicsPipelineCreateInfo.pViewportState		= viewportState;
		graphicsPipelineCreateInfo.pRasterizationState	= rasterizationState;
		graphicsPipelineCreateInfo.pMultisampleState	= multisampleState;
		graphicsPipelineCreateInfo.pDepthStencilState	= depthStencilState;
		graphicsPipelineCreateInfo.pColorBlendState		= colorBlendState;
		graphicsPipelineCreateInfo.pDynamicState		= dynamicState;
		graphicsPipelineCreateInfo.layout				= layout;
		graphicsPipelineCreateInfo.renderPass			= renderPass;
		graphicsPipelineCreateInfo.subpass				= subpass;
		graphicsPipelineCreateInfo.basePipelineHandle	= basePipelineHandle;
		graphicsPipelineCreateInfo.basePipelineIndex	= basePipelineIndex;
		graphicsPipelineCreateInfo.flags				= flags;
		
        return graphicsPipelineCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo (
        VkPipelineLayout layout,
        const VkPipelineShaderStageCreateInfo* stages,
        uint32_t stageCount,
        VkRenderPass renderPass,
        uint32_t subpass,
        VkPipeline basePipelineHandle,
        int32_t basePipelineIndex,
        const VkPipelineVertexInputStateCreateInfo* vertexInputState,
        const VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState,
        const VkPipelineTessellationStateCreateInfo* tessellationState,
        const VkPipelineViewportStateCreateInfo* viewportState,
        const VkPipelineRasterizationStateCreateInfo* rasterizationState,
        const VkPipelineMultisampleStateCreateInfo* multisampleState,
        const VkPipelineDepthStencilStateCreateInfo* depthStencilState,
        const VkPipelineColorBlendStateCreateInfo* colorBlendState,
        const VkPipelineDynamicStateCreateInfo* dynamicState,
        VkPipelineCreateFlags flags = 0
    ) {
        DECL_VK_STRUCT ( VkGraphicsPipelineCreateInfo, graphicsPipelineCreateInfo );
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.pNext                = NULL;
        graphicsPipelineCreateInfo.layout               = layout;
        graphicsPipelineCreateInfo.pStages              = stages;
        graphicsPipelineCreateInfo.stageCount           = stageCount;
        graphicsPipelineCreateInfo.renderPass           = renderPass;
        graphicsPipelineCreateInfo.subpass              = subpass;
        graphicsPipelineCreateInfo.basePipelineHandle   = basePipelineHandle;
        graphicsPipelineCreateInfo.basePipelineIndex    = basePipelineIndex;
        graphicsPipelineCreateInfo.pVertexInputState    = vertexInputState;
        graphicsPipelineCreateInfo.pInputAssemblyState  = inputAssemblyState;
        graphicsPipelineCreateInfo.pTessellationState   = tessellationState;
        graphicsPipelineCreateInfo.pViewportState       = viewportState;
        graphicsPipelineCreateInfo.pRasterizationState  = rasterizationState;
        graphicsPipelineCreateInfo.pMultisampleState    = multisampleState;
        graphicsPipelineCreateInfo.pDepthStencilState   = depthStencilState;
        graphicsPipelineCreateInfo.pColorBlendState     = colorBlendState;
        graphicsPipelineCreateInfo.pDynamicState        = dynamicState;
        graphicsPipelineCreateInfo.flags                = flags;
        return graphicsPipelineCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkImageCreateInfo imageCreateInfo (
            VkImageType imageType,
            VkFormat format,
            VkExtent3D extent,
            VkImageUsageFlags usage					= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            uint32_t mipLevels                      = 1,
            uint32_t arrayLayers                    = 1,
            VkSampleCountFlagBits samples           = VK_SAMPLE_COUNT_1_BIT,
            VkImageTiling tiling                    = VK_IMAGE_TILING_OPTIMAL,
            VkSharingMode sharingMode               = VK_SHARING_MODE_EXCLUSIVE,
            const uint32_t* queueFamilyIndices      = NULL,
            uint32_t queueFamilyIndexCount          = 0 ,
            VkImageLayout initialLayout             = VK_IMAGE_LAYOUT_UNDEFINED,
            VkImageCreateFlags flags                = 0
        ) {
        DECL_VK_STRUCT ( VkImageCreateInfo, imageCreateInfo );
        imageCreateInfo.sType                   = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext                   = NULL;
        imageCreateInfo.imageType               = imageType;
        imageCreateInfo.format                  = format;
        imageCreateInfo.extent                  = extent;
        imageCreateInfo.usage                   = usage;
        imageCreateInfo.mipLevels               = mipLevels;
        imageCreateInfo.arrayLayers             = arrayLayers;
        imageCreateInfo.samples                 = samples;
        imageCreateInfo.tiling                  = tiling;
        imageCreateInfo.sharingMode             = sharingMode;
        imageCreateInfo.pQueueFamilyIndices     = queueFamilyIndices;
        imageCreateInfo.queueFamilyIndexCount   = queueFamilyIndexCount;
        imageCreateInfo.initialLayout           = initialLayout;
        imageCreateInfo.flags                   = flags;
        return imageCreateInfo;
    }

    //----------------------------------------------------------------//
    /** @brief Initialize an image memory barrier with no image transfer ownership */
    static VkImageMemoryBarrier imageMemoryBarrier () {
        DECL_VK_STRUCT ( VkImageMemoryBarrier, imageMemoryBarrier );
        imageMemoryBarrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext                = NULL;
        imageMemoryBarrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
        return imageMemoryBarrier;
    }
    
    //----------------------------------------------------------------//
    static VkImageSubresourceRange imageSubresourceRange (
        VkImageAspectFlags aspectMask,
        uint32_t baseMipLevel           = 0,
        uint32_t levelCount             = 1,
        uint32_t baseArrayLayer         = 0,
        uint32_t layerCount             = 1
    ) {
        DECL_VK_STRUCT ( VkImageSubresourceRange, imageSubresourceRange );
        imageSubresourceRange.aspectMask        = aspectMask;
        imageSubresourceRange.baseMipLevel      = baseMipLevel;
        imageSubresourceRange.levelCount        = levelCount;
        imageSubresourceRange.baseArrayLayer    = baseArrayLayer;
        imageSubresourceRange.layerCount        = layerCount;
        return imageSubresourceRange;
    }
    
    //----------------------------------------------------------------//
    static VkImageViewCreateInfo imageViewCreateInfo (
            VkImageViewType viewType,
            VkImage image,
            VkFormat format,
            VkComponentMapping components,
            VkImageSubresourceRange subresourceRange,
            VkImageViewCreateFlags flags = 0
        ) {
        DECL_VK_STRUCT ( VkImageViewCreateInfo, imageViewCreateInfo );
        imageViewCreateInfo.sType               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext               = NULL;
        imageViewCreateInfo.viewType            = viewType;
        imageViewCreateInfo.image               = image;
        imageViewCreateInfo.format              = format;
        imageViewCreateInfo.components          = components;
        imageViewCreateInfo.subresourceRange    = subresourceRange;
        imageViewCreateInfo.flags               = flags;
        return imageViewCreateInfo;
    }
    
    //----------------------------------------------------------------//
    static VkInstanceCreateInfo instanceCreateInfo (
        const VkApplicationInfo* applicationInfo,
        const char* const* enabledExtensionNames    = NULL,
        uint32_t enabledExtensionCount              = 0,
        const char* const* enabledLayerNames        = NULL,
        uint32_t enabledLayerCount                  = 0,
        VkInstanceCreateFlags flags                 = 0
    ) {
        DECL_VK_STRUCT ( VkInstanceCreateInfo, instanceCreateInfo );
        instanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext                    = NULL;
        instanceCreateInfo.pApplicationInfo         = applicationInfo;
        instanceCreateInfo.ppEnabledLayerNames      = enabledLayerNames;
        instanceCreateInfo.enabledLayerCount        = enabledLayerCount;
        instanceCreateInfo.ppEnabledExtensionNames  = enabledExtensionNames;
        instanceCreateInfo.enabledExtensionCount    = enabledExtensionCount;
        instanceCreateInfo.flags                    = flags;
        return instanceCreateInfo;
    }
    
    //----------------------------------------------------------------//
    static VkMemoryAllocateInfo memoryAllocateInfo ( VkDeviceSize allocationSize, uint32_t memoryTypeIndex ) {
        DECL_VK_STRUCT ( VkMemoryAllocateInfo, memAllocInfo );
        memAllocInfo.sType                  = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAllocInfo.pNext                  = NULL;
        memAllocInfo.allocationSize         = allocationSize;
        memAllocInfo.memoryTypeIndex        = memoryTypeIndex;
        return memAllocInfo;
    }

    //----------------------------------------------------------------//
    static VkMemoryBarrier memoryBarrier () {
        DECL_VK_STRUCT ( VkMemoryBarrier, memoryBarrier );
        memoryBarrier.sType                 = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        memoryBarrier.pNext                 = NULL;
        return memoryBarrier;
    }

    //----------------------------------------------------------------//
    static VkMappedMemoryRange mappedMemoryRange () {
        DECL_VK_STRUCT ( VkMappedMemoryRange, mappedMemoryRange );
        mappedMemoryRange.sType             = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedMemoryRange.pNext             = NULL;
        return mappedMemoryRange;
    }

	//----------------------------------------------------------------//
	static VkOffset3D offset3D ( int32_t x, int32_t y, int32_t z ) {
		VkOffset3D offset3D;
		offset3D.x = x;
		offset3D.y = y;
		offset3D.z = z;
		return offset3D;
	}

    //----------------------------------------------------------------//
    static VkPipelineCacheCreateInfo pipelineCacheCreateInfo ( const void* initialData = NULL, size_t initialDataSize = 0, VkPipelineCacheCreateFlags flags = 0 ) {
        DECL_VK_STRUCT ( VkPipelineCacheCreateInfo, pipelineCacheCreateInfo );
        pipelineCacheCreateInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        pipelineCacheCreateInfo.pNext               = NULL;
        pipelineCacheCreateInfo.pInitialData        = initialData;
        pipelineCacheCreateInfo.initialDataSize     = initialDataSize;
        pipelineCacheCreateInfo.flags               = flags;
        return pipelineCacheCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState (
        VkBool32 blendEnable                    = VK_FALSE,
        VkColorComponentFlags colorWriteMask    = 0xf,
        VkBlendOp colorBlendOp                  = VK_BLEND_OP_ADD,
        VkBlendFactor srcColorBlendFactor       = VK_BLEND_FACTOR_ZERO,
        VkBlendFactor dstColorBlendFactor       = VK_BLEND_FACTOR_ZERO,
        VkBlendOp alphaBlendOp                  = VK_BLEND_OP_ADD,
        VkBlendFactor srcAlphaBlendFactor       = VK_BLEND_FACTOR_ZERO,
        VkBlendFactor dstAlphaBlendFactor       = VK_BLEND_FACTOR_ZERO
    ) {
        DECL_VK_STRUCT ( VkPipelineColorBlendAttachmentState, pipelineColorBlendAttachmentState );
        pipelineColorBlendAttachmentState.blendEnable           = blendEnable;
        pipelineColorBlendAttachmentState.colorWriteMask        = colorWriteMask;
        pipelineColorBlendAttachmentState.colorBlendOp          = colorBlendOp;
        pipelineColorBlendAttachmentState.srcColorBlendFactor   = srcColorBlendFactor;
        pipelineColorBlendAttachmentState.dstColorBlendFactor   = dstColorBlendFactor;
        pipelineColorBlendAttachmentState.alphaBlendOp          = alphaBlendOp;
        pipelineColorBlendAttachmentState.srcAlphaBlendFactor   = srcAlphaBlendFactor;
        pipelineColorBlendAttachmentState.dstAlphaBlendFactor   = dstAlphaBlendFactor;
        return pipelineColorBlendAttachmentState;
    }

    //----------------------------------------------------------------//
    static VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo (
        const VkPipelineColorBlendAttachmentState* attachments  = NULL,
        uint32_t attachmentCount                                = 0,
        VkBool32 logicOpEnable                                  = VK_FALSE,
        VkLogicOp logicOp                                       = VK_LOGIC_OP_NO_OP,
        float r                                                 = 0.0,
        float g                                                 = 0.0,
        float b                                                 = 0.0,
        float a                                                 = 0.0,
        VkPipelineColorBlendStateCreateFlags flags              = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineColorBlendStateCreateInfo, pipelineColorBlendStateCreateInfo );
        pipelineColorBlendStateCreateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipelineColorBlendStateCreateInfo.pNext                 = NULL;
        pipelineColorBlendStateCreateInfo.pAttachments          = attachments;
        pipelineColorBlendStateCreateInfo.attachmentCount       = attachmentCount;
        pipelineColorBlendStateCreateInfo.logicOpEnable         = logicOpEnable;
        pipelineColorBlendStateCreateInfo.logicOp               = logicOp;
        pipelineColorBlendStateCreateInfo.blendConstants [ 0 ]  = r;
        pipelineColorBlendStateCreateInfo.blendConstants [ 1 ]  = g;
        pipelineColorBlendStateCreateInfo.blendConstants [ 2 ]  = b;
        pipelineColorBlendStateCreateInfo.blendConstants [ 3 ]  = a;
        pipelineColorBlendStateCreateInfo.flags                 = flags;
        
        return pipelineColorBlendStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo (
        VkBool32 depthTestEnable                        = VK_TRUE,
        VkCompareOp depthCompareOp                      = VK_COMPARE_OP_LESS_OR_EQUAL,
        VkBool32 depthWriteEnable                       = VK_TRUE,
        VkBool32 stencilTestEnable                      = VK_FALSE,
        VkBool32 depthBoundsTestEnable                  = VK_FALSE,
        float minDepthBounds                            = 0.0,
        float maxDepthBounds                            = 0.0,
        VkPipelineDepthStencilStateCreateFlags flags    = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineDepthStencilStateCreateInfo, pipelineDepthStencilStateCreateInfo );
		
        pipelineDepthStencilStateCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        pipelineDepthStencilStateCreateInfo.pNext                   = NULL;
        
        pipelineDepthStencilStateCreateInfo.depthTestEnable         = depthTestEnable;
        pipelineDepthStencilStateCreateInfo.depthCompareOp          = depthCompareOp;
        pipelineDepthStencilStateCreateInfo.depthWriteEnable        = depthWriteEnable;
        pipelineDepthStencilStateCreateInfo.stencilTestEnable       = stencilTestEnable;
        pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable   = depthBoundsTestEnable;
        pipelineDepthStencilStateCreateInfo.minDepthBounds          = minDepthBounds;
        pipelineDepthStencilStateCreateInfo.maxDepthBounds          = maxDepthBounds;
        pipelineDepthStencilStateCreateInfo.flags                   = flags;
        
        pipelineDepthStencilStateCreateInfo.front.failOp            = VK_STENCIL_OP_KEEP;
        pipelineDepthStencilStateCreateInfo.front.passOp            = VK_STENCIL_OP_KEEP;
        pipelineDepthStencilStateCreateInfo.front.compareOp         = VK_COMPARE_OP_ALWAYS;

        pipelineDepthStencilStateCreateInfo.back.failOp             = VK_STENCIL_OP_KEEP;
        pipelineDepthStencilStateCreateInfo.back.passOp             = VK_STENCIL_OP_KEEP;
        pipelineDepthStencilStateCreateInfo.back.compareOp           = VK_COMPARE_OP_ALWAYS;

        return pipelineDepthStencilStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo (
        const VkDynamicState* dynamicStates,
        uint32_t dynamicStateCount,
        VkPipelineDynamicStateCreateFlags flags = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineDynamicStateCreateInfo, pipelineDynamicStateCreateInfo );
        pipelineDynamicStateCreateInfo.sType                = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipelineDynamicStateCreateInfo.pNext                = NULL;
        pipelineDynamicStateCreateInfo.pDynamicStates       = dynamicStates;
        pipelineDynamicStateCreateInfo.dynamicStateCount    = dynamicStateCount;
        pipelineDynamicStateCreateInfo.flags                = flags;
        return pipelineDynamicStateCreateInfo;
    }

//    //----------------------------------------------------------------//
//    static VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo ( uint32_t setLayoutCount = 1 ) {
//        DECL_VK_STRUCT ( VkPipelineLayoutCreateInfo, pipelineLayoutCreateInfo );
//        pipelineLayoutCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//        pipelineLayoutCreateInfo.pNext                      = NULL;
//        pipelineLayoutCreateInfo.setLayoutCount             = setLayoutCount;
//        return pipelineLayoutCreateInfo;
//    }

    //----------------------------------------------------------------//
    static VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo ( const VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount = 1 ) {
        DECL_VK_STRUCT ( VkPipelineLayoutCreateInfo, pipelineLayoutCreateInfo );
        pipelineLayoutCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pNext                  = NULL;
        pipelineLayoutCreateInfo.setLayoutCount         = setLayoutCount;
        pipelineLayoutCreateInfo.pSetLayouts            = pSetLayouts;
        return pipelineLayoutCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo (
        VkSampleCountFlagBits rasterizationSamples      = VK_SAMPLE_COUNT_1_BIT,
        const VkSampleMask* sampleMask                  = NULL,
        VkBool32 sampleShadingEnable                    = VK_FALSE,
        float minSampleShading                          = VK_FALSE,
        VkBool32 alphaToCoverageEnable                  = VK_FALSE,
        VkBool32 alphaToOneEnable                       = VK_FALSE,
        VkPipelineMultisampleStateCreateFlags flags     = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineMultisampleStateCreateInfo, pipelineMultisampleStateCreateInfo );
        pipelineMultisampleStateCreateInfo.sType                        = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipelineMultisampleStateCreateInfo.pNext                        = NULL;
        pipelineMultisampleStateCreateInfo.rasterizationSamples         = rasterizationSamples;
        pipelineMultisampleStateCreateInfo.pSampleMask                  = sampleMask;
        pipelineMultisampleStateCreateInfo.sampleShadingEnable          = sampleShadingEnable;
        pipelineMultisampleStateCreateInfo.minSampleShading             = minSampleShading;
        pipelineMultisampleStateCreateInfo.alphaToCoverageEnable        = alphaToCoverageEnable;
        pipelineMultisampleStateCreateInfo.alphaToOneEnable             = alphaToOneEnable;
        pipelineMultisampleStateCreateInfo.flags                        = flags;
        return pipelineMultisampleStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo ( VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable, VkPipelineInputAssemblyStateCreateFlags flags ) {
        DECL_VK_STRUCT ( VkPipelineInputAssemblyStateCreateInfo, pipelineInputAssemblyStateCreateInfo );
        pipelineInputAssemblyStateCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipelineInputAssemblyStateCreateInfo.pNext                      = NULL;
        pipelineInputAssemblyStateCreateInfo.topology                   = topology;
        pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable     = primitiveRestartEnable;
        pipelineInputAssemblyStateCreateInfo.flags                      = flags;
        return pipelineInputAssemblyStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo (
        VkPolygonMode polygonMode                       = VK_POLYGON_MODE_FILL,
        VkCullModeFlags cullMode                        = VK_CULL_MODE_NONE,
        VkFrontFace frontFace                           = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        float lineWidth                                 = 1.0,
        VkBool32 rasterizerDiscardEnable                = VK_FALSE,
        VkBool32 depthClampEnable                       = VK_FALSE,
        VkBool32 depthBiasEnable                        = VK_FALSE,
        float depthBiasConstantFactor                   = 0.0,
        float depthBiasClamp                            = 0.0,
        float depthBiasSlopeFactor                      = 0.0,
        VkPipelineRasterizationStateCreateFlags flags   = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineRasterizationStateCreateInfo, pipelineRasterizationStateCreateInfo );
        pipelineRasterizationStateCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipelineRasterizationStateCreateInfo.pNext                      = NULL;
        pipelineRasterizationStateCreateInfo.polygonMode                = polygonMode;
        pipelineRasterizationStateCreateInfo.cullMode                   = cullMode;
        pipelineRasterizationStateCreateInfo.frontFace                  = frontFace;
        pipelineRasterizationStateCreateInfo.lineWidth                  = lineWidth;
        pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable    = rasterizerDiscardEnable;
        pipelineRasterizationStateCreateInfo.depthClampEnable           = depthClampEnable;
        pipelineRasterizationStateCreateInfo.depthBiasEnable            = depthBiasEnable;
        pipelineRasterizationStateCreateInfo.depthBiasConstantFactor    = depthBiasConstantFactor;
        pipelineRasterizationStateCreateInfo.depthBiasClamp             = depthBiasClamp;
        pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor       = depthBiasSlopeFactor;
        pipelineRasterizationStateCreateInfo.flags                      = flags;
        return pipelineRasterizationStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo (
        VkShaderStageFlagBits stage,
        VkShaderModule module,
        const char* name,
        const VkSpecializationInfo* specializationInfo      = NULL,
        VkPipelineShaderStageCreateFlags flags              = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineShaderStageCreateInfo, pipelineShaderStageCreateInfo );
        pipelineShaderStageCreateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStageCreateInfo.pNext                 = NULL;
        pipelineShaderStageCreateInfo.stage                 = stage;
        pipelineShaderStageCreateInfo.module                = module;
        pipelineShaderStageCreateInfo.pName                 = name;
        pipelineShaderStageCreateInfo.pSpecializationInfo   = specializationInfo;
        pipelineShaderStageCreateInfo.flags                 = flags;
        return pipelineShaderStageCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo ( uint32_t patchControlPoints ) {
        DECL_VK_STRUCT ( VkPipelineTessellationStateCreateInfo, pipelineTessellationStateCreateInfo );
        pipelineTessellationStateCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        pipelineTessellationStateCreateInfo.pNext                   = NULL;
        pipelineTessellationStateCreateInfo.patchControlPoints      = patchControlPoints;
        return pipelineTessellationStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo (
        const VkVertexInputBindingDescription* vertexBindingDescriptions        = NULL,
        uint32_t vertexBindingDescriptionCount                                  = 0,
        const VkVertexInputAttributeDescription* vertexAttributeDescriptions    = NULL,
        uint32_t vertexAttributeDescriptionCount                                = 0,
        VkPipelineVertexInputStateCreateFlags flags                             = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineVertexInputStateCreateInfo, pipelineVertexInputStateCreateInfo );
        pipelineVertexInputStateCreateInfo.sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pipelineVertexInputStateCreateInfo.pNext                            = NULL;
        pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions       = vertexBindingDescriptions;
        pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount    = vertexBindingDescriptionCount;
        pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions     = vertexAttributeDescriptions;
        pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount  = vertexAttributeDescriptionCount;
        pipelineVertexInputStateCreateInfo.flags                            = flags;
        return pipelineVertexInputStateCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo (
        const VkViewport* viewports                 = NULL,
        uint32_t viewportCount                      = 0,
        const VkRect2D* scissors                    = NULL,
        uint32_t scissorCount                       = 0,
        VkPipelineViewportStateCreateFlags flags    = 0
    ) {
        DECL_VK_STRUCT ( VkPipelineViewportStateCreateInfo, pipelineViewportStateCreateInfo );
        pipelineViewportStateCreateInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipelineViewportStateCreateInfo.pNext               = NULL;
        pipelineViewportStateCreateInfo.pViewports          = viewports;
        pipelineViewportStateCreateInfo.viewportCount       = viewportCount;
        pipelineViewportStateCreateInfo.pScissors           = scissors;
        pipelineViewportStateCreateInfo.scissorCount        = scissorCount;
        pipelineViewportStateCreateInfo.flags               = flags;
        return pipelineViewportStateCreateInfo;
    }

	//----------------------------------------------------------------//
    static VkPresentInfoKHR presentInfoKHR (
		const VkSemaphore* pWaitSemaphores	= NULL,
        uint32_t waitSemaphoreCount			= 0,
		const VkSwapchainKHR* pSwapchains	= NULL,
		uint32_t swapchainCount				= 0,
		const uint32_t* pImageIndices		= NULL,
		VkResult* pResults					= NULL
    ) {
        DECL_VK_STRUCT ( VkPresentInfoKHR, presentInfoKHR );
        presentInfoKHR.sType               	= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKHR.pNext               	= NULL;
        presentInfoKHR.pWaitSemaphores		= pWaitSemaphores;
        presentInfoKHR.waitSemaphoreCount	= waitSemaphoreCount;
        presentInfoKHR.pSwapchains          = pSwapchains;
        presentInfoKHR.swapchainCount		= swapchainCount;
        presentInfoKHR.pImageIndices		= pImageIndices;
        presentInfoKHR.pResults          	= pResults;
        return presentInfoKHR;
    }

    //----------------------------------------------------------------//
    static VkPushConstantRange pushConstantRange ( VkShaderStageFlags stageFlags, uint32_t size, uint32_t offset ) {
        DECL_VK_STRUCT ( VkPushConstantRange, pushConstantRange );
        pushConstantRange.stageFlags    = stageFlags;
        pushConstantRange.offset        = offset;
        pushConstantRange.size          = size;
        return pushConstantRange;
    }

    //----------------------------------------------------------------//
    static VkRect2D rect2D ( int32_t width, int32_t height, int32_t offsetX = 0, int32_t offsetY = 0 ) {
        DECL_VK_STRUCT ( VkRect2D, rect2D );
        rect2D.extent.width     = width;
        rect2D.extent.height    = height;
        rect2D.offset.x         = offsetX;
        rect2D.offset.y         = offsetY;
        return rect2D;
    }

    //----------------------------------------------------------------//
    static VkRenderPassBeginInfo renderPassBeginInfo (
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkRect2D renderArea,
        const VkClearValue* clearValues,
        uint32_t clearValueCount
    ) {
        DECL_VK_STRUCT ( VkRenderPassBeginInfo, renderPassBeginInfo );
        renderPassBeginInfo.sType                   = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext                   = NULL;
        renderPassBeginInfo.renderPass              = renderPass;
        renderPassBeginInfo.framebuffer             = framebuffer;
        renderPassBeginInfo.renderArea              = renderArea;
        renderPassBeginInfo.pClearValues            = clearValues;
        renderPassBeginInfo.clearValueCount         = clearValueCount;
        return renderPassBeginInfo;
    }

    //----------------------------------------------------------------//
    static VkRenderPassCreateInfo renderPassCreateInfo (
        const VkAttachmentDescription* attachments,
        uint32_t attachmentCount,
        const VkSubpassDescription* subpasses,
        uint32_t subpassCount,
        const VkSubpassDependency* dependencies,
        uint32_t dependencyCount,
        VkRenderPassCreateFlags flags       = 0
    ) {
        DECL_VK_STRUCT ( VkRenderPassCreateInfo, renderPassCreateInfo );
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.pNext                  = NULL;
        renderPassCreateInfo.pAttachments           = attachments;
        renderPassCreateInfo.attachmentCount        = attachmentCount;
        renderPassCreateInfo.pSubpasses             = subpasses;
        renderPassCreateInfo.subpassCount           = subpassCount;
        renderPassCreateInfo.pDependencies          = dependencies;
        renderPassCreateInfo.dependencyCount        = dependencyCount;
        renderPassCreateInfo.flags                  = flags;
        
        return renderPassCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkSamplerCreateInfo samplerCreateInfo () {
        DECL_VK_STRUCT ( VkSamplerCreateInfo, samplerCreateInfo );
        samplerCreateInfo.sType             = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.pNext             = NULL;
        samplerCreateInfo.maxAnisotropy     = 1.0f;
        return samplerCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkShaderModuleCreateInfo shaderModuleCreateInfo ( const uint32_t* code, size_t codeSize, VkShaderModuleCreateFlags flags = 0 ) {
        DECL_VK_STRUCT ( VkShaderModuleCreateInfo, shaderModuleCreateInfo );
        shaderModuleCreateInfo.sType        = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.pNext        = NULL;
        shaderModuleCreateInfo.pCode        = code;
        shaderModuleCreateInfo.codeSize     = codeSize;
        shaderModuleCreateInfo.flags        = flags;
        return shaderModuleCreateInfo;
    }

    //----------------------------------------------------------------//
    static VkSemaphoreCreateInfo semaphoreCreateInfo () {
        DECL_VK_STRUCT ( VkSemaphoreCreateInfo, semaphoreCreateInfo );
        semaphoreCreateInfo.sType           = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext           = NULL;
        return semaphoreCreateInfo;
    }

    //----------------------------------------------------------------//
    /** @brief Initialize a specialization constant info structure to pass to a shader stage */
    static VkSpecializationInfo specializationInfo ( uint32_t mapEntryCount, const VkSpecializationMapEntry* mapEntries, size_t dataSize, const void* data ) {
        DECL_VK_STRUCT ( VkSpecializationInfo, specializationInfo );
        specializationInfo.mapEntryCount    = mapEntryCount;
        specializationInfo.pMapEntries      = mapEntries;
        specializationInfo.dataSize         = dataSize;
        specializationInfo.pData            = data;
        return specializationInfo;
    }

    //----------------------------------------------------------------//
    /** @brief Initialize a map entry for a shader specialization constant */
    static VkSpecializationMapEntry specializationMapEntry ( uint32_t constantID, uint32_t offset, size_t size ) {
        DECL_VK_STRUCT ( VkSpecializationMapEntry, specializationMapEntry );
        specializationMapEntry.constantID   = constantID;
        specializationMapEntry.offset       = offset;
        specializationMapEntry.size         = size;
        return specializationMapEntry;
    }

    //----------------------------------------------------------------//
    static VkSubmitInfo submitInfo (
        const VkCommandBuffer* pCommandBuffers,
        uint32_t commandBufferCount,
        const VkSemaphore* pSignalSemaphores            = NULL,
        uint32_t signalSemaphoreCount                   = 0,
        const VkSemaphore* pWaitSemaphores              = NULL,
        uint32_t waitSemaphoreCount                     = 0,
        const VkPipelineStageFlags* pWaitDstStageMask   = NULL
    ) {
        DECL_VK_STRUCT ( VkSubmitInfo, submitInfo );
        submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext                    = NULL;
        submitInfo.waitSemaphoreCount       = waitSemaphoreCount;
        submitInfo.pWaitSemaphores          = pWaitSemaphores;
        submitInfo.pWaitDstStageMask        = pWaitDstStageMask;
        submitInfo.commandBufferCount       = commandBufferCount;
        submitInfo.pCommandBuffers          = pCommandBuffers;
        submitInfo.signalSemaphoreCount     = signalSemaphoreCount;
        submitInfo.pSignalSemaphores        = pSignalSemaphores;
        return submitInfo;
    }

    //----------------------------------------------------------------//
    static VkSubpassDependency subpassDependency (
        uint32_t srcSubpass,
        uint32_t dstSubpass,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkDependencyFlags dependencyFlags
    ) {
        DECL_VK_STRUCT ( VkSubpassDependency, subpassDependency );
        subpassDependency.srcSubpass        = srcSubpass;
        subpassDependency.dstSubpass        = dstSubpass;
        subpassDependency.srcStageMask      = srcStageMask;
        subpassDependency.dstStageMask      = dstStageMask;
        subpassDependency.srcAccessMask     = srcAccessMask;
        subpassDependency.dstAccessMask     = dstAccessMask;
        subpassDependency.dependencyFlags   = dependencyFlags;
        return subpassDependency;
    };

    //----------------------------------------------------------------//
    static VkSubpassDescription subpassDescription (
        VkPipelineBindPoint pipelineBindPoint,
        const VkAttachmentReference* colorAttachments,
        uint32_t colorAttachmentCount,
        const VkAttachmentReference* depthStencilAttachment,
        const VkAttachmentReference* inputAttachments           = NULL,
        uint32_t inputAttachmentCount                           = 0,
        const uint32_t* preserveAttachments                     = NULL,
        uint32_t preserveAttachmentCount                        = 0,
        const VkAttachmentReference* resolveAttachments         = NULL,
        VkSubpassDescriptionFlags flags                         = 0
    ) {
        DECL_VK_STRUCT ( VkSubpassDescription, subpassDescription );
        subpassDescription.pipelineBindPoint        = pipelineBindPoint;
        subpassDescription.pColorAttachments        = colorAttachments;
        subpassDescription.colorAttachmentCount     = colorAttachmentCount;
        subpassDescription.pDepthStencilAttachment  = depthStencilAttachment;
        subpassDescription.pInputAttachments        = inputAttachments;
        subpassDescription.inputAttachmentCount     = inputAttachmentCount;
        subpassDescription.pPreserveAttachments     = preserveAttachments;
        subpassDescription.preserveAttachmentCount  = preserveAttachmentCount;
        subpassDescription.pResolveAttachments      = resolveAttachments;
        subpassDescription.flags                    = flags;
        return subpassDescription;
    }

	//----------------------------------------------------------------//
    static VkSwapchainCreateInfoKHR swapchainCreateInfoKHR (
		VkSurfaceKHR                     surface,
		uint32_t                         minImageCount,
		VkFormat                         imageFormat,
		VkColorSpaceKHR                  imageColorSpace,
		VkExtent2D                       imageExtent,
		uint32_t                         imageArrayLayers,
		VkImageUsageFlags                imageUsage,
		VkSharingMode                    imageSharingMode,
		const uint32_t*                  pQueueFamilyIndices,
		uint32_t                         queueFamilyIndexCount,
		VkSurfaceTransformFlagBitsKHR    preTransform,
		VkCompositeAlphaFlagBitsKHR      compositeAlpha,
		VkPresentModeKHR                 presentMode,
		VkBool32                         clipped,
		VkSwapchainKHR                   oldSwapchain,
		VkSwapchainCreateFlagsKHR        flags						= 0
    ) {
        DECL_VK_STRUCT ( VkSwapchainCreateInfoKHR, swapchainCreateInfoKHR );
		swapchainCreateInfoKHR.surface					= surface;
		swapchainCreateInfoKHR.minImageCount			= minImageCount;
		swapchainCreateInfoKHR.imageFormat				= imageFormat;
		swapchainCreateInfoKHR.imageColorSpace			= imageColorSpace;
		swapchainCreateInfoKHR.imageExtent				= imageExtent;
		swapchainCreateInfoKHR.imageArrayLayers			= imageArrayLayers;
		swapchainCreateInfoKHR.imageUsage				= imageUsage;
		swapchainCreateInfoKHR.imageSharingMode			= imageSharingMode;
		swapchainCreateInfoKHR.queueFamilyIndexCount	= queueFamilyIndexCount;
		swapchainCreateInfoKHR.pQueueFamilyIndices		= pQueueFamilyIndices;
		swapchainCreateInfoKHR.preTransform				= preTransform;
		swapchainCreateInfoKHR.compositeAlpha			= compositeAlpha;
		swapchainCreateInfoKHR.presentMode				= presentMode;
		swapchainCreateInfoKHR.clipped					= clipped;
		swapchainCreateInfoKHR.oldSwapchain				= oldSwapchain;
		swapchainCreateInfoKHR.flags					= flags;
        return swapchainCreateInfoKHR;
    }

    //----------------------------------------------------------------//
    static VkWriteDescriptorSet writeDescriptorSet ( VkDescriptorSet dstSet, uint32_t binding, VkDescriptorType type, VkDescriptorBufferInfo* bufferInfo, uint32_t descriptorCount = 1 ) {
        DECL_VK_STRUCT ( VkWriteDescriptorSet, writeDescriptorSet );
        writeDescriptorSet.sType                    = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext                    = NULL;
        writeDescriptorSet.dstSet                   = dstSet;
        writeDescriptorSet.descriptorType           = type;
        writeDescriptorSet.dstBinding               = binding;
        writeDescriptorSet.pImageInfo 				= NULL;
        writeDescriptorSet.pBufferInfo              = bufferInfo;
        writeDescriptorSet.descriptorCount          = descriptorCount;
        return writeDescriptorSet;
    }

    //----------------------------------------------------------------//
    static VkWriteDescriptorSet writeDescriptorSet ( VkDescriptorSet dstSet, uint32_t binding, VkDescriptorType type, VkDescriptorImageInfo *imageInfo, uint32_t descriptorCount = 1 ) {
        DECL_VK_STRUCT ( VkWriteDescriptorSet, writeDescriptorSet );
        writeDescriptorSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext                = NULL;
        writeDescriptorSet.dstSet               = dstSet;
        writeDescriptorSet.descriptorType       = type;
        writeDescriptorSet.dstBinding           = binding;
        writeDescriptorSet.pImageInfo           = imageInfo;
        writeDescriptorSet.pBufferInfo			= NULL;
        writeDescriptorSet.descriptorCount      = descriptorCount;
        return writeDescriptorSet;
    }

    //----------------------------------------------------------------//
    static VkVertexInputAttributeDescription vertexInputAttributeDescription (
        uint32_t binding,
        uint32_t location,
        VkFormat format,
        uint32_t offset
    ) {
        DECL_VK_STRUCT ( VkVertexInputAttributeDescription, vInputAttribDescription );
        vInputAttribDescription.location    = location;
        vInputAttribDescription.binding     = binding;
        vInputAttribDescription.format      = format;
        vInputAttribDescription.offset      = offset;
        return vInputAttribDescription;
    }

    //----------------------------------------------------------------//
    static VkVertexInputBindingDescription vertexInputBindingDescription (
        uint32_t binding,
        uint32_t stride,
        VkVertexInputRate inputRate
    ) {
        DECL_VK_STRUCT ( VkVertexInputBindingDescription, vInputBindDescription );
        vInputBindDescription.binding       = binding;
        vInputBindDescription.stride        = stride;
        vInputBindDescription.inputRate     = inputRate;
        return vInputBindDescription;
    }

    //----------------------------------------------------------------//
    static VkViewport viewport ( float width, float height, float minDepth, float maxDepth ) {
        DECL_VK_STRUCT ( VkViewport, viewport );
        viewport.width          = width;
        viewport.height         = height;
        viewport.minDepth       = minDepth;
        viewport.maxDepth       = maxDepth;
        return viewport;
    }
};

#endif
