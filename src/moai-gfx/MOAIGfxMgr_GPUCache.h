// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_GPUCACHE_H
#define	MOAIGFXMGR_GPUCACHE_H

#include <moai-gfx/MOAIGfxMgrComponents.h>
#include <moai-gfx/MOAIBlendMode.h>

class MOAIShader;
class MOAITexture;
class MOAIFrameBuffer;
class MOAIIndexBuffer;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxMgr_GPUCache
//================================================================//
class MOAIGfxMgr_GPUCache :
	public virtual MOAIGfxMgrComponents {
protected:

	//----------------------------------------------------------------//
	virtual void						ZLAbstractGPU_ClearSurface				() = 0; // takes zgl clear flags
	virtual size_t						ZLAbstractGPU_CountTextureUnits			() = 0;
	virtual void						ZLAbstractGPU_DrawPrims					( u32 primType, u32 base, u32 count ) = 0;
	virtual MOAIBlendMode				ZLAbstractGPU_GetBlendMode				() const = 0;
	virtual u32							ZLAbstractGPU_GetBufferHeight			() const = 0;
	virtual u32							ZLAbstractGPU_GetBufferWidth			() const = 0;
	virtual MOAIFrameBuffer*			ZLAbstractGPU_GetCurrentFrameBuffer		() = 0;
	virtual MOAIShader*			ZLAbstractGPU_GetCurrentShader			() = 0;
	virtual MOAIVertexFormat*	ZLAbstractGPU_GetCurrentVtxFormat		() = 0;
	virtual MOAIFrameBuffer*			ZLAbstractGPU_GetDefaultFrameBuffer		() = 0;
	virtual MOAITexture*		ZLAbstractGPU_GetDefaultTexture			() = 0;
	virtual bool						ZLAbstractGPU_GetDepthMask				() const = 0;
	virtual float						ZLAbstractGPU_GetViewHeight				() const = 0;
	virtual ZLRect						ZLAbstractGPU_GetViewRect				() const = 0;
	virtual float						ZLAbstractGPU_GetViewWidth				() const = 0;
	virtual void						ZLAbstractGPU_ResetGPUState				() = 0;
	virtual void						ZLAbstractGPU_SetBlendMode				() = 0;
	virtual void						ZLAbstractGPU_SetBlendMode				( const MOAIBlendMode& blendMode ) = 0;
	virtual void						ZLAbstractGPU_SetBlendMode				( int srcFactor, int dstFactor, int equation ) = 0;
	virtual void						ZLAbstractGPU_SetCullFunc				() = 0;
	virtual void						ZLAbstractGPU_SetCullFunc				( int cullFunc ) = 0;
	virtual void						ZLAbstractGPU_SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer ) = 0;
	virtual void						ZLAbstractGPU_SetDefaultTexture			( MOAITexture* texture ) = 0;
	virtual void						ZLAbstractGPU_SetDepthFunc				() = 0;
	virtual void						ZLAbstractGPU_SetDepthFunc				( int depthFunc ) = 0;
	virtual void						ZLAbstractGPU_SetDepthMask				( bool depthMask ) = 0;
	virtual bool						ZLAbstractGPU_SetFrameBuffer			( MOAIFrameBuffer* frameBuffer ) = 0;
	virtual bool						ZLAbstractGPU_SetIndexBuffer			( MOAIIndexBuffer* buffer ) = 0;
	virtual void						ZLAbstractGPU_SetPenWidth				( float penWidth ) = 0;
	virtual void						ZLAbstractGPU_SetScissorRect			() = 0;
	virtual void						ZLAbstractGPU_SetScissorRect			( ZLRect rect ) = 0;
	virtual bool						ZLAbstractGPU_SetShader					( MOAIShader* shader ) = 0;
	virtual bool						ZLAbstractGPU_SetTexture				( MOAITexture* texture, ZLIndex textureUnit ) = 0;
	virtual bool						ZLAbstractGPU_SetVertexArray			( MOAIVertexArray* vtxArray ) = 0;
	virtual bool						ZLAbstractGPU_SetVertexBuffer			( MOAIVertexBuffer* buffer ) = 0;
	virtual void						ZLAbstractGPU_SetVertexFormat			( MOAIVertexFormat* format ) = 0;
	virtual void						ZLAbstractGPU_SetViewRect				() = 0;
	virtual void						ZLAbstractGPU_SetViewRect				( ZLRect rect ) = 0;
	virtual void						ZLAbstractGPU_UnbindAll					() = 0;

public:
	
	//----------------------------------------------------------------//
	void						ClearSurface					(); // takes zgl clear flags
	size_t						CountTextureUnits				();
	void						DrawPrims						( u32 primType, u32 base, u32 count );
	MOAIBlendMode				GetBlendMode					() const;
	u32							GetBufferHeight					() const;
	u32							GetBufferWidth					() const;
	MOAIFrameBuffer*	GetCurrentFrameBuffer		();
	MOAIShader*			GetCurrentShader			();
	MOAIVertexFormat*	GetCurrentVtxFormat			();
	MOAIFrameBuffer*	GetDefaultFrameBuffer		();
	MOAITexture*		GetDefaultTexture			();
	bool						GetDepthMask				() const;
	float						GetViewHeight				() const;
	ZLRect						GetViewRect					() const;
	float						GetViewWidth				() const;
								MOAIGfxMgr_GPUCache		();
	virtual						~MOAIGfxMgr_GPUCache		();
	void						ResetGPUState				();
	void						SetBlendMode				();
	void						SetBlendMode				( const MOAIBlendMode& blendMode );
	void						SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );
	void						SetCullFunc					();
	void						SetCullFunc					( int cullFunc );
	void						SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer );
	void						SetDefaultTexture			( MOAITexture* texture );
	void						SetDepthFunc				();
	void						SetDepthFunc				( int depthFunc );
	void						SetDepthMask				( bool depthMask );
	bool						SetFrameBuffer				( MOAIFrameBuffer* frameBuffer = NULL );
	bool						SetIndexBuffer				( MOAIIndexBuffer* buffer = NULL );
	void						SetPenWidth					( float penWidth );
	void						SetScissorRect				();
	void						SetScissorRect				( ZLRect rect );
	bool						SetShader					( MOAIShader* shader = NULL );
	bool						SetTexture					( MOAITexture* texture = NULL, ZLIndex textureUnit = ZLIndexOp::ZERO );
	bool						SetVertexArray				( MOAIVertexArray* vtxArray = NULL );
	bool						SetVertexBuffer				( MOAIVertexBuffer* buffer = NULL );
	void						SetVertexFormat				( MOAIVertexFormat* format = NULL );
	void						SetViewRect					();
	void						SetViewRect					( ZLRect rect );
	void						UnbindAll					();
};

#endif
