// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTNAMEVK_H
#define MOAIDESCRIPTORSETLAYOUTNAMEVK_H

//================================================================//
// MOAIDescriptorSetLayoutNameVK
//================================================================//
class MOAIDescriptorSetLayoutNameVK :
	public ZLLeanArray < VkDescriptorSetLayoutBinding > {
public:
	
	//----------------------------------------------------------------//
				MOAIDescriptorSetLayoutNameVK		();
				~MOAIDescriptorSetLayoutNameVK		();
	void		SetBinding							( u32 binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, u32 descriptorCount = 1 );
};

#endif
