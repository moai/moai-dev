// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGLCOMPONENTS_H
#define	MOAIGFXMGRGLCOMPONENTS_H

class MOAIGfxMgrGL;
class MOAIGfxMgrGL_PipelineClerkGL;
class MOAIGfxMgrGL_ResourceClerkGL;
class MOAIGfxMgrGL_GPUCacheGL;
class MOAIGfxMgrGL_VertexCacheGL;

//================================================================//
// MOAIGfxMgrGLComponents
//================================================================//
class MOAIGfxMgrGLComponents :
	public virtual MOAIGfxMgrComponents {
protected:

	//----------------------------------------------------------------//
	virtual MOAIGfxMgrGL&					MOAIGfxMgrGLComponents_GetGfxMgrGL				() = 0;
	virtual MOAIGfxMgrGL_GPUCacheGL&			MOAIGfxMgrGLComponents_GetGPUCacheGL		() = 0;
	virtual MOAIGfxMgrGL_PipelineClerkGL&		MOAIGfxMgrGLComponents_GetPipelineClerkGL	() = 0;
	virtual MOAIGfxMgrGL_ResourceClerkGL&		MOAIGfxMgrGLComponents_GetResourceClerkGL	() = 0;
	virtual MOAIGfxMgrGL_VertexCacheGL&		MOAIGfxMgrGLComponents_GetVertexCacheGL			() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL& GetGfxMgrGL () {
		return MOAIGfxMgrGLComponents_GetGfxMgrGL ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL_GPUCacheGL& GetGPUCacheGL () {
		return MOAIGfxMgrGLComponents_GetGPUCacheGL ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL_PipelineClerkGL& GetPipelineClerkGL () {
		return MOAIGfxMgrGLComponents_GetPipelineClerkGL ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL_ResourceClerkGL& GetResourceClerkGL () {
		return MOAIGfxMgrGLComponents_GetResourceClerkGL ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL_VertexCacheGL& GetVertexCacheGL () {
		return MOAIGfxMgrGLComponents_GetVertexCacheGL ();
	}
	
	//----------------------------------------------------------------//
	MOAIGfxMgrGLComponents () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIGfxMgrGLComponents () {
	}
};

#endif
