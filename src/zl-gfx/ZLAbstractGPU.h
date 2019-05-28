// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGPU_H
#define	ZLABSTRACTGPU_H

#include <zl-gfx/ZLBlendMode.h>
#include <zl-gfx/ZLGfxEnum.h>

class ZLAbstractShader;
class ZLAbstractTexture;
class ZLFrameBufferGL;
class ZLIndexBufferGL;
class ZLVertexArrayGL;
class ZLVertexBufferGL;
class ZLVertexFormatGL;

//================================================================//
// ZLAbstractGPU
//================================================================//
class ZLAbstractGPU {
protected:

	//----------------------------------------------------------------//
	virtual void				ZLAbstractGPU_ClearSurface				() = 0; // takes zgl clear flags
	virtual size_t				ZLAbstractGPU_CountTextureUnits			() = 0;
	virtual void				ZLAbstractGPU_DrawPrims					( u32 primType, u32 base, u32 count ) = 0;
	virtual ZLBlendMode			ZLAbstractGPU_GetBlendMode				() const = 0;
	virtual u32					ZLAbstractGPU_GetBufferHeight			() const = 0;
	virtual u32					ZLAbstractGPU_GetBufferWidth			() const = 0;
	virtual ZLFrameBufferGL*		ZLAbstractGPU_GetCurrentFrameBuffer		() = 0;
	virtual ZLAbstractShader*	ZLAbstractGPU_GetCurrentShader			() = 0;
	virtual ZLVertexFormatGL*		ZLAbstractGPU_GetCurrentVtxFormat		() = 0;
	virtual ZLFrameBufferGL*		ZLAbstractGPU_GetDefaultFrameBuffer		() = 0;
	virtual ZLAbstractTexture*	ZLAbstractGPU_GetDefaultTexture			() = 0;
	virtual bool				ZLAbstractGPU_GetDepthMask				() const = 0;
	virtual float				ZLAbstractGPU_GetViewHeight				() const = 0;
	virtual ZLRect				ZLAbstractGPU_GetViewRect				() const = 0;
	virtual float				ZLAbstractGPU_GetViewWidth				() const = 0;
	virtual void				ZLAbstractGPU_ResetGPUState				() = 0;
	virtual void				ZLAbstractGPU_SetBlendMode				() = 0;
	virtual void				ZLAbstractGPU_SetBlendMode				( const ZLBlendMode& blendMode ) = 0;
	virtual void				ZLAbstractGPU_SetBlendMode				( int srcFactor, int dstFactor, int equation ) = 0;
	virtual void				ZLAbstractGPU_SetCullFunc				() = 0;
	virtual void				ZLAbstractGPU_SetCullFunc				( int cullFunc ) = 0;
	virtual void				ZLAbstractGPU_SetDefaultFrameBuffer		( ZLFrameBufferGL* frameBuffer ) = 0;
	virtual void				ZLAbstractGPU_SetDefaultTexture			( ZLAbstractTexture* texture ) = 0;
	virtual void				ZLAbstractGPU_SetDepthFunc				() = 0;
	virtual void				ZLAbstractGPU_SetDepthFunc				( int depthFunc ) = 0;
	virtual void				ZLAbstractGPU_SetDepthMask				( bool depthMask ) = 0;
	virtual bool				ZLAbstractGPU_SetFrameBuffer			( ZLFrameBufferGL* frameBuffer ) = 0;
	virtual bool				ZLAbstractGPU_SetIndexBuffer			( ZLIndexBufferGL* buffer ) = 0;
	virtual void				ZLAbstractGPU_SetPenWidth				( float penWidth ) = 0;
	virtual void				ZLAbstractGPU_SetScissorRect			() = 0;
	virtual void				ZLAbstractGPU_SetScissorRect			( ZLRect rect ) = 0;
	virtual bool				ZLAbstractGPU_SetShader					( ZLAbstractShader* shader ) = 0;
	virtual bool				ZLAbstractGPU_SetTexture				( ZLAbstractTexture* texture, ZLIndex textureUnit ) = 0;
	virtual bool				ZLAbstractGPU_SetVertexArray			( ZLVertexArrayGL* vtxArray ) = 0;
	virtual bool				ZLAbstractGPU_SetVertexBuffer			( ZLVertexBufferGL* buffer ) = 0;
	virtual void				ZLAbstractGPU_SetVertexFormat			( ZLVertexFormatGL* format ) = 0;
	virtual void				ZLAbstractGPU_SetViewRect				() = 0;
	virtual void				ZLAbstractGPU_SetViewRect				( ZLRect rect ) = 0;
	virtual void				ZLAbstractGPU_UnbindAll					() = 0;

public:
	
	//----------------------------------------------------------------//
	void					ClearSurface				(); // takes zgl clear flags
	size_t					CountTextureUnits			();
	void					DrawPrims					( u32 primType, u32 base, u32 count );
	ZLBlendMode				GetBlendMode				() const;
	u32						GetBufferHeight				() const;
	u32						GetBufferWidth				() const;
	ZLFrameBufferGL*			GetCurrentFrameBuffer		();
	ZLAbstractShader*		GetCurrentShader			();
	ZLVertexFormatGL*			GetCurrentVtxFormat			();
	ZLFrameBufferGL*			GetDefaultFrameBuffer		();
	ZLAbstractTexture*		GetDefaultTexture			();
	bool					GetDepthMask				() const;
	float					GetViewHeight				() const;
	ZLRect					GetViewRect					() const;
	float					GetViewWidth				() const;
	void					ResetGPUState				();
	void					SetBlendMode				();
	void					SetBlendMode				( const ZLBlendMode& blendMode );
	void					SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );
	void					SetCullFunc					();
	void					SetCullFunc					( int cullFunc );
	void					SetDefaultFrameBuffer		( ZLFrameBufferGL* frameBuffer );
	void					SetDefaultTexture			( ZLAbstractTexture* texture );
	void					SetDepthFunc				();
	void					SetDepthFunc				( int depthFunc );
	void					SetDepthMask				( bool depthMask );
	bool					SetFrameBuffer				( ZLFrameBufferGL* frameBuffer = NULL );
	bool					SetIndexBuffer				( ZLIndexBufferGL* buffer = NULL );
	void					SetPenWidth					( float penWidth );
	void					SetScissorRect				();
	void					SetScissorRect				( ZLRect rect );
	bool					SetShader					( ZLAbstractShader* shader = NULL );
	bool					SetTexture					( ZLAbstractTexture* texture = NULL, ZLIndex textureUnit = ZLIndexOp::ZERO );
	bool					SetVertexArray				( ZLVertexArrayGL* vtxArray = NULL );
	bool					SetVertexBuffer				( ZLVertexBufferGL* buffer = NULL );
	void					SetVertexFormat				( ZLVertexFormatGL* format = NULL );
	void					SetViewRect					();
	void					SetViewRect					( ZLRect rect );
	void					UnbindAll					();
							ZLAbstractGPU				();
	virtual					~ZLAbstractGPU				();
};

#endif
