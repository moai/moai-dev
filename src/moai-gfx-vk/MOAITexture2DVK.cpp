// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/ZLTextureFormat.h>

//================================================================//
// MOAITexture2DVK
//================================================================//

//----------------------------------------------------------------//
MOAITexture2DVK::MOAITexture2DVK () {

	RTTI_BEGIN ( MOAITexture2DVK )
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DVK::~MOAITexture2DVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAIAbstractDescriptorElementVK_GetPinnedData ( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer ) {

	assert ( writeDescriptorSet.pImageInfo );
	VkDescriptorImageInfo& imageInfo = *this->GetSnapshot ( commandBuffer );
	(( VkDescriptorImageInfo* )writeDescriptorSet.pImageInfo )[ index ] = imageInfo; // TODO: casting away const is gross; fix it later
}

//----------------------------------------------------------------//
MOAITextureSnapshot2DVK* MOAITexture2DVK::MOAIAbstractSnapshotFactoryVK_GetSnapshot ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );

	if ( !this->mSnapshot && this->mImage && this->mImage->IsOK ()) {

		this->mSnapshot = new MOAITextureSnapshot2DVK ();
		this->mSnapshot->Initialize ( *this->mImage );
		this->mTextureSize = this->mSnapshot->GetSize ();
	}
	return this->mSnapshot;
}

//----------------------------------------------------------------//
bool MOAITexture2DVK::MOAIGfxResource_FinishLoading () {

	this->MOAITexture2D::MOAIGfxResource_FinishLoading ();
//	if ( this->mImage && this->mImage->IsOK ()) {
//		this->CreateTextureFromImage ( *this->mImage );
//	}
}

//----------------------------------------------------------------//
bool MOAITexture2DVK::MOAIGfxResource_IsReadyForUse () const {
}

//----------------------------------------------------------------//
