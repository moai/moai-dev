// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURESNAPSHOT2DVK_H
#define	MOAITEXTURESNAPSHOT2DVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

class MOAILogicalDeviceVK;
class ZLTextureFormat;

//================================================================//
// MOAITextureSnapshot2DVK
//================================================================//
class MOAITextureSnapshot2DVK :
	public virtual MOAIAbstractSnapshotVK,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
protected:

	VkImage					mTextureImage;
	VkDeviceMemory			mTextureImageMemory;
	VkImageView				mTextureImageView;
	VkSampler				mTextureSampler;
	VkDescriptorImageInfo	mTextureDescriptor;

	ZLSize					mTextureSize;

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		() {}

public:
	
	IMPLEMENT_FINALIZABLE ( MOAITextureSnapshot2DVK )
	
	GET ( ZLSize, Size, mTextureSize )
	
	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mTextureSize > 0 );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorImageInfo* () {
		return &this->mTextureDescriptor;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorImageInfo& () {
		return this->mTextureDescriptor;
	}
	
	//----------------------------------------------------------------//
	bool			Initialize							( ZLImage& srcImage );
					MOAITextureSnapshot2DVK				();
					~MOAITextureSnapshot2DVK			();
};

#endif
