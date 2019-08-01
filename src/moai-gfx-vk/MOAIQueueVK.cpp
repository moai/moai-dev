// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIQueueVK.h>

//================================================================//
// MOAIQueueVK
//================================================================//

//----------------------------------------------------------------//
MOAIQueueVK::MOAIQueueVK () :
	mQueuePresentKHR ( NULL ),
	mIndex (( u32 )-1 ),
	mPool ( VK_NULL_HANDLE ),
	mQueue ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIQueueVK::~MOAIQueueVK () {
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::PresentKHR ( const VkPresentInfoKHR& presentInfo ) {

	if ( this->mQueuePresentKHR ) {
		this->mQueuePresentKHR ( this->mQueue, &presentInfo );
	}
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::Submit ( const VkSubmitInfo* submits, u32 submitCount, VkFence fence ) {

	return vkQueueSubmit ( this->mQueue, submitCount, submits, fence );
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::WaitIdle () {

	return vkQueueWaitIdle ( this->mQueue );
}
