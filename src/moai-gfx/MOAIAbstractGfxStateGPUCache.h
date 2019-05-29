// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSTATEGPUCACHE_H
#define	MOAIABSTRACTGFXSTATEGPUCACHE_H

#include <moai-gfx/MOAIAbstractGfxMgrComponents.h>
#include <moai-gfx/MOAIBlendMode.h>

class MOAIAbstractShader;
class MOAIAbstractTexture;
class MOAIAbstractFrameBuffer;
class MOAIAbstractIndexBuffer;
class MOAIAbstractVertexArray;
class MOAIAbstractVertexBuffer;
class MOAIAbstractVertexFormat;

//================================================================//
// MOAIAbstractGfxStateGPUCache
//================================================================//
class MOAIAbstractGfxStateGPUCache :
	public virtual MOAIAbstractGfxMgrComponents {
protected:

	//----------------------------------------------------------------//
	virtual void						ZLAbstractGPU_ClearSurface				() = 0; // takes zgl clear flags
	virtual size_t						ZLAbstractGPU_CountTextureUnits			() = 0;
	virtual void						ZLAbstractGPU_DrawPrims					( u32 primType, u32 base, u32 count ) = 0;
	virtual MOAIBlendMode				ZLAbstractGPU_GetBlendMode				() const = 0;
	virtual u32							ZLAbstractGPU_GetBufferHeight			() const = 0;
	virtual u32							ZLAbstractGPU_GetBufferWidth			() const = 0;
	virtual MOAIAbstractFrameBuffer*			ZLAbstractGPU_GetCurrentFrameBuffer		() = 0;
	virtual MOAIAbstractShader*			ZLAbstractGPU_GetCurrentShader			() = 0;
	virtual MOAIAbstractVertexFormat*	ZLAbstractGPU_GetCurrentVtxFormat		() = 0;
	virtual MOAIAbstractFrameBuffer*			ZLAbstractGPU_GetDefaultFrameBuffer		() = 0;
	virtual MOAIAbstractTexture*		ZLAbstractGPU_GetDefaultTexture			() = 0;
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
	virtual void						ZLAbstractGPU_SetDefaultFrameBuffer		( MOAIAbstractFrameBuffer* frameBuffer ) = 0;
	virtual void						ZLAbstractGPU_SetDefaultTexture			( MOAIAbstractTexture* texture ) = 0;
	virtual void						ZLAbstractGPU_SetDepthFunc				() = 0;
	virtual void						ZLAbstractGPU_SetDepthFunc				( int depthFunc ) = 0;
	virtual void						ZLAbstractGPU_SetDepthMask				( bool depthMask ) = 0;
	virtual bool						ZLAbstractGPU_SetFrameBuffer			( MOAIAbstractFrameBuffer* frameBuffer ) = 0;
	virtual bool						ZLAbstractGPU_SetIndexBuffer			( MOAIAbstractIndexBuffer* buffer ) = 0;
	virtual void						ZLAbstractGPU_SetPenWidth				( float penWidth ) = 0;
	virtual void						ZLAbstractGPU_SetScissorRect			() = 0;
	virtual void						ZLAbstractGPU_SetScissorRect			( ZLRect rect ) = 0;
	virtual bool						ZLAbstractGPU_SetShader					( MOAIAbstractShader* shader ) = 0;
	virtual bool						ZLAbstractGPU_SetTexture				( MOAIAbstractTexture* texture, ZLIndex textureUnit ) = 0;
	virtual bool						ZLAbstractGPU_SetVertexArray			( MOAIAbstractVertexArray* vtxArray ) = 0;
	virtual bool						ZLAbstractGPU_SetVertexBuffer			( MOAIAbstractVertexBuffer* buffer ) = 0;
	virtual void						ZLAbstractGPU_SetVertexFormat			( MOAIAbstractVertexFormat* format ) = 0;
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
	MOAIAbstractFrameBuffer*	GetCurrentFrameBuffer		();
	MOAIAbstractShader*			GetCurrentShader			();
	MOAIAbstractVertexFormat*	GetCurrentVtxFormat			();
	MOAIAbstractFrameBuffer*	GetDefaultFrameBuffer		();
	MOAIAbstractTexture*		GetDefaultTexture			();
	bool						GetDepthMask				() const;
	float						GetViewHeight				() const;
	ZLRect						GetViewRect					() const;
	float						GetViewWidth				() const;
								MOAIAbstractGfxStateGPUCache		();
	virtual						~MOAIAbstractGfxStateGPUCache		();
	void						ResetGPUState				();
	void						SetBlendMode				();
	void						SetBlendMode				( const MOAIBlendMode& blendMode );
	void						SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );
	void						SetCullFunc					();
	void						SetCullFunc					( int cullFunc );
	void						SetDefaultFrameBuffer		( MOAIAbstractFrameBuffer* frameBuffer );
	void						SetDefaultTexture			( MOAIAbstractTexture* texture );
	void						SetDepthFunc				();
	void						SetDepthFunc				( int depthFunc );
	void						SetDepthMask				( bool depthMask );
	bool						SetFrameBuffer				( MOAIAbstractFrameBuffer* frameBuffer = NULL );
	bool						SetIndexBuffer				( MOAIAbstractIndexBuffer* buffer = NULL );
	void						SetPenWidth					( float penWidth );
	void						SetScissorRect				();
	void						SetScissorRect				( ZLRect rect );
	bool						SetShader					( MOAIAbstractShader* shader = NULL );
	bool						SetTexture					( MOAIAbstractTexture* texture = NULL, ZLIndex textureUnit = ZLIndexOp::ZERO );
	bool						SetVertexArray				( MOAIAbstractVertexArray* vtxArray = NULL );
	bool						SetVertexBuffer				( MOAIAbstractVertexBuffer* buffer = NULL );
	void						SetVertexFormat				( MOAIAbstractVertexFormat* format = NULL );
	void						SetViewRect					();
	void						SetViewRect					( ZLRect rect );
	void						UnbindAll					();
};

#endif
