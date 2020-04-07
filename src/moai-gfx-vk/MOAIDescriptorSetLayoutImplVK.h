// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTIMPLVK_H
#define MOAIDESCRIPTORSETLAYOUTIMPLVK_H

#include <moai-gfx-vk/MOAIDescriptorSetVK.h>

class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetStateVK;
class MOAILogicalDeviceVK;

typedef ZLArrayKey < VkDescriptorSetLayoutBinding > MOAIDescriptorSetLayoutKeyVK;

//================================================================//
// MOAIDescriptorPoolVK
//================================================================//
class MOAIDescriptorPoolVK {
private:

	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;

	STLSet < MOAIDescriptorSetVK* >			mAllSnapshots;
	STLSet < MOAIDescriptorSetVK* >			mExpiredSnapshots;
	VkDescriptorPool						mPool;
};

//================================================================//
// MOAIDescriptorSetLayoutImplVK
//================================================================//
class MOAIDescriptorSetLayoutImplVK :
	public ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	static const ZLSize POOL_SIZE = 16;

	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetStateVK;
	friend class MOAIDescriptorSetVK;

	MOAIDescriptorSetLayoutKeyVK					mKey;
	ZLLeanArray < VkDescriptorSetLayoutBinding >	mLayoutBindings;

	ZLLeanArray < ZLIndex >							mSignatureOffsets;
	ZLSize											mSignatureSize;

	VkDescriptorPoolCreateInfo						mPoolCreateInfo;
	ZLLeanArray < VkDescriptorPoolSize >			mTypeCounts;

	VkDescriptorSetLayout							mLayout;

	STLSet < MOAIDescriptorPoolVK* >				mAllPools;
	STLSet < MOAIDescriptorPoolVK* >				mOpenPools;

	STLMap < MOAIDescriptorSetKeyVK, MOAIDescriptorSetVK* > mActiveSnapshots;
	
	//----------------------------------------------------------------//
	void						DeletePool							( MOAIDescriptorPoolVK* pool );
	MOAIDescriptorSetVK*		ProcureDescriptorSet				( const MOAIDescriptorSetStateVK& descriptorSetState );
	void						RetireDescriptorSet					( MOAIDescriptorSetVK& snapshot );

	//----------------------------------------------------------------//
	void						_Finalize							();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetLayoutVK )

	//----------------------------------------------------------------//
	operator MOAIDescriptorSetLayoutKeyVK& () {
		return this->mKey;
	}

	//----------------------------------------------------------------//
	void						Initialize							( MOAILogicalDeviceVK& logicalDevice, const MOAIDescriptorSetLayoutKeyVK& key );
								MOAIDescriptorSetLayoutImplVK		();
								~MOAIDescriptorSetLayoutImplVK		();
};

#endif
