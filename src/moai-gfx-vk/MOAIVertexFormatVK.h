// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATVK_H
#define	MOAIVERTEXFORMATVK_H

//================================================================//
// MOAIVertexFormatVK
//================================================================//
/**	@lua	MOAIVertexFormatVK
	@text	Vertex format class.
*/
class MOAIVertexFormatVK :
	public virtual MOAIVertexFormat {
protected:

	ZLLeanArray < VkVertexInputAttributeDescription >	mVertexInputAttributeDescriptionsVK;
	VkVertexInputBindingDescription						mVertexInputBindingDescriptionVK;
	VkPipelineVertexInputStateCreateInfo				mPipelineVertexInputStateCreateInfoVK;

	//----------------------------------------------------------------//
	static VkFormat		GuessFormat							( const MOAIVertexAttribute& attribute );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormatVK )
		
	//----------------------------------------------------------------//
						MOAIVertexFormatVK				();
						~MOAIVertexFormatVK				();
	void				UpdatePipelineCreateInfo		( VkGraphicsPipelineCreateInfo& info );
};

#endif
