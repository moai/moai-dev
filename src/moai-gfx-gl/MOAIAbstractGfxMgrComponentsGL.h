// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXMGRCOMPONENTSGL_H
#define	MOAIABSTRACTGFXMGRCOMPONENTSGL_H

class MOAIGfxMgrGL;
class MOAIGfxPipelineClerkGL;
class MOAIGfxResourceClerkGL;
class MOAIGfxStateGPUCacheGL;
class MOAIVertexCacheGL;

//================================================================//
// MOAIAbstractGfxMgrComponentsGL
//================================================================//
class MOAIAbstractGfxMgrComponentsGL {
protected:

	//----------------------------------------------------------------//
	virtual MOAIGfxMgrGL&				MOAIAbstractGfxMgrComponentsGL_GetGfxMgrGL					() = 0;
	virtual MOAIGfxPipelineClerkGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxPipelineClerkGL		() = 0;
	virtual MOAIGfxResourceClerkGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxResourceClerkGL		() = 0;
	virtual MOAIGfxStateGPUCacheGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxStateGPUCacheGL		() = 0;
	virtual MOAIVertexCacheGL&			MOAIAbstractGfxMgrComponentsGL_GetVertexCacheGL				() = 0;

public:

	//----------------------------------------------------------------//
	inline MOAIGfxMgrGL& GetGfxMgrGL () {
		return MOAIAbstractGfxMgrComponentsGL_GetGfxMgrGL ();
	}

	//----------------------------------------------------------------//
	inline MOAIGfxPipelineClerkGL& GetGfxPipelineClerkGL () {
		return MOAIAbstractGfxMgrComponentsGL_GetGfxPipelineClerkGL ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxResourceClerkGL& GetGfxResourceClerkGL () {
		return MOAIAbstractGfxMgrComponentsGL_GetGfxResourceClerkGL ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIGfxStateGPUCacheGL& GetGfxStateGPUCacheGL () {
		return MOAIAbstractGfxMgrComponentsGL_GetGfxStateGPUCacheGL ();
	}
	
	//----------------------------------------------------------------//
	inline MOAIVertexCacheGL& GetVertexCacheGL () {
		return MOAIAbstractGfxMgrComponentsGL_GetVertexCacheGL ();
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractGfxMgrComponentsGL () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractGfxMgrComponentsGL () {
	}
};

#endif
