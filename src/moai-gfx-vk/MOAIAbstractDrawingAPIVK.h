// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIGL_H
#define	MOAIDRAWINGAPIGL_H

//================================================================//
// MOAIAbstractDrawingAPIVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIVK :
	public virtual MOAIAbstractDrawingAPI {
protected:

public:

	//----------------------------------------------------------------//
	void				LoadShaderUniformVK				( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
						MOAIAbstractDrawingAPIVK		();
	virtual				~MOAIAbstractDrawingAPIVK		();
};

#endif
