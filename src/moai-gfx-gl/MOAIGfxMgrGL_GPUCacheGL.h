// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_GPUCACHEGL_H
#define	MOAIGFXMGRGL_GPUCACHEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>
#include <moai-gfx-gl/MOAIGfxStateGPUCacheFrameGL.h>

class MOAIFrameBufferGL;
class MOAIIndexBufferGL;
class MOAIShaderGL;
class MOAIShaderProgramGL;
class MOAIVertexArrayGL;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;
class MOAIGfxResourceGL;
class MOAIVertexBufferWithFormatGL;
class MOAITextureGL;

//================================================================//
// MOAIGfxMgrGL_GPUCacheGL
//================================================================//
class MOAIGfxMgrGL_GPUCacheGL :
	public virtual MOAIGfxMgrGLComponents,
	public virtual MOAIGfxMgr_GPUCache {
protected:

	friend class MOAIGfxMgrGL_VertexCacheGL;
	friend class MOAIVertexArrayGL;

	friend class MOAIGfxMgr;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	//----------------------------------------------------------------//
	void			Clear							();
	void			ApplyStateChange				( u32 stateID );
	void			ApplyStateChanges				();
	void			FlushBlendMode					();
	void			FlushCullFunc					();
	void			FlushDepthFunc					();
	void			FlushDepthMask					();
	void			FlushFrameBuffer				();
	void			FlushIndexBuffer				();
	void			FlushPenWidth					();
	void			FlushScissorRect				();
	void			FlushShader						();
	void			FlushTexture					( ZLIndex textureUnit );
	void			FlushVertexArray				();
	void			FlushVertexBuffer				();
	void			FlushVertexFormat				();
	void			FlushViewRect					();
	void			ForceIndexBuffer				( MOAIIndexBufferGL* buffer );
	void			ForceVertexBuffer				( MOAIVertexBufferGL* buffer );
	void			RecalculateDirtyFlags			();
	void			RestoreGPUState					( const MOAIGfxStateGPUCacheFrame& frame );
	void			StoreGPUState					( MOAIGfxStateGPUCacheFrame& frame ) const;

	//----------------------------------------------------------------//
	void			MOAIGfxMgr_GPUCache_ClearSurface				(); // takes zgl clear flags
	size_t			MOAIGfxMgr_GPUCache_CountTextureUnits			();
	void			MOAIGfxMgr_GPUCache_DrawPrims					( u32 primType, u32 base, u32 count );
	void			MOAIGfxMgr_GPUCache_ResetGPUState				();
	void			MOAIGfxMgr_GPUCache_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrGL_GPUCacheGL			();
	virtual			~MOAIGfxMgrGL_GPUCacheGL		();
};

#endif
