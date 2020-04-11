// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRVK_RESOURCECACHEVK_H
#define	MOAIGFXMGRVK_RESOURCECACHEVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>
#include <moai-gfx-vk/MOAIPipelineVK.h>

class MOAIGfxMgrVK;

template < typename KEY_TYPE, typename TYPE >
class MOAIResourceCache;

//================================================================//
// MOAIResourceCacheMember
//================================================================//
template < typename KEY_TYPE, typename TYPE >
class MOAIResourceCacheMember :
	virtual public ZLRefCountedObject,
	public TYPE {
public:

	typedef MOAIResourceCache < KEY_TYPE, TYPE >								Cache;
	typedef STLMap < KEY_TYPE, MOAIResourceCacheMember < KEY_TYPE, TYPE >* >	CacheMap;

	Cache*		mCache;

	//----------------------------------------------------------------//
	MOAIResourceCacheMember ( Cache& cache ) :
		mCache ( &cache ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIResourceCacheMember () {
	
		if ( !this->mCache ) return;
	
		const KEY_TYPE& key = *this;
		if ( this->mCache->contains ( key )) {
			this->mCache->erase ( key );
		}
	}
};

//================================================================//
// MOAIResourceCache
//================================================================//
template < typename KEY_TYPE, typename TYPE >
class MOAIResourceCache :
	public MOAIResourceCacheMember < KEY_TYPE, TYPE >::CacheMap {
public:

	//----------------------------------------------------------------//
	MOAIResourceCache () {
	}
	
	//----------------------------------------------------------------//
	~MOAIResourceCache () {
		
		typename MOAIResourceCacheMember < KEY_TYPE, TYPE >::CacheMap::iterator cacheIt = this->begin ();
		for ( ; cacheIt != this->end (); ++cacheIt ) {
			cacheIt->second->mCache = NULL;
		}
	}
};

//================================================================//
// MOAIGfxMgrVK_ResourceCacheVK
//================================================================//
class MOAIGfxMgrVK_ResourceCacheVK :
	public virtual MOAIGfxMgrVKComponents {
protected:

	typedef MOAIResourceCacheMember < MOAIDescriptorSetLayoutKeyVK, MOAIDescriptorSetLayoutImplVK > DescriptorSetLayoutImpl;

	DescriptorSetLayoutImpl::Cache mDescriptorLayoutImplCache;

	STLMap < MOAIPipelineKeyVK, ZLStrongPtr < MOAIPipelineVK > > mPipelinePool; // TODO: VkPipelineCache

	//----------------------------------------------------------------//
	
public:
	
	friend class MOAIGfxMgr;
	
	//----------------------------------------------------------------//
	MOAIPipelineVK* 								AffirmPipeline 							( const MOAIPipelineParamsVK& params );
													MOAIGfxMgrVK_ResourceCacheVK			();
													~MOAIGfxMgrVK_ResourceCacheVK			();
	ZLStrongPtr < MOAIDescriptorSetLayoutImplVK >	ProcureDescriptorSetLayoutImpl			( MOAIDescriptorSetLayoutKeyVK key );
};

#endif
