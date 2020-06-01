// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGAPIGL_H
#define	MOAIABSTRACTDRAWINGAPIGL_H

class MOAIAbstractDescriptorElementStateVK;

//================================================================//
// MOAIAbstractDrawingAPIVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIVK :
	public virtual MOAIAbstractDrawingAPI {
public:

	//----------------------------------------------------------------//
	void				LoadDescriptorElementVK					( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK& element );
	void				LoadDescriptorFromTextureUnitVK			( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, ZLIndex textureUnit );
	void				LoadShaderUniformVK						( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
						MOAIAbstractDrawingAPIVK				();
	virtual				~MOAIAbstractDrawingAPIVK				();
};

#endif
