// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTIMPLVK_H
#define MOAIDESCRIPTORSETLAYOUTIMPLVK_H

#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>

class MOAIDescriptorSetLayoutImplCacheVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetVK;
class MOAILogicalDeviceVK;

typedef ZLArrayKey < VkDescriptorSetLayoutBinding > MOAIDescriptorSetLayoutKeyVK;

//================================================================//
// MOAIDescriptorPoolVK
//================================================================//
class MOAIDescriptorPoolVK {
private:

	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;

	STLSet < MOAIDescriptorSetSnapshotVK* >			mAllSnapshots;
	STLSet < MOAIDescriptorSetSnapshotVK* >			mExpiredSnapshots;
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

	friend class MOAIDescriptorSetLayoutImplCacheVK;
	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetVK;
	friend class MOAIDescriptorSetSnapshotVK;

	MOAIDescriptorSetLayoutImplCacheVK*				mCache;

	MOAIDescriptorSetLayoutKeyVK					mKey;
	ZLLeanArray < VkDescriptorSetLayoutBinding >	mLayoutBindings;

	ZLLeanArray < ZLIndex >							mSignatureOffsets;
	ZLSize											mSignatureSize;

	VkDescriptorPoolCreateInfo						mPoolCreateInfo;
	ZLLeanArray < VkDescriptorPoolSize >			mTypeCounts;

	VkDescriptorSetLayout							mLayout;

	STLSet < MOAIDescriptorPoolVK* >				mAllPools;
	STLSet < MOAIDescriptorPoolVK* >				mOpenPools;

	STLMap < MOAIDescriptorSetKeyVK, MOAIDescriptorSetSnapshotVK* > mActiveSnapshots;
	
	//----------------------------------------------------------------//
	void							DeletePool						( MOAIDescriptorPoolVK* pool );
	MOAIDescriptorSetSnapshotVK*	ProcureDescriptorSet			( const MOAIDescriptorSetVK& descriptorSetState );
	void							RetireDescriptorSet				( MOAIDescriptorSetSnapshotVK& snapshot );

	//----------------------------------------------------------------//
	void						_Finalize							();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetLayoutVK )

	//----------------------------------------------------------------//
	operator MOAIDescriptorSetLayoutKeyVK& () {
		return this->mKey;
	}

	//----------------------------------------------------------------//
								MOAIDescriptorSetLayoutImplVK		( MOAILogicalDeviceVK& logicalDevice, const MOAIDescriptorSetLayoutKeyVK& key, MOAIDescriptorSetLayoutImplCacheVK* cache = NULL );
								~MOAIDescriptorSetLayoutImplVK		();
};

#endif
