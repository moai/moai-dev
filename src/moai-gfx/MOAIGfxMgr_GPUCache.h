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
	virtual void					MOAIGfxMgr_GPUCache_ClearSurface				() = 0; // takes zgl clear flags
	virtual size_t					MOAIGfxMgr_GPUCache_CountTextureUnits			() = 0;
	virtual void					MOAIGfxMgr_GPUCache_DrawPrims					( u32 primType, u32 base, u32 count ) = 0;
	virtual MOAIBlendMode			MOAIGfxMgr_GPUCache_GetBlendMode				() const = 0;
	virtual u32						MOAIGfxMgr_GPUCache_GetBufferHeight				() const = 0;
	virtual u32						MOAIGfxMgr_GPUCache_GetBufferWidth				() const = 0;
	virtual MOAIFrameBuffer*		MOAIGfxMgr_GPUCache_GetCurrentFrameBuffer		() = 0;
	virtual MOAIShader*				MOAIGfxMgr_GPUCache_GetCurrentShader			() = 0;
	virtual MOAIVertexFormat*		MOAIGfxMgr_GPUCache_GetCurrentVtxFormat			() = 0;
	virtual MOAIFrameBuffer*		MOAIGfxMgr_GPUCache_GetDefaultFrameBuffer		() = 0;
	virtual MOAITexture*			MOAIGfxMgr_GPUCache_GetDefaultTexture			() = 0;
	virtual bool					MOAIGfxMgr_GPUCache_GetDepthMask				() const = 0;
	virtual float					MOAIGfxMgr_GPUCache_GetViewHeight				() const = 0;
	virtual ZLRect					MOAIGfxMgr_GPUCache_GetViewRect					() const = 0;
	virtual float					MOAIGfxMgr_GPUCache_GetViewWidth				() const = 0;
	virtual void					MOAIGfxMgr_GPUCache_ResetGPUState				() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetBlendMode				() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetBlendMode				( const MOAIBlendMode& blendMode ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetBlendMode				( int srcFactor, int dstFactor, int equation ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetCullFunc					() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetCullFunc					( int cullFunc ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetDefaultTexture			( MOAITexture* texture ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetDepthFunc				() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetDepthFunc				( int depthFunc ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetDepthMask				( bool depthMask ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetFrameBuffer				( MOAIFrameBuffer* frameBuffer ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetIndexBuffer				( MOAIIndexBuffer* buffer ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetPenWidth					( float penWidth ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetScissorRect				() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetScissorRect				( ZLRect rect ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetShader					( MOAIShader* shader ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetTexture					( MOAITexture* texture, ZLIndex textureUnit ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetVertexArray				( MOAIVertexArray* vtxArray ) = 0;
	virtual bool					MOAIGfxMgr_GPUCache_SetVertexBuffer				( MOAIVertexBuffer* buffer ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetVertexFormat				( MOAIVertexFormat* format ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetViewRect					() = 0;
	virtual void					MOAIGfxMgr_GPUCache_SetViewRect					( ZLRect rect ) = 0;
	virtual void					MOAIGfxMgr_GPUCache_UnbindAll					() = 0;

public:
	
	//----------------------------------------------------------------//
	void					ClearSurface				(); // takes zgl clear flags
	size_t					CountTextureUnits			();
	void					DrawPrims					( u32 primType, u32 base, u32 count );
	MOAIBlendMode			GetBlendMode				() const;
	u32						GetBufferHeight				() const;
	u32						GetBufferWidth				() const;
	MOAIFrameBuffer*		GetCurrentFrameBuffer		();
	MOAIShader*				GetCurrentShader			();
	MOAIVertexFormat*		GetCurrentVtxFormat			();
	MOAIFrameBuffer*		GetDefaultFrameBuffer		();
	MOAITexture*			GetDefaultTexture			();
	bool					GetDepthMask				() const;
	float					GetViewHeight				() const;
	ZLRect					GetViewRect					() const;
	float					GetViewWidth				() const;
							MOAIGfxMgr_GPUCache			();
	virtual					~MOAIGfxMgr_GPUCache		();
	void					ResetGPUState				();
	void					SetBlendMode				();
	void					SetBlendMode				( const MOAIBlendMode& blendMode );
	void					SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );
	void					SetCullFunc					();
	void					SetCullFunc					( int cullFunc );
	void					SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer );
	void					SetDefaultTexture			( MOAITexture* texture );
	void					SetDepthFunc				();
	void					SetDepthFunc				( int depthFunc );
	void					SetDepthMask				( bool depthMask );
	bool					SetFrameBuffer				( MOAIFrameBuffer* frameBuffer = NULL );
	bool					SetIndexBuffer				( MOAIIndexBuffer* buffer = NULL );
	void					SetPenWidth					( float penWidth );
	void					SetScissorRect				();
	void					SetScissorRect				( ZLRect rect );
	bool					SetShader					( MOAIShader* shader = NULL );
	bool					SetTexture					( MOAITexture* texture = NULL, ZLIndex textureUnit = ZLIndexOp::ZERO );
	bool					SetVertexArray				( MOAIVertexArray* vtxArray = NULL );
	bool					SetVertexBuffer				( MOAIVertexBuffer* buffer = NULL );
	void					SetVertexFormat				( MOAIVertexFormat* format = NULL );
	void					SetViewRect					();
	void					SetViewRect					( ZLRect rect );
	void					UnbindAll					();
};

#endif
