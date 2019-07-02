// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISURFACEVK_H
#define MOAISURFACEVK_H

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>

//================================================================//
// MOAISurfaceVK
//================================================================//
class MOAISurfaceVK {
public:

	VkSurfaceKHR						mSurface;
	
	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mSurface != VK_NULL_HANDLE );
	}

	//----------------------------------------------------------------//
	operator VkSurfaceKHR () {
		return this->mSurface;
	}

	//----------------------------------------------------------------//
	void			Cleanup					( MOAIGfxInstanceVK& instance );
	void			Init					( VkSurfaceKHR surface );
					MOAISurfaceVK			();
};

#endif
