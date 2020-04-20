// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFrameBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIImageBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	getGrabbedImage
//	@text	Returns the image into which frame(s) will be (or were) grabbed (if any).
//
//	@in		MOAIFrameBufferVK self
//	@opt	boolean discard			If true, image will be discarded from the frame buffer.
//	@out	MOAIImage image			The frame grab image, or nil if none exists.
//*/
//int MOAIFrameBufferVK::_getGrabbedImage ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferVK, "U" )
//
//	bool discard = state.GetValue < bool >( 2, false );
//
//	self->mFrameImage.PushRef ( state );
//
//	if ( discard ) {
//		self->mFrameImage.Set ( *self, 0 );
//	}
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	grabNextFrame
//	@text	Save the next frame rendered to an image. If no image is
//			provided, one will be created tp match the size of the frame
//			buffer.
//
//	@in		MOAIFrameBufferVK self
//	@opt	MOAIImage image			Image to save the backbuffer to
//	@opt	function callback		The function to execute when the frame has been saved into the image specified
//	@out	nil
//*/
//int MOAIFrameBufferVK::_grabNextFrame ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferVK, "U" )
//
//	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, false );
//
//	if ( image ) {
//		self->mFrameImage.Set ( *self, image );
//	}
//	else if ( !self->mFrameImage ) {
//
//		image = new MOAIImage ();
//		image->Init ( self->mBufferWidth, self->mBufferHeight, ZLColor::RGBA_8888, MOAIImage::TRUECOLOR );
//		self->mFrameImage.Set ( *self, image );
//	}
//
//	self->mGrabNextFrame = self->mFrameImage != 0;
//
//	if ( self->mGrabNextFrame ) {
//		self->mOnFrameFinish.SetRef ( *self, state, 3 );
//	}
//	else{
//		self->mOnFrameFinish.Clear ();
//	}
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	isPendingGrab
//	@text	True if a frame grab has been requested but not yet grabbed.
//
//	@in		MOAIFrameBufferVK self
//	@out	table renderTable
//*/
//int MOAIFrameBufferVK::_isPendingGrab ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferVK, "U" )
//	state.Push ( self->mGrabNextFrame );
//	return 1;
//}

//================================================================//
// MOAIFrameBufferVK
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferVK::MOAIFrameBufferVK () {
	
	RTTI_BEGIN ( MOAIFrameBufferVK )
		RTTI_EXTEND ( MOAIFrameBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferVK::~MOAIFrameBufferVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferVK::ReserveAttachments ( ZLSize count ) {

	this->mAttachments.Init ( count );
}

//----------------------------------------------------------------//
void MOAIFrameBufferVK::SetAttachment ( ZLIndex index, MOAIImageBufferSnapshotVK::Factory& imageViewSnapshot ) {

	this->mAttachments [ index ] = &imageViewSnapshot;
}

//----------------------------------------------------------------//
void MOAIFrameBufferVK::SetLogicalDevice ( MOAILogicalDeviceVK& logicalDevice ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
}

//----------------------------------------------------------------//
void MOAIFrameBufferVK::SetRenderPass ( MOAIRenderPassVK& renderPass ) {

	this->SetDependency < MOAIRenderPassVK >( renderPass );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferVK::_Finalize () {
}

//----------------------------------------------------------------//
MOAIFrameBufferSnapshotVK* MOAIFrameBufferVK::MOAISnapshotFactoryVK_GetSnapshot () {

	if ( !this->mSnapshot ) {
	
		if ( !this->HasDependency < MOAILogicalDeviceVK >()) return NULL;
		if ( !this->HasDependency < MOAIRenderPassVK >()) return NULL;
	
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		MOAIRenderPassVK& renderPass = this->GetDependency < MOAIRenderPassVK >();

		MOAIFrameBufferSnapshotVK* snapshot = new MOAIFrameBufferSnapshotVK ();

		snapshot->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
		snapshot->SetDependency < MOAIRenderPassVK >( renderPass );

		ZLSize nAttachments = this->mAttachments.Size ();
		snapshot->mAttachmentSnapshots.Init ( nAttachments );
		snapshot->mAttachments.Init ( nAttachments );
		
		for ( ZLIndex i = 0; i < nAttachments; ++i ) {
			assert ( this->mAttachments [ i ]);
			snapshot->mAttachmentSnapshots [ i ] = this->mAttachments [ i ].GetSnapshot ();
			snapshot->mAttachments [ i ] = *snapshot->mAttachmentSnapshots [ i ];
		}
		
		VkFramebufferCreateInfo framebufferCreateInfo = MOAIGfxStructVK::framebufferCreateInfo (
			renderPass,
			snapshot->mAttachments.GetBuffer (),
			( u32 )nAttachments,
			this->mBufferWidth,
			this->mBufferHeight
		);
		snapshot->mFrameBuffer = logicalDevice.CreateFramebuffer ( framebufferCreateInfo );
		this->mSnapshot = snapshot;
	}
	return this->mSnapshot;
}
