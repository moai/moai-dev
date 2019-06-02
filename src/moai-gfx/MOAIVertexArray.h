// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAY_H
#define	MOAIVERTEXARRAY_H

class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIVertexArray
//================================================================//
class MOAIVertexArray :
	public virtual MOAILuaObject {
protected:

	//----------------------------------------------------------------//
	virtual MOAIVertexBuffer*		MOAIVertexArray_GetVertexBuffer			( ZLIndex idx ) = 0;
	virtual MOAIVertexFormat*		MOAIVertexArray_GetVertexFormat			( ZLIndex idx ) = 0;
	virtual void					MOAIVertexArray_SetVertexBuffer			( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) = 0;

public:

	//----------------------------------------------------------------//
	MOAIVertexBuffer*		GetVertexBuffer				( ZLIndex idx );
	MOAIVertexFormat*		GetVertexFormat				( ZLIndex idx );
							MOAIVertexArray				();
							~MOAIVertexArray			();
	void					SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
};

#endif
