// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGAPIGL_H
#define	MOAIABSTRACTDRAWINGAPIGL_H

//================================================================//
// MOAIAbstractDrawingAPIGL
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIGL :
	public virtual MOAIAbstractDrawingAPI {
protected:

public:

	//----------------------------------------------------------------//
	void				LoadShaderUniformGL				( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
						MOAIAbstractDrawingAPIGL		();
	virtual				~MOAIAbstractDrawingAPIGL		();
};

#endif
