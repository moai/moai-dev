// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINELAYOUTVK_H
#define MOAIPIPELINELAYOUTVK_H

//================================================================//
// MOAIPipelineLayoutVK
//================================================================//
class MOAIPipelineLayoutVK {
public:

	VkPipelineLayout	mPipelineLayout;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mPipelineLayout != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkPipelineLayout* () {
	
		return &this->mPipelineLayout;
	}
	
	//----------------------------------------------------------------//
	operator VkPipelineLayout& () {
	
		return this->mPipelineLayout;
	}
	
	//----------------------------------------------------------------//
						MOAIPipelineLayoutVK			();
						~MOAIPipelineLayoutVK			();
};

#endif
