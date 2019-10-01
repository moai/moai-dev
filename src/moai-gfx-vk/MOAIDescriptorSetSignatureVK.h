// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSIGNATUREVK_H
#define MOAIDESCRIPTORSETSIGNATUREVK_H

class MOAIDescriptorSetLayoutVK;

//================================================================//
// MOAIDescriptorSetSignatureVK
//================================================================//
class MOAIDescriptorSetSignatureVK :
	public ZLLeanArray < VkWriteDescriptorSet > {
public:
	
	//----------------------------------------------------------------//
	void		Initialize							( const MOAIDescriptorSetLayoutVK& layout );
				MOAIDescriptorSetSignatureVK		();
				~MOAIDescriptorSetSignatureVK		();
};

#endif
