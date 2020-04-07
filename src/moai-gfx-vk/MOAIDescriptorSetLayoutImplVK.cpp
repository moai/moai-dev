// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetLayoutImplVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplVK::DeletePool ( MOAIDescriptorPoolVK* pool ) {

	if ( !pool ) return;
	assert ( this->mAllPools.contains ( pool ));
	
	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyDescriptorPool ( logicalDevice, pool->mPool, NULL );
	}
	
	// TODO: clean up snapshots
	STLSet < MOAIDescriptorSetVK* >::iterator snapshotIt = pool->mAllSnapshots.begin ();
	for ( ; snapshotIt != pool->mAllSnapshots.end (); ++snapshotIt ) {
		( *snapshotIt )->Release ();
	}
	
	if ( this->mOpenPools.contains ( pool )) {
		this->mOpenPools.erase ( pool );
	}
	this->mAllPools.erase ( pool );
	delete pool;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, const ZLLeanArray < VkDescriptorSetLayoutBinding >& layoutBindings ) {
	
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	
	this->mLayoutBindings.CloneFrom ( layoutBindings );
	ZLSize nBindings = this->mLayoutBindings.Size ();
	
	// set up the layout
	VkDescriptorSetLayoutCreateInfo descriptorLayout = MOAIGfxStructVK::descriptorSetLayoutCreateInfo ( this->mLayoutBindings.GetBuffer (), ( u32 )nBindings );
	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mLayout ));

	// do some analysis - build histogram (for pool), find offsets (for write array)
	this->mSignatureOffsets.Init ( nBindings );
	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > > histogram;

	this->mSignatureSize = 0;
	for ( ZLIndex i = 0; i < nBindings; ++i ) {
		const VkDescriptorSetLayoutBinding& binding = this->mLayoutBindings [ i ];
		histogram [ binding.descriptorType ] += binding.descriptorCount;
		this->mSignatureOffsets [ i ] = this->mSignatureSize;
		this->mSignatureSize += binding.descriptorCount;
	}

	ZLSize nTypes = histogram.size ();
	this->mTypeCounts.Init ( nTypes );

	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > >::const_iterator histogramIt = histogram.cbegin ();
	for ( ZLIndex i = 0; histogramIt != histogram.cend (); ++histogramIt, ++i ) {
		VkDescriptorPoolSize& poolSize = this->mTypeCounts [ i ];
		poolSize.type = histogramIt->first;
		poolSize.descriptorCount = histogramIt->second;
	}

	this->mPoolCreateInfo = MOAIGfxStructVK::descriptorPoolCreateInfo ( this->mTypeCounts.GetBuffer (), ( u32 )nTypes, POOL_SIZE ); // TODO: max sets is hardcoded; need a pool of pools
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutImplVK::MOAIDescriptorSetLayoutImplVK () :
	mKey ( this->mLayoutBindings ),
	mLayout ( VK_NULL_HANDLE ),
	mSignatureSize ( 0 ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutImplVK::~MOAIDescriptorSetLayoutImplVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK* MOAIDescriptorSetLayoutImplVK::ProcureDescriptorSet ( const MOAIDescriptorSetStateVK& descriptorSetState ) {
	
	MOAIDescriptorSetKeyVK key ( descriptorSetState.mSignature );
	if ( this->mActiveSnapshots.contains ( key )) {
		return this->mActiveSnapshots [ key ];
	}
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	MOAIDescriptorSetVK* snapshot = NULL;
	MOAIDescriptorPoolVK* pool = NULL;
	
	if ( this->mOpenPools.size ()) {
		pool = *this->mOpenPools.begin ();
	}
	else {
		pool = new MOAIDescriptorPoolVK ();
		VK_CHECK_RESULT ( vkCreateDescriptorPool ( logicalDevice, &this->mPoolCreateInfo, NULL, &pool->mPool ));
		this->mAllPools.insert ( pool );
		this->mOpenPools.insert ( pool );
	}
	
	if ( pool->mExpiredSnapshots.size ()) {
		snapshot = *pool->mExpiredSnapshots.begin ();
		pool->mExpiredSnapshots.erase ( snapshot );
	}
	else {
		snapshot = new MOAIDescriptorSetVK ();
		snapshot->Retain ();
		snapshot->SetDependency < MOAIDescriptorSetLayoutImplVK >( *this );
		VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( pool->mPool, &this->mLayout );
		VK_CHECK_RESULT ( vkAllocateDescriptorSets ( logicalDevice, &allocInfo, &snapshot->mDescriptorSet ));
		pool->mAllSnapshots.insert ( snapshot );
		snapshot->mPool = pool;
	}
	
	if ( pool->mAllSnapshots.size () >= POOL_SIZE ) {
		this->mOpenPools.erase ( pool );
	}
	
	ZLSize totalSets = descriptorSetState.mDescriptors.Size ();
	
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( totalSets * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = 0; i < totalSets; ++i ) {
		writeDescriptors [ i ] = descriptorSetState.mWriteDescriptors [ i ];
		writeDescriptors [ i ].dstSet = *snapshot;
	}
	snapshot->mSignature.CloneFrom ( descriptorSetState.mSignature );
	snapshot->mIsValid = true;
	
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalSets, writeDescriptors, 0, NULL );
	
	this->mActiveSnapshots [ *snapshot ] = snapshot;
	
	return snapshot;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplVK::RetireDescriptorSet ( MOAIDescriptorSetVK& snapshot ) {

	MOAIDescriptorPoolVK* pool = snapshot.mPool;
	assert ( this->mAllPools.contains ( pool ));

	if ( !snapshot.IsValid ()) {
		assert ( this->mActiveSnapshots.contains ( snapshot ));
		this->mActiveSnapshots.erase ( snapshot );
		pool->mExpiredSnapshots.insert ( &snapshot );
		
		if ( pool->mExpiredSnapshots.size () >= POOL_SIZE ) {
			this->DeletePool ( pool );
		}
		else if ( !this->mOpenPools.contains ( pool )) {
			this->mOpenPools.insert ( pool );
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutImplVK::_Finalize () {

	while ( this->mAllPools.size ()) {
		this->DeletePool ( *this->mAllPools.begin ());
	}

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyDescriptorSetLayout ( logicalDevice, this->mLayout, NULL );
	}
	
	this->mActiveSnapshots.clear ();
}
