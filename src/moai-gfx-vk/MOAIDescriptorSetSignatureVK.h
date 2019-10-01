// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSIGNATUREVK_H
#define MOAIDESCRIPTORSETSIGNATUREVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>

class MOAIDescriptorSetLayoutVK;

//================================================================//
// MOAIDescriptorSetSignatureVK
//================================================================//
class MOAIDescriptorSetSignatureVK :
	public MOAIAbstractSnapshotSignatureVK < MOAIDescriptorSetSignatureVK >,
	public ZLLeanArray < VkWriteDescriptorSet > {
protected:

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotSignatureVK_CloneFrom		( const MOAIDescriptorSetSignatureVK& other );
	bool		MOAIAbstractSnapshotSignatureVK_IsSame			( const MOAIDescriptorSetSignatureVK& other ) const;

public:
	
	//----------------------------------------------------------------//
	void		Initialize							( const MOAIDescriptorSetLayoutVK& layout );
				MOAIDescriptorSetSignatureVK		();
				~MOAIDescriptorSetSignatureVK		();
};

#endif
