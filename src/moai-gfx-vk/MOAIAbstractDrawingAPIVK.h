// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIGL_H
#define	MOAIDRAWINGAPIGL_H

class MOAIAbstractDescriptorElementVK;

//================================================================//
// MOAIAbstractDrawingAPIVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIVK :
	public virtual MOAIAbstractDrawingAPI {
public:

	//----------------------------------------------------------------//
	void				LoadDescriptorElementVK					( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementVK& element );
	void				LoadDescriptorFromTextureUnitVK			( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, ZLIndex textureUnit );
	void				LoadShaderUniformVK						( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
						MOAIAbstractDrawingAPIVK				();
	virtual				~MOAIAbstractDrawingAPIVK				();
};

#endif
