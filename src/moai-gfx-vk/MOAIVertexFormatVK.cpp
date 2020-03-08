// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIVertexFormatVK
//================================================================//

//----------------------------------------------------------------//
VkFormat MOAIVertexFormatVK::GuessFormat ( const MOAIVertexAttribute& attribute ) {
	
	switch ( attribute.mType ) {
	
		case MOAIGfxTypeEnum::BYTE:
			switch ( attribute.mSize ) {
				case 1:		return VK_FORMAT_R8_SINT;
				case 2:		return VK_FORMAT_R8G8_SINT;
				case 3:		return VK_FORMAT_R8G8B8_SINT;
				case 4:		return VK_FORMAT_R8G8B8A8_SINT;
			}
			break;
			
		case MOAIGfxTypeEnum::FLOAT:
			switch ( attribute.mSize ) {
				case 1:		return VK_FORMAT_R32_SFLOAT;
				case 2:		return VK_FORMAT_R32G32_SFLOAT;
				case 3:		return VK_FORMAT_R32G32B32_SFLOAT;
				case 4:		return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
			break;
		
		case MOAIGfxTypeEnum::SHORT:
			switch ( attribute.mSize ) {
				case 1:		return VK_FORMAT_R16_SINT;
				case 2:		return VK_FORMAT_R16G16_SINT;
				case 3:		return VK_FORMAT_R16G16B16_SINT;
				case 4:		return VK_FORMAT_R16G16B16A16_SINT;
			}
			break;
		
		case MOAIGfxTypeEnum::UNSIGNED_BYTE:
		
			if ( attribute.mNormalized ) {
				switch ( attribute.mSize ) {
					case 1:		return VK_FORMAT_R8_UNORM;
					case 2:		return VK_FORMAT_R8G8_UNORM;
					case 3:		return VK_FORMAT_R8G8B8_UNORM;
					case 4:		return VK_FORMAT_R8G8B8A8_UNORM;
				}
			}
			else {
				switch ( attribute.mSize ) {
					case 1:		return VK_FORMAT_R8_UINT;
					case 2:		return VK_FORMAT_R8G8_UINT;
					case 3:		return VK_FORMAT_R8G8B8_UINT;
					case 4:		return VK_FORMAT_R8G8B8A8_UINT;
				}
			}
			break;
		
		case MOAIGfxTypeEnum::UNSIGNED_SHORT:
			switch ( attribute.mSize ) {
				case 1:		return VK_FORMAT_R16_UINT;
				case 2:		return VK_FORMAT_R16G16_UINT;
				case 3:		return VK_FORMAT_R16G16B16_UINT;
				case 4:		return VK_FORMAT_R16G16B16A16_UINT;
			}
			break;
	}
	
	assert ( false );
	return VK_FORMAT_UNDEFINED;
};

//----------------------------------------------------------------//
MOAIVertexFormatVK::MOAIVertexFormatVK () {
	
	RTTI_BEGIN ( MOAIVertexFormatVK )
		RTTI_EXTEND ( MOAIVertexFormat )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexFormatVK::~MOAIVertexFormatVK () {
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::UpdatePipelineCreateInfo ( VkGraphicsPipelineCreateInfo& info ) {

	info.pVertexInputState = NULL;

	ZLSize nAttributes = this->mAttributes.Size ();

	if ( nAttributes == 0 ) return;

	if ( this->mVertexInputAttributeDescriptionsVK.Size () != nAttributes ) {
	
		this->mVertexInputAttributeDescriptionsVK.Init ( nAttributes );
		
		for ( ZLIndex i = 0; i < nAttributes; ++i ) {
		
			MOAIVertexAttribute& attribute = this->mAttributes [ i ];
	
			this->mVertexInputAttributeDescriptionsVK [ i ] = MOAIGfxStructVK::vertexInputAttributeDescription (
				0,
				( u32 )attribute.mIndex,
				this->GuessFormat ( attribute ),
				attribute.mOffset
			);
		}
		
		this->mVertexInputBindingDescriptionVK = MOAIGfxStructVK::vertexInputBindingDescription (
			0,
			( u32 )this->GetVertexSize (),
			VK_VERTEX_INPUT_RATE_VERTEX
		);
		
		this->mPipelineVertexInputStateCreateInfoVK = MOAIGfxStructVK::pipelineVertexInputStateCreateInfo (
			&this->mVertexInputBindingDescriptionVK,
			1,
			this->mVertexInputAttributeDescriptionsVK.GetBuffer (),
			( u32 )nAttributes
		);
	}

	info.pVertexInputState = &this->mPipelineVertexInputStateCreateInfoVK;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
}
