// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>

//================================================================//
// MOAIDescriptorSetLayoutImplCacheVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplCacheVK::InsertDescriptorSetLayoutImpl ( MOAIDescriptorSetLayoutImplVK& descriptorSetLayoutImpl ) {

	this->mWeakCache [ descriptorSetLayoutImpl ] = &descriptorSetLayoutImpl;
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutImplCacheVK::MOAIDescriptorSetLayoutImplCacheVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutImplCacheVK::~MOAIDescriptorSetLayoutImplCacheVK () {

	CacheIt cacheIt = this->mWeakCache.begin ();
	for ( ; cacheIt != this->mWeakCache.end (); ++cacheIt ) {
		cacheIt->second->mCache = NULL;
	}
}

//----------------------------------------------------------------//
ZLStrongPtr < MOAIDescriptorSetLayoutImplVK > MOAIDescriptorSetLayoutImplCacheVK::ProcureDescriptorSetLayoutImpl ( MOAIDescriptorSetLayoutKeyVK key ) {

	if ( this->mWeakCache.contains ( key )) {
		return this->mWeakCache [ key ];
	}
	
	return new MOAIDescriptorSetLayoutImplVK (
		MOAIGfxMgrVK::Get ().GetLogicalDevice (),
		key.mSignature,
		this
	);
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplCacheVK::RemoveDescriptorSetLayoutImpl ( MOAIDescriptorSetLayoutImplVK& descriptorSetLayoutImpl ) {

	if ( this->mWeakCache.contains ( descriptorSetLayoutImpl.mKey )) {
		this->mWeakCache.erase ( descriptorSetLayoutImpl.mKey );
	}
}
