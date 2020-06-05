// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

//================================================================//
// MOAIRenderPassVK
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderPassVK::AffirmDescription () {

	if ( !this->mDescription ) {
		this->mDescription = new MOAIRenderPassDescriptionVK ();
	}
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::AffirmRenderPass ( MOAILogicalDeviceVK& logicalDevice ) {

	if ( !this->mDescription ) return;
	ZLStrongPtr < MOAIRenderPassDescriptionVK > description = this->mDescription;
	this->Destruct ();
	
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	
	VkRenderPassCreateInfo renderPassInfo = MOAIGfxStructVK::renderPassCreateInfo (
		description->mAttachments.GetBuffer (),
		description->mAttachments.Size (),
		description->mSubpasses.GetBuffer (),
		description->mSubpasses.Size (),
		description->mDependencies.GetBuffer (),
		description->mDependencies.Size ()
	);

	this->mRenderPass = logicalDevice.CreateRenderPass ( renderPassInfo );
}

//----------------------------------------------------------------//
ZLSize MOAIRenderPassVK::CountAttachments () const {

	return this->mDescription->mAttachments.Size ();
}

//----------------------------------------------------------------//
u32 MOAIRenderPassVK::GetAttachmentClearType ( ZLIndex index ) const {

	return this->mDescription->mClearTypes [ index ];
}

//----------------------------------------------------------------//
MOAIRenderPassVK::MOAIRenderPassVK () :
	mDescription ( NULL ),
	mRenderPass ( VK_NULL_HANDLE )  {
}

//----------------------------------------------------------------//
MOAIRenderPassVK::~MOAIRenderPassVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReserveAttachments ( ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mAttachments.Init ( count );
	this->mDescription->mClearTypes.Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReserveDependencies ( ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mDependencies.Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReservePreserveArrays ( ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mPreserveArrays.Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReservePreserves ( ZLIndex index, ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mPreserveArrays [ index ].Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReserveReferenceArrays ( ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mReferenceArrays.Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReserveReferences ( ZLIndex index, ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mReferenceArrays [ index ].Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::ReserveSubpasses ( ZLSize count ) {

	this->AffirmDescription ();
	this->mDescription->mSubpasses.Init ( count );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetAttachment ( ZLIndex index, const VkAttachmentDescription& description, u32 clearType ) {

	assert ( this->mDescription );
	this->mDescription->mAttachments [ index ] = description;
	this->mDescription->mClearTypes [ index ] = clearType;
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetDependency ( ZLIndex index, const VkSubpassDependency& dependency ) {

	assert ( this->mDescription );
	this->mDescription->mDependencies [ index ] = dependency;
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetPreserve ( ZLIndex arrayIndex, ZLIndex subIndex, ZLIndex preserveIndex ) {

	assert ( this->mDescription );
	this->mDescription->mPreserveArrays [ arrayIndex ][ subIndex ] = ( u32 )preserveIndex;
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetReference ( ZLIndex arrayIndex, ZLIndex subIndex, const VkAttachmentReference& reference ) {

	assert ( this->mDescription );
	this->mDescription->mReferenceArrays [ arrayIndex ][ subIndex ] = reference;
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpass ( ZLIndex index, VkPipelineBindPoint pipelineBindPoint, VkSubpassDescriptionFlags flags ) {

	assert ( this->mDescription );

	DECL_VK_STRUCT ( VkSubpassDescription, subpassDescription );
	subpassDescription.pipelineBindPoint = pipelineBindPoint;
	subpassDescription.flags = flags;
	this->mDescription->mSubpasses [ index ] = subpassDescription;
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpassColorReferenceArray ( ZLIndex index, ZLIndex arrayID ) {

	assert ( this->mDescription );

	VkSubpassDescription& subpassDescription = this->mDescription->mSubpasses [ index ];
	MOAIRenderPassDescriptionVK::ReferenceArray& referenceArray = this->mDescription->mReferenceArrays [ arrayID ];
	
	subpassDescription.pColorAttachments = referenceArray.GetBuffer ();
	subpassDescription.colorAttachmentCount = ( u32 )referenceArray.Size ();
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpassDepthStencilReference ( ZLIndex index, ZLIndex arrayID, ZLIndex refIndex ) {

	assert ( this->mDescription );

	VkSubpassDescription& subpassDescription = this->mDescription->mSubpasses [ index ];
	MOAIRenderPassDescriptionVK::ReferenceArray& referenceArray = this->mDescription->mReferenceArrays [ arrayID ];
	
	subpassDescription.pDepthStencilAttachment = &referenceArray [ refIndex ];
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpassInputReferenceArray ( ZLIndex index, ZLIndex arrayID ) {

	assert ( this->mDescription );

	VkSubpassDescription& subpassDescription = this->mDescription->mSubpasses [ index ];
	MOAIRenderPassDescriptionVK::ReferenceArray& referenceArray = this->mDescription->mReferenceArrays [ arrayID ];
	
	subpassDescription.pInputAttachments = referenceArray.GetBuffer ();
	subpassDescription.inputAttachmentCount = ( u32 )referenceArray.Size ();
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpassPreserveIndexArray ( ZLIndex index, ZLIndex arrayID ) {

	assert ( this->mDescription );

	VkSubpassDescription& subpassDescription = this->mDescription->mSubpasses [ index ];
	MOAIRenderPassDescriptionVK::PreserveArray& preserveArray = this->mDescription->mPreserveArrays [ arrayID ];
	
	subpassDescription.pPreserveAttachments = preserveArray.GetBuffer ();
	subpassDescription.preserveAttachmentCount = ( u32 )preserveArray.Size ();
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::SetSubpassResolveReferenceArray ( ZLIndex index, ZLIndex arrayID ) {

	assert ( this->mDescription );

	VkSubpassDescription& subpassDescription = this->mDescription->mSubpasses [ index ];
	MOAIRenderPassDescriptionVK::ReferenceArray& referenceArray = this->mDescription->mReferenceArrays [ arrayID ];
	
	subpassDescription.pResolveAttachments = referenceArray.GetBuffer ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderPassVK::_Finalize () {

	this->mDescription = NULL;
	
	if ( this->mRenderPass ) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyRenderPass ( logicalDevice, this->mRenderPass, NULL );
	}
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::MOAIAbstractSnapshotVK_OnPin ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );
}

//----------------------------------------------------------------//
void MOAIRenderPassVK::MOAIAbstractSnapshotVK_OnUnpin () {
}

//----------------------------------------------------------------//
MOAIRenderPassVK* MOAIRenderPassVK::MOAISnapshotFactoryVK_GetSnapshot () {

	return this;
}
