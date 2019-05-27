// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGPU_H
#define	ZLABSTRACTGPU_H

#include <zl-gfx/ZLBlendMode.h>
#include <zl-gfx/ZLGfxEnum.h>

class ZLFrameBuffer;
class ZLIndexBuffer;
class ZLShader;
class ZLTextureBase;
class ZLVertexArray;
class ZLVertexBuffer;
class ZLVertexFormat;

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
	virtual ZLFrameBuffer*		ZLAbstractGPU_GetCurrentFrameBuffer		() = 0;
	virtual ZLShader*			ZLAbstractGPU_GetCurrentShader			() = 0;
	virtual ZLVertexFormat*		ZLAbstractGPU_GetCurrentVtxFormat		() = 0;
	virtual ZLFrameBuffer*		ZLAbstractGPU_GetDefaultFrameBuffer		() = 0;
	virtual ZLTextureBase*		ZLAbstractGPU_GetDefaultTexture			() = 0;
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
	virtual void				ZLAbstractGPU_SetDefaultFrameBuffer		( ZLFrameBuffer* frameBuffer ) = 0;
	virtual void				ZLAbstractGPU_SetDefaultTexture			( ZLTextureBase* texture ) = 0;
	virtual void				ZLAbstractGPU_SetDepthFunc				() = 0;
	virtual void				ZLAbstractGPU_SetDepthFunc				( int depthFunc ) = 0;
	virtual void				ZLAbstractGPU_SetDepthMask				( bool depthMask ) = 0;
	virtual bool				ZLAbstractGPU_SetFrameBuffer			( ZLFrameBuffer* frameBuffer ) = 0;
	virtual bool				ZLAbstractGPU_SetIndexBuffer			( ZLIndexBuffer* buffer ) = 0;
	virtual void				ZLAbstractGPU_SetPenWidth				( float penWidth ) = 0;
	virtual void				ZLAbstractGPU_SetScissorRect			() = 0;
	virtual void				ZLAbstractGPU_SetScissorRect			( ZLRect rect ) = 0;
	virtual bool				ZLAbstractGPU_SetShader					( ZLShader* shader ) = 0;
	virtual bool				ZLAbstractGPU_SetTexture				( ZLTextureBase* texture, ZLIndex textureUnit ) = 0;
	virtual bool				ZLAbstractGPU_SetVertexArray			( ZLVertexArray* vtxArray ) = 0;
	virtual bool				ZLAbstractGPU_SetVertexBuffer			( ZLVertexBuffer* buffer ) = 0;
	virtual void				ZLAbstractGPU_SetVertexFormat			( ZLVertexFormat* format ) = 0;
	virtual void				ZLAbstractGPU_SetViewRect				() = 0;
	virtual void				ZLAbstractGPU_SetViewRect				( ZLRect rect ) = 0;
	virtual void				ZLAbstractGPU_UnbindAll					() = 0;

public:
	
	//----------------------------------------------------------------//
	void				ClearSurface				(); // takes zgl clear flags
	size_t				CountTextureUnits			();
	void				DrawPrims					( u32 primType, u32 base, u32 count );
	ZLBlendMode			GetBlendMode				() const;
	u32					GetBufferHeight				() const;
	u32					GetBufferWidth				() const;
	ZLFrameBuffer*		GetCurrentFrameBuffer		();
	ZLShader*			GetCurrentShader			();
	ZLVertexFormat*		GetCurrentVtxFormat			();
	ZLFrameBuffer*		GetDefaultFrameBuffer		();
	ZLTextureBase*		GetDefaultTexture			();
	bool				GetDepthMask				() const;
	float				GetViewHeight				() const;
	ZLRect				GetViewRect					() const;
	float				GetViewWidth				() const;
	void				ResetGPUState				();
	void				SetBlendMode				();
	void				SetBlendMode				( const ZLBlendMode& blendMode );
	void				SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );
	void				SetCullFunc					();
	void				SetCullFunc					( int cullFunc );
	void				SetDefaultFrameBuffer		( ZLFrameBuffer* frameBuffer );
	void				SetDefaultTexture			( ZLTextureBase* texture );
	void				SetDepthFunc				();
	void				SetDepthFunc				( int depthFunc );
	void				SetDepthMask				( bool depthMask );
	bool				SetFrameBuffer				( ZLFrameBuffer* frameBuffer = NULL );
	bool				SetIndexBuffer				( ZLIndexBuffer* buffer = NULL );
	void				SetPenWidth					( float penWidth );
	void				SetScissorRect				();
	void				SetScissorRect				( ZLRect rect );
	bool				SetShader					( ZLShader* shader = NULL );
	bool				SetTexture					( ZLTextureBase* texture = NULL, ZLIndex textureUnit = ZLIndexOp::ZERO );
	bool				SetVertexArray				( ZLVertexArray* vtxArray = NULL );
	bool				SetVertexBuffer				( ZLVertexBuffer* buffer = NULL );
	void				SetVertexFormat				( ZLVertexFormat* format = NULL );
	void				SetViewRect					();
	void				SetViewRect					( ZLRect rect );
	void				UnbindAll					();
						ZLAbstractGPU				();
	virtual				~ZLAbstractGPU				();
};

#endif
