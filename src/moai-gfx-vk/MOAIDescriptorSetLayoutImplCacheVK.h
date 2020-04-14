// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTIMPLCACHEVK_H
#define MOAIDESCRIPTORSETLAYOUTIMPLCACHEVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplVK.h>

//================================================================//
// MOAIDescriptorSetLayoutImplCacheVK
//================================================================//
class MOAIDescriptorSetLayoutImplCacheVK {
private:

	friend class MOAIDescriptorSetLayoutImplVK;

	typedef STLMap < MOAIDescriptorSetLayoutKeyVK, MOAIDescriptorSetLayoutImplVK* >::iterator CacheIt;

	STLMap < MOAIDescriptorSetLayoutKeyVK, MOAIDescriptorSetLayoutImplVK* > mWeakCache;

	//----------------------------------------------------------------//
	void		InsertDescriptorSetLayoutImpl			( MOAIDescriptorSetLayoutImplVK& descriptorSetLayoutImpl );
	void		RemoveDescriptorSetLayoutImpl			( MOAIDescriptorSetLayoutImplVK& descriptorSetLayoutImpl );

public:

	//----------------------------------------------------------------//
													MOAIDescriptorSetLayoutImplCacheVK		();
													~MOAIDescriptorSetLayoutImplCacheVK		();
	ZLStrongPtr < MOAIDescriptorSetLayoutImplVK >	ProcureDescriptorSetLayoutImpl			( MOAIDescriptorSetLayoutKeyVK key );
};

#endif
